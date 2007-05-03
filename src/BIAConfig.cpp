// ============================================================================
//
// = CONTEXT
//    TANGO Project - ImgBeamAnalyzer DeviceServer - Config class
//
// = File
//    BIAConfig.cpp
//
// = AUTHOR
//    Julien Malik
//
// ============================================================================

// ============================================================================
// DEPENDENCIES
// ============================================================================
#include "BIAConfig.h"

namespace ImgBeamAnalyzer_ns
{
  const bool    kDEFAULT_ENABLE_IMAGE_STATS = true;
  const bool    kDEFAULT_ENABLE_PROFILE = true;
  const bool    kDEFAULT_ENABLE_HISTOGRAM = true;
  const bool    kDEFAULT_ENABLE_USER_ROI = true;
  const bool    kDEFAULT_ENABLE_AUTO_ROI = true;
  const bool    kDEFAULT_ENABLE_2D_GAUSSIAN_FIT = false;
  const double  kDEFAULT_AUTOROI_MAGFACTOR = 1.0f;
  const long    kDEFAULT_COMPUT_PERIOD = 1000;
  const long    kDEFAULT_ALARM_ZONE = 10;
  const long    kDEFAULT_FIT2D_NB_ITER = 30;
  const double  kDEFAULT_FIT2D_MAX_REL_CHANGE = 1.0E-6;
  const long    kDEFAULT_FIT1D_NB_ITER = 30;
  const double  kDEFAULT_FIT1D_MAX_REL_CHANGE = 1.0E-6;
  const double  kDEFAULT_PIXELSIZE_X = 1.0f;
  const double  kDEFAULT_PIXELSIZE_Y = 1.0f;
  const double  kDEFAULT_OPTICAL_MAGNIFICATION = 1.0f;
  const bool    kDEFAULT_PROFILEFIT_FIXEDBG = false;
  const long    kDEFAULT_USER_ROI_ORIGIN_X = 0;
  const long    kDEFAULT_USER_ROI_ORIGIN_Y = 0;
  const long    kDEFAULT_USER_ROI_WIDTH = 0; //- means 'whole image'
  const long    kDEFAULT_USER_ROI_HEIGHT = 0; //- means 'whole image'
  const long    kDEFAULT_ROTATION = 0;
  const bool    kDEFAULT_HORIZONTAL_FLIP = false;
  const double  kDEFAULT_GAMMA_CORRECTION = 1.0f;
  const long    kDEFAULT_PIXEL_DEPTH = 10;
  const long    kDEFAULT_HISTO_NB_BINS = 0; //- means : maximum nb of bins
  const long    kDEFAULT_HISTO_RANGE_MIN = 0;
  const long    kDEFAULT_HISTO_RANGE_MAX = 0; //- means : 2^pixel_depth


  BIAConfig::BIAConfig() :
    enable_image_stats(kDEFAULT_ENABLE_IMAGE_STATS),
    enable_profile(kDEFAULT_ENABLE_PROFILE),
    enable_histogram(kDEFAULT_ENABLE_HISTOGRAM),
    enable_user_roi(kDEFAULT_ENABLE_USER_ROI),
    enable_auto_roi(kDEFAULT_ENABLE_AUTO_ROI),
    enable_2d_gaussian_fit(kDEFAULT_ENABLE_2D_GAUSSIAN_FIT),
    auto_roi_mag_factor_x(kDEFAULT_AUTOROI_MAGFACTOR),
    auto_roi_mag_factor_y(kDEFAULT_AUTOROI_MAGFACTOR),
    comput_period(kDEFAULT_COMPUT_PERIOD),
    alarm_zone(kDEFAULT_ALARM_ZONE),
    fit2d_nb_iter(kDEFAULT_FIT2D_NB_ITER),
    fit2d_max_rel_change(kDEFAULT_FIT2D_MAX_REL_CHANGE),
    fit1d_nb_iter(kDEFAULT_FIT1D_NB_ITER),
    fit1d_max_rel_change(kDEFAULT_FIT1D_MAX_REL_CHANGE),
    pixel_size_x(kDEFAULT_PIXELSIZE_X),
    pixel_size_y(kDEFAULT_PIXELSIZE_Y),
    optical_mag(kDEFAULT_OPTICAL_MAGNIFICATION),
    profilefit_fixedbg(kDEFAULT_PROFILEFIT_FIXEDBG),
    user_roi_origin_x(kDEFAULT_USER_ROI_ORIGIN_X),
    user_roi_origin_y(kDEFAULT_USER_ROI_ORIGIN_Y),
    user_roi_width(kDEFAULT_USER_ROI_WIDTH),
    user_roi_height(kDEFAULT_USER_ROI_HEIGHT),
    rotation(kDEFAULT_ROTATION),
    horizontal_flip(kDEFAULT_HORIZONTAL_FLIP),
    gamma_correction(kDEFAULT_GAMMA_CORRECTION),
    pixel_depth(kDEFAULT_PIXEL_DEPTH),
    histo_nb_bins(kDEFAULT_HISTO_NB_BINS),
    histo_range_min(kDEFAULT_HISTO_RANGE_MIN),
    histo_range_max(kDEFAULT_HISTO_RANGE_MAX)
  {}

