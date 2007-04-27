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
    covariance_xy (0),
    correlation_xy (0),
    skew_x(0),
    skew_y(0),
    skew_x2y(0),
    skew_xy2(0),
    profile_x(0),
    profile_x_fit_converged(false),
    profile_x_center(0),
    profile_x_mag(0),
    profile_x_sigma(0),
    profile_x_fwhm(0),
    profile_x_bg(0),
    profile_x_chi2(0),
    profile_x_nb_iter(0),
    profile_x_eps(0),
    profile_y(0),
    profile_y_fit_converged(false),
    profile_y_center(0),
    profile_y_mag(0),
    profile_y_sigma(0),
    profile_y_fwhm(0),
    profile_y_bg(0),
    profile_y_chi2(0),
    profile_y_nb_iter(0),
    profile_y_eps(0),
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
