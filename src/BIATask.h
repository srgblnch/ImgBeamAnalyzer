// ============================================================================
//
// = CONTEXT
//    TANGO Project - ImgBeamAnalyzer DeviceServer - Data class
//
// = File
//    Data.h
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
#include <DeviceTask.h>

namespace ImgBeamAnalyzer_ns
{

const int MODE_ONESHOT = 0;
const int MODE_CONTINUOUS = 1;

#define kMSG_CONFIG            (adtb::FIRST_USER_MSG)
#define kMSG_PROCESS           (adtb::FIRST_USER_MSG + 1)
#define kMSG_START             (adtb::FIRST_USER_MSG + 2)
#define kMSG_STOP              (adtb::FIRST_USER_MSG + 3)
#define kMSG_START_LEARN_NOISE (adtb::FIRST_USER_MSG + 4)
#define kMSG_STOP_LEARN_NOISE  (adtb::FIRST_USER_MSG + 5)


// ============================================================================
//! Data abstraction class.
// ============================================================================
//!
//! detailed description to be written
//!
// ============================================================================
class BIATask : public adtb::DeviceTask , public Tango::LogAdapter
{
  friend class BIAProcessor;

public:
	//- ctor ---------------------------------
	BIATask (Tango::DeviceImpl * _host_device, 
           const BIAConfig& _config,
           bool _dev_proxy_allowed,
           const std::string& _image_dev_name,
           const std::string& _image_attr_name,
           bool _auto_start,
           int  _mode);

	//- dtor ---------------------------------
	virtual ~BIATask (void);

  //- returns the last available data ------
  void get_data (BIAData*& data)
    throw (Tango::DevFailed);

  void get_config(BIAConfig&);

  void configure(const BIAConfig&);

  void get_state (Tango::DevState& state);
  void get_status(std::string& status);
  
  //- retrieves an isl::Image from the remote device
  isl::Image* get_remote_image(bool throws = false);

protected:
	//- handle_message -----------------------
	virtual void handle_message (adtb::Message& msg)
		throw (Tango::DevFailed);

private:
  Tango::DeviceProxy* dev_proxy_;
  bool                dev_proxy_allowed_;
  std::string         image_dev_name_;
  std::string         image_attr_name_;
  int                 mode_;
 
  BIAProcessor        proc_;


  //- Error handling
  void clear_error();
  void fault(const char* description);

  adtb::DeviceMutex state_status_mutex_;
  Tango::DevState   state_;
  std::string       status_;

  bool initialized_;

};


} // namespace

#if defined (__INLINE_IMPL__)
# include "BIATask.i"
#endif // __INLINE_IMPL__

#endif
