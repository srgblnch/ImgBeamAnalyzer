// ============================================================================
//
// = CONTEXT
//    TANGO Project - ImgBeamAnalyzer DeviceServer - DataProcessing class
//
// = File
//    DataProcessing.cpp
//
// = AUTHOR
//    Julien Malik
//
// ============================================================================

// ============================================================================
// DEPENDENCIES
// ============================================================================
#include <isl/ErrorHandler.h>
#include "BIAProcessor.h"
#include "BIATask.h"

#include <isl/geometry/Rectangle.h>

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

#if !defined (__INLINE_IMPL__)
# include "BIAProcessor.i"
#endif // __INLINE_IMPL__


namespace ImgBeamAnalyzer_ns
{

BIAProcessor::BIAProcessor(Tango::DeviceImpl * _host_device, BIATask& task, const BIAConfig& config)
: Tango::LogAdapter (_host_device),
  host_device_(_host_device),
  data_(0),
  task_(task),
  config_(config),
  learn_noise_(false),
  mean_noise_image_(0,0, isl::ISL_STORAGE_USHORT)
{
  DEBUG_TRACE("BIAProcessor::BIAProcessor");
  isl::ErrorHandler::init();
}


BIAProcessor::~BIAProcessor ()
{
  DEBUG_TRACE("BIAProcessor::~BIAProcessor");
}


void
BIAProcessor::process (const isl::Image& image)
{
  DEBUG_TRACE("BIAProcessor::process");

	TIMEVAL	start_time, end_time;
  GET_TIME(start_time);

  //- Try to allocate the returned Data
  BIAData* data = 0;
  try
  {
    //- create an instance of Data...
    data = new BIAData ();
    if (data == 0)
      throw std::bad_alloc ();
  }
  catch(const std::bad_alloc &)
  {
    this->task_.fault("A memory error occured while allocating data");
    return;
  }
  catch(...)
  {
    this->task_.fault("Unknown exception caught while allocating data");
    return;
  }

  //-	Process the image
	//------------------------------------------
  isl::Image* input_image = 0; //- the preprocessed image
  isl::Image* roi_image = 0;
  isl::Image* roi_image_d = 0;
  isl::Image* noise_roi_image = 0;
  BIAConfig   config;

  try
  {
    //- make a raw copy of the configuration
    {
      adtb::DeviceMutexLock<> guard(this->config_mutex_);
      config = this->config_;  
    }
    
    data->config = config;
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
    data->input_image.dimensions(input_image->width(), input_image->height());
    data->image_width = input_image->width();
    data->image_height = input_image->height();
    input_image->serialize(data->input_image.base());


    if (this->learn_noise_ == true)
    {
      this->mean_noise_image_ = this->noise_estim_.add_image(*input_image);

      data->mean_noise_image.dimensions(this->mean_noise_image_.width(), this->mean_noise_image_.height());
      this->mean_noise_image_.serialize(data->mean_noise_image.base());

      data->nb_noise_image = this->noise_estim_.nb_image();
      data->user_roi_alarm = true;
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
          data->user_roi_alarm = true;
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
        //- try to determine the beam box automatically
        isl::Image thresholded_image(*input_image);
        thresholded_image.convert(isl::ISL_STORAGE_FLOAT);
        thresholded_image.threshold(config.threshold, 255, isl::ISL_THRESH_BINARY);
        thresholded_image.convert(isl::ISL_STORAGE_UCHAR);
        thresholded_image.morphology(isl::ISL_MORPHO_ERODE, 3, 3, 1, 1, isl::ISL_SHAPE_ELLIPSE, 1);
      
        data->thresholded_image.dimensions(input_image->width(), input_image->height());
        thresholded_image.serialize(data->thresholded_image.base());

        data->bb_found = this->beam_box_.compute(thresholded_image, config.auto_roi_mag_factor);

        if (data->bb_found)
        {
          roi = isl::Rectangle(static_cast<int>(this->beam_box_.corner(0).x()),
                               static_cast<int>(this->beam_box_.corner(0).y()),
                               static_cast<int>(this->beam_box_.width()),
                               static_cast<int>(this->beam_box_.height()));

          roi_image = input_image->get_roi(roi);

          data->ellipse_centroid_x  = this->beam_box_.equivalent_ellipse().center().x() * pixel_size;
          data->ellipse_centroid_y  = this->beam_box_.equivalent_ellipse().center().y() * pixel_size;
          data->ellipse_major_axis  = this->beam_box_.equivalent_ellipse().major_axis() * pixel_size;
          data->ellipse_minor_axis  = this->beam_box_.equivalent_ellipse().minor_axis() * pixel_size;
          data->ellipse_tilt        = this->beam_box_.equivalent_ellipse().tilt();
        }
        else
        {
          roi = isl::Rectangle(0,
                               0,
                               input_image->width(),
                               input_image->height());


          roi_image = new isl::Image(*input_image);
        }
      
        data->auto_roi_origin_x = roi.origin().x();
        data->auto_roi_origin_y = roi.origin().y();
        data->auto_roi_width    = roi.width();
        data->auto_roi_height   = roi.height();
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
        data->mean_noise_image.dimensions(this->mean_noise_image_.width(), this->mean_noise_image_.height());
        this->mean_noise_image_.serialize(data->mean_noise_image.base());
        
        data->nb_noise_image = this->noise_estim_.nb_image();

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

      data->roi_image.dimensions( roi.width(), roi.height() );
      roi_image->serialize(data->roi_image.base());


      //- Compute image statistics
      if (config.enable_image_stats)
      {
        //- moments calculation
        this->moments2d_.compute(*roi_image);
        double m00 = this->moments2d_.get_spatial_moment(0,0);

        if (m00 == 0)
        {
          //- we are sure the image is null
          data->mean_intensity = 0;
          data->centroid_x = 0;
          data->centroid_y = 0;
          data->variance_x = 0;
          data->covariance_xy = 0;
          data->variance_y = 0;
          data->correlation_xy = 0;
          data->skew_x = 0;
          data->skew_x2y = 0;
          data->skew_xy2 = 0;
          data->skew_y = 0;
          data->max_intensity = 0;
        }
        else
        {
          data->mean_intensity = m00 / (roi.width() * roi.height());
      
          data->centroid_x     = (this->moments2d_.get_spatial_moment(1,0) / m00 + roi.origin().x()) * pixel_size;
          data->centroid_y     = (this->moments2d_.get_spatial_moment(0,1) / m00 + roi.origin().y()) * pixel_size;
      
          data->variance_x     = this->moments2d_.get_central_moment(2,0) / m00 * pixel_size * pixel_size;
          data->covariance_xy  = this->moments2d_.get_central_moment(1,1) / m00 * pixel_size * pixel_size;
          data->variance_y     = this->moments2d_.get_central_moment(0,2) / m00 * pixel_size * pixel_size;
          data->correlation_xy = data->covariance_xy / ::sqrt(data->variance_x * data->variance_y);
      
          data->skew_x         = this->moments2d_.get_central_moment(3,0) / m00 * pixel_size * pixel_size * pixel_size;
          data->skew_x2y       = this->moments2d_.get_central_moment(2,1) / m00 * pixel_size * pixel_size * pixel_size;
          data->skew_xy2       = this->moments2d_.get_central_moment(1,2) / m00 * pixel_size * pixel_size * pixel_size;
          data->skew_y         = this->moments2d_.get_central_moment(0,3) / m00 * pixel_size * pixel_size * pixel_size;
      
          //- max calculation
          Tango::DevDouble max = 0;
          Tango::DevUShort* cur_pix = data->roi_image.base();
          for (size_t i = 0; i < data->roi_image.length(); i++)
          {
            max = *cur_pix > max ? *cur_pix : max;
            cur_pix++;
          }
          data->max_intensity = max;
        }
      }



      if (config.enable_profile)
      {
        this->profiles_.compute(*roi_image_d);

        const double* x_profile = this->profiles_.get_x_profile();
        size_t  x_profile_size = this->profiles_.size_x();

        data->profile_x.length(x_profile_size);
        data->profile_x_fitted.length(x_profile_size);
        data->profile_x_error.length(x_profile_size);
        data->profile_x = x_profile;
      
        const double* y_profile = this->profiles_.get_y_profile();
        size_t  y_profile_size = this->profiles_.size_y();
      
        data->profile_y.length(y_profile_size);
        data->profile_y_fitted.length(y_profile_size);
        data->profile_y_error.length(y_profile_size);
        data->profile_y = y_profile;

        //- fit X profile to a gaussian
        {
          this->moments1d_.compute(x_profile, x_profile_size);

          if (this->moments1d_.get_spatial_moment(0) == 0)
          {
            data->profile_x_center  = 0;
            data->profile_x_mag     = 0;
            data->profile_x_sigma   = 0;
            data->profile_x_fwhm    = 0;
            data->profile_x_bg      = 0;
            data->profile_x_chi2    = 0;

            data->profile_x_fitted.fill(0);
            data->profile_x_error.fill(0);
          }
          else
          {
            double covar = this->moments1d_.get_central_moment(2) / this->moments1d_.get_spatial_moment(0);

            this->gauss1d_fitter_.nb_iter(100);
            this->gauss1d_fitter_.epsilon(0.000001);

            this->gauss1d_fitter_.compute(x_profile, 
                                          x_profile_size,
                                          this->profiles_.max_val_x(),
                                          this->profiles_.max_pos_x(),
                                          covar,
                                          x_profile[0]);
            data->profile_x_center  = (this->gauss1d_fitter_.mean() + roi.origin().x()) * pixel_size;
            data->profile_x_mag     = this->gauss1d_fitter_.magnitude();
            data->profile_x_sigma   = this->gauss1d_fitter_.standard_deviation() * pixel_size;
            data->profile_x_fwhm    = data->profile_x_sigma * SIGMA2FWHM_SCALE_FACTOR;
            data->profile_x_bg      = this->gauss1d_fitter_.background();
            data->profile_x_chi2    = this->gauss1d_fitter_.chi2();
        
            for (size_t i = 0; i < x_profile_size; i++)
            {
              data->profile_x_fitted[i] = this->gauss1d_fitter_.get_fitted_value(i);
              data->profile_x_error[i]  = this->gauss1d_fitter_.get_fitted_error(i);
            }
          }
        }
        //- fit Y profile to a gaussian
        {
          this->moments1d_.compute(y_profile, y_profile_size);   
        
          if (this->moments1d_.get_spatial_moment(0) == 0)
          {
            data->profile_y_center  = 0;
            data->profile_y_mag     = 0;
            data->profile_y_sigma   = 0;
            data->profile_y_fwhm    = 0;
            data->profile_y_bg      = 0;
            data->profile_y_chi2    = 0;

            data->profile_y_fitted.fill(0);
            data->profile_y_error.fill(0);
          }
          else
          {
            double covar = this->moments1d_.get_central_moment(2) / this->moments1d_.get_spatial_moment(0);
    
            this->gauss1d_fitter_.nb_iter(config.fit1d_nb_iter);
            this->gauss1d_fitter_.epsilon(config.fit1d_max_rel_change);

            this->gauss1d_fitter_.compute(y_profile, 
                                          y_profile_size,
                                          this->profiles_.max_val_y(),
                                          this->profiles_.max_pos_y(),
                                          covar,
                                          y_profile[0]);
            data->profile_y_center  = (this->gauss1d_fitter_.mean() + roi.origin().y()) * pixel_size;
            data->profile_y_mag     = this->gauss1d_fitter_.magnitude();
            data->profile_y_sigma   = this->gauss1d_fitter_.standard_deviation() * pixel_size;
            data->profile_y_fwhm    = data->profile_y_sigma * SIGMA2FWHM_SCALE_FACTOR;
            data->profile_y_bg      = this->gauss1d_fitter_.background();
            data->profile_y_chi2    = this->gauss1d_fitter_.chi2();
        
            for (size_t i = 0; i < y_profile_size; i++)
            {
              data->profile_y_fitted[i] = this->gauss1d_fitter_.get_fitted_value(i);
              data->profile_y_error[i]  = this->gauss1d_fitter_.get_fitted_error(i);
            }
          }
        }
      }

      if (config.enable_image_stats && config.enable_2d_gaussian_fit)
      {
        double m00 = this->moments2d_.get_spatial_moment(0,0);

        if (m00 == 0)
        {
          data->gaussfit_magnitude = 0;
          data->gaussfit_centroid_x = 0;
          data->gaussfit_centroid_y = 0;
          data->gaussfit_variance_x = 0;
          data->gaussfit_variance_y = 0;
          data->gaussfit_covariance_xy = 0;
          data->gaussfit_correlation_xy = 0;
          data->gaussfit_major_axis_fwhm = 0;
          data->gaussfit_minor_axis_fwhm = 0;
          data->gaussfit_tilt = 0;
          data->gaussfit_bg = 0;
          data->gaussfit_chi2 = 0;
          data->gaussfit_parameters_covariance.dimensions(7, 7);
          data->gaussfit_parameters_covariance.fill(0);
        }
        else
        {
          double initial_magnitude = roi_image_d->value(static_cast<int>(this->moments2d_.get_spatial_moment(1,0) / m00),
                                                        static_cast<int>(this->moments2d_.get_spatial_moment(0,1) / m00));

          this->gauss2d_fitter_.nb_iter(config.fit2d_nb_iter);
          this->gauss2d_fitter_.epsilon(config.fit2d_max_rel_change);

          this->gauss2d_fitter_.compute(*roi_image_d,
                                        initial_magnitude,
                                        this->moments2d_.get_spatial_moment(1,0) / m00,
                                        this->moments2d_.get_spatial_moment(0,1) / m00,
                                        data->variance_x,
                                        data->covariance_xy,
                                        data->variance_y,
                                        0);
          data->gaussfit_magnitude = this->gauss2d_fitter_.magnitude();
          data->gaussfit_centroid_x = ( this->gauss2d_fitter_.mean().x() + roi.origin().x() ) * pixel_size;
          data->gaussfit_centroid_y = ( this->gauss2d_fitter_.mean().y() + roi.origin().y() ) * pixel_size;

          this->gauss2d_fitter_.covariance(data->gaussfit_variance_x,
                                           data->gaussfit_covariance_xy,
                                           data->gaussfit_variance_y);
          data->gaussfit_correlation_xy = data->gaussfit_covariance_xy  / ::sqrt(data->gaussfit_variance_x * data->gaussfit_variance_y);
      
          data->gaussfit_variance_x    *= pixel_size * pixel_size;
          data->gaussfit_variance_y    *= pixel_size * pixel_size;
          data->gaussfit_covariance_xy *= pixel_size * pixel_size;

          data->gaussfit_bg = this->gauss2d_fitter_.background();

          data->gaussfit_parameters_covariance.dimensions(7, 7);
          this->gauss2d_fitter_.parameters_cov(data->gaussfit_parameters_covariance.base());

          data->gaussfit_chi2 = this->gauss2d_fitter_.chi2();
      
          //- Principal Axis of the gaussian
          this->principal_axis_.compute(data->gaussfit_centroid_x,
                                        data->gaussfit_centroid_y,
                                        data->gaussfit_variance_x,
                                        data->gaussfit_covariance_xy,
                                        data->gaussfit_variance_y);
          double x,y;
          this->principal_axis_.get_first (x,y, data->gaussfit_major_axis_fwhm);
          data->gaussfit_major_axis_fwhm *= SIGMA2FWHM_SCALE_FACTOR * pixel_size;
      
          this->principal_axis_.get_second(x,y, data->gaussfit_minor_axis_fwhm);
          data->gaussfit_minor_axis_fwhm *= SIGMA2FWHM_SCALE_FACTOR * pixel_size;
      
          data->gaussfit_tilt = this->principal_axis_.get_angle ();
        }
      }

      data->update_alarm();
    }



    GET_TIME(end_time);
    data->estim_comput_time = ELAPSED_TIME_MS(start_time, end_time);

    SAFE_DELETE_PTR(input_image);
    SAFE_DELETE_PTR(roi_image);
    SAFE_DELETE_PTR(roi_image_d);
  }
  catch(isl::Exception & ex)
  {
    ERROR_STREAM << ex;
    isl::ErrorHandler::reset();
    this->task_.fault("Software Failure during image processing");
    SAFE_DELETE_PTR(data);
    SAFE_DELETE_PTR(input_image);
    SAFE_DELETE_PTR(roi_image);
    SAFE_DELETE_PTR(roi_image_d);
    return;
  }
  catch(...)
  {
    this->task_.fault("Software Failure during image processing");
    SAFE_DELETE_PTR(data);
    SAFE_DELETE_PTR(input_image);
    SAFE_DELETE_PTR(roi_image);
    SAFE_DELETE_PTR(roi_image_d);
    return;
  }

  {
    adtb::DeviceMutexLock<>(this->data_mutex_);
    //- Release any existing data and assign the new ones
    if (this->data_)
    {
      this->data_->release ();
      this->data_ = 0;
    }
    this->data_ = data;
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
