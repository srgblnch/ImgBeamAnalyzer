// ============================================================================
//
// = CONTEXT
//    TANGO Project - ImgBeamAnalyzer DeviceServer - Data class
//
// = File
//    Data.cpp
//
// = AUTHOR
//    Julien Malik
//
// ============================================================================

// ============================================================================
// DEPENDENCIES
// ============================================================================
#include "BIAData.h"

#if !defined (__INLINE_IMPL__)
# include "BIAData.i"
#endif // __INLINE_IMPL___


namespace ImgBeamAnalyzer_ns
{

BIAData::BIAData (void)
  : SharedObject (),
    input_image (0,0),
    image_width(0),
    image_height(0),
    user_roi_alarm(false),
    estim_comput_time(0),
    ellipse_centroid_x(0),
    ellipse_centroid_y(0),
    ellipse_major_axis(0),
    ellipse_minor_axis(0),
    ellipse_tilt(0),
    thresholded_image(0,0),
    auto_roi_origin_x(0),
    auto_roi_origin_y(0),
    auto_roi_width(0),
    auto_roi_height(0),
    roi_image(0,0),
    bb_found(false),
    nb_noise_image(0),
    mean_noise_image(0,0),
    max_intensity (0),
    mean_intensity (0),
    centroid_x (0),
    centroid_y (0),
    variance_x (0),
    variance_y (0),
    covariance_xy (0),
    correlation_xy (0),
    skew_x(0),
    skew_y(0),
    skew_x2y(0),
    skew_xy2(0),
    profile_x(0),
    profile_x_center(0),
    profile_x_mag(0),
    profile_x_sigma(0),
    profile_x_fwhm(0),
    profile_x_bg(0),
    profile_x_chi2(0),
    profile_y(0),
    profile_y_center(0),
    profile_y_mag(0),
    profile_y_sigma(0),
    profile_y_fwhm(0),
    profile_y_bg(0),
    profile_y_chi2(0),
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
    gaussfit_parameters_covariance(0,0),
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
