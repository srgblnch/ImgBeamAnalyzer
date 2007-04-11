// ============================================================================
//
// = CONTEXT
//    TANGO Project - ImgBeamAnalyzer DeviceServer - DataProcessing class
//
// = File
//    BIAProcessor.h
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
#include <DeviceTask.h>

namespace ImgBeamAnalyzer_ns
{

class BIAProcessor
{
  friend class BIATask;

private:

  BIAProcessor ();
  virtual ~BIAProcessor ();

  void process(const isl::Image& image, const BIAConfig& config, BIAData& data)
    throw (Tango::DevFailed);

  void start_learn_noise(void);
  
  void stop_learn_noise(void);
   
  isl::NoiseEstimator noise_estim_;

  bool learn_noise_;
  isl::Image mean_noise_image_;
};

}  // namespace

#endif// _DATA_PROC_H_
