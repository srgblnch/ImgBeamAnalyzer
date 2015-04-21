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
//    TANGO Project - ImgBeamAnalyzer DeviceServer - Data class
//
// = File
//    BIAData.h
//
// = AUTHOR
//    Julien Malik
//
// ============================================================================

#ifndef _BIA_DATA_H_
#define _BIA_DATA_H_

// ============================================================================
// DEPENDENCIES
// ============================================================================
#include "BIAConfig.h"
#include <yat/threading/SharedObject.h>
#include <yat/memory/DataBuffer.h>

namespace ImgBeamAnalyzer_ns
{

class BIAData : public yat::SharedObject
{
  friend class ImgBeamAnalyzerTask;
  friend class BIAProcessor;

public:

  /**
   * Duplicate (shallow copy) this shared object 
   */
  BIAData * duplicate (void);

  /**
   * Release this shared object 
   */
  void release (void);


  /**
   * The data accessors
   */


  //- processed image
  // Mantis bug 14571 : enable 32 bit data
  yat::ImageBuffer<unsigned long> input_image;

  //- input parameters
  BIAConfig            config;
  bool                 user_roi_alarm;

  //- computation time
  yat_int32_t              estim_comput_time;

  //- beam box
  bool                 auto_roi_found;
  yat_int32_t              auto_roi_origin_x;
  yat_int32_t              auto_roi_origin_y;
  yat_int32_t              auto_roi_width;
  yat_int32_t              auto_roi_height;
  // Mantis bug 14571 : enable 32 bit data
  // NRA long becomes short for optim
  yat::ImageBuffer<unsigned short> roi_image;
  // yat::ImageBuffer<unsigned long> roi_image;	
  yat::ImageBuffer<unsigned short> roi_imageMedium;	

  //- noise
  yat_int32_t              nb_noise_image;
  yat::ImageBuffer<unsigned short> binned_image;

  //- moments inside ROI
  double               max_intensity;
  double               mean_intensity;
  double               centroid_x;
  double               centroid_y;
  double               variance_x;
  double               variance_y;
  double               rms_x;
  double               rms_y;
  double               covariance_xy;
  double               correlation_xy;
  double               skew_x;
  double               skew_y;
  double               skew_x2y;
  double               skew_xy2;
  bool                 centroid_saturated;
  double               peak_x;
  double               peak_y;

  //- projections & profile
  yat::Buffer<double>  xproj;
  bool                 xproj_fit_converged;
  yat::Buffer<double>  xproj_fitted;
  yat::Buffer<double>  xproj_error;
  double               xproj_center;
  double               xproj_mag;
  double               xproj_sigma;
  double               xproj_fwhm;
  double               xproj_bg;
  double               xproj_chi2;
  yat_int32_t          xproj_nb_iter;
  double               xproj_eps;
  double               xproj_1e2;
  double		       xproj_order;
  double		       xproj_flat;
  double 			   xproj_divergence;
  
  yat::Buffer<double>  yproj;
  bool                 yproj_fit_converged;
  yat::Buffer<double>  yproj_fitted;
  yat::Buffer<double>  yproj_error;
  double               yproj_center;
  double               yproj_mag;
  double               yproj_sigma;
  double               yproj_fwhm;
  double               yproj_bg;
  double               yproj_chi2;
  yat_int32_t          yproj_nb_iter;
  double               yproj_eps;
  double               yproj_1e2;
  double			   yproj_order;
  double		       yproj_flat;
  double 			   yproj_divergence;

  yat::Buffer<double>  line_profile;
  bool                 line_profile_fit_converged;
  yat::Buffer<double>  line_profile_fitted;
  yat::Buffer<double>  line_profile_error;
  double               line_profile_center;
  double               line_profile_mag;
  double               line_profile_sigma;
  double               line_profile_fwhm;
  double               line_profile_bg;
  double               line_profile_chi2;
  yat_int32_t          line_profile_nb_iter;
  double               line_profile_eps;
  yat::ImageBuffer<float> line_profile_helper_img;
  double               line_profile_1e2;
  double		       line_profile_order;
  double		       line_profile_flat;	

  //- histogram
  yat::Buffer<float>   histogram;

  //- gaussian fit
  bool                 gaussfit_converged;
  double               gaussfit_magnitude;
  double               gaussfit_centroid_x;
  double               gaussfit_centroid_y;
  double               gaussfit_variance_x;
  double               gaussfit_variance_y;
  double               gaussfit_covariance_xy;
  double               gaussfit_correlation_xy;

  bool                 gaussfit_major_converged;
  double               gaussfit_major_axis_fwhm;
  double               gaussfit_major_axis_1e2;
  double		       gaussfit_major_axis_order;
  double               gaussfit_major_axis_center;
  double               gaussfit_major_axis_mag;
  double               gaussfit_major_axis_flat;

  bool                 gaussfit_minor_converged;
  double               gaussfit_minor_axis_fwhm;
  double               gaussfit_minor_axis_1e2;
  double		       gaussfit_minor_axis_order;
  double               gaussfit_minor_axis_center;
  double               gaussfit_minor_axis_mag;
  double               gaussfit_minor_axis_flat;

  int				   gaussfit_major_pt1_x;
  int				   gaussfit_major_pt1_y;
  int			   	   gaussfit_major_pt2_x;
  int			   	   gaussfit_major_pt2_y;
  int			   	   gaussfit_minor_pt1_x;
  int			   	   gaussfit_minor_pt1_y;
  int			   	   gaussfit_minor_pt2_x;
  int			   	   gaussfit_minor_pt2_y;

  double               gaussfit_tilt;
  double               gaussfit_ellipticity;
  double               gaussfit_eccentricity;

  double			   gaussfit_flat_X;
  double			   gaussfit_flat_Y;

  double               gaussfit_bg;
  double               gaussfit_chi2;
  yat_int32_t              gaussfit_nb_iter;
  double               gaussfit_eps;
  yat::ImageBuffer<double> gaussfit_parameters_covariance;
  yat::Buffer<double>  gaussfit_major_axis;
  yat::Buffer<double>  gaussfit_major_axis_fitted;
  yat::Buffer<double>  gaussfit_minor_axis;
  yat::Buffer<double>  gaussfit_minor_axis_fitted;

  double               chamber_centroid_x;
  double               chamber_centroid_y;
  double               chamber_xproj_center;
  double               chamber_yproj_center;


  double               reductionPercent;

  bool              alarm;

private:

  BIAData (void);

  virtual ~ BIAData (void);

  BIAData & operator= (const BIAData &);

  BIAData (const BIAData &);

  void update_alarm();
};


} // namespace

#if defined (YAT_INLINE_IMPL)
# include "BIAData.i"
#endif // YAT_INLINE_IMPL

#endif
