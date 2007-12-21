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
#include <isl/Exception.h>
#include <isl/ErrorHandler.h>

#include "BIAData.h"
#include "BIAConfig.h"



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

  private:
    void preprocess(isl::Image& image, const BIAConfig& config, BIAData& data)
      throw (isl::Exception);

    void clip(const isl::Image& image, const BIAConfig& config, isl::Image*& roi_image, isl::Rectangle& roi, BIAData& data)
      throw (isl::Exception);

    void gamma_correction(isl::Image& roi_image_d, const BIAConfig& config, BIAData& data)
      throw (isl::Exception);

    void background_substraction(isl::Image& roi_image_d, const BIAConfig& config, BIAData& data)
      throw (isl::Exception);

    void histogram(isl::Image& roi_image, const BIAConfig& config, BIAData& data)
      throw (isl::Exception);

    void moments(isl::Image& roi_image, isl::Rectangle& roi, const BIAConfig& config, BIAData& data)
      throw (isl::Exception);
    
    void profiles(isl::Image& roi_image_d, isl::Image& roi_image_f, isl::Rectangle& roi, const BIAConfig& config, BIAData& data, bool fixed_bg, double bg_value)
      throw (isl::Exception);

    void gaussian_fit_2d(isl::Image& roi_image_d, isl::Rectangle& roi, const BIAConfig& config, BIAData& data)
      throw (isl::Exception);
  };

}  // namespace

#endif// _DATA_PROC_H_
