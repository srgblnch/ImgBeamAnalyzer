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

#include <yat/CommonHeader.h>

namespace ImgBeamAnalyzer_ns
{

struct BIAConfig
{
  typedef enum
  {
    AUTOROI_PROFILES,
    AUTOROI_THRESHOLD
  } AutoROIMethod;

  BIAConfig();

  bool          enable_image_stats;
  bool          enable_profile;
  bool          enable_histogram;
  bool          enable_user_roi;
  bool          enable_auto_roi;
  bool          enable_2d_gaussian_fit;
  AutoROIMethod auto_roi_method;
  long          auto_roi_threshold;
  double        auto_roi_mag_factor_x;
  double        auto_roi_mag_factor_y;
  long          comput_period;
  long          alarm_zone;
  long          fit2d_nb_iter;
  double        fit2d_max_rel_change;
  long          fit1d_nb_iter;
  double        fit1d_max_rel_change;
  double        pixel_size_x;
  double        pixel_size_y;
  double        optical_mag;
  bool          profilefit_fixedbg;
  long          user_roi_origin_x;
  long          user_roi_origin_y;
  long          user_roi_width;
  long          user_roi_height;
  long          rotation;
  bool          horizontal_flip;
  double        gamma_correction;
  long          pixel_depth;
  long          histo_nb_bins;
  long          histo_range_min;
  long          histo_range_max;
  double        bg_substraction;
  long          profile_origin_x;
  long          profile_origin_y;
  long          profile_end_x;
  long          profile_end_y;
  long          profile_thickness;

  bool is_user_roi_empty(void) const;

  void validate(void) const
    throw (yat::Exception);
};


} // namespace

#endif
