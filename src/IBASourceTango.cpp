
#include "IBASourceTango.h"
#include "BIAProcessor.h"
#include <memory>
#include <cstring>

using ImgBeamAnalyzer_ns::ISL2YATException;

/*static*/ IBASourceFactoryTango IBASourceFactoryTango::s_singleton;

/// This is registering the factory, so it is the default that will run
/// if other registered factories decline to build objects..
/*static*/ IBASourceFactory* IBASourceFactory::s_first_ = &IBASourceFactoryTango::s_singleton;

IBASourceTango::IBASourceTango(const std::string &deviceName)
{
  this->dev_proxy_ = 0;
  this->eventId_ = 0;

  this->dev_proxy_ = new Tango::DeviceProxy(deviceName.c_str());

  try {
    // Now we are going to search for attributes named
    // "BitDepth" or "Depth" in the device. If found, we will use
    // them as the bit depth of the image.
    static const char* possible_names[] = { "BitDepth", "Depth" };
    static const size_t total_names = sizeof(possible_names)/sizeof(char*);
    typedef std::vector< std::string > string_vector;
    
    std::auto_ptr< string_vector > attribute_list;
    attribute_list.reset(this->dev_proxy_->get_attribute_list());
    
    for (size_t n=0; n < total_names; ++n) {
      string_vector::const_iterator i, e = attribute_list->end();
      for (i = attribute_list->begin(); i != e; ++i) {
        int cmp_result;
#ifdef YAT_WIN32
        cmp_result = _stricmp(i->c_str(), possible_names[n]);
#else
        cmp_result = strcasecmp(i->c_str(), possible_names[n]);
#endif
        if (cmp_result == 0) {
          this->bit_depth_attribute_name_ = *i;
          return;
        }
      }
    }
  } catch (...) {
    YAT_LOG("Unknown error trying to determine if there's"
        " a bit depth attribute");
  }
}

IBASourceTango::~IBASourceTango()
{
   // Unsubscribe the events if it's subscribed
  if (this->eventId_ != 0)
    this->dev_proxy_->unsubscribe_event(this->eventId_);
  this->eventId_ = 0;
  delete this->dev_proxy_;
  this->dev_proxy_ = 0;
}

void IBASourceTango::push_event(Tango::EventData *ed)
{
  if(!ed->attr_value) {
    return;
  }
  
  ImageAndInfo imginf;
  this->get_image(imginf);
  this->process(imginf);
}


IBASourceTango::TangoYATException::TangoYATException( Tango::DevFailed& df )
{
  const Tango::DevErrorList& tango_err_list = df.errors;
  for (unsigned long i = 0; i < tango_err_list.length(); i++) 
  {
    Tango::ErrSeverity df_sev = df.errors[i].severity;
    yat::ErrorSeverity yat_sev = (df_sev == Tango::WARN ? yat::WARN
                                  : (df_sev == Tango::ERR ? yat::ERR
                                     : (df_sev == Tango::PANIC ? yat::PANIC
                                        : yat::ERR)));
    
    this->push_error( df.errors[i].reason,
                      df.errors[i].desc,
                      df.errors[i].origin,
                      -1,
                      yat_sev);
  }
}

