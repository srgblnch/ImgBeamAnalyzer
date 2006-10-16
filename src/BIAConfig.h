// ============================================================================
//
// = CONTEXT
//    TANGO Project - ImgBeamAnalyzer DeviceServer - Config class
//
// = File
//    Config.h
//
// = AUTHOR
//    Julien Malik
//
// ============================================================================

#ifndef _BIA_CONFIG_H_
#define _BIA_CONFIG_H_

// ============================================================================
// DEPENDENCIES
// ============================================================================
#include "BIACommon.h"


namespace ImgBeamAnalyzer_ns
{


const Tango::DevBoolean kDEFAULT_ENABLE_IMAGE_STATS = true;
const Tango::DevBoolean kDEFAULT_ENABLE_PROFILE = true;
const Tango::DevBoolean kDEFAULT_ENABLE_USER_ROI = true;
const Tango::DevBoolean kDEFAULT_ENABLE_AUTO_ROI = true;
const Tango::DevBoolean kDEFAULT_ENABLE_2D_GAUSSIAN_FIT = false;
const Tango::DevDouble  kDEFAULT_AUTOROI_MAGFACTOR = 1.0f;
const Tango::DevDouble  kDEFAULT_THRESHOLD = 200.0f;
const Tango::DevLong    kDEFAULT_COMPUT_PERIOD = 1000;
const Tango::DevLong    kDEFAULT_ALARM_ZONE = 10;
const Tango::DevLong    kDEFAULT_FIT2D_NB_ITER = 100;
const Tango::DevDouble  kDEFAULT_FIT2D_MAX_REL_CHANGE = 1.0E-6;
const Tango::DevLong    kDEFAULT_FIT1D_NB_ITER = 100;
const Tango::DevDouble  kDEFAULT_FIT1D_MAX_REL_CHANGE = 1.0E-6;
const Tango::DevDouble  kDEFAULT_PIXELSIZE = 1.0f;
const Tango::DevLong    kDEFAULT_USER_ROI_ORIGIN_X = 0;
const Tango::DevLong    kDEFAULT_USER_ROI_ORIGIN_Y = 0;
const Tango::DevLong    kDEFAULT_USER_ROI_WIDTH = 0; //- means 'whole image'
const Tango::DevLong    kDEFAULT_USER_ROI_HEIGHT = 0; //- means 'whole image'
const Tango::DevLong    kDEFAULT_ROTATION = 0;
const Tango::DevBoolean kDEFAULT_HORIZONTAL_FLIP = false;
const Tango::DevDouble  kDEFAULT_GAMMA_CORRECTION = 1.0f;
const Tango::DevLong    kDEFAULT_PIXEL_DEPTH = 10;


class BIAConfig
{
public:

  BIAConfig();
  BIAConfig(const BIAConfig&);
  ~BIAConfig();
  BIAConfig& operator = (const BIAConfig&);

  Tango::DevBoolean enable_image_stats;
  Tango::DevBoolean enable_profile;
  Tango::DevBoolean enable_user_roi;
  Tango::DevBoolean enable_auto_roi;
  Tango::DevBoolean enable_2d_gaussian_fit;
  Tango::DevDouble  auto_roi_mag_factor;
  Tango::DevDouble  threshold;
  Tango::DevLong    comput_period;
  Tango::DevLong    alarm_zone;
  Tango::DevLong    fit2d_nb_iter;
  Tango::DevDouble  fit2d_max_rel_change;
  Tango::DevLong    fit1d_nb_iter;
  Tango::DevDouble  fit1d_max_rel_change;
  Tango::DevDouble  pixel_size;
  Tango::DevLong    user_roi_origin_x;
  Tango::DevLong    user_roi_origin_y;
  Tango::DevLong    user_roi_width;
  Tango::DevLong    user_roi_height;
  Tango::DevLong    rotation;
  Tango::DevBoolean horizontal_flip;
  Tango::DevDouble  gamma_correction;
  Tango::DevLong    pixel_depth;
  
  bool is_user_roi_empty(void);
};


} // namespace

#endif
