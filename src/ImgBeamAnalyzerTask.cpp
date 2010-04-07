// ============================================================================
//
// = CONTEXT
//    TANGO Project - ImgBeamAnalyzer DeviceServer - Data class
//
// = File
//    ImgBeamAnalyzerTask.cpp
//
// = AUTHOR
//    Julien Malik
//
// ============================================================================

// ============================================================================
// DEPENDENCIES
// ============================================================================
#include "ImgBeamAnalyzerTask.h"
#include <yat4tango/ExceptionHelper.h>
#include <memory>

#if !defined (YAT_INLINE_IMPL)
# include "ImgBeamAnalyzerTask.i"
#endif // YAT_INLINE_IMPL


namespace ImgBeamAnalyzer_ns
{

const char* kINIT_STATUS_MSG    = "The device is initializing";
const char* kRUNNING_STATUS_MSG = "The device is up and running";
const char* kSTANDBY_STATUS_MSG = "The device is in standby";

ImgBeamAnalyzerTask::ImgBeamAnalyzerTask()
: yat::Task(),
  mode_(MODE_CONTINUOUS),
  data_(new BIAData()),
  state_(INIT),
  status_(kINIT_STATUS_MSG),
  initialized_(false)
{

}

ImgBeamAnalyzerTask::~ImgBeamAnalyzerTask()
{
}

void ImgBeamAnalyzerTask::handle_message (yat::Message& _msg)
  throw (yat::Exception)
{
  ImageAndInfo imginf;
  switch (_msg.type())
  {
    //- TASK_INIT ----------------------
    case yat::TASK_INIT:
      {
        this->initialized_ = false;

        try
        {
          ImgBeamAnalyzerInit* init_config = 0;
          try
          {
            _msg.detach_data(init_config);
          }
          catch( yat::Exception& ex )
          {
            RETHROW_YAT_ERROR( ex,
                               "SOFTWARE_FAILURE",
                               "Unable to detach data from the TASK_INIT msg",
                               "ImgBeamAnalyzerTask::handle_message");
          }
          catch(...)
          {
            THROW_YAT_ERROR("UNKNOWN_ERROR",
                            "Unable to detach data from the TASK_INIT msg",
                            "ImgBeamAnalyzerTask::handle_message");
          }

          //- declare a std::auto_ptr to be sure 'init_config' will be deleted even in case an exception is thrown
          std::auto_ptr<ImgBeamAnalyzerInit> init_config_ptr(init_config);

          try
          {
            this->init(*init_config);
          }
          catch( yat::Exception& ex )
          {
            RETHROW_YAT_ERROR(ex,
                              "SOFTWARE_FAILURE",
                              "Initialization of task failed",
                              "ImgBeamAnalyzerTask::handle_message");
          }
          catch(...)
          {
            THROW_YAT_ERROR("UNKNOWN_ERROR",
                            "Initialization of task failed",
                            "ImgBeamAnalyzerTask::handle_message");
          }

          this->set_state_status(STANDBY, kSTANDBY_STATUS_MSG);
          this->initialized_ = true;
        }
        catch ( yat::Exception& ex )
        {
          yat::OSStream os;
          os << "Error during initialization ["
             << ex.errors[0].desc
             << "]";
          this->set_state_status(FAULT, os.str());
          this->initialized_ = false;

          throw ex;
        }
      }
      break;
    //- THREAD_EXIT ----------------------
    case yat::TASK_EXIT:
      {
        YAT_LOG("thread is quitting");
        SAFE_RELEASE( this->data_ );
        this->initialized_ = false;
      }
      break;
    //- THREAD_PERIODIC -----------------
    case yat::TASK_PERIODIC:
      {
        YAT_LOG("Task wokenup on timeout");
        if (this->initialized_ == false)
        {
          //- should never happen, but just in case, the device will do nothing
          YAT_LOG("Not propery initialized: exiting...");
          return;
        }

        if (this->mode_ == MODE_ONESHOT)
        {
          //- should never happen, but just in case, the device will do nothing
          YAT_LOG("Timeout messages are disabled in ONE SHOT mode");
          return;
        }

        try
        {
          //- get a new image and send a kMSG_PROCESS message with it.
          try
          {
            this->get_img_callback_( imginf );
          }
          catch(yat::Exception& ex)
          {
            RETHROW_YAT_ERROR(ex,
                              "SOFTWARE_FAILURE",
                              "Unable to get remote image",
                              "ImgBeamAnalyzerTask::handle_message");
          }
          catch(...)
          {
            THROW_YAT_ERROR("UNKNOWN_ERROR",
                            "Unable to get remote image",
                            "ImgBeamAnalyzerTask::handle_message");
          }

          try
          {
            //- post a process message (without waiting)
            //- it transfers ownership of 'image' pointer
            this->process(imginf, false);
          }
          catch( yat::Exception &ex )
          {
            RETHROW_YAT_ERROR(ex,
                              "SOFTWARE_FAILURE",
                              "Error while sending PROCESS msg",
                              "ImgBeamAnalyzerTask::handle_message");
          }
          catch(...)
          {
            THROW_YAT_ERROR("UNKNOWN_ERROR",
                            "Error while sending PROCESS msg",
                            "ImgBeamAnalyzerTask::handle_message");
          }
        }
        catch( yat::Exception &ex )
        {
          yat::OSStream os;
          os << "Error during periodic activity ["
             << ex.errors[0].desc
             << "]";
          this->set_state_status(FAULT, os.str());
          break;
        }
        catch( ... )
        {
          yat::OSStream os;
          os << "Error during periodic activity ["
             << "Unknown exception caught"
             << "]";
          this->set_state_status(FAULT, os.str());
          break;
        }

        //- ok, every thing went fine
        this->set_state_status(RUNNING, kRUNNING_STATUS_MSG);
      }
      break;
    case kMSG_PROCESS:
      {
//        DEBUG_STREAM << "ImgBeamAnalyzerTask::handle_message::handling PROCESS" << std::endl;

        if (this->mode_ == MODE_ONESHOT)
        {
          this->set_state_status(RUNNING, kRUNNING_STATUS_MSG);
        }

        isl::Image* image = 0;
        size_t bit_depth = 0;
        BIAData* data = 0;
        try
        {
          try
          {
            ImageAndInfo* imginf=0;
            _msg.detach_data(imginf);

            image = imginf->image;
            bit_depth = imginf->bit_depth;

            delete imginf;
          }
          catch(yat::Exception& ex)
          {
            RETHROW_YAT_ERROR(ex,
                              "SOFTWARE_FAILURE",
                              "Unable to dettach image from a PROCESS message",
                              "ImgBeamAnalyzerTask::handle_message");
          }
          catch(...)
          {
            THROW_YAT_ERROR("UNKNOWN_ERROR",
                            "Unable to dettach image from a PROCESS message",
                            "ImgBeamAnalyzerTask::handle_message");
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
              THROW_YAT_ERROR("OUT_OF_MEMORY",
                              "Allocation of BIAData failed",
                              "ImgBeamAnalyzerTask::handle_message");
            }

            //- get the current config
            BIAConfig config;
            {
              yat::MutexLock guard(this->config_mutex_);
              config = this->config_;
            }
            
            // If we got information about the bit_depth of the original
            // image, ignore the current set bit depth and just use it!
            if (bit_depth)
              config.pixel_depth = bit_depth;
            
            data->config = config;
            
            this->proc_.process(*image, config, *data);

            //- update the available data
            BIAData* last_data = this->data_;
            {
              yat::MutexLock guard(this->data_mutex_);
              this->data_ = data;
            }
            if (last_data)
            {
              last_data->release();
              last_data = 0;
            }
            this->on_img_processed_callback_(data);
          }
          catch(yat::Exception& ex)
          {
            if (this->data_)
              this->data_->alarm = true;
            RETHROW_YAT_ERROR(ex,
                              "SOFTWARE_FAILURE",
                              "Error when processing an image",
                              "ImgBeamAnalyzerTask::handle_message");
          }
          catch(...)
          {
            if (this->data_)
              this->data_->alarm = true;
            THROW_YAT_ERROR("UNKNOWN_ERROR",
                            "Error when processing an image",
                            "ImgBeamAnalyzerTask::handle_message");
          }
          SAFE_DELETE_PTR(image);
        }
        catch(yat::Exception& ex)
        {
          SAFE_RELEASE(data);
          SAFE_DELETE_PTR(image);
          if (this->mode_ == MODE_ONESHOT)
          {
            this->set_state_status(STANDBY, kSTANDBY_STATUS_MSG);
            RETHROW_YAT_ERROR(ex,
                              "SOFTWARE_FAILURE",
                              "Error when processing an image",
                              "ImgBeamAnalyzerTask::handle_message");
          }
          else
          {
            this->set_state_status(FAULT, ex.errors[0].desc);
            return;
          }
        }
        catch(...)
        {
          SAFE_RELEASE(data);
          SAFE_DELETE_PTR(image);
          if (this->mode_ == MODE_ONESHOT)
          {
            this->set_state_status(STANDBY, kSTANDBY_STATUS_MSG);
            THROW_YAT_ERROR("UNKNOWN_ERROR",
                            "Error when processing an image",
                            "ImgBeamAnalyzerTask::handle_message");
          }
          else
          {
            this->set_state_status(FAULT, "Unknown error while processing an image");
            return;
          }
        }

        //- everything went fine
        //- -> put the task back to a standby state
        if (this->mode_ == MODE_ONESHOT)
        {
          this->set_state_status(STANDBY, kSTANDBY_STATUS_MSG);
        }
      }
      break;
    case kMSG_STOP:
      {
//        DEBUG_STREAM << "ImgBeamAnalyzerTask::handle_message::handling STOP" << std::endl;
        this->set_state_status(STANDBY, kSTANDBY_STATUS_MSG);
      }
      break;
    default:
//      DEBUG_STREAM << "ImgBeamAnalyzerTask::handle_message::unhandled msg type received"<< std::endl;
      break;
  }

