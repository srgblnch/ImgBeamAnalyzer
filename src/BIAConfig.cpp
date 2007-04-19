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
  BIAConfig::BIAConfig() :
    enable_image_stats(kDEFAULT_ENABLE_IMAGE_STATS),
    enable_profile(kDEFAULT_ENABLE_PROFILE),
    enable_user_roi(kDEFAULT_ENABLE_USER_ROI),
    enable_auto_roi(kDEFAULT_ENABLE_AUTO_ROI),
    enable_2d_gaussian_fit(kDEFAULT_ENABLE_2D_GAUSSIAN_FIT),
    auto_roi_mag_factor(kDEFAULT_AUTOROI_MAGFACTOR),
    comput_period(kDEFAULT_COMPUT_PERIOD),
    alarm_zone(kDEFAULT_ALARM_ZONE),
    fit2d_nb_iter(kDEFAULT_FIT2D_NB_ITER),
    fit2d_max_rel_change(kDEFAULT_FIT2D_MAX_REL_CHANGE),
    fit1d_nb_iter(kDEFAULT_FIT1D_NB_ITER),
    fit1d_max_rel_change(kDEFAULT_FIT1D_MAX_REL_CHANGE),
    pixel_size_x(kDEFAULT_PIXELSIZE_X),
    pixel_size_y(kDEFAULT_PIXELSIZE_Y),
    growth(kDEFAULT_GROWTH),
    user_roi_origin_x(kDEFAULT_USER_ROI_ORIGIN_X),
    user_roi_origin_y(kDEFAULT_USER_ROI_ORIGIN_Y),
    user_roi_width(kDEFAULT_USER_ROI_WIDTH),
    user_roi_height(kDEFAULT_USER_ROI_HEIGHT),
    rotation(kDEFAULT_ROTATION),
    horizontal_flip(kDEFAULT_HORIZONTAL_FLIP),
    gamma_correction(kDEFAULT_GAMMA_CORRECTION),
    pixel_depth(kDEFAULT_PIXEL_DEPTH)
  {}

  BIAConfig::~BIAConfig()
  {}

  BIAConfig::BIAConfig(const BIAConfig& _c)
  {
    *this = _c;
  }

  BIAConfig& BIAConfig::operator=(const BIAConfig& _c)
  {
    this->enable_image_stats     = _c.enable_image_stats;
    this->enable_profile         = _c.enable_profile;
    this->enable_user_roi        = _c.enable_user_roi;
    this->enable_auto_roi        = _c.enable_auto_roi;
    this->enable_2d_gaussian_fit = _c.enable_2d_gaussian_fit;
    this->auto_roi_mag_factor    = _c.auto_roi_mag_factor;
    this->comput_period          = _c.comput_period;
    this->alarm_zone             = _c.alarm_zone;
    this->fit2d_nb_iter          = _c.fit2d_nb_iter;
    this->fit2d_max_rel_change   = _c.fit2d_max_rel_change;
    this->fit1d_nb_iter          = _c.fit1d_nb_iter;
    this->fit1d_max_rel_change   = _c.fit1d_max_rel_change;
    this->pixel_size_x           = _c.pixel_size_x;
    this->pixel_size_y           = _c.pixel_size_y;
    this->growth                 = _c.growth;
    this->user_roi_origin_x      = _c.user_roi_origin_x;
    this->user_roi_origin_y      = _c.user_roi_origin_y;
    this->user_roi_width         = _c.user_roi_width;
    this->user_roi_height        = _c.user_roi_height;
    this->rotation               = _c.rotation;
    this->horizontal_flip        = _c.horizontal_flip;
    this->gamma_correction       = _c.gamma_correction;
    this->pixel_depth            = _c.pixel_depth;
    return *this;
  }

  bool BIAConfig::is_user_roi_empty(void) const
  {
    return( this->user_roi_width == 0 || this->user_roi_height == 0 );
  }

  void BIAConfig::check(void) const
    throw (Tango::DevFailed)
  {

    if (this->enable_2d_gaussian_fit && !this->enable_image_stats)
    {
      THROW_DEVFAILED("CONFIGURATION_ERROR",
                      "EnableImageStats must be true if Enable2DGaussianFit is true",
                      "BIAConfig::check");
    }


#define CHECK( member_name, condition, attr_name )        \
    if ( !(this->member_name condition) )                      \
    {                                                     \
      THROW_DEVFAILED("CONFIGURATION_ERROR",              \
                      "" #attr_name  " must be positive", \
                      "BIAConfig::check");                \
    }

    CHECK( auto_roi_mag_factor,  > 0 , AutoROIMagFactor );
    CHECK( comput_period,        > 0 , ComputationPeriod );
    CHECK( alarm_zone,           > 0 , AlarmZone );
    CHECK( fit2d_nb_iter,        > 0 , Fit2DNbIter );
    CHECK( fit2d_max_rel_change, > 0 , Fit2DMaxRelChange );
    CHECK( fit1d_nb_iter,        > 0 , Fit1DNbIter );
    CHECK( fit1d_max_rel_change, > 0 , Fit1DMaxRelChange );
    CHECK( pixel_size_x,         > 0 , PixelSizeX );
    CHECK( pixel_size_y,         > 0 , PixelSizeY );
    CHECK( growth,               > 0 , Growth );
    CHECK( user_roi_origin_x,    >=0 , UserROIOriginX );
    CHECK( user_roi_origin_y,    >=0 , UserROIOriginY );
    CHECK( user_roi_width,       >=0 , UserROIWidth );
    CHECK( user_roi_height,      >=0 , UserROIHeight );
    CHECK( gamma_correction,     > 0 , GammaCorrection );
    CHECK( pixel_depth,          > 0 , PixelDepth );

  }

} // namespace
