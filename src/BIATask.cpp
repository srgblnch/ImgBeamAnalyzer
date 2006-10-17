// ============================================================================
//
// = CONTEXT
//    TANGO Project - ImgBeamAnalyzer DeviceServer - Data class
//
// = File
//    Data.cpp
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

: adtb::DeviceTask(_host_device),
  dev_proxy_(0),
  dev_proxy_allowed_(_dev_proxy_allowed),
  image_dev_name_(_image_dev_name),
  image_attr_name_(_image_attr_name),
  mode_(_mode),
  proc_(_host_device, *this, _config),
  state_(Tango::RUNNING),
  status_(kRUNNING_STATUS_MSG),
  initialized_(false)
{
  if (this->mode_ == MODE_CONTINUOUS)
  {
    this->enable_periodic_msg(_auto_start);
    this->set_periodic_timeout(_config.comput_period);

		if (_auto_start == false)
    {
  		this->state_ = Tango::STANDBY;
      this->status_ = kSTANDBY_STATUS_MSG;
    }
  }
  else if (this->mode_ == MODE_ONESHOT)
  {
    //- the "normal" state in ONE SHOT mode is STANDBY
    this->state_ = Tango::STANDBY;
    this->status_ = kSTANDBY_STATUS_MSG;
  }
}

BIATask::~BIATask()
{
}

void BIATask::fault(const char* description)
{
  this->state_  = Tango::FAULT;
  this->status_ = description;
  ERROR_STREAM << description << std::endl;
}

void BIATask::clear_error()
{
  //- this function is only used in the CONTINUOUS_MODE, before processing a new image.
  //- it resets the internal state of the task
  //- any error that can happen during the processing is reported by setting the state to FAULT
  this->state_  = Tango::RUNNING;
  this->status_ = kRUNNING_STATUS_MSG;
}

BIAData* BIATask::get_data (void)
  throw (Tango::DevFailed)
{
  //- enter critical section
  adtb::DeviceMutexLock guard(this->lock_);

  if (this->mode_ == MODE_CONTINUOUS && this->state_ == Tango::STANDBY)
    return(0);
  else
    return( this->proc_.get_data() );
}


const BIAConfig& BIATask::get_config(void)
{
  //- enter critical section
  adtb::DeviceMutexLock guard(this->lock_);

  return( this->proc_.get_config() );
}


void BIATask::configure(const BIAConfig& config)
{
  DEBUG_TRACE("BIATask::configure");
  
  //- try to send a CONFIG message
  adtb::Message* msg = 0;
  try
  {
	  msg = new adtb::Message(kMSG_CONFIG);
	  if (msg == 0)
		  throw std::bad_alloc();
  }
  catch(...)
  {
	  Tango::Except::throw_exception (_CPTC ("OUT_OF_MEMORY"),
																	  _CPTC ("Error while creating a CONFIG message"),
																	  _CPTC ("BIATask::configure"));
  }


  BIAConfig* sent_config = 0;
  try
  {
	  sent_config = new BIAConfig(config);
	  if (sent_config == 0)
		  throw std::bad_alloc();
  }
  catch (const std::bad_alloc &)
  {
	  SAFE_DELETE( msg );
	  Tango::Except::throw_exception (_CPTC ("OUT_OF_MEMORY"),
																	  _CPTC ("Error while creating a CONFIG instance"),
																	  _CPTC ("BIATask::configure"));
  }
  catch(...)
  {
	  SAFE_DELETE( msg );
	  Tango::Except::throw_exception (_CPTC ("UNKNOWN_ERROR"),
																	  _CPTC ("Error while creating a CONFIG instance"),
																	  _CPTC ("BIATask::configure"));
  }

  try
  {
	  msg->attach_data(sent_config);
  }
  catch (const Tango::DevFailed &ex)
  {
	  SAFE_DELETE( sent_config );
	  SAFE_DELETE( msg );
	  Tango::Except::re_throw_exception (const_cast<Tango::DevFailed &>(ex),
																		   _CPTC ("SOFTWARE_FAILURE"),
																		   _CPTC ("Error while attaching the Config instance to the CONFIG message"),
																		   _CPTC ("BIATask::configure"));
  }
  catch(...)
  {
	  SAFE_DELETE( sent_config );
	  SAFE_DELETE( msg );
	  Tango::Except::throw_exception (_CPTC ("SOFTWARE_FAILURE"),
																	  _CPTC ("Error while attaching the Config instance to the CONFIG message"),
																	  _CPTC ("BIATask::configure"));
  }


  //- post the message
  try
  {
	  this->post(msg);
  }
  catch (const Tango::DevFailed &ex)
  {
	  SAFE_DELETE( msg ); //- will automatically delete the associated sent_config instance
	  Tango::Except::re_throw_exception (const_cast<Tango::DevFailed &>(ex),
																		   _CPTC ("SOFTWARE_FAILURE"),
																		   _CPTC ("Error while posting the CONFIG message"),
																		   _CPTC ("BIATask::configure"));
  }
  catch(...)
  {
	  SAFE_DELETE( msg ); //- will automatically delete the associated sent_config instance
	  Tango::Except::throw_exception (_CPTC ("SOFTWARE_FAILURE"),
																	  _CPTC ("Error while posting the CONFIG message"),
																	  _CPTC ("BIATask::configure"));
  }
}


