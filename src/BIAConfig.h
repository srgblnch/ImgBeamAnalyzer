//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************
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

  typedef struct _Size
  {
	yat_int32_t x;
	yat_int32_t y;
  }Size;

  BIAConfig();

  bool          enable_image_stats;
  bool          enable_profile;
  bool          enable_histogram;
  bool          enable_user_roi;
  bool          enable_auto_roi;
  bool          enable_2d_gaussian_fit;
  bool          enable_super_gaussian_fit;
  AutoROIMethod auto_roi_method;
  yat_int32_t       auto_roi_threshold;
  double        auto_roi_mag_factor_x;
  double        auto_roi_mag_factor_y;
  yat_int32_t       comput_period;
  yat_int32_t       alarm_zone;
  yat_int32_t       fit2d_nb_iter;
  double        fit2d_max_rel_change;
  yat_int32_t       fit1d_nb_iter;
  double        fit1d_max_rel_change;
  double        pixel_size_x;
  double        pixel_size_y;
  double        optical_mag;
  bool          profilefit_fixedbg;
  yat_int32_t       user_roi_origin_x;
  yat_int32_t       user_roi_origin_y;
  yat_int32_t       user_roi_width;
  yat_int32_t       user_roi_height;
  yat_int32_t       rotation;
  bool          horizontal_flip;
  double        gamma_correction;
  yat_int32_t       pixel_depth;
  yat_int32_t       histo_nb_bins;
  yat_int32_t       histo_range_min;
  yat_int32_t       histo_range_max;
  double        bg_substraction;
  yat_int32_t       profile_origin_x;
  yat_int32_t       profile_origin_y;
  yat_int32_t       profile_end_x;
  yat_int32_t       profile_end_y;
  yat_int32_t       profile_thickness;
  double        chamber_offset_x;
  double        chamber_offset_y;
  yat_int32_t	centroid_saturation_region_side;
  yat_int32_t	centroid_saturation_region_threshold;

  bool is_user_roi_empty(void) const;

  double 		xproj_divergence;
  double 		yproj_divergence;

  double        reductionPercent;

  Size		beforeProcessResize;
  Size		rOIImageMediumSize;

  void validate(void) const
    throw (yat::Exception);
};


} // namespace

#endif
