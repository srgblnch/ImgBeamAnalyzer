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
#include <SharedObject.h>
#include <DataBuffer.h>
#include "BIAConfig.h"

namespace ImgBeamAnalyzer_ns
{

class BIAData : private adtb::SharedObject
{
  friend class BIATask;
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
  adtb::ImageBuffer<Tango::DevUShort>     input_image;
  Tango::DevLong                 image_width;
  Tango::DevLong                 image_height;

  //- input parameters
  BIAConfig                      config;
  Tango::DevBoolean              user_roi_alarm;

  //- computation time
  Tango::DevLong                 estim_comput_time;

  //- ellipse fitting
  Tango::DevDouble               ellipse_centroid_x;
  Tango::DevDouble               ellipse_centroid_y;
  Tango::DevDouble               ellipse_major_axis;
  Tango::DevDouble               ellipse_minor_axis;
  Tango::DevDouble               ellipse_tilt;

  //- beam box
  adtb::ImageBuffer<Tango::DevUChar>      thresholded_image;
  Tango::DevLong                 auto_roi_origin_x;
  Tango::DevLong                 auto_roi_origin_y;
  Tango::DevLong                 auto_roi_width;
  Tango::DevLong                 auto_roi_height;
  adtb::ImageBuffer<Tango::DevUShort>     roi_image;
  Tango::DevBoolean              bb_found;

  //- noise
  Tango::DevLong                 nb_noise_image;
  adtb::ImageBuffer<Tango::DevUShort>     mean_noise_image;

  //- moments inside beam box
  Tango::DevDouble               max_intensity;
  Tango::DevDouble               mean_intensity;
  Tango::DevDouble               centroid_x;
  Tango::DevDouble               centroid_y;
  Tango::DevDouble               variance_x;
  Tango::DevDouble               variance_y;
  Tango::DevDouble               covariance_xy;
  Tango::DevDouble               correlation_xy;
  Tango::DevDouble               skew_x;
  Tango::DevDouble               skew_y;
  Tango::DevDouble               skew_x2y;
  Tango::DevDouble               skew_xy2;

  //- profile
  adtb::Buffer<Tango::DevDouble> profile_x;
  adtb::Buffer<Tango::DevDouble> profile_x_fitted;
  adtb::Buffer<Tango::DevDouble> profile_x_error;
  Tango::DevDouble               profile_x_center;
  Tango::DevDouble               profile_x_mag;
  Tango::DevDouble               profile_x_sigma;
  Tango::DevDouble               profile_x_fwhm;
  Tango::DevDouble               profile_x_bg;
  Tango::DevDouble               profile_x_chi2;
  adtb::Buffer<Tango::DevDouble> profile_y;
  adtb::Buffer<Tango::DevDouble> profile_y_fitted;
  adtb::Buffer<Tango::DevDouble> profile_y_error;
  Tango::DevDouble               profile_y_center;
  Tango::DevDouble               profile_y_mag;
  Tango::DevDouble               profile_y_sigma;
  Tango::DevDouble               profile_y_fwhm;
  Tango::DevDouble               profile_y_bg;
  Tango::DevDouble               profile_y_chi2;

  //- gaussian fit
  Tango::DevDouble               gaussfit_magnitude;
  Tango::DevDouble               gaussfit_centroid_x;
  Tango::DevDouble               gaussfit_centroid_y;
  Tango::DevDouble               gaussfit_variance_x;
  Tango::DevDouble               gaussfit_variance_y;
  Tango::DevDouble               gaussfit_covariance_xy;
  Tango::DevDouble               gaussfit_correlation_xy;
  Tango::DevDouble               gaussfit_major_axis_fwhm;
  Tango::DevDouble               gaussfit_minor_axis_fwhm;
  Tango::DevDouble               gaussfit_tilt;
  Tango::DevDouble               gaussfit_bg;
  Tango::DevDouble               gaussfit_chi2;
  adtb::ImageBuffer<Tango::DevDouble>     gaussfit_parameters_covariance;

  Tango::DevBoolean              alarm;

private:

  BIAData (void);

  virtual ~ BIAData (void);

  BIAData & operator= (const BIAData &);

  BIAData (const BIAData &);

  void update_alarm();
};


} // namespace

#if defined (__INLINE_IMPL__)
# include "BIAData.i"
#endif // __INLINE_IMPL__

#endif