void BIATask::handle_message (const adtb::Message& _msg)
  throw (Tango::DevFailed)
{

  //- The DeviceTask's lock_ is locked
	//-------------------------------------------------------------------
  DEBUG_TRACE("BIATask::handle_message::message_handler");
	DEBUG_STREAM << "BIATask::handle_message::receiving msg " << _msg.to_string() << std::endl;

	//- handle msg
  switch (_msg.type())
	{
	  //- THREAD_INIT ----------------------
	  case adtb::THREAD_INIT:
	    {
        //- Create the DeviceProxy
        this->initialized_ = false;

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
            this->fault("Allocation error during DeviceProxy initialization");
            return;
          }
          catch (const Tango::DevFailed& ex)
          {
            ERROR_STREAM << ex << std::endl;
            this->fault("Allocation error during DeviceProxy initialization");
            return;
          }
          catch (...)
          {
            this->fault("Allocation error during DeviceProxy initialization");
            return;
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
  		  DEBUG_STREAM << "BIATask::handle_message::task wokenup on timeout" << std::endl;
        
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

        this->clear_error();

        //- get a new image and send a kMSG_PROCESS message with it.
        isl::Image* image = 0;
        image = this->get_remote_image();

        if (image == 0)
          return;

        adtb::Message* msg = 0;
        try
        {
          msg = new adtb::Message(kMSG_PROCESS);
          if (msg == 0)
            throw std::bad_alloc();
        }
        catch(...)
        {
	        this->fault("Allocation of a adtb::Message failed");
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
	        this->fault("Attaching data to a adtb::Message failed");
          SAFE_DELETE_PTR( image );
          SAFE_DELETE_PTR( msg );
          return;
        }
        catch(...)
        {
	        this->fault("Attaching data to a adtb::Message failed");
          SAFE_DELETE_PTR( image );
          SAFE_DELETE_PTR( msg );
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
          this->fault("The posting of a adtb::Message failed");
          SAFE_DELETE_PTR( msg ); //- will automatically delete the associated image
          return;
        }
        catch(...)
        {
          this->fault("The posting of a adtb::Message failed");
          SAFE_DELETE_PTR( msg ); //- will automatically delete the associated image
          return;
        }
      }
		  break;
		//- USER_DEFINED_MSG -----------------
		case kMSG_CONFIG:
		  {
  		  DEBUG_STREAM << "BIATask::handle_message::handling kMSG_CONFIG" << std::endl;

        BIAConfig* config = 0;
        try
        {
          _msg.dettach_data(config);
        }
        catch(const Tango::DevFailed& ex)
        {
          ERROR_STREAM << ex << std::endl;
	        this->fault("Error during configuration [Tango exception caught when dettaching config from CONFIG message]");
        }
        catch(...)
        {
	        this->fault("Error during configuration [Unknown exception caught when dettaching config from CONFIG message]");
        }

        this->proc_.configure(*config);
        this->set_periodic_timeout(config->comput_period);
        SAFE_DELETE_PTR(config);
  		}
  		break;
		case kMSG_PROCESS:
		  {
  		  DEBUG_STREAM << "BIATask::handle_message::handling kMSG_PROCESS" << std::endl;

        if (this->mode_ == MODE_ONESHOT)
        {
          this->state_  = Tango::RUNNING;
          this->status_ = kRUNNING_STATUS_MSG;
        }

        isl::Image* image = 0;
        try
        {
          _msg.dettach_data(image);
        }
        catch(const Tango::DevFailed& ex)
        {
          ERROR_STREAM << ex << std::endl;
	        this->fault("Error during image processing [Tango exception caught when dettaching image from PROCESS message]");
        }
        catch(...)
        {
	        this->fault("Error during image processing [Unknown exception caught when dettaching image from PROCESS message]");
        }

        this->proc_.process(*image);
        SAFE_DELETE_PTR(image);

        if (this->mode_ == MODE_ONESHOT)
        {
          this->state_  = Tango::STANDBY;
          this->status_ = kSTANDBY_STATUS_MSG;
        }

  		}
  		break;
		case kMSG_START:
		  {
  		  DEBUG_STREAM << "BIATask::handle_message::handling kMSG_START" << std::endl;
        this->enable_periodic_msg(true);
        //- the state/status will be updated when entering in the PERIODIC message handling routine
  		}
  		break;
		case kMSG_STOP:
		  {
  		  DEBUG_STREAM << "BIATask::handle_message::handling kMSG_STOP" << std::endl;
        this->enable_periodic_msg(false);
        this->state_  = Tango::STANDBY;
        this->status_ = kSTANDBY_STATUS_MSG;
  		}
  		break;
		case kMSG_START_LEARN_NOISE:
		  {
  		  DEBUG_STREAM << "BIATask::handle_message::handling kMSG_START_LEARN_NOISE" << std::endl;
        this->proc_.start_learn_noise();
  		}
  		break;
		case kMSG_STOP_LEARN_NOISE:
		  {
  		  DEBUG_STREAM << "BIATask::handle_message::handling kMSG_STOP_LEARN_NOISE" << std::endl;
        this->proc_.stop_learn_noise();
  		}
  		break;
  	default:
  		DEBUG_STREAM << "BIATask::handle_message::unhandled msg type received"<< std::endl;
  		break;
	}

	DEBUG_STREAM << "BIATask::handle_message::message_handler:msg " << _msg.to_string() << " successfully handled" << std::endl;
}


