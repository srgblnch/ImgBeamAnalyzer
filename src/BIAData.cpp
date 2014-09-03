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
//    BIAData.cpp
//
// = AUTHOR
//    Julien Malik
//
// ============================================================================

// ============================================================================
// DEPENDENCIES
// ============================================================================
#include "BIAData.h"

#if !defined (YAT_INLINE_IMPL)
# include "BIAData.i"
#endif // YAT_INLINE_IMPL


namespace ImgBeamAnalyzer_ns
{

BIAData::BIAData (void)
  : SharedObject (),
    input_image (0,0),
    user_roi_alarm(false),
    estim_comput_time(0),
    auto_roi_found(false),
    auto_roi_origin_x(0),
    auto_roi_origin_y(0),
    auto_roi_width(0),
    auto_roi_height(0),
    roi_image(0,0),
    nb_noise_image(0),
    mean_noise_image(0,0),
    max_intensity (0),
    mean_intensity (0),
    centroid_x (0),
    centroid_y (0),
    variance_x (0),
    variance_y (0),
    rms_x (0),
    rms_y (0),
    covariance_xy (0),
    correlation_xy (0),
    skew_x(0),
    skew_y(0),
    skew_x2y(0),
    skew_xy2(0),
    centroid_saturated(false),
    xproj(0),
    xproj_fit_converged(false),
    xproj_fitted(0),
    xproj_error(0),
    xproj_center(0),
    xproj_mag(0),
    xproj_sigma(0),
    xproj_fwhm(0),
    xproj_bg(0),
    xproj_chi2(0),
    xproj_nb_iter(0),
    xproj_eps(0),
    yproj(0),
    yproj_fit_converged(false),
    yproj_fitted(0),
    yproj_error(0),
    yproj_center(0),
    yproj_mag(0),
    yproj_sigma(0),
    yproj_fwhm(0),
    yproj_bg(0),
    yproj_chi2(0),
    yproj_nb_iter(0),
    yproj_eps(0),
    line_profile(0),
    line_profile_fit_converged(false),
    line_profile_fitted(0),
    line_profile_error(0),
    line_profile_center(0),
    line_profile_mag(0),
    line_profile_sigma(0),
    line_profile_fwhm(0),
    line_profile_bg(0),
    line_profile_chi2(0),
    line_profile_nb_iter(0),
    line_profile_eps(0),
    gaussfit_converged(false),
    gaussfit_magnitude(0),
    gaussfit_centroid_x(0),
    gaussfit_centroid_y(0),
    gaussfit_variance_x(0),
    gaussfit_variance_y(0),
    gaussfit_covariance_xy(0),
    gaussfit_correlation_xy(0),
    gaussfit_major_axis_fwhm(0),
    gaussfit_minor_axis_fwhm(0),
    gaussfit_tilt(0),
    gaussfit_bg(0),
    gaussfit_chi2(0),
    gaussfit_nb_iter(0),
    gaussfit_eps(0),
    gaussfit_parameters_covariance(0,0),
    chamber_centroid_x(0.0),
    chamber_centroid_y(0.0),
    chamber_xproj_center(0.0),
    chamber_yproj_center(0.0),
    alarm(false)
{
  //- noop
}

BIAData::~BIAData (void)
{
 //- noop dtor
}

void
BIAData::update_alarm(void)
{
  this->alarm = false; //- NOT SUPPORTED
}

}