  //DEBUG_STREAM << "ImgBeamAnalyzerTask::handle_message::message_handler:msg " << _msg.to_string() << " successfully handled" << std::endl;
}



void ImgBeamAnalyzerTask::init( const ImgBeamAnalyzerInit& init_config )
  throw (yat::Exception)
{
  this->get_img_callback_ = init_config.get_img;
  this->get_img_allowed_ = init_config.get_img_allowed;
  this->on_img_processed_callback_ = init_config.img_processed_cb;
  this->mode_ = init_config.mode;
  this->configure(init_config.processing_config);

  if (init_config.mode == MODE_CONTINUOUS)
  {
    this->enable_periodic_msg(init_config.auto_start);
  }

  isl::ErrorHandler::init();
}


void ImgBeamAnalyzerTask::start( size_t )
  throw (yat::Exception)
{
  this->enable_periodic_msg(true);

  //- no need to send a msg,
  //- the state will be updated in the next periodic msg handling
}

void ImgBeamAnalyzerTask::stop( size_t timeout )
  throw (yat::Exception)
{
  this->enable_periodic_msg(false);

  //- send a STOP msg : this will update the state to STANDBY
  //- this is the only way to do it properly since the processing thread
  //- may be currently inside the processing function, so the state could
  //- be change to FAULT by the processing thread when exiting this function

  yat::Message* msg = 0;
  try
  {
    msg = yat::Message::allocate(kMSG_STOP, DEFAULT_MSG_PRIORITY, true);
  }
  catch(yat::Exception& ex)
  {
    RETHROW_YAT_ERROR(ex,
                      "OUT_OF_MEMORY",
                      "Error while creating a STOP message",
                      "ImgBeamAnalyzer::stop()");
  }
  catch(...)
  {
    THROW_YAT_ERROR("OUT_OF_MEMORY",
                    "Error while creating a STOP message",
                    "ImgBeamAnalyzer::stop()");
  }

  //- post the message
  try
  {
    this->wait_msg_handled(msg, timeout);
  }
  catch (yat::Exception& ex)
  {
    RETHROW_YAT_ERROR(ex,
                      "SOFTWARE_FAILURE",
                      "Error while posting a STOP message",
                      "ImgBeamAnalyzer::stop()");
  }
  catch(...)
  {
    THROW_YAT_ERROR("UNKNOWN_ERROR",
                    "Unknown error while posting a STOP message",
                    "ImgBeamAnalyzer::stop()");
  }
}






