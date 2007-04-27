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
#include <yat/DataBuffer.h>

namespace ImgBeamAnalyzer_ns
{

class BIAData : private yat::SharedObject
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
  yat::ImageBuffer<unsigned short> input_image;

  //- input parameters
  BIAConfig                      config;
  bool              user_roi_alarm;

  //- computation time
  long                 estim_comput_time;

  //- beam box
  bool              auto_roi_found;
  long                 auto_roi_origin_x;
  long                 auto_roi_origin_y;
  long                 auto_roi_width;
  long                 auto_roi_height;
  yat::ImageBuffer<unsigned short> roi_image;

  //- noise
  long                 nb_noise_image;
  yat::ImageBuffer<unsigned short> mean_noise_image;

  //- moments inside ROI
  double               max_intensity;
  double               mean_intensity;
  double               centroid_x;
  double               centroid_y;
  double               variance_x;
  double               variance_y;
  double               covariance_xy;
  double               correlation_xy;
  double               skew_x;
  double               skew_y;
  double               skew_x2y;
  double               skew_xy2;

  //- profile
  yat::Buffer<double>  profile_x;
  bool              profile_x_fit_converged;
  yat::Buffer<double>  profile_x_fitted;
  yat::Buffer<double>  profile_x_error;
  double               profile_x_center;
  double               profile_x_mag;
  double               profile_x_sigma;
  double               profile_x_fwhm;
  double               profile_x_bg;
  double               profile_x_chi2;
  long                 profile_x_nb_iter;
  double               profile_x_eps;

  yat::Buffer<double>  profile_y;
  bool              profile_y_fit_converged;
  yat::Buffer<double>  profile_y_fitted;
  yat::Buffer<double>  profile_y_error;
  double               profile_y_center;
  double               profile_y_mag;
  double               profile_y_sigma;
  double               profile_y_fwhm;
  double               profile_y_bg;
  double               profile_y_chi2;
  long                 profile_y_nb_iter;
  double               profile_y_eps;

  //- histogram
  yat::Buffer<float>   histogram;

  
  //- gaussian fit
  bool              gaussfit_converged;
  double               gaussfit_magnitude;
  double               gaussfit_centroid_x;
  double               gaussfit_centroid_y;
  double               gaussfit_variance_x;
  double               gaussfit_variance_y;
  double               gaussfit_covariance_xy;
  double               gaussfit_correlation_xy;
  double               gaussfit_major_axis_fwhm;
  double               gaussfit_minor_axis_fwhm;
  double               gaussfit_tilt;
  double               gaussfit_bg;
  double               gaussfit_chi2;
  long                 gaussfit_nb_iter;
  double               gaussfit_eps;
  yat::ImageBuffer<double> gaussfit_parameters_covariance;

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