isl::Image* BIATask::get_remote_image(bool throws)
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

		//- try to get the attribute type to test if the dev_attr is empty 
    //- : if the attr is empty, an exception will be thrown
    dev_attr.get_type();

  }
	catch (Tango::ConnectionFailed &ex)
  {
    if (throws)
    {
      Tango::Except::re_throw_exception (ex,
                                         _CPTC ("SOFTWARE_FAILURE"),
                                         _CPTC ("Error while getting remote image"),
                                         _CPTC ("BIATask::get_remote_image"));
    }
    else
    {
      this->fault("Tango::ConnectionFailed exception caught while reading remote attribute");
      return(0);
    }
  }
	catch (Tango::CommunicationFailed &ex)
  {
    if (throws)
    {
      Tango::Except::re_throw_exception (ex,
                                         _CPTC ("SOFTWARE_FAILURE"),
                                         _CPTC ("Error while getting remote image"),
                                         _CPTC ("BIATask::get_remote_image"));
    }
    else
    {
      this->fault("Tango::CommunicationFailed exception caught while reading remote attribute");
      return(0);
    }
  }
	catch (Tango::DevFailed &ex)
  {
    if (throws)
    {
      Tango::Except::re_throw_exception (ex,
                                         _CPTC ("SOFTWARE_FAILURE"),
                                         _CPTC ("Error while getting remote image"),
                                         _CPTC ("BIATask::get_remote_image"));
    }
    else
    {
      this->fault("Tango::DevFailed exception caught while reading remote attribute");
      return(0);
    }
  }
  catch(...)
  {
    if (throws)
    {
      Tango::Except::throw_exception (_CPTC ("SOFTWARE_FAILURE"),
                                      _CPTC ("Error while getting remote image"),
                                      _CPTC ("BIATask::get_remote_image"));
    }
    else
    {
      this->fault("Unknown exception caught while reading remote attribute");
      return(0);
    }
  }


  try
  {
    image = new isl::Image(dim_x, dim_y, isl::ISL_STORAGE_USHORT);
    if (image == 0)
      throw std::bad_alloc();
  }
  catch(std::bad_alloc &)
  {
    if (throws)
    {
      Tango::Except::throw_exception (_CPTC ("OUT_OF_MEMORY"),
                                      _CPTC ("Allocation of isl::Image failed [std::bad_alloc]"),
                                      _CPTC ("BIATask::get_remote_image"));
    }
    else
    {
      this->fault("An allocation of isl::Image failed while reading remote attribute [std::bad_alloc]");
      return(0);
    }
  }
  catch(isl::Exception & ex)
  {
    ERROR_STREAM << ex;
    isl::ErrorHandler::reset();
    if (throws)
    {
      Tango::Except::throw_exception (_CPTC ("OUT_OF_MEMORY"),
                                      _CPTC ("Allocation of isl::Image failed [isl::Exception]"),
                                      _CPTC ("BIATask::get_remote_image"));
    }
    else
    {
      this->fault("An allocation of isl::Image failed while reading remote attribute [isl::Exception]");
      return(0);
    }
  }
  catch(Tango::DevFailed & df)
  {
    if (throws)
    {
      Tango::Except::re_throw_exception (df,
                                         _CPTC ("OUT_OF_MEMORY"),
                                         _CPTC ("Allocation of isl::Image failed"),
                                         _CPTC ("BIATask::get_remote_image"));
    }
    else
    {
      this->fault("An allocation of isl::Image failed while reading remote attribute [Tango::DevFailed]");
      return(0);
    }
  }
  catch(...)
  {
    if (throws)
    {
      Tango::Except::throw_exception (_CPTC ("OUT_OF_MEMORY"),
                                      _CPTC ("Allocation of isl::Image failed [unknown exception]"),
                                      _CPTC ("BIATask::get_remote_image"));
    }
    else
    {
      this->fault("An allocation of isl::Image failed while reading remote attribute [unknown exception]");
      return(0);
    }
  }


  int attr_elem_size = 0;

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
          if (throws)
          {
            Tango::Except::throw_exception (_CPTC ("SOFTWARE_FAILURE"),
                                            _CPTC ("Extraction of data from remote attribute failed"),
                                            _CPTC ("BIATask::get_remote_image"));
          }
          else
          {
            this->fault("Extraction of data from remote attribute failed");
            return(0);
          }
        }
      }
      catch(std::bad_alloc &)
      {
        SAFE_DELETE_PTR(image);
        if (throws)
        {
          Tango::Except::throw_exception (_CPTC ("SOFTWARE_FAILURE"),
                                          _CPTC ("Extraction of data from remote attribute failed [std::bad_alloc]"),
                                          _CPTC ("BIATask::get_remote_image"));
        }
        else
        {
          this->fault("Extraction of data from remote attribute failed [std::bad_alloc]");
          return(0);
        }
      }
      catch(Tango::DevFailed & ex)
      {
        SAFE_DELETE_PTR(image);
        if (throws)
        {
          Tango::Except::re_throw_exception (ex,
                                             _CPTC ("SOFTWARE_FAILURE"),
                                             _CPTC ("Extraction of data from remote attribute failed [Tango::DevFailed]"),
                                             _CPTC ("BIATask::get_remote_image"));
        }
        else
        {
          this->fault("Extraction of data from remote attribute failed [Tango::DevFailed]");
          return(0);
        }
      }
      catch(...)
      {
        SAFE_DELETE_PTR(image);
        if (throws)
        {
          Tango::Except::throw_exception (_CPTC ("SOFTWARE_FAILURE"),
                                          _CPTC ("Extraction of data from remote attribute failed [unknown exception]"),
                                          _CPTC ("BIATask::get_remote_image"));
        }
        else
        {
          this->fault("Extraction of data from remote attribute failed [unknown exception]");
          return(0);
        }
      }

      Tango::DevUChar* attr_data = reinterpret_cast<Tango::DevUChar*>(serialized_image->get_buffer());
      attr_elem_size = sizeof(Tango::DevUChar);     
      Tango::DevUShort* dst_image_base = static_cast<Tango::DevUShort*>(image->data());

      for (int j = 0; j < dim_y; j++)
      {
        Tango::DevUShort* current_line = dst_image_base + j * image->row_byte_step() / sizeof(Tango::DevUShort);
        for (int i = 0; i < dim_x; i++)
        {
          *current_line++ = static_cast<Tango::DevUShort>(*attr_data++);
        }
      }
      SAFE_DELETE_PTR(serialized_image);
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
          if (throws)
          {
            Tango::Except::throw_exception (_CPTC ("SOFTWARE_FAILURE"),
                                            _CPTC ("Extraction of data from remote attribute failed"),
                                            _CPTC ("BIATask::get_remote_image"));
          }
          else
          {
            this->fault("Extraction of data from remote attribute failed");
            return(0);
          }
        }
      }
      catch(std::bad_alloc &)
      {
        SAFE_DELETE_PTR(image);
        if (throws)
        {
          Tango::Except::throw_exception (_CPTC ("SOFTWARE_FAILURE"),
                                          _CPTC ("Extraction of data from remote attribute failed [std::bad_alloc]"),
                                          _CPTC ("BIATask::get_remote_image"));
        }
        else
        {
          this->fault("Extraction of data from remote attribute failed [std::bad_alloc]");
          return(0);
        }
      }
      catch(Tango::DevFailed & ex)
      {
        SAFE_DELETE_PTR(image);
        if (throws)
        {
          Tango::Except::re_throw_exception (ex,
                                             _CPTC ("SOFTWARE_FAILURE"),
                                             _CPTC ("Extraction of data from remote attribute failed [Tango::DevFailed]"),
                                             _CPTC ("BIATask::get_remote_image"));
        }
        else
        {
          this->fault("Extraction of data from remote attribute failed [Tango::DevFailed]");
          return(0);
        }
      }
      catch(...)
      {
        SAFE_DELETE_PTR(image);
        if (throws)
        {
          Tango::Except::throw_exception (_CPTC ("SOFTWARE_FAILURE"),
                                          _CPTC ("Extraction of data from remote attribute failed [unknown exception]"),
                                          _CPTC ("BIATask::get_remote_image"));
        }
        else
        {
          this->fault("Extraction of data from remote attribute failed [unknown exception]");
          return(0);
        }
      }

      char* attr_data = reinterpret_cast<char*>(serialized_image->get_buffer());
      attr_elem_size = sizeof(Tango::DevShort);
      
      //- extract data to the internal isl::Image
      char* dst_image_base = static_cast<char*>(image->data());
      char* src_image_base = attr_data;
      int src_image_row_step = dim_x * attr_elem_size;
      int dst_image_row_step = image->row_byte_step();

      for (int j = 0; j < dim_y; j++)
      {
        ::memcpy(dst_image_base, src_image_base, src_image_row_step);
        dst_image_base += dst_image_row_step;
        src_image_base += src_image_row_step;
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
          if (throws)
          {
            Tango::Except::throw_exception (_CPTC ("SOFTWARE_FAILURE"),
                                            _CPTC ("Extraction of data from remote attribute failed"),
                                            _CPTC ("BIATask::get_remote_image"));
          }
          else
          {
            this->fault("Extraction of data from remote attribute failed");
            return(0);
          }
        }
      }
      catch(std::bad_alloc &)
      {
        SAFE_DELETE_PTR(image);
        if (throws)
        {
          Tango::Except::throw_exception (_CPTC ("SOFTWARE_FAILURE"),
                                          _CPTC ("Extraction of data from remote attribute failed [std::bad_alloc]"),
                                          _CPTC ("BIATask::get_remote_image"));
        }
        else
        {
          this->fault("Extraction of data from remote attribute failed [std::bad_alloc]");
          return(0);
        }
      }
      catch(Tango::DevFailed & ex)
      {
        SAFE_DELETE_PTR(image);
        if (throws)
        {
          Tango::Except::re_throw_exception (ex,
                                             _CPTC ("SOFTWARE_FAILURE"),
                                             _CPTC ("Extraction of data from remote attribute failed [Tango::DevFailed]"),
                                             _CPTC ("BIATask::get_remote_image"));
        }
        else
        {
          this->fault("Extraction of data from remote attribute failed [Tango::DevFailed]");
          return(0);
        }
      }
      catch(...)
      {
        SAFE_DELETE_PTR(image);
        if (throws)
        {
          Tango::Except::throw_exception (_CPTC ("SOFTWARE_FAILURE"),
                                          _CPTC ("Extraction of data from remote attribute failed [unknown exception]"),
                                          _CPTC ("BIATask::get_remote_image"));
        }
        else
        {
          this->fault("Extraction of data from remote attribute failed [unknown exception]");
          return(0);
        }
      }

      char* attr_data = reinterpret_cast<char*>(serialized_image->get_buffer());
      attr_elem_size = sizeof(Tango::DevUShort);

      //- extract data to the internal isl::Image
      char* dst_image_base = static_cast<char*>(image->data());
      char* src_image_base = attr_data;
      int src_image_row_step = dim_x * attr_elem_size;
      int dst_image_row_step = image->row_byte_step();

      for (int j = 0; j < dim_y; j++)
      {
        ::memcpy(dst_image_base, src_image_base, src_image_row_step);
        dst_image_base += dst_image_row_step;
        src_image_base += src_image_row_step;
      }

      SAFE_DELETE_PTR(serialized_image);
    }
    break;
  default:
    {
      SAFE_DELETE_PTR(image);
      if (throws)
      {
        Tango::Except::throw_exception (_CPTC ("SOFTWARE_FAILURE"),
                                        _CPTC ("The remote attribute must be of type DEV_UCHAR, DEV_SHORT or DEV_USHORT"),
                                        _CPTC ("BIATask::get_remote_image"));
      }
      else
      {
        this->fault("The remote attribute must be of type DEV_UCHAR, DEV_SHORT or DEV_USHORT");
        return(0);
      }
    }
    break;
  }

  return(image);
}



}
