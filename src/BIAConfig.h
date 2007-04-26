// ============================================================================
//
// = CONTEXT
//    TANGO Project - ImgBeamAnalyzer DeviceServer - Config class
//
// = File
//    BIAConfig.h
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
#include <tango.h>

#include <DeviceTask.h>

namespace ImgBeamAnalyzer_ns
{


class BIAConfig
{
public:

  BIAConfig();
  BIAConfig(const BIAConfig&);
  ~BIAConfig();
  BIAConfig& operator = (const BIAConfig&);

  Tango::DevBoolean enable_image_stats;
  Tango::DevBoolean enable_profile;
  Tango::DevBoolean enable_histogram;
  Tango::DevBoolean enable_user_roi;
  Tango::DevBoolean enable_auto_roi;
  Tango::DevBoolean enable_2d_gaussian_fit;
  Tango::DevDouble  auto_roi_mag_factor;
  Tango::DevLong    comput_period;
  Tango::DevLong    alarm_zone;
  Tango::DevLong    fit2d_nb_iter;
  Tango::DevDouble  fit2d_max_rel_change;
  Tango::DevLong    fit1d_nb_iter;
  Tango::DevDouble  fit1d_max_rel_change;
  Tango::DevDouble  pixel_size_x;
  Tango::DevDouble  pixel_size_y;
  Tango::DevDouble  optical_mag;
  Tango::DevLong    user_roi_origin_x;
  Tango::DevLong    user_roi_origin_y;
  Tango::DevLong    user_roi_width;
  Tango::DevLong    user_roi_height;
  Tango::DevLong    rotation;
  Tango::DevBoolean horizontal_flip;
  Tango::DevDouble  gamma_correction;
  Tango::DevLong    pixel_depth;
  Tango::DevLong    histo_nb_bins;
  Tango::DevLong    histo_range_min;
  Tango::DevLong    histo_range_max;
 
  bool is_user_roi_empty(void) const;

  void check(void) const
    throw (Tango::DevFailed);
};


} // namespace

#endif
