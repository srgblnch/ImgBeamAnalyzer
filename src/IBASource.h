//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************

#pragma once

#include <string>
#include <yat4tango/ExceptionHelper.h>
#include "ImageAndInfo.h"

#ifdef WIN32
  #ifdef IBA_DLL_EXPORT
     #define IBA_DLLDIR  __declspec(dllexport)
  #else
     #ifdef IBA_DLL_IMPORT
        #define IBA_DLLDIR  __declspec(dllimport)
     #else
        #define IBA_DLLDIR 
     #endif
  #endif
#else
  #if defined(IBA_DLL_EXPORT) || defined(IBA_DLL_IMPORT)
    #define IBA_DLLDIR __attribute__ ((visibility("default")))
  #else
    #define IBA_DLLDIR
  #endif
#endif

namespace isl
{
  class Image;
}

namespace ImgBeamAnalyzer_ns
{
  class ImgBeamAnalyzer;
}

///
/// IBASource defines the abstract interface that controls the communcations
/// between the ImgBeamAnalyzer and the object that provides images to it.
/// It provides two main use cases:
///      - Read an image from the image source
///      - Subscribe to an image source so that it informs us when new images
///        are available.
///
/// There's a default implementation of it IBASourceTango that reads the
/// values from properties of some external tango device attribute.
/// The idea is to use ImgBeamAnalyzer as a DLL and link it to an
/// image source. Then implementing this interface, bypass tango and get
/// really fast acces to the images.
///
/// ImgBeamAnalyzer is built as a library and has no knowledge of the
/// details of the IBASource nor how to build the IBASource object. So
/// it is handled by IBASourceFactory. When creating an image source you
/// must implement an IBASource derivate and an IBASourceFactory derivate,
/// and then register the later so that ImgBeamAnalyzer uses it.
///
class IBA_DLLDIR IBASource
{
public:
  typedef ImgBeamAnalyzer_ns::ImgBeamAnalyzer ObserverType;

private:
  ObserverType* observer_;
  std::string image_attribute_name_;

protected:
  IBASource() : observer_(0) {};

  void process(ImageAndInfo &imginf);

public:
  inline bool observer_registered() const
  {
    return this->observer_ != 0;
  }

  virtual ~IBASource() {};

  virtual void get_image(ImageAndInfo& image) throw (yat::Exception) = 0;
  virtual void set_callback_attribute(const std::string & /*name*/) {};

  void set_image_attribute_name(const std::string & ian)
  {
    this->image_attribute_name_ = ian;
  }

  const std::string & image_attribute_name() const
  {
    return this->image_attribute_name_;
  }

  // Do it only if really interested in events
  void register_observer(ObserverType* observer)
  {
    this->observer_ = observer;
  }
};


/// see IBASource
class IBA_DLLDIR IBASourceFactory
{
protected:
  IBASourceFactory() : next_(0) {};
  virtual ~IBASourceFactory() {};

  virtual IBASource* create_object(const std::string & name) = 0;
  IBASource* create_next(const std::string & name);

  static void register_factory(IBASourceFactory* builder);

private:
  static IBASourceFactory* s_first_;

  IBASourceFactory* next_;

public:
  static IBASource* create(const std::string & name);
};
