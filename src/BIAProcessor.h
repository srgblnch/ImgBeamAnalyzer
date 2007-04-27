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
#include "BIAData.h"
#include "BIAConfig.h"

#include <isl/Image.h>
#include <isl/statistics/NoiseEstimator.h>
#include <isl/Exception.h>
#include <isl/ErrorHandler.h>


namespace ImgBeamAnalyzer_ns
{

  class ISL2YATException : public yat::Exception
  {
  public:
    ISL2YATException(const isl::Exception&);
  };


  class BIAProcessor
  {
  public:
    BIAProcessor ();
    virtual ~BIAProcessor ();

    void process(const isl::Image& image, const BIAConfig& config, BIAData& data)
      throw (yat::Exception);

    void start_learn_noise(void);
    
    void stop_learn_noise(void);

  private:
    void preprocess(isl::Image& image, const BIAConfig& config, BIAData& data);

    void clip(const isl::Image& image, const BIAConfig& config, isl::Image*& roi_image, isl::Rectangle& roi, BIAData& data);

    void substract_noise(isl::Image& roi_image, const isl::Rectangle& roi, BIAData& data);

    void gamma_correction(isl::Image& roi_image_d, const BIAConfig& config, BIAData& data);

    void histogram(isl::Image& roi_image, const BIAConfig& config, BIAData& data);

    void moments(isl::Image& roi_image, isl::Rectangle& roi, const BIAConfig& config, BIAData& data);
    
    void profiles(isl::Image& roi_image_d, isl::Rectangle& roi, const BIAConfig& config, BIAData& data);

    void gaussian_fit_2d(isl::Image& roi_image_d, isl::Rectangle& roi, const BIAConfig& config, BIAData& data);


    isl::NoiseEstimator noise_estim_;

    bool learn_noise_;
    isl::Image mean_noise_image_;
  };

}  // namespace

#endif// _DATA_PROC_H_
