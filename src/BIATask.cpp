// ============================================================================
//
// = CONTEXT
//    TANGO Project - ImgBeamAnalyzer DeviceServer - Data class
//
// = File
//    BIATask.cpp
//
// = AUTHOR
//    Julien Malik
//
// ============================================================================

// ============================================================================
// DEPENDENCIES
// ============================================================================
#include "BIATask.h"

#if !defined (__INLINE_IMPL__)
# include "BIATask.i"
#endif // __INLINE_IMPL___


namespace ImgBeamAnalyzer_ns
{

const char* kRUNNING_STATUS_MSG = "The device is up and running";
const char* kSTANDBY_STATUS_MSG = "The device is in standby";

BIATask::BIATask(Tango::DeviceImpl* _host_device, 
                 const BIAConfig&   _config,
                 bool               _dev_proxy_allowed,
                 const std::string& _image_dev_name,
                 const std::string& _image_attr_name,
                 bool               _auto_start,
                 int                _mode)

: adtb::DeviceTask(),
  Tango::LogAdapter(_host_device),
  dev_proxy_(0),
  dev_proxy_allowed_(_dev_proxy_allowed),
  image_dev_name_(_image_dev_name),
  image_attr_name_(_image_attr_name),
  mode_(_mode),
  data_(0),
  state_(Tango::RUNNING),
  status_(kRUNNING_STATUS_MSG),
  initialized_(false)
{
  if (this->mode_ == MODE_CONTINUOUS)
  {
    this->enable_periodic_msg(_auto_start);
    this->set_periodic_timeout(_config.comput_period);
  }

  this->set_state_status(Tango::STANDBY, kSTANDBY_STATUS_MSG);
}

BIATask::~BIATask()
{
}

void BIATask::handle_message (adtb::Message& _msg)
  throw (Tango::DevFailed)
{
	//- handle msg
  switch (_msg.type())
	{
	  //- THREAD_INIT ----------------------
	  case adtb::THREAD_INIT:
	    {
        this->initialized_ = false;

        isl::Errorhandler::init();

        if (this->dev_proxy_allowed_ == true)
        {
          try
          {
            this->dev_proxy_ = new Tango::DeviceProxy(this->image_dev_name_);
            if (this->dev_proxy_ == 0)
              throw std::bad_alloc();
          }
          catch (const std::bad_alloc&)
          {
            THROW_DEVFAILED("OUT_OF_MEMORY",
                            "DeviceProxy allocation failed",
                            "BIATask::handle_message");
          }
          catch (Tango::DevFailed& ex)
          {
            RETHROW_DEVFAILED(ex,
                              "OUT_OF_MEMORY",
                              "DeviceProxy allocation failed",
                              "BIATask::handle_message");
          }
          catch (...)
          {
            THROW_DEVFAILED("UNKNOWN_ERROR",
                            "DeviceProxy allocation failed",
                            "BIATask::handle_message");
          }
        }

        this->initialized_ = true;
      }
		  break;
		//- THREAD_EXIT ----------------------
		case adtb::THREAD_EXIT:
		  {
  			DEBUG_STREAM << "BIATask::handle_message::thread is quitting" << std::endl;
        SAFE_DELETE_PTR(this->dev_proxy_);
        this->initialized_ = false;
      }
			break;
		//- THREAD_PERIODIC -----------------
		case adtb::THREAD_PERIODIC:
		  {
  		  //DEBUG_STREAM << "BIATask::handle_message::task wokenup on timeout" << std::endl;
        
        if (this->initialized_ == false)
        {
          DEBUG_STREAM << "Not properly initialized : exiting..." << std::endl;
          return;
        }

        if (this->mode_ == MODE_ONESHOT)
        {
          DEBUG_STREAM << "Timeout messages are disabled in ONE SHOT mode" << std::endl;
          return;
        }

        this->set_state_status(Tango::RUNNING, kRUNNING_STATUS_MSG);

        //- get a new image and send a kMSG_PROCESS message with it.
        isl::Image* image = 0;

        try
        {
          image = this->get_remote_image();
        }
        catch(const Tango::DevFailed& ex)
        {
          ERROR_STREAM << ex << std::endl;
          this->set_state_status(Tango::FAULT, ex.errors[0].desc);
          return;
        }
        catch(...)
        {
          this->set_state_status(Tango::FAULT, "Unknown error while getting remote image");
          return;
        }

        adtb::Message* msg = 0;
        try
        {
          msg = new adtb::Message(kMSG_PROCESS);
          if (msg == 0)
            throw std::bad_alloc();
        }
        catch(...)
        {
          this->set_state_status(Tango::FAULT, "Allocation of a adtb::Message failed");
          SAFE_DELETE_PTR( image );
          return;
        }

        try
        {
          msg->attach_data(image);
        }
        catch (const Tango::DevFailed &ex)
        {
          ERROR_STREAM << ex << std::endl;
          this->set_state_status(Tango::FAULT, "Attaching data to a adtb::Message failed");
          SAFE_DELETE_PTR( image );
          SAFE_RELEASE( msg );
          return;
        }
        catch(...)
        {
          this->set_state_status(Tango::FAULT, "Attaching data to a adtb::Message failed");
          SAFE_DELETE_PTR( image );
          SAFE_RELEASE( msg );
          return;
        }

        //- post the message
        try
        {
          this->post(msg);
        }
	      catch (const Tango::DevFailed &ex)
        {
          ERROR_STREAM << ex << std::endl;
          this->set_state_status(Tango::FAULT, "The posting of a adtb::Message failed");
          SAFE_DELETE_PTR( msg ); //- will automatically delete the associated image
          return;
        }
        catch(...)
        {
          this->set_state_status(Tango::FAULT, "The posting of a adtb::Message failed");
          SAFE_DELETE_PTR( msg ); //- will automatically delete the associated image
          return;
        }
      }
		  break;
		case kMSG_PROCESS:
		  {
  		  DEBUG_STREAM << "BIATask::handle_message::handling PROCESS" << std::endl;

        if (this->mode_ == MODE_ONESHOT)
        {
          this->state_  = Tango::RUNNING;
          this->status_ = kRUNNING_STATUS_MSG;
        }

        isl::Image* image = 0;
        BIAData* data = 0;
        try
        {
          try
          {
            _msg.dettach_data(image);
          }
          catch(Tango::DevFailed& ex)
          {
            RETHROW_DEVFAILED(ex,
                              "SOFTWARE_FAILURE",
                              "Unable to dettach image from a PROCESS message",
                              "BIATask::handle_message");
          }
          catch(...)
          {
            THROW_DEVFAILED("UNKNOWN_ERROR",
                            "Unable to dettach image from a PROCESS message",
                            "BIATask::handle_message");
          }

          try
          {
            //- allocate the new Data
            try
            {
              data = new BIAData ();
              if (data == 0)
                throw std::bad_alloc ();
            }
            catch(...)
            {
              THROW_DEVFAILED("OUT_OF_MEMORY",
                              "Allocation of BIAData failed",
                              "BIATask::handle_message");
            }

            //- get the current config
            BIAConfig config;
            {
              adtb::DeviceMutexLock<> guard(this->config_mutex_);
              config = this->config_;
            }

            data->config = config;
            
            this->proc_.process(*image, config, *data);

            //- update the available data
            BIAData* last_data = this->data_;
            {
              adtb::DeviceMutexLock<> guard(this->data_mutex_);
              this->data_ = data;
            }
            if (last_data)
            {
              last_data->release();
              last_data = 0;
            }
          }
          catch(Tango::DevFailed& ex)
          {
            RETHROW_DEVFAILED(ex,
                              "SOFTWARE_FAILURE",
                              "Error when processing an image",
                              "BIATask::handle_message");
          }
          catch(...)
          {
            THROW_DEVFAILED("UNKNOWN_ERROR",
                            "Error when processing an image",
                            "BIATask::handle_message");
          }
          SAFE_DELETE_PTR(image);
        }
        catch(Tango::DevFailed& ex)
        {
          SAFE_RELEASE(data);
          SAFE_DELETE_PTR(image);
          if (this->mode_ == MODE_ONESHOT)
          {
            this->set_state_status(Tango::STANDBY, kSTANDBY_STATUS_MSG);
            throw;
          }
          else
          {
            this->set_state_status(Tango::FAULT, ex.errors[0].desc);
            return;
          }
        }
        catch(...)
        {
          SAFE_RELEASE(data);
          SAFE_DELETE_PTR(image);
          if (this->mode_ == MODE_ONESHOT)
          {
            this->set_state_status(Tango::STANDBY, kSTANDBY_STATUS_MSG);
            THROW_DEVFAILED("UNKNOWN_ERROR",
                            "Error when processing an image",
                            "BIATask::handle_message");
          }
          else
          {
            this->set_state_status(Tango::FAULT, "Unknown error while processing an image");
            return;
          }
        }
        if (this->mode_ == MODE_ONESHOT)
        {
          this->state_  = Tango::STANDBY;
          this->status_ = kSTANDBY_STATUS_MSG;
        }
  		}
  		break;
		case kMSG_START:
		  {
  		  DEBUG_STREAM << "BIATask::handle_message::handling START" << std::endl;
        this->enable_periodic_msg(true);
        //- the state/status will be updated when entering in the PERIODIC message handling routine
  		}
  		break;
		case kMSG_STOP:
		  {
  		  DEBUG_STREAM << "BIATask::handle_message::handling STOP" << std::endl;
        this->enable_periodic_msg(false);
        this->state_  = Tango::STANDBY;
        this->status_ = kSTANDBY_STATUS_MSG;
  		}
  		break;
		case kMSG_START_LEARN_NOISE:
		  {
  		  DEBUG_STREAM << "BIATask::handle_message::handling START_LEARN_NOISE" << std::endl;
        this->proc_.start_learn_noise();
  		}
  		break;
		case kMSG_STOP_LEARN_NOISE:
		  {
  		  DEBUG_STREAM << "BIATask::handle_message::handling STOP_LEARN_NOISE" << std::endl;
        this->proc_.stop_learn_noise();
  		}
  		break;
  	default:
  		DEBUG_STREAM << "BIATask::handle_message::unhandled msg type received"<< std::endl;
  		break;
	}

	//DEBUG_STREAM << "BIATask::handle_message::message_handler:msg " << _msg.to_string() << " successfully handled" << std::endl;
}


isl::Image* BIATask::get_remote_image()
  throw (Tango::DevFailed)
{
  isl::Image* image = 0;

  //-	Read the attribute
	//------------------------------------------
  Tango::DeviceAttribute dev_attr;
  int dim_x = 0;
  int dim_y = 0;
  
  try
  {
    dev_attr = this->dev_proxy_->read_attribute(this->image_attr_name_);
    dim_x = dev_attr.get_dim_x();
    dim_y = dev_attr.get_dim_y();

		//- just make a call to get_type() to test if the dev_attr is empty 
    //- if the attr is empty, a Tango::DevFailed will be thrown
    dev_attr.get_type();

  }
	catch (Tango::DevFailed &ex)
  {
    RETHROW_DEVFAILED(ex,
                      "SOFTWARE_FAILURE",
                      "Error while getting remote image",
                      "BIATask::get_remote_image");
  }
  catch(...)
  {
    THROW_DEVFAILED("UNKNOWN_ERROR",
                    "Error while getting remote image",
                    "BIATask::get_remote_image");
  }


  try
  {
    image = new isl::Image(dim_x, dim_y, isl::ISL_STORAGE_USHORT);
    if (image == 0)
      throw std::bad_alloc();
  }
  catch(std::bad_alloc &)
  {
    THROW_DEVFAILED("OUT_OF_MEMORY",
                    "Allocation of isl::Image failed [std::bad_alloc]",
                    "BIATask::get_remote_image");
  }
  catch(isl::Exception & ex)
  {
    Tango::DevFailed df = isl_to_tango_exception(ex);
    isl::ErrorHandler::reset();
    throw df;
  }
  catch(Tango::DevFailed & ex)
  {
    RETHROW_DEVFAILED(ex,
                      "SOFTWARE_FAILURE",
                      "Allocation of isl::Image failed [Tango::DevFailed]",
                      "BIATask::get_remote_image");
  }
  catch(...)
  {
    THROW_DEVFAILED("UNKNOWN_ERROR",
                    "Allocation of isl::Image failed [Tango::DevFailed]",
                    "BIATask::get_remote_image");
  }

  switch(dev_attr.get_type())
  {
  case Tango::DEV_UCHAR:
    {
      Tango::DevVarUCharArray* serialized_image = 0;
      
      try
      {
        if ((dev_attr >> serialized_image) == false)
        {
          SAFE_DELETE_PTR(image);
          THROW_DEVFAILED("OUT_OF_MEMORY",
                          "Extraction of data from Tango::Attribute failed",
                          "BIATask::get_remote_image");
        }
      }
      catch(std::bad_alloc &)
      {
        SAFE_DELETE_PTR(image);
        THROW_DEVFAILED("OUT_OF_MEMORY",
                        "Extraction of data from Tango::Attribute failed [std::bad_alloc]",
                        "BIATask::get_remote_image");
      }
      catch(Tango::DevFailed & ex)
      {
        SAFE_DELETE_PTR(image);
        RETHROW_DEVFAILED(ex,
                          "SOFTWARE_FAILURE",
                          "Extraction of data from Tango::Attribute failed [Tango::DevFailed]",
                          "BIATask::get_remote_image");
      }
      catch(...)
      {
        SAFE_DELETE_PTR(image);
        THROW_DEVFAILED("UNKNOWN_ERROR",
                        "Extraction of data from Tango::Attribute failed [unknown exception]",
                        "BIATask::get_remote_image");
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
        THROW_DEVFAILED("OUT_OF_MEMORY",
                        "Allocation of isl::Image failed [std::bad_alloc]",
                        "BIATask::get_remote_image");
      }
      catch(isl::Exception & ex)
      {
        Tango::DevFailed df = isl_to_tango_exception(ex);
        isl::ErrorHandler::reset();
        throw df;
      }
      catch(Tango::DevFailed & ex)
      {
        RETHROW_DEVFAILED(ex,
                          "SOFTWARE_FAILURE",
                          "Allocation of isl::Image failed [Tango::DevFailed]",
                          "BIATask::get_remote_image");
      }
      catch(...)
      {
        THROW_DEVFAILED("UNKNOWN_ERROR",
                        "Allocation of isl::Image failed [Tango::DevFailed]",
                        "BIATask::get_remote_image");
      }
      
      try
      {
        uchar_image->unserialize( serialized_image->get_buffer() );
        uchar_image->convert( *image );
      }
      catch(isl::Exception & ex)
      {
        Tango::DevFailed df = isl_to_tango_exception(ex);
        isl::ErrorHandler::reset();
        RETHROW_DEVFAILED(df,
                          "SOFTWARE_FAILURE",
                          "Unable to convert the UChar image to a UShort image",
                          "BIATask::get_remote_image");
      }
      catch(...)
      {
        THROW_DEVFAILED("UNKNOWN_ERROR",
                        "Unable to convert the UChar image to a UShort image",
                        "BIATask::get_remote_image");
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
        if ((dev_attr >> serialized_image) == false)
        {
          SAFE_DELETE_PTR(image);
          THROW_DEVFAILED("OUT_OF_MEMORY",
                          "Extraction of data from Tango::Attribute failed",
                          "BIATask::get_remote_image");
        }
      }
      catch(std::bad_alloc &)
      {
        SAFE_DELETE_PTR(image);
        THROW_DEVFAILED("OUT_OF_MEMORY",
                        "Extraction of data from Tango::Attribute failed [std::bad_alloc]",
                        "BIATask::get_remote_image");
      }
      catch(Tango::DevFailed & ex)
      {
        SAFE_DELETE_PTR(image);
        RETHROW_DEVFAILED(ex,
                          "SOFTWARE_FAILURE",
                          "Extraction of data from Tango::Attribute failed [Tango::DevFailed]",
                          "BIATask::get_remote_image");
      }
      catch(...)
      {
        SAFE_DELETE_PTR(image);
        THROW_DEVFAILED("UNKNOWN_ERROR",
                        "Extraction of data from Tango::Attribute failed [unknown exception]",
                        "BIATask::get_remote_image");
      }

      try
      {
        image->unserialize(serialized_image->get_buffer());
      }
      catch(isl::Exception & ex)
      {
        Tango::DevFailed df = isl_to_tango_exception(ex);
        isl::ErrorHandler::reset();
        RETHROW_DEVFAILED(df,
                          "SOFTWARE_FAILURE",
                          "Unable to unserialize image",
                          "BIATask::get_remote_image");
      }
      catch(...)
      {
        THROW_DEVFAILED("UNKNOWN_ERROR",
                        "Unable to unserialize image",
                        "BIATask::get_remote_image");
      }
      SAFE_DELETE_PTR(serialized_image);
     }
    break;
  case Tango::DEV_USHORT:
    {
      Tango::DevVarUShortArray* serialized_image = 0;
      
      try
      {
        if ((dev_attr >> serialized_image) == false)
        {
          SAFE_DELETE_PTR(image);
          THROW_DEVFAILED("OUT_OF_MEMORY",
                          "Extraction of data from Tango::Attribute failed",
                          "BIATask::get_remote_image");
        }
      }
      catch(std::bad_alloc &)
      {
        SAFE_DELETE_PTR(image);
        THROW_DEVFAILED("OUT_OF_MEMORY",
                        "Extraction of data from Tango::Attribute failed [std::bad_alloc]",
                        "BIATask::get_remote_image");
      }
      catch(Tango::DevFailed & ex)
      {
        SAFE_DELETE_PTR(image);
        RETHROW_DEVFAILED(ex,
                          "SOFTWARE_FAILURE",
                          "Extraction of data from Tango::Attribute failed [Tango::DevFailed]",
                          "BIATask::get_remote_image");
      }
      catch(...)
      {
        SAFE_DELETE_PTR(image);
        THROW_DEVFAILED("UNKNOWN_ERROR",
                        "Extraction of data from Tango::Attribute failed [unknown exception]",
                        "BIATask::get_remote_image");
      }

      try
      {
        image->unserialize(serialized_image->get_buffer());
      }
      catch(isl::Exception & ex)
      {
        Tango::DevFailed df = isl_to_tango_exception(ex);
        isl::ErrorHandler::reset();
        RETHROW_DEVFAILED(df,
                          "SOFTWARE_FAILURE",
                          "Unable to unserialize image",
                          "BIATask::get_remote_image");
      }
      catch(...)
      {
        THROW_DEVFAILED("UNKNOWN_ERROR",
                        "Unable to unserialize image",
                        "BIATask::get_remote_image");
      }
      SAFE_DELETE_PTR(serialized_image);
    }
    break;
  default:
    {
      SAFE_DELETE_PTR(image);
      THROW_DEVFAILED("SOFTWARE_FAILURE",
                      "The remote attribute must be of type DEV_UCHAR, DEV_SHORT or DEV_USHORT",
                      "BIATask::get_remote_image");
     }
    break;
  }

  return(image);
}

// ============================================================================
// BIATask::isl_to_tango_exception
// ============================================================================
Tango::DevFailed BIATask::isl_to_tango_exception (const isl::Exception& e)
{
  Tango::DevErrorList error_list;

  error_list.length(e.errors.size());

  for (size_t i = 0; i < e.errors.size(); i++) 
  {
    error_list[i].reason   = CORBA::string_dup(e.errors[i].reason.c_str());
    error_list[i].desc     = CORBA::string_dup(e.errors[i].description.c_str());
    error_list[i].origin   = CORBA::string_dup(e.errors[i].origin.c_str());
    error_list[i].severity = Tango::ERR;
  }

  return Tango::DevFailed(error_list);
}


}
