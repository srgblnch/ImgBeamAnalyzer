
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
