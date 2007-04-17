// ============================================================================
//
// = CONTEXT
//    TANGO Project - ImgBeamAnalyzer DeviceServer - DataProcessing class
//
// = File
//    BIAProcessor.cpp
//
// = AUTHOR
//    Julien Malik
//
// ============================================================================

// ============================================================================
// DEPENDENCIES
// ============================================================================
#include "BIAProcessor.h"
#include "BIATask.h"

#include <isl/AutoROI.h>

#include <sys/timeb.h>
// ============================================================================
// Time relative Macros:
// ============================================================================
#define TIMEVAL struct timeb
#define	GET_TIME(T) ftime(&T)
#define	ELAPSED_TIME_MS(start_time, end_time) \
  static_cast<Tango::DevLong>( 1000 * (end_time.time - start_time.time) + (end_time.millitm - start_time.millitm) )

//- the scale factor used to convert the sigma of a distribution to its Full Width at Half Maximum
//- it is approximately 2.35
const Tango::DevDouble SIGMA2FWHM_SCALE_FACTOR = 2 * ::sqrt( 2 * ::log(2) ); 


namespace ImgBeamAnalyzer_ns
{

BIAProcessor::BIAProcessor()
: learn_noise_(false),
  mean_noise_image_(0,0, isl::ISL_STORAGE_USHORT)
{
}


BIAProcessor::~BIAProcessor ()
{
}


void
BIAProcessor::process (const isl::Image& image, const BIAConfig& config, BIAData& data)
  throw (Tango::DevFailed)
{
  DEBUG_TRACE("BIAProcessor::process");

	TIMEVAL	start_time, end_time;
  GET_TIME(start_time);


  //-	Process the image
	//------------------------------------------
  isl::Image* input_image = 0; //- the preprocessed image
  isl::Image* roi_image = 0;
  isl::Image* roi_image_d = 0;
  isl::Image* noise_roi_image = 0;

  try
  {    
    input_image = new isl::Image(image);
    
    //- Preprocess the input image (rotation & flip)
    int operation;
    bool flip = config.horizontal_flip;
    long rotation = config.rotation % 360;
    rotation = rotation < 0 ? rotation + 360 : rotation;

    switch (rotation)
    {
    case 0:
      operation = flip ? (isl::ISL_ROTATE_NONE_FLIP_H) : (isl::ISL_ROTATE_NONE_FLIP_NONE);
      break;
    case 90:
      operation = flip ? (isl::ISL_ROTATE_90_FLIP_H) : (isl::ISL_ROTATE_90_FLIP_NONE);
      break;
    case 180:
      operation = flip ? (isl::ISL_ROTATE_180_FLIP_H) : (isl::ISL_ROTATE_180_FLIP_NONE);
      break;
    case 270:
      operation = flip ? (isl::ISL_ROTATE_270_FLIP_H) : (isl::ISL_ROTATE_270_FLIP_NONE);
      break;
    default:
      operation = isl::ISL_ROTATE_NONE_FLIP_NONE;
      break;
    }
    input_image->rotate_flip(operation);

    //- make a raw copy of the input image
    data.input_image.set_dimensions(input_image->width(), input_image->height());
    data.image_width = input_image->width();
    data.image_height = input_image->height();
    input_image->serialize(data.input_image.base());


    if (this->learn_noise_ == true)
    {
      this->mean_noise_image_ = this->noise_estim_.add_image(*input_image);

      data.mean_noise_image.set_dimensions(this->mean_noise_image_.width(), this->mean_noise_image_.height());
      this->mean_noise_image_.serialize(data.mean_noise_image.base());

      data.nb_noise_image = this->noise_estim_.nb_image();
      data.user_roi_alarm = true;
    }
    else
    {
      double pixel_size = config.pixel_size;
    
      //- Determine the ROI
      isl::Rectangle roi;
      if (config.enable_user_roi)
      {
        if ( config.is_user_roi_empty()
             || config.user_roi_origin_x >=  input_image->width()
             || config.user_roi_origin_y >=  input_image->height() )
        {
          roi = isl::Rectangle(0,
                               0,
                               input_image->width(),
                               input_image->height());
          data.user_roi_alarm = true;
        }
        else
        {
          roi = isl::Rectangle(config.user_roi_origin_x,
                               config.user_roi_origin_y,
                               config.user_roi_width,
                               config.user_roi_height);
        }
        roi_image = input_image->get_roi(roi);
      }
      else if (config.enable_auto_roi)
      {
        
        try
        {
          roi = isl::AutoROI(*input_image, config.auto_roi_mag_factor);
          roi_image = input_image->get_roi(roi);
        }
        catch(isl::Exception&)
        {
          // failed to determine the ROI automatically : take the whole image
          roi = isl::Rectangle(0,
                               0,
                               input_image->width(),
                               input_image->height());
          roi_image = new isl::Image(*input_image);
        }
      
        data.auto_roi_origin_x = roi.origin().x();
        data.auto_roi_origin_y = roi.origin().y();
        data.auto_roi_width    = roi.width();
        data.auto_roi_height   = roi.height();
      }
      else
      {
        //- roi is set to the whole image
        roi = isl::Rectangle(0,
                             0,
                             input_image->width(),
                             input_image->height());


        roi_image = new isl::Image(*input_image);
      }

      if (this->noise_estim_.nb_image() > 0)
      {
        data.mean_noise_image.set_dimensions(this->mean_noise_image_.width(), this->mean_noise_image_.height());
        this->mean_noise_image_.serialize(data.mean_noise_image.base());
        
        data.nb_noise_image = this->noise_estim_.nb_image();

        noise_roi_image = this->mean_noise_image_.get_roi(roi);
        *roi_image -= *noise_roi_image;
        SAFE_DELETE_PTR(noise_roi_image);
      }


      roi_image_d = new isl::Image(roi_image->width(), roi_image->height(), isl::ISL_STORAGE_DOUBLE);
    
      //- copy integer ROI image to floating point representation
      roi_image->convert(*roi_image_d);

      //- apply the gamma correction
      double max_pixel_value = ::pow(2, config.pixel_depth);
      roi_image_d->gamma_correction(config.gamma_correction, max_pixel_value);

      //- convert the floating point values back to the integer image
      roi_image_d->convert(*roi_image);

      data.roi_image.set_dimensions( roi.width(), roi.height() );
      roi_image->serialize(data.roi_image.base());


      //- Compute image statistics
      if (config.enable_image_stats)
      {
        isl::Moments2D img_moments(*roi_image);
        double m00 = img_moments.m00();

        if (img_moments.m00() == 0)
        {
          //- we are sure the image is completely null
          data.mean_intensity = 0;
          data.centroid_x = 0;
          data.centroid_y = 0;
          data.variance_x = 0;
          data.covariance_xy = 0;
          data.variance_y = 0;
          data.correlation_xy = 0;
          data.skew_x = 0;
          data.skew_x2y = 0;
          data.skew_xy2 = 0;
          data.skew_y = 0;
          data.max_intensity = 0;
        }
        else
        {
          data.mean_intensity = m00 / (roi.width() * roi.height());
      
          data.centroid_x     = (img_moments.m10() / m00 + roi.origin().x()) * pixel_size;
          data.centroid_y     = (img_moments.m01() / m00 + roi.origin().y()) * pixel_size;
      
          data.variance_x     = img_moments.mu20() / m00 * pixel_size * pixel_size;
          data.covariance_xy  = img_moments.mu11() / m00 * pixel_size * pixel_size;
          data.variance_y     = img_moments.mu02() / m00 * pixel_size * pixel_size;
          data.correlation_xy = data.covariance_xy / ::sqrt(data.variance_x * data.variance_y);
      
          data.skew_x         = img_moments.mu30() / m00 * pixel_size * pixel_size * pixel_size;
          data.skew_x2y       = img_moments.mu21() / m00 * pixel_size * pixel_size * pixel_size;
          data.skew_xy2       = img_moments.mu12() / m00 * pixel_size * pixel_size * pixel_size;
          data.skew_y         = img_moments.mu03() / m00 * pixel_size * pixel_size * pixel_size;
      
          //- max calculation
          Tango::DevDouble max = 0;
          Tango::DevUShort* cur_pix = data.roi_image.base();
          for (size_t i = 0; i < data.roi_image.length(); i++)
          {
            max = *cur_pix > max ? *cur_pix : max;
            cur_pix++;
          }
          data.max_intensity = max;
        }

        if (config.enable_2d_gaussian_fit)
        {
          if (m00 == 0)
          {
            data.gaussfit_magnitude = 0;
            data.gaussfit_centroid_x = 0;
            data.gaussfit_centroid_y = 0;
            data.gaussfit_variance_x = 0;
            data.gaussfit_variance_y = 0;
            data.gaussfit_covariance_xy = 0;
            data.gaussfit_correlation_xy = 0;
            data.gaussfit_major_axis_fwhm = 0;
            data.gaussfit_minor_axis_fwhm = 0;
            data.gaussfit_tilt = 0;
            data.gaussfit_bg = 0;
            data.gaussfit_chi2 = 0;
            data.gaussfit_parameters_covariance.set_dimensions(7, 7);
            data.gaussfit_parameters_covariance.fill(0);
          }
          else
          {
            double initial_magnitude = roi_image_d->value(static_cast<int>(img_moments.m10() / m00),
                                                          static_cast<int>(img_moments.m01() / m00));

            isl::GaussianFit2D gauss_fit2d;
            gauss_fit2d.nb_iter(config.fit2d_nb_iter);
            gauss_fit2d.epsilon(config.fit2d_max_rel_change);

            TIMEVAL	before_fit, after_fit;
            GET_TIME(before_fit);
            gauss_fit2d.compute(*roi_image_d,
                                initial_magnitude,
                                img_moments.m10() / m00,
                                img_moments.m01() / m00,
                                img_moments.mu20() / m00,
                                img_moments.mu11() / m00,
                                img_moments.mu02() / m00,
                                0);
            GET_TIME(after_fit);
            data.gaussfit_magnitude = gauss_fit2d.magnitude();
            data.gaussfit_centroid_x = ( gauss_fit2d.mean().x() + roi.origin().x() ) * pixel_size;
            data.gaussfit_centroid_y = ( gauss_fit2d.mean().y() + roi.origin().y() ) * pixel_size;

            gauss_fit2d.covariance(data.gaussfit_variance_x,
                                   data.gaussfit_covariance_xy,
                                   data.gaussfit_variance_y);
            data.gaussfit_correlation_xy = data.gaussfit_covariance_xy  
                                           / ::sqrt(data.gaussfit_variance_x * data.gaussfit_variance_y);
      
            data.gaussfit_variance_x    *= pixel_size * pixel_size;
            data.gaussfit_variance_y    *= pixel_size * pixel_size;
            data.gaussfit_covariance_xy *= pixel_size * pixel_size;

            data.gaussfit_bg = gauss_fit2d.background();

            data.gaussfit_parameters_covariance.set_dimensions(7, 7);
            gauss_fit2d.parameters_cov(data.gaussfit_parameters_covariance.base());

            data.gaussfit_chi2 = gauss_fit2d.chi2();
      
            //- Principal Axis of the gaussian
            isl::PrincipalAxis princ_ax(img_moments.m10() / m00,
                                        img_moments.m01() / m00,
                                        img_moments.m20() / m00,
                                        img_moments.m11() / m00,
                                        img_moments.m02() / m00);
            double x,y;
            princ_ax.get_first (x,y, data.gaussfit_major_axis_fwhm);
            data.gaussfit_major_axis_fwhm *= SIGMA2FWHM_SCALE_FACTOR * pixel_size;
      
            princ_ax.get_second(x,y, data.gaussfit_minor_axis_fwhm);
            data.gaussfit_minor_axis_fwhm *= SIGMA2FWHM_SCALE_FACTOR * pixel_size;
      
            data.gaussfit_tilt = princ_ax.get_angle ();
          }
        }
      }



      if (config.enable_profile)
      {
        isl::Profiles profiles(*roi_image_d);
        
        data.profile_x.length(profiles.size_x());
        data.profile_x_fitted.length(profiles.size_x());
        data.profile_x_error.length(profiles.size_x());
        data.profile_x = profiles.get_x_profile();
        try
        {
          isl::GaussianFit1D gaussian_fit;
          gaussian_fit.nb_iter(config.fit1d_nb_iter);
          gaussian_fit.epsilon(config.fit1d_max_rel_change);

          gaussian_fit.compute(profiles.get_x_profile(), profiles.size_x());
          
          data.profile_x_center  = (gaussian_fit.mean() + roi.origin().x()) * pixel_size;
          data.profile_x_mag     = gaussian_fit.magnitude();
          data.profile_x_sigma   = gaussian_fit.standard_deviation() * pixel_size;
          data.profile_x_fwhm    = data.profile_x_sigma * SIGMA2FWHM_SCALE_FACTOR;
          data.profile_x_bg      = gaussian_fit.background();
          data.profile_x_chi2    = gaussian_fit.chi2();
          
          for (size_t i = 0; i < profiles.size_x(); i++)
          {
            data.profile_x_fitted[i] = gaussian_fit.get_fitted_value(i);
            data.profile_x_error[i]  = gaussian_fit.get_fitted_error(i);
          }
        }
        catch(isl::Exception&)
        {
          data.profile_x_center  = 0;
          data.profile_x_mag     = 0;
          data.profile_x_sigma   = 0;
          data.profile_x_fwhm    = 0;
          data.profile_x_bg      = 0;
          data.profile_x_chi2    = 0;

          data.profile_x_fitted.fill(0);
          data.profile_x_error.fill(0);
        }

        data.profile_y.length(profiles.size_y());
        data.profile_y_fitted.length(profiles.size_y());
        data.profile_y_error.length(profiles.size_y());
        data.profile_y = profiles.get_y_profile();
        try
        {
          isl::GaussianFit1D gaussian_fit;
          gaussian_fit.nb_iter(config.fit1d_nb_iter);
          gaussian_fit.epsilon(config.fit1d_max_rel_change);

          gaussian_fit.compute(profiles.get_y_profile(), profiles.size_y());
          
          data.profile_y_center  = (gaussian_fit.mean() + roi.origin().y()) * pixel_size;
          data.profile_y_mag     = gaussian_fit.magnitude();
          data.profile_y_sigma   = gaussian_fit.standard_deviation() * pixel_size;
          data.profile_y_fwhm    = data.profile_y_sigma * SIGMA2FWHM_SCALE_FACTOR;
          data.profile_y_bg      = gaussian_fit.background();
          data.profile_y_chi2    = gaussian_fit.chi2();
          
          for (size_t i = 0; i < profiles.size_y(); i++)
          {
            data.profile_y_fitted[i] = gaussian_fit.get_fitted_value(i);
            data.profile_y_error[i]  = gaussian_fit.get_fitted_error(i);
          }
        }
        catch(isl::Exception&)
        {
          data.profile_y_center  = 0;
          data.profile_y_mag     = 0;
          data.profile_y_sigma   = 0;
          data.profile_y_fwhm    = 0;
          data.profile_y_bg      = 0;
          data.profile_y_chi2    = 0;

          data.profile_y_fitted.fill(0);
          data.profile_y_error.fill(0);
        }
      }

      data.update_alarm();
    }

    GET_TIME(end_time);
    data.estim_comput_time = ELAPSED_TIME_MS(start_time, end_time);

    SAFE_DELETE_PTR(input_image);
    SAFE_DELETE_PTR(roi_image);
    SAFE_DELETE_PTR(roi_image_d);
  }
  catch(isl::Exception & ex)
  {
    SAFE_DELETE_PTR(input_image);
    SAFE_DELETE_PTR(roi_image);
    SAFE_DELETE_PTR(roi_image_d);
    Tango::DevFailed df = BIATask::isl_to_tango_exception(ex);
    isl::ErrorHandler::reset();
    RETHROW_DEVFAILED(df,
                      "SOFTWARE_FAILURE",
                      "Error during processing",
                      "BIAProcessor::process");
  }
  catch(...)
  {
    SAFE_DELETE_PTR(input_image);
    SAFE_DELETE_PTR(roi_image);
    SAFE_DELETE_PTR(roi_image_d);
    THROW_DEVFAILED("UNKNOWN_ERROR",
                    "Error during processing",
                    "BIAProcessor::process");
  }
}


void
BIAProcessor::start_learn_noise (void)
{
  this->noise_estim_.init();
  this->learn_noise_ = true;
}


void
BIAProcessor::stop_learn_noise (void)
{
  this->learn_noise_ = false;
}


} // namespace
