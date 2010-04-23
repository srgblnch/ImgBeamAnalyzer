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

#include <isl/Constants.h>
#include <isl/BeamBox.h>
#include <isl/AutoROI.h>
#include <isl/statistics/Moments.h>
#include <isl/statistics/Projections.h>
#include <isl/statistics/PrincipalAxis.h>
#include <isl/statistics/GaussianFit1D.h>
#include <isl/statistics/GaussianFit2D.h>
#include <isl/statistics/Histogram.h>
#include <isl/statistics/LineProfile.h>

#include <sys/timeb.h>

#include <memory>

#include <math.h>
// ============================================================================
// Time relative Macros:
// ============================================================================
#define TIMEVAL struct timeb
#define  GET_TIME(T) ftime(&T)
#define  ELAPSED_TIME_MS(start_time, end_time) \
  static_cast<yat_int32_t>( 1000 * (end_time.time - start_time.time) + (end_time.millitm - start_time.millitm) )


// #define _IBA_TIMING_TESTS_
// #define _IBA_USE_FLOAT64_

#ifdef _IBA_TIMING_TESTS_
  #include <yat/Timer.h>
  #define CHRONO_BEGIN(name) \
      _TIMESTAMP __lalausecpo_ ## name ## _a, __lalausecpo_ ## name ## _b; \
      _GET_TIME(__lalausecpo_ ## name ## _a)

  #define CHRONO_END(name) \
    if(true) { \
      _GET_TIME(__lalausecpo_ ## name ## _b); \
      const double milis = _ELAPSED_MSEC(__lalausecpo_ ## name ## _a, __lalausecpo_ ## name ## _b); \
      std::cout << "PROFILE " << #name << ": " << milis << "ms" << std::endl; \
    } else (void)0

  struct _ProfileFunction {
    const char*const func_name;
    _TIMESTAMP begin_ts;
    _ProfileFunction(const char* funcName) : func_name(funcName) {
      _GET_TIME(begin_ts);
    }
    ~_ProfileFunction() {
      _TIMESTAMP end_ts;
      _GET_TIME(end_ts);
      const double milis = _ELAPSED_MSEC(begin_ts, end_ts);
      std::cout << "PROFILE " << func_name << "(): " << milis << "ms" << std::endl;
    }
  };
  #define CHRONO_PROFILE() _ProfileFunction __erprofileeer__(__FUNCTION__)
  #define CHRONO_PROFILE_PART(DESC) _ProfileFunction __erprofileeer__ ## DESC(#DESC)

#else
  #define CHRONO_BEGIN(name)
  #define CHRONO_END(name)
  #define CHRONO_PROFILE()
  #define CHRONO_PROFILE_PART(DESC)
#endif // _IBA_TIMING_TESTS_


namespace ImgBeamAnalyzer_ns
{

  //- the scale factor used to convert the sigma of a distribution to its Full Width at Half Maximum
  //- it is approximately 2.35
  const double SIGMA2FWHM_SCALE_FACTOR = ::sqrt( 8.0f * ::log(2.0f) ); 

  ISL2YATException::ISL2YATException(const isl::Exception& ex)
  {
    const isl::ErrorList& isl_errors = ex.errors;
    for (size_t i = 0; i < isl_errors.size(); i++)
    {
      if (isl_errors[i].description != "")
        this->push_error( isl_errors[i].reason,
                          isl_errors[i].description,
                          isl_errors[i].origin);
      else
        this->push_error( isl_errors[i].reason,
                          isl_errors[i].reason,
                          isl_errors[i].origin);
    }
  }

  BIAProcessor::BIAProcessor()
  {
  }

  BIAProcessor::~BIAProcessor ()
  {
  }

  void
  BIAProcessor::process (const isl::Image& image, const BIAConfig& config, BIAData& data) const
  throw (yat::Exception)
  {
    YAT_TRACE("BIAProcessor::process");

    TIMEVAL  start_time, end_time;
    GET_TIME(start_time);


    //- Validate configuration
    //------------------------------------------
    //- will throw an exception if config is invalid
    config.validate();


    //-  Process the image
    //------------------------------------------
    isl::Image* input_image = 0; //- the preprocessed image
    isl::Image* roi_image = 0;
    isl::Image* roi_image_f = 0;

    try
    {
      input_image = new isl::Image(image);

      this->preprocess( *input_image, config, data );

      double estim_bg = 0;
      if (config.profilefit_fixedbg == true)
      {
        CHRONO_PROFILE_PART(ESTIMATE_BACKGROUND);
        estim_bg = input_image->estimate_background( 5 );
      }

      isl::Rectangle roi;

      this->clip(*input_image, config, roi_image, roi, data);

      //- Convert roi_image to floating point value
  CHRONO_BEGIN(CREATE_IMG);
#ifdef _IBA_USE_FLOAT64_
      isl::Image roi_image_d(roi_image->width(), roi_image->height(), isl::ISL_STORAGE_DOUBLE);
#else
      isl::Image roi_image_d(roi_image->width(), roi_image->height(), isl::ISL_STORAGE_FLOAT);
#endif
  CHRONO_END(CREATE_IMG);
      
  CHRONO_BEGIN(CONVERT_IMG);
      roi_image->convert(roi_image_d);
  CHRONO_END(CONVERT_IMG);

      //- convert to roi_image to float
      if (config.enable_profile) {
        roi_image_f = new isl::Image(roi_image->width(), roi_image->height(), isl::ISL_STORAGE_FLOAT);
        try {
          roi_image->convert(*roi_image_f);
        } catch (...) {
          delete roi_image_f;
          throw;
        }
      }
      std::auto_ptr<isl::Image> roi_image_f_guard(roi_image_f);

      this->gamma_correction(roi_image_d, config, data);

      this->background_substraction(roi_image_d, config, data);

  CHRONO_BEGIN(ROI_IMG_COPY);
      roi_image_d.convert(*roi_image);

      data.roi_image.set_dimensions( roi.width(), roi.height() );
      roi_image->serialize(data.roi_image.base());
  CHRONO_END(ROI_IMG_COPY);

      this->histogram(roi_image_d, config, data);;

      this->moments(roi_image_d, roi, config, data);

      this->profiles(roi_image_d, *roi_image_f, roi, config, data, config.profilefit_fixedbg, estim_bg);

      this->gaussian_fit_2d(roi_image_d, roi, config, data);

      data.update_alarm();

      GET_TIME(end_time);
      data.estim_comput_time = ELAPSED_TIME_MS(start_time, end_time);

#ifdef _IBA_TIMING_TESTS_
      std::cout << "TOTAL_TIME: " << data.estim_comput_time << "ms!!\n" << std::endl;
#endif

      SAFE_DELETE_PTR(input_image);
      SAFE_DELETE_PTR(roi_image);
    }
    catch(isl::Exception & ex)
    {
      SAFE_DELETE_PTR(input_image);
      SAFE_DELETE_PTR(roi_image);
      ISL2YATException yat_exc(ex);
      isl::ErrorHandler::reset();
      RETHROW_YAT_ERROR(yat_exc,
                        "SOFTWARE_FAILURE",
                        "Error during processing",
                        "BIAProcessor::process");
    }
    catch(...)
    {
      SAFE_DELETE_PTR(input_image);
      SAFE_DELETE_PTR(roi_image);
      THROW_YAT_ERROR("UNKNOWN_ERROR",
                      "Error during processing",
                      "BIAProcessor::process");
    }
  }

  void
  BIAProcessor::preprocess ( isl::Image& image, const BIAConfig& config, BIAData& data ) const
    throw (isl::Exception)
  {
      CHRONO_PROFILE();
      int operation;
      bool flip = config.horizontal_flip;
      yat_int32_t rotation = config.rotation % 360;
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
      image.rotate_flip(operation);

      //- make a raw copy of the input image
      data.input_image.set_dimensions(image.width(), image.height());
      image.serialize(data.input_image.base());
  }

  
  void
  BIAProcessor::clip(const isl::Image& image, const BIAConfig& config, isl::Image*& roi_image, isl::Rectangle& roi, BIAData& data) const
    throw (isl::Exception)
  {
    CHRONO_PROFILE();
    roi_image = 0;

    isl::Image* user_roi_image = 0;
    isl::Image* auto_roi_image = 0;

    //- First, clip the image with the user ROI
    isl::Rectangle user_roi;
    if (config.enable_user_roi)
    {
      if ( config.is_user_roi_empty()
           || config.user_roi_origin_x >=  image.width()
           || config.user_roi_origin_y >=  image.height() )
      {
        user_roi = isl::Rectangle(0,
                                  0,
                                  image.width(),
                                  image.height());
        data.user_roi_alarm = true;
      }
      else
      {
        user_roi = isl::Rectangle(config.user_roi_origin_x,
                                  config.user_roi_origin_y,
                                  config.user_roi_width,
                                  config.user_roi_height);
      }
      user_roi_image = image.get_roi(user_roi);
    }
    else
    {
      //- duplicate the input image
      user_roi = isl::Rectangle(0,
                                0,
                                image.width(),
                                image.height());
      user_roi_image = new isl::Image(image);
    }

    //- ok, user roi image has been created
    //- be sure it will be deleted when exiting the function
    std::auto_ptr<isl::Image> user_roi_image_ptr(user_roi_image);

    isl::Rectangle auto_roi;
    if (config.enable_auto_roi && config.auto_roi_method == BIAConfig::AUTOROI_PROFILES)
    {
      try
      {
        //- here auto_roi is expressed in the user_roi_image coordinate system
        auto_roi = isl::AutoROI(*user_roi_image, config.auto_roi_mag_factor_x, config.auto_roi_mag_factor_y);

        if (auto_roi.is_empty())
        {
          auto_roi = user_roi;
        }
        else
        {
          data.auto_roi_found = true;
        }

        auto_roi_image = user_roi_image->get_roi(auto_roi);
      }
      catch(isl::Exception& ex)
      {
        std::cerr << ex << std::endl;
        isl::ErrorHandler::reset();
       
        // failed to determine the ROI automatically : take the user roi image (already clipped by user roi)
        auto_roi = user_roi;
        //- use the user_roi_image already allocated
        auto_roi_image = user_roi_image_ptr.release();
      }

      auto_roi.translate(user_roi.x(), user_roi.y());
      data.auto_roi_origin_x = auto_roi.origin().x();
      data.auto_roi_origin_y = auto_roi.origin().y();
      data.auto_roi_width    = auto_roi.width();
      data.auto_roi_height   = auto_roi.height();
    }
    else if (config.enable_auto_roi && config.auto_roi_method == BIAConfig::AUTOROI_THRESHOLD)
    {
      try
      {
        // threshold() does not work for 'short' images.
        isl::Image user_roi_img_working(  user_roi_image->width(),
                                          user_roi_image->height(),
                                          isl::ISL_STORAGE_FLOAT );
        user_roi_image->convert(user_roi_img_working);

        user_roi_img_working.threshold(config.auto_roi_threshold, 255);
        user_roi_img_working.convert(isl::ISL_STORAGE_UCHAR);
        auto_roi = isl::BeamBox(user_roi_img_working, config.auto_roi_mag_factor_x, config.auto_roi_mag_factor_y);
        auto_roi_image = user_roi_image->get_roi(auto_roi);
      }
      catch(isl::Exception& ex)
      {
        std::cerr << ex << std::endl;
        isl::ErrorHandler::reset();
        // failed to determine the ROI automatically : take the whole image
        // (already clipped by user roi)
        auto_roi = user_roi;
        //- use the user_roi_image already allocated
        auto_roi_image = user_roi_image_ptr.release();
      }
      auto_roi.translate(user_roi.x(), user_roi.y());
      data.auto_roi_origin_x = auto_roi.origin().x();
      data.auto_roi_origin_y = auto_roi.origin().y();
      data.auto_roi_width    = auto_roi.width();
      data.auto_roi_height   = auto_roi.height();
    }
    else
    {
      //- roi is set to the whole image (already clipped by user roi)
      auto_roi = user_roi;
      //- use the user_roi_image already allocated
      auto_roi_image = user_roi_image_ptr.release();
    }

    roi_image = auto_roi_image;
    roi = auto_roi;
  }
  
  void
  BIAProcessor::gamma_correction(isl::Image& roi_image_d, const BIAConfig& config, BIAData&) const
    throw (isl::Exception)
  {
    CHRONO_PROFILE();
    if (::fabs(config.gamma_correction - 1) > DBL_EPSILON)
    {
      //- apply the gamma correction
      const double max_pixel_value = static_cast<double>((1 << config.pixel_depth) - 1);
      roi_image_d.gamma_correction(config.gamma_correction, max_pixel_value);
    }
  }
  
  void
  BIAProcessor::background_substraction(isl::Image& roi_image_d, const BIAConfig& config, BIAData&) const
    throw (isl::Exception)
  {
    CHRONO_PROFILE();
    roi_image_d -= config.bg_substraction;

    // Image::threshold() does not work on 64F
#ifdef _IBA_USE_FLOAT64_
    // Copy to a float image
    isl::Image roi_image(roi_image_d.width(), roi_image_d.height(), isl::ISL_STORAGE_FLOAT);
    roi_image_d.convert(roi_image);
    // Threshold it
    roi_image.threshold(0.0, 0.0, isl::ISL_THRESH_TOZERO);
    // Copy back to the double image
    roi_image.convert(roi_image_d);
#else
    roi_image_d.threshold(0.0, 0.0, isl::ISL_THRESH_TOZERO);
#endif
    
  }

  void
  BIAProcessor::histogram(const isl::Image& roi_image, const BIAConfig& config, BIAData& data) const
    throw (isl::Exception)
  {
    CHRONO_PROFILE();
    if (config.enable_histogram)
    {
      const float low_thresh = float(config.histo_range_min);

      const float high_thresh = config.histo_range_max > 0
                              ? float(config.histo_range_max)
                              : float((1 << config.pixel_depth) - 1);

      const int nb_bins = config.histo_nb_bins > 0
                        ? config.histo_nb_bins
                        : int(high_thresh - low_thresh);

      // cvCalcHist only supports 8U and 32F, not 64F
#ifdef _IBA_USE_FLOAT64_
      //- copy integer ROI image to floating point representation
      isl::Image roi_image_f(roi_image.width(), roi_image.height(), isl::ISL_STORAGE_FLOAT);
      roi_image.convert(roi_image_f);
      const isl::Histogram hist(roi_image_f, nb_bins, low_thresh, high_thresh);
#else
      const isl::Histogram hist(roi_image, nb_bins, low_thresh, high_thresh);
#endif

      data.histogram.capacity(nb_bins);
      data.histogram.force_length(nb_bins);
      data.histogram = hist.bins();
    }
  }

  
  void
  BIAProcessor::profiles(const isl::Image& roi_image_d, const isl::Image& roi_image_f, const isl::Rectangle& roi, const BIAConfig& config, BIAData& data, bool fixed_bg, double bg_value) const
    throw (isl::Exception)
  {
    CHRONO_PROFILE();
    if (config.enable_profile)
    {
      double px = config.pixel_size_x / config.optical_mag;
      double py = config.pixel_size_y / config.optical_mag;

      isl::Projections projections(roi_image_d);
      
      data.xproj.capacity(projections.size_x());
      data.xproj.force_length(projections.size_x());
      data.xproj_fitted.capacity(projections.size_x());
      data.xproj_fitted.force_length(projections.size_x());
      data.xproj_error.capacity(projections.size_x());
      data.xproj_error.force_length(projections.size_x());
      data.xproj = projections.get_x_projection();
      try
      {
        isl::GaussianFit1D gaussian_fit;
        gaussian_fit.nb_iter(config.fit1d_nb_iter);
        gaussian_fit.epsilon(config.fit1d_max_rel_change);

        if (fixed_bg == false)
        {
          gaussian_fit.compute(projections.get_x_projection(), projections.size_x());
        }
        else
        {
          gaussian_fit.compute_fixed_bg(projections.get_x_projection(), projections.size_x(), bg_value);
        }
        
        /*if (gaussian_fit.has_converged())
        {*/
          data.xproj_center  = (gaussian_fit.mean() + roi.origin().x()) * px;
          data.xproj_mag     = gaussian_fit.magnitude();
          data.xproj_sigma   = gaussian_fit.standard_deviation() * px;
          data.xproj_fwhm    = data.xproj_sigma * SIGMA2FWHM_SCALE_FACTOR;
          data.xproj_bg      = gaussian_fit.background();
          data.xproj_chi2    = gaussian_fit.chi2();
          data.xproj_nb_iter = gaussian_fit.nb_iter();
          data.xproj_eps     = gaussian_fit.epsilon();

          data.chamber_xproj_center = data.xproj_center - config.chamber_offset_x;
        
          for (int i = 0; i < projections.size_x(); i++)
          {
            data.xproj_fitted[i] = gaussian_fit.get_fitted_value(i);
            data.xproj_error[i]  = gaussian_fit.get_fitted_error(i);
          }
          data.xproj_fit_converged = gaussian_fit.has_converged();//true;
        /*}
        else
        {
          data.xproj_center  = 0;
          data.xproj_mag     = 0;
          data.xproj_sigma   = 0;
          data.xproj_fwhm    = 0;
          data.xproj_bg      = 0;
          data.xproj_chi2    = 0;
          data.xproj_nb_iter = gaussian_fit.nb_iter();
          data.xproj_eps     = gaussian_fit.epsilon();
          data.xproj_fitted.fill(0);
          data.xproj_error.fill(0);
        }*/
      }
      catch(isl::Exception&)
      {
        isl::ErrorHandler::reset();
        data.xproj_center  = 0;
        data.xproj_mag     = 0;
        data.xproj_sigma   = 0;
        data.xproj_fwhm    = 0;
        data.xproj_bg      = 0;
        data.xproj_chi2    = 0;
        data.xproj_nb_iter = 0;
        data.xproj_eps     = 0;
        data.xproj_fitted.fill(0);
        data.xproj_error.fill(0);

        data.chamber_xproj_center = 0 - config.chamber_offset_x;
      }

      data.yproj.capacity(projections.size_y());
      data.yproj.force_length(projections.size_y());
      data.yproj_fitted.capacity(projections.size_y());
      data.yproj_fitted.force_length(projections.size_y());
      data.yproj_error.capacity(projections.size_y());
      data.yproj_error.force_length(projections.size_y());
      data.yproj = projections.get_y_projection();
      try
      {
        isl::GaussianFit1D gaussian_fit;
        gaussian_fit.nb_iter(config.fit1d_nb_iter);
        gaussian_fit.epsilon(config.fit1d_max_rel_change);

        if (fixed_bg == false)
        {
          gaussian_fit.compute(projections.get_y_projection(), projections.size_y());
        }
        else
        {
          gaussian_fit.compute_fixed_bg(projections.get_y_projection(), projections.size_y(), bg_value);
        }
        
        /*if (gaussian_fit.has_converged())
        {*/
          data.yproj_center  = (gaussian_fit.mean() + roi.origin().y()) * py;
          data.yproj_mag     = gaussian_fit.magnitude();
          data.yproj_sigma   = gaussian_fit.standard_deviation() * py;
          data.yproj_fwhm    = data.yproj_sigma * SIGMA2FWHM_SCALE_FACTOR;
          data.yproj_bg      = gaussian_fit.background();
          data.yproj_chi2    = gaussian_fit.chi2();
          data.yproj_nb_iter = gaussian_fit.nb_iter();
          data.yproj_eps     = gaussian_fit.epsilon();

          data.chamber_yproj_center = config.chamber_offset_y - data.yproj_center;
        
          for (int i = 0; i < projections.size_y(); i++)
          {
            data.yproj_fitted[i] = gaussian_fit.get_fitted_value(i);
            data.yproj_error[i]  = gaussian_fit.get_fitted_error(i);
          }

          data.yproj_fit_converged = gaussian_fit.has_converged();//true;
        /*}
        else
        {
          data.yproj_center  = 0;
          data.yproj_mag     = 0;
          data.yproj_sigma   = 0;
          data.yproj_fwhm    = 0;
          data.yproj_bg      = 0;
          data.yproj_chi2    = 0;
          data.yproj_nb_iter = gaussian_fit.nb_iter();
          data.yproj_eps     = gaussian_fit.epsilon();
          data.yproj_fitted.fill(0);
          data.yproj_error.fill(0);
        }*/
      }
      catch(isl::Exception&)
      {
        isl::ErrorHandler::reset();
        data.yproj_center  = 0;
        data.yproj_mag     = 0;
        data.yproj_sigma   = 0;
        data.yproj_fwhm    = 0;
        data.yproj_bg      = 0;
        data.yproj_chi2    = 0;
        data.yproj_nb_iter = 0;
        data.yproj_eps     = 0;
        data.yproj_fitted.fill(0);
        data.yproj_error.fill(0);

        data.chamber_yproj_center = config.chamber_offset_y - data.yproj_center;
      }

      isl::Point2D<int> p( config.profile_origin_x, config.profile_origin_y );
      isl::Point2D<int> q( config.profile_end_x, config.profile_end_y );

      if ( p != q )
      {
        isl::LineProfile lprofile;
        isl::Image* helper_img = lprofile.compute( roi_image_f,
                                                   isl::Point2D<int>( config.profile_origin_x, config.profile_origin_y ),
                                                   isl::Point2D<int>( config.profile_end_x, config.profile_end_y ),
                                                   config.profile_thickness );
        std::auto_ptr<isl::Image> helper_img_guard(helper_img);
        data.line_profile_helper_img.set_dimensions( helper_img->width(), helper_img->height() );
        helper_img->serialize( data.line_profile_helper_img.base() );
        
        data.line_profile.capacity(lprofile.size());
        data.line_profile.force_length(lprofile.size());
        data.line_profile_fitted.capacity(lprofile.size());
        data.line_profile_fitted.force_length(lprofile.size());
        data.line_profile_error.capacity(lprofile.size());
        data.line_profile_error.force_length(lprofile.size());
        data.line_profile = lprofile.values();
        try
        {
          isl::GaussianFit1D gaussian_fit;
          gaussian_fit.nb_iter(config.fit1d_nb_iter);
          gaussian_fit.epsilon(config.fit1d_max_rel_change);

          if (fixed_bg == false)
          {
            gaussian_fit.compute(lprofile.values(), lprofile.size());
          }
          else
          {
            gaussian_fit.compute_fixed_bg(lprofile.values(), lprofile.size(), bg_value);
          }
          
          if (gaussian_fit.has_converged())
          {
            data.line_profile_center  = (gaussian_fit.mean() + roi.origin().y());
            data.line_profile_mag     = gaussian_fit.magnitude();
            data.line_profile_sigma   = gaussian_fit.standard_deviation();
            data.line_profile_fwhm    = data.line_profile_sigma * SIGMA2FWHM_SCALE_FACTOR;
            data.line_profile_bg      = gaussian_fit.background();
            data.line_profile_chi2    = gaussian_fit.chi2();
            data.line_profile_nb_iter = gaussian_fit.nb_iter();
            data.line_profile_eps     = gaussian_fit.epsilon();
          
            for (int i = 0; i < lprofile.size(); i++)
            {
              data.line_profile_fitted[i] = gaussian_fit.get_fitted_value(i);
              data.line_profile_error[i]  = gaussian_fit.get_fitted_error(i);
            }

            data.line_profile_fit_converged = true;
          }
          else
          {
            data.line_profile_center  = 0;
            data.line_profile_mag     = 0;
            data.line_profile_sigma   = 0;
            data.line_profile_fwhm    = 0;
            data.line_profile_bg      = 0;
            data.line_profile_chi2    = 0;
            data.line_profile_nb_iter = gaussian_fit.nb_iter();
            data.line_profile_eps     = gaussian_fit.epsilon();
            data.line_profile_fitted.fill(0);
            data.line_profile_error.fill(0);
          }
        }
        catch(isl::Exception&)
        {
          isl::ErrorHandler::reset();
          data.line_profile_center  = 0;
          data.line_profile_mag     = 0;
          data.line_profile_sigma   = 0;
          data.line_profile_fwhm    = 0;
          data.line_profile_bg      = 0;
          data.line_profile_chi2    = 0;
          data.line_profile_nb_iter = 0;
          data.line_profile_eps     = 0;
          data.line_profile_fitted.fill(0);
          data.line_profile_error.fill(0);
        }
      }
    }
  }
    
  void
  BIAProcessor::moments(const isl::Image& roi_image, const isl::Rectangle& roi, const BIAConfig& config, BIAData& data) const
    throw (isl::Exception)
  {
    CHRONO_PROFILE();
    if (config.enable_image_stats)
    {
      isl::Moments2D img_moments(roi_image);
      double m00 = img_moments.m00();

      if (img_moments.m00() == 0)
      {
        //- we are sure the image is completely null
        data.mean_intensity = 0;
        data.centroid_x = 0;
        data.centroid_y = 0;
        data.variance_x = 0;
        data.rms_x = 0;
        data.covariance_xy = 0;
        data.variance_y = 0;
        data.rms_y = 0;
        data.correlation_xy = 0;
        data.skew_x = 0;
        data.skew_x2y = 0;
        data.skew_xy2 = 0;
        data.skew_y = 0;
        data.max_intensity = 0;
        data.chamber_centroid_x = 0 - config.chamber_offset_x;
        data.chamber_centroid_y = config.chamber_offset_y - 0;
      }
      else
      {
        double px = config.pixel_size_x / config.optical_mag;
        double py = config.pixel_size_y / config.optical_mag;

        data.mean_intensity = m00 / (roi.width() * roi.height());

        const double centroid_pixel_roi_image_x = img_moments.m10() / m00;
        const double centroid_pixel_roi_image_y = img_moments.m01() / m00;
    
        data.centroid_x     = (img_moments.m10() / m00 + roi.origin().x()) * px;
        data.centroid_y     = (img_moments.m01() / m00 + roi.origin().y()) * py;
    
        data.variance_x     = img_moments.mu20() / m00 * px * px;
        data.rms_x          = sqrt(data.variance_x);
        data.covariance_xy  = img_moments.mu11() / m00 * px * py;
        data.variance_y     = img_moments.mu02() / m00 * py * py;
        data.rms_y          = sqrt(data.variance_y);
        data.correlation_xy = data.covariance_xy / ::sqrt(data.variance_x * data.variance_y);
    
        data.skew_x         = img_moments.mu30() / m00 * px * px * px;
        data.skew_x2y       = img_moments.mu21() / m00 * px * px * py;
        data.skew_xy2       = img_moments.mu12() / m00 * px * py * py;
        data.skew_y         = img_moments.mu03() / m00 * py * py * py;

        data.chamber_centroid_x = data.centroid_x - config.chamber_offset_x;
        data.chamber_centroid_y = config.chamber_offset_y - data.centroid_y;
 
        //- max calculation
        unsigned short max = 0;
        unsigned short* cur_pix = data.roi_image.base();
        for (size_t i = 0; i < data.roi_image.length(); ++i, ++cur_pix)
        {
          if (*cur_pix > max)
            max = *cur_pix;
        }
        data.max_intensity = max;
        
        this->saturation(roi_image, config, data, centroid_pixel_roi_image_x, centroid_pixel_roi_image_y);
      }
    }
  }

  void
  BIAProcessor::saturation(const isl::Image& roi_image, const BIAConfig& config, BIAData& data, const double centroid_x_, const double centroid_y_) const
      throw (isl::Exception)
  {
    //typedef unsigned long index;
    typedef int index;

    const index centroid_x = static_cast<index>(centroid_x_);
    const index centroid_y = static_cast<index>(centroid_y_);
    
    const double centroid_value = roi_image.value(centroid_x, centroid_y);
    
    /**
      @todo We should use the max possible pixel value here. But this may be
            too complex. Now we are assuming we already substracted background
            to the image. We do, but it is no very maintanable, some
            modifications in the source may change it! And also there may be
            other things affecting the max possible value. The current
            implementation of gamma correction is not one of these, but
            we may add some other transformation. So, you should be really
            careful with this code.
            \code
              const double max_pixel_value = 
                          static_cast<double>((1 << config.pixel_depth) - 1)
                          - config.bg_substraction;
            \endcode
    
            Alternatively, there's another safer solution. Instead of using
            the max possible value on the image, use the max value of the
            image. If the centroid is not the maximum, we are for sure
            not saturated. This fails, there will be false positives. But the
            client requested something simpler, just check the pixels around
            the centroid to be equal, so they may be happy with this...
            \code
              const double max_pixel_value = data.max_intensity;
            \endcode
    */

    const double max_pixel_value = static_cast<double>((1 << config.pixel_depth) - 1) - config.bg_substraction;
    //- const double max_pixel_value = data.max_intensity;
    
    if (centroid_value < max_pixel_value - DBL_EPSILON) {
      data.centroid_saturated = false;
      return;
    }
    
    const index sat_matrix_width = 5;
    const index sat_matrix_height = 5;
    
    const index half_width = sat_matrix_width / 2;
    const index half_height = sat_matrix_height / 2;
    
    const index x_begin = (centroid_x > half_width) ? (centroid_x - half_width) : 0;
    const index y_begin = (centroid_y > half_height) ? (centroid_y - half_height) : 0;
    
    const index pre_x_end = centroid_x + half_width + (sat_matrix_width % 2);
    const index pre_y_end = centroid_y + half_height + (sat_matrix_height % 2);
    
    const index x_end = (pre_x_end > roi_image.width()) ? roi_image.width() : pre_x_end;
    const index y_end = (pre_y_end > roi_image.height()) ? roi_image.height() : pre_y_end;
    
    // Check all the pixels around the centroid
    for (index y = y_begin; y < y_end; ++y) {
      for (index x = x_begin; x < x_end; ++x) {
        const double value = roi_image.value(x, y);
        if (::fabs(max_pixel_value - value) > DBL_EPSILON ) {
          data.centroid_saturated = false;
          return;
        }
      }
    }
    // All the pixels around the centroid have the same value => saturated
    data.centroid_saturated = true;

    assert(cvGetErrStatus() == 0);
  }

  void
  BIAProcessor::gaussian_fit_2d(const isl::Image& roi_image_d, const isl::Rectangle& roi, const BIAConfig& config, BIAData& data) const
    throw (isl::Exception)
  {
    CHRONO_PROFILE();
    if (config.enable_image_stats && config.enable_2d_gaussian_fit)
    {
      if (data.mean_intensity == 0)
      {
        //- the image is completely null

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
        data.gaussfit_nb_iter = 0;
        data.gaussfit_eps = 0;
        data.gaussfit_parameters_covariance.set_dimensions(7, 7);
        data.gaussfit_parameters_covariance.fill(0);
      }
      else
      {
        try
        {
          isl::GaussianFit2D gauss_fit2d;
          gauss_fit2d.nb_iter(config.fit2d_nb_iter);
          gauss_fit2d.epsilon(config.fit2d_max_rel_change);

          gauss_fit2d.compute(roi_image_d);

          if (gauss_fit2d.has_converged())
          {
            double px = config.pixel_size_x / config.optical_mag;
            double py = config.pixel_size_y / config.optical_mag;

            data.gaussfit_magnitude = gauss_fit2d.magnitude();
            data.gaussfit_centroid_x = ( gauss_fit2d.mean().x() + roi.origin().x() ) * px;
            data.gaussfit_centroid_y = ( gauss_fit2d.mean().y() + roi.origin().y() ) * py;

            gauss_fit2d.covariance(data.gaussfit_variance_x,
                                    data.gaussfit_covariance_xy,
                                    data.gaussfit_variance_y);
            data.gaussfit_correlation_xy = data.gaussfit_covariance_xy  
                                            / ::sqrt(data.gaussfit_variance_x * data.gaussfit_variance_y);
  
            data.gaussfit_variance_x    *= px * px;
            data.gaussfit_variance_y    *= py * py;
            data.gaussfit_covariance_xy *= px * py;

            data.gaussfit_bg = gauss_fit2d.background();

            data.gaussfit_parameters_covariance.set_dimensions(7, 7);

            /// @todo I really dislike it. A copy to a void* and without lenght!
            /// @todo And it's not the only place with similar things in this file!
            gauss_fit2d.parameters_cov(data.gaussfit_parameters_covariance.base());

            data.gaussfit_chi2 = gauss_fit2d.chi2();

            //- Principal Axis of the gaussian
            isl::PrincipalAxis princ_ax(data.gaussfit_centroid_x,
                                        data.gaussfit_centroid_y,
                                        data.gaussfit_variance_x,
                                        data.gaussfit_covariance_xy,
                                        data.gaussfit_variance_y);
            double x,y;
            princ_ax.get_first (x,y, data.gaussfit_major_axis_fwhm);
            data.gaussfit_major_axis_fwhm *= SIGMA2FWHM_SCALE_FACTOR * px;
  
            princ_ax.get_second(x,y, data.gaussfit_minor_axis_fwhm);
            data.gaussfit_minor_axis_fwhm *= SIGMA2FWHM_SCALE_FACTOR * py;
  
            data.gaussfit_tilt = princ_ax.get_angle () * 180 / isl::PI;

            data.gaussfit_nb_iter = gauss_fit2d.nb_iter();
            data.gaussfit_eps = gauss_fit2d.epsilon();

            data.gaussfit_converged = true;
          }
          else
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
            data.gaussfit_nb_iter = gauss_fit2d.nb_iter();
            data.gaussfit_eps = gauss_fit2d.epsilon();
            data.gaussfit_parameters_covariance.set_dimensions(7, 7);
            data.gaussfit_parameters_covariance.fill(0);
          }

        }
        catch(isl::Exception &ex)
        {
          std::cout << ex << std::endl;
          isl::ErrorHandler::reset();
          //- Unable to do the fit
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
          data.gaussfit_nb_iter = 0;
          data.gaussfit_eps = 0;
          data.gaussfit_parameters_covariance.set_dimensions(7, 7);
          data.gaussfit_parameters_covariance.fill(0);
        }
      }
    }
  }



} // namespace