/*virtual from IBASource*/
void IBASourceTango::get_image(ImageAndInfo & imginf) throw (yat::Exception)
{
  isl::Image *& image = imginf.image;
  image = 0;

  //-	Read the attribute
  //------------------------------------------
  std::auto_ptr< std::vector<Tango::DeviceAttribute> > attributes;
  Tango::DeviceAttribute* dev_attr = 0;
  int dim_x = 0;
  int dim_y = 0;
  const bool read_bit_depth = !this->bit_depth_attribute_name_.empty();
  
  try
  {
    std::vector<std::string> attribute_names;
    attribute_names.push_back(this->image_attribute_name());
    if (read_bit_depth) {
      attribute_names.push_back(this->bit_depth_attribute_name_);
    }
    
    //- Read the attribute contents
    attributes.reset(this->dev_proxy_->read_attributes(attribute_names));
    
    dev_attr = &((*attributes)[0]);
    
    dim_x = (*attributes)[0].get_dim_x();
    dim_y = (*attributes)[0].get_dim_y();

    //- just make a call to get_type() to test if the dev_attr is empty 
    //- if the attr is empty, a Tango::DevFailed will be thrown
    (*attributes)[0].get_type();
    
    if (read_bit_depth) {
      Tango::DeviceAttribute & depth_attr = (*attributes)[1];
      switch( depth_attr.get_type() ) {
         case Tango::DEV_LONG: {
            Tango::DevLong bd;
            depth_attr >> bd;
            imginf.bit_depth = bd;
            break;
         }
         case Tango::DEV_SHORT: {
            Tango::DevShort bd;
            depth_attr >> bd;
            imginf.bit_depth = bd;
            break;
         }
         case Tango::DEV_LONG64: {
            Tango::DevLong64 bd;
            depth_attr >> bd;
            imginf.bit_depth = bd;
            break;
         }
         case Tango::DEV_ULONG: {
            Tango::DevULong bd;
            depth_attr >> bd;
            imginf.bit_depth = bd;
            break;
         }
         case Tango::DEV_USHORT: {
            Tango::DevUShort bd;
            depth_attr >> bd;
            imginf.bit_depth = bd;
            break;
         }
         case Tango::DEV_ULONG64: {
            Tango::DevULong64 bd;
            depth_attr >> bd;
            imginf.bit_depth = bd;
            break;
         }
         default:
           imginf.bit_depth = 0;
      }
    } else {
      imginf.bit_depth = 0;
    }
  }
  catch (Tango::DevFailed &df)
  {
    TangoYATException ex(df);
    RETHROW_YAT_ERROR(ex,
                      "SOFTWARE_FAILURE",
                      "Error while getting remote image",
                      "ImgBeamAnalyzerTask::get_remote_image");
  }
  catch(...)
  {
    THROW_YAT_ERROR("UNKNOWN_ERROR",
                    "Error while getting remote image",
                    "ImgBeamAnalyzerTask::get_remote_image");
  }


  try
  {
    image = new isl::Image(dim_x, dim_y, isl::ISL_STORAGE_USHORT);
    if (image == 0)
      throw std::bad_alloc();
  }
  catch(std::bad_alloc &)
  {
    THROW_YAT_ERROR("OUT_OF_MEMORY",
                    "Allocation of isl::Image failed [std::bad_alloc]",
                    "ImgBeamAnalyzerTask::get_remote_image");
  }
  catch(isl::Exception & ex)
  {
    ISL2YATException yat_exc(ex);
    isl::ErrorHandler::reset();
    RETHROW_YAT_ERROR(yat_exc,
                      "SOFTWARE_FAILURE",
                      "Allocation of isl::Image failed [isl::Exception]",
                      "ImgBeamAnalyzerTask::get_remote_image");
  }
  catch(...)
  {
    THROW_YAT_ERROR("UNKNOWN_ERROR",
                    "Allocation of isl::Image failed [Unknown exception caught]",
                    "ImgBeamAnalyzerTask::get_remote_image");
  }

  switch(dev_attr->get_type())
  {
  case Tango::DEV_UCHAR:
    {
      Tango::DevVarUCharArray* serialized_image = 0;
      
      try
      {
        if ((*dev_attr >> serialized_image) == false)
        {
          SAFE_DELETE_PTR(image);
          THROW_YAT_ERROR("OUT_OF_MEMORY",
                          "Extraction of data from Tango::Attribute failed",
                          "ImgBeamAnalyzerTask::get_remote_image");
        }
      }
      catch(std::bad_alloc &)
      {
        SAFE_DELETE_PTR(image);
        THROW_YAT_ERROR("OUT_OF_MEMORY",
                        "Extraction of data from Tango::Attribute failed [std::bad_alloc]",
                        "ImgBeamAnalyzerTask::get_remote_image");
      }
      catch(Tango::DevFailed & df)
      {
        SAFE_DELETE_PTR(image);
        TangoYATException ex(df);
        RETHROW_YAT_ERROR(ex,
                          "SOFTWARE_FAILURE",
                          "Extraction of data from Tango::Attribute failed [Tango::DevFailed]",
                          "ImgBeamAnalyzerTask::get_remote_image");
      }
      catch(...)
      {
        SAFE_DELETE_PTR(image);
        THROW_YAT_ERROR("UNKNOWN_ERROR",
                        "Extraction of data from Tango::Attribute failed [unknown exception]",
                        "ImgBeamAnalyzerTask::get_remote_image");
      }

      isl::Image* uchar_image = 0;
      try
      {
        uchar_image = new isl::Image(dim_x, dim_y, isl::ISL_STORAGE_UCHAR);
        if (uchar_image == 0)
          throw std::bad_alloc();
      }
      catch(std::bad_alloc &)
      {
        THROW_YAT_ERROR("OUT_OF_MEMORY",
                        "Allocation of isl::Image failed [std::bad_alloc]",
                        "ImgBeamAnalyzerTask::get_remote_image");
      }
      catch(isl::Exception & ex)
      {
        ISL2YATException yat_exc(ex);
        isl::ErrorHandler::reset();
        RETHROW_YAT_ERROR(yat_exc,
                          "OUT_OF_MEMORY",
                          "Allocation of isl::Image failed [isl::Exception]",
                          "ImgBeamAnalyzerTask::get_remote_image");
      }
      catch(...)
      {
        THROW_YAT_ERROR("UNKNOWN_ERROR",
                        "Allocation of isl::Image failed [Tango::DevFailed]",
                        "ImgBeamAnalyzerTask::get_remote_image");
      }
      
      try
      {
        uchar_image->unserialize( serialized_image->get_buffer() );
        uchar_image->convert( *image );
      }
      catch(isl::Exception & ex)
      {
        ISL2YATException yat_exc(ex);
        isl::ErrorHandler::reset();
        RETHROW_YAT_ERROR(yat_exc,
                          "SOFTWARE_FAILURE",
                          "Unable to convert the UChar image to a UShort image",
                          "ImgBeamAnalyzerTask::get_remote_image");
      }
      catch(...)
      {
        THROW_YAT_ERROR("UNKNOWN_ERROR",
                        "Unable to convert the UChar image to a UShort image",
                        "ImgBeamAnalyzerTask::get_remote_image");
      }
      SAFE_DELETE_PTR(serialized_image);
      SAFE_DELETE_PTR(uchar_image);
    }
    break;
  case Tango::DEV_SHORT:
    {
      Tango::DevVarShortArray* serialized_image = 0;
      
      try
      {
        if ((*dev_attr >> serialized_image) == false)
        {
          SAFE_DELETE_PTR(image);
          THROW_YAT_ERROR("OUT_OF_MEMORY",
                          "Extraction of data from Tango::Attribute failed",
                          "ImgBeamAnalyzerTask::get_remote_image");
        }
      }
      catch(std::bad_alloc &)
      {
        SAFE_DELETE_PTR(image);
        THROW_YAT_ERROR("OUT_OF_MEMORY",
                        "Extraction of data from Tango::Attribute failed [std::bad_alloc]",
                        "ImgBeamAnalyzerTask::get_remote_image");
      }
      catch(Tango::DevFailed & df)
      {
        SAFE_DELETE_PTR(image);
        TangoYATException ex(df);
        RETHROW_YAT_ERROR(ex,
                          "SOFTWARE_FAILURE",
                          "Extraction of data from Tango::Attribute failed [Tango::DevFailed]",
                          "ImgBeamAnalyzerTask::get_remote_image");
      }
      catch(...)
      {
        SAFE_DELETE_PTR(image);
        THROW_YAT_ERROR("UNKNOWN_ERROR",
                        "Extraction of data from Tango::Attribute failed [unknown exception]",
                        "ImgBeamAnalyzerTask::get_remote_image");
      }

      try
      {
        image->unserialize(serialized_image->get_buffer());
      }
      catch(isl::Exception & ex)
      {
        ISL2YATException yat_exc(ex);
        isl::ErrorHandler::reset();
        RETHROW_YAT_ERROR(yat_exc,
                          "SOFTWARE_FAILURE",
                          "Unable to unserialize image",
                          "ImgBeamAnalyzerTask::get_remote_image");
      }
      catch(...)
      {
        THROW_YAT_ERROR("UNKNOWN_ERROR",
                        "Unable to unserialize image",
                        "ImgBeamAnalyzerTask::get_remote_image");
      }
      SAFE_DELETE_PTR(serialized_image);
     }
    break;
  case Tango::DEV_USHORT:
    {
      Tango::DevVarUShortArray* serialized_image = 0;
      
      try
      {
        if ((*dev_attr >> serialized_image) == false)
        {
          SAFE_DELETE_PTR(image);
          THROW_YAT_ERROR("OUT_OF_MEMORY",
                          "Extraction of data from Tango::Attribute failed",
                          "ImgBeamAnalyzerTask::get_remote_image");
        }
      }
      catch(std::bad_alloc &)
      {
        SAFE_DELETE_PTR(image);
        THROW_YAT_ERROR("OUT_OF_MEMORY",
                        "Extraction of data from Tango::Attribute failed [std::bad_alloc]",
                        "ImgBeamAnalyzerTask::get_remote_image");
      }
      catch(Tango::DevFailed & df)
      {
        SAFE_DELETE_PTR(image);
        TangoYATException ex(df);
        RETHROW_YAT_ERROR(ex,
                          "SOFTWARE_FAILURE",
                          "Extraction of data from Tango::Attribute failed [Tango::DevFailed]",
                          "ImgBeamAnalyzerTask::get_remote_image");
      }
      catch(...)
      {
        SAFE_DELETE_PTR(image);
        THROW_YAT_ERROR("UNKNOWN_ERROR",
                        "Extraction of data from Tango::Attribute failed [unknown exception]",
                        "ImgBeamAnalyzerTask::get_remote_image");
      }

      try
      {
        image->unserialize(serialized_image->get_buffer());
      }
      catch(isl::Exception & ex)
      {
        ISL2YATException yat_exc(ex);
        isl::ErrorHandler::reset();
        RETHROW_YAT_ERROR(yat_exc,
                          "SOFTWARE_FAILURE",
                          "Unable to unserialize image",
                          "ImgBeamAnalyzerTask::get_remote_image");
      }
      catch(...)
      {
        THROW_YAT_ERROR("UNKNOWN_ERROR",
                        "Unable to unserialize image",
                        "ImgBeamAnalyzerTask::get_remote_image");
      }
      SAFE_DELETE_PTR(serialized_image);
    }
    break;
  default:
    {
      SAFE_DELETE_PTR(image);
      THROW_YAT_ERROR("SOFTWARE_FAILURE",
                      "The remote attribute must be of type DEV_UCHAR, DEV_SHORT or DEV_USHORT",
                      "ImgBeamAnalyzerTask::get_remote_image");
     }
    break;
  }
}

/*virtual from IBASource*/
void IBASourceTango::set_callback_attribute(const std::string & attrName)
{
  // Unsubscribe the events if it's subscribed
  if (this->eventId_ != 0)
    this->dev_proxy_->unsubscribe_event(this->eventId_);
  this->eventId_ = 0;

  // New subscription
  std::vector<std::string> filters;
  this->eventId_ = this->dev_proxy_->subscribe_event( attrName,
                                                      Tango::CHANGE_EVENT,
                                                      this,
                                                      filters // unused
                                                      );
}
