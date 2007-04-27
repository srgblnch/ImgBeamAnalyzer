// ============================================================================
//
// = CONTEXT
//    TANGO Project - ImgBeamAnalyzer DeviceServer - Data class
//
// = File
//    ImgBeamAnalyzerTask.h
//
// = AUTHOR
//    Julien Malik
//
// ============================================================================

#ifndef _BIA_TASK_H_
#define _BIA_TASK_H_

// ============================================================================
// DEPENDENCIES
// ============================================================================
#include "BIAProcessor.h"
#include <yat/Callback.h>
#include <yat/threading/Task.h>

namespace ImgBeamAnalyzer_ns
{

const int MODE_ONESHOT = 0;
const int MODE_CONTINUOUS = 1;

#define kMSG_PROCESS           (yat::FIRST_USER_MSG + 1)
#define kMSG_START             (yat::FIRST_USER_MSG + 2)
#define kMSG_STOP              (yat::FIRST_USER_MSG + 3)
#define kMSG_START_LEARN_NOISE (yat::FIRST_USER_MSG + 4)
#define kMSG_STOP_LEARN_NOISE  (yat::FIRST_USER_MSG + 5)

YAT_DEFINE_CALLBACK( GetImgCB, isl::Image*& );

//---------------------------------------------------------------------------
//- ImgBeamAnalyzerInit
//---------------------------------------------------------------------------
struct ImgBeamAnalyzerInit
{
  GetImgCB  get_img;
  bool      get_img_allowed;
  BIAConfig processing_config;
  bool      auto_start;
  int       mode;
};

//---------------------------------------------------------------------------
//- ImgBeamAnalyzerTask
//---------------------------------------------------------------------------
class ImgBeamAnalyzerTask : public yat::Task
{
  friend class BIAProcessor;

public:
  typedef enum {
    INIT,
    STANDBY,
    RUNNING,
    FAULT
  } State;

	//- ctor ---------------------------------
	ImgBeamAnalyzerTask ();

	//- dtor ---------------------------------
	virtual ~ImgBeamAnalyzerTask (void);

  //- returns the last available data ------
  void get_data ( BIAData*& data )
    throw (yat::Exception);

  //- start
  void start( size_t timeout )
    throw (yat::Exception);

  //- stop
  void stop( size_t timeout )
    throw (yat::Exception);

  //- process
  //- gets ownership of 'image' pointer
  //- the pointer is deleted in case an exception occurs
  void process( isl::Image* image, bool wait, size_t wait_timeout = 0 )
    throw (yat::Exception);

  void get_config( BIAConfig& );

  void configure( const BIAConfig& );

  void get_state_status (State& state, std::string& status);

protected:
	//- handle_message -----------------------
	virtual void handle_message (yat::Message& msg)
    throw (yat::Exception);

private:

  void init( const ImgBeamAnalyzerInit& init_config )
    throw (yat::Exception);

  int                 mode_;
 
  GetImgCB            get_img_callback_;
  bool                get_img_allowed_;
  BIAProcessor        proc_;

  yat::Mutex          config_mutex_;
  BIAConfig           config_;
  
  yat::Mutex          data_mutex_;
  BIAData*            data_;

  //- Error handling
  //static Tango::DevFailed isl_to_tango_exception (const isl::Exception& e);
  void set_state_status(State state, std::string status);
  yat::Mutex  state_status_mutex_;
  State       state_;
  std::string status_;

  bool initialized_;

};


} // namespace

#if defined (YAT_INLINE_IMPL)
# include "ImgBeamAnalyzerTask.i"
#endif // YAT_INLINE_IMPL

#endif
