//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************

#pragma once

namespace isl {
  class Image;
}

struct ImageAndInfo {
  isl::Image* image;
  size_t bit_depth;
  
  ImageAndInfo(isl::Image* image_=0, size_t bit_depth_=0) : 
      image(image_), bit_depth(bit_depth_)
  {}
  
  ImageAndInfo(const ImageAndInfo & ii) :
      image(ii.image), bit_depth(ii.bit_depth)
  {}
};