void ImgBeamAnalyzerTask::process( ImageAndInfo &imginf, bool wait, size_t wait_timeout )
  throw (yat::Exception)
{
  isl::Image* image = imginf.image;
  
  yat::Message* msg = 0;
  try
  {
    msg = yat::Message::allocate(kMSG_PROCESS, DEFAULT_MSG_PRIORITY, wait);
  }
  catch(yat::Exception& ex)
  {
    SAFE_DELETE_PTR( image );
    RETHROW_YAT_ERROR(ex,
                      "OUT_OF_MEMORY",
                      "Error while creating a PROCESS message",
                      "ImgBeamAnalyzer::process()");
  }
  catch(...)
  {
    SAFE_DELETE_PTR( image );
    THROW_YAT_ERROR("OUT_OF_MEMORY",
                    "Error while creating a PROCESS message",
                    "ImgBeamAnalyzer::process()");
  }

  try
  {
    // Here msg takes ownership of the image. So, from
    // now on we won't bother to SAFE_DELETE_PTR(image)
    // in this function.
    msg->attach_data(imginf);
  }
  catch(yat::Exception& ex)
  {
    SAFE_DELETE_PTR( image );
    SAFE_RELEASE( msg );
    RETHROW_YAT_ERROR(ex,
                      "SOFTWARE_FAILURE",
                      "Attaching data to a adtb::Message failed",
                      "ImgBeamAnalyzer::process");
  }
  catch(...)
  {
    SAFE_DELETE_PTR( image );
    SAFE_RELEASE( msg );
    THROW_YAT_ERROR("UNKNOWN_ERROR",
                    "Attaching data to a adtb::Message failed",
                    "ImgBeamAnalyzer::process");
  }

  //- post the message
  try
  {
    // Here we lose ownership of msg, even if the call fails,
  // so we won't be releasing it anymore
    if (wait)
      this->wait_msg_handled(msg, wait_timeout);
    else
      this->post(msg);
  }
  catch(yat::Exception& ex)
  {
    RETHROW_YAT_ERROR(ex,
                      "SOFTWARE_FAILURE",
                      "Error while posting a PROCESS message",
                      "ImgBeamAnalyzer::process");
  }
  catch(...)
  {
    THROW_YAT_ERROR("UNKNOWN_ERROR",
                    "Unknown error while posting a PROCESS message",
                    "ImgBeamAnalyzer::process");
  }
}


}
