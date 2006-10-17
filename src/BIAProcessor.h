// ============================================================================
//
// = CONTEXT
//    TANGO Project - ImgBeamAnalyzer DeviceServer - DataProcessing class
//
// = File
//    DataProcessing.h
//
// = AUTHOR
//    Julien Malik
//
// ============================================================================

#ifndef _BIA_PROC_H_
#define _BIA_PROC_H_

// ============================================================================
// DEPENDENCIES
// ============================================================================
#include <isl/Image.h>
#include <isl/statistics/Moments.h>
#include <isl/statistics/Profiles.h>
#include <isl/statistics/PrincipalAxis.h>
#include <isl/BeamBox.h>
#include <isl/statistics/GaussianFit1D.h>
#include <isl/statistics/GaussianFit2D.h>
#include <isl/statistics/NoiseEstimator.h>
#include <isl/Exception.h>
#include <isl/ErrorHandler.h>

#include "BIAData.h"
#include "BIAConfig.h"
// ============================================================================
// DEFINEs
// ============================================================================



namespace ImgBeamAnalyzer_ns
{

class BIATask;

class BIAProcessor : public Tango::LogAdapter
{
  //- This class is dedicated to BIATask
  //- Each method must be called in a thred safe manner. This is handled
  //- by the class BIATask where each method (process, configure, get_data, get_config)
  //- is called after locking a mutex.
  friend class BIATask;

private:

  BIAProcessor (Tango::DeviceImpl * host_device, BIATask& task, const BIAConfig& config);
  virtual ~BIAProcessor ();

  void
    process(const isl::Image& image);

  void 
    configure(const BIAConfig& config);

  BIAData* 
    get_data(void);

  const BIAConfig& 
    get_config(void);

  void
    start_learn_noise(void);
  
  void
    stop_learn_noise(void);
  
  Tango::DeviceImpl * host_device_;
 
  isl::Moments2D      moments2d_;
  isl::PrincipalAxis  principal_axis_;
  isl::BeamBox        beam_box_;
  isl::Profiles       profiles_;
  isl::GaussianFit1D  gauss1d_fitter_;
  isl::Moments1D      moments1d_;
  isl::GaussianFit2D  gauss2d_fitter_;
  isl::NoiseEstimator noise_estim_;
  


  BIAData*            data_;
  BIATask&            task_;
  BIAConfig           config_;

  bool learn_noise_;
  isl::Image mean_noise_image_;
};

}  // namespace

#if defined (__INLINE_IMPL__)
# include "BIAProcessor.i"
#endif // __INLINE_IMPL__

#endif// _DATA_PROC_H_
