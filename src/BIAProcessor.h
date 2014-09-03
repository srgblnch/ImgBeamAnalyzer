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

    void process(const isl::Image& image, const BIAConfig& config, BIAData& data) const
      throw (yat::Exception);

  private:
    void preprocess(isl::Image& image, const BIAConfig& config, BIAData& data) const
      throw (isl::Exception);

    void clip(const isl::Image& image, const BIAConfig& config, isl::Image*& roi_image, isl::Rectangle& roi, BIAData& data) const
      throw (isl::Exception);

    void gamma_correction(isl::Image& roi_image_d, const BIAConfig& config, BIAData& data) const
      throw (isl::Exception);

    void background_substraction(isl::Image& roi_image_d, const BIAConfig& config, BIAData& data) const
      throw (isl::Exception);

    void histogram(const isl::Image& roi_image, const BIAConfig& config, BIAData& data) const
      throw (isl::Exception);

    void moments(const isl::Image& roi_image, const isl::Rectangle& roi, const BIAConfig& config, BIAData& data) const
      throw (isl::Exception);
    
    void profiles(const isl::Image& roi_image_d, const isl::Image& roi_image_f, const isl::Rectangle& roi, const BIAConfig& config, BIAData& data, bool fixed_bg, double bg_value) const
      throw (isl::Exception);

    void saturation(const isl::Image& roi_image, const BIAConfig& config, BIAData& data, const double centroid_x_, const double centroid_y_) const
      throw (isl::Exception);

    void gaussian_fit_2d(const isl::Image& roi_image_d, const isl::Rectangle& roi, const BIAConfig& config, BIAData& data) const
      throw (isl::Exception);
  };

}  // namespace

#endif// _DATA_PROC_H_