  bool BIAConfig::is_user_roi_empty(void) const
  {
    return( this->user_roi_width == 0 || this->user_roi_height == 0 );
  }

  void BIAConfig::validate(void) const
    throw (yat::Exception)
  {

#define CHECK( condition, error_msg )                     \
    if ( condition )                                      \
    {                                                     \
      THROW_YAT_ERROR("CONFIGURATION_ERROR",              \
                      error_msg,                          \
                      "BIAConfig::check");                \
    }

    CHECK( this->enable_2d_gaussian_fit == true && this->enable_image_stats == false , "EnableImageStats must be true when Enable2DGaussianFit is true" );
    CHECK( this->auto_roi_mag_factor_x <=  0 , "AutoROIMagFactorX must be stricly positive" );
    CHECK( this->auto_roi_mag_factor_y <=  0 , "AutoROIMagFactorY must be stricly positive" );
    CHECK( this->comput_period         <=  0 , "ComputationPeriod must be stricly positive" );
    CHECK( this->alarm_zone            <=  0 , "AlarmZone must be stricly positive" );
    CHECK( this->fit2d_nb_iter         <=  0 , "Fit2DNbIter must be stricly positive" );
    CHECK( this->fit2d_max_rel_change  <=  0 , "Fit2DMaxRelChange must be stricly positive" );
    CHECK( this->fit1d_nb_iter         <=  0 , "Fit1DNbIter must be stricly positive" );
    CHECK( this->fit1d_max_rel_change  <=  0 , "Fit1DMaxRelChange must be stricly positive" );
    CHECK( this->pixel_size_x          <=  0 , "PixelSizeX must be stricly positive" );
    CHECK( this->pixel_size_y          <=  0 , "PixelSizeY must be stricly positive" );
    CHECK( this->optical_mag           <=  0 , "OpticalMagnification must be stricly positive" );
    CHECK( this->user_roi_origin_x     < 0 , "UserROIOriginX must be positive" );
    CHECK( this->user_roi_origin_y     < 0 , "UserROIOriginY must be positive" );
    CHECK( this->user_roi_width        < 0 , "UserROIWidth must be positive" );
    CHECK( this->user_roi_height       < 0 , "UserROIHeight must be positive" );
    CHECK( this->gamma_correction      <=  0 , "GammaCorrection must be stricly positive" );
    CHECK( this->pixel_depth           <=  0 , "PixelDepth must be stricly positive" );
    CHECK( this->histo_nb_bins         < 0 , "HistogramNbBins must be positive" );
    CHECK( this->histo_range_min       < 0 , "HistogramRangeMin must be positive" );
    CHECK( this->histo_range_max       < 0 , "HistogramRangeMax must be positive" );
    CHECK( this->histo_range_max > 0 && this->histo_range_min >= this->histo_range_max  , "HistogramRangeMin must be inferior to HistogramRangeMax" );
    CHECK( this->histo_range_max > ((1 << pixel_depth) - 1) , "HistogramRangeMax must be inferior to 2^PixelDepth - 1" );

  }

} // namespace
