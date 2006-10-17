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
class BIATask : public adtb::DeviceTask
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
  BIAData* get_data (void)
    throw (Tango::DevFailed);

  const BIAConfig& get_config(void);

  void configure(const BIAConfig&);

  Tango::DevState    state (void);
  const std::string& status(void);
  
  //- retrieves an isl::Image from the remote device
  isl::Image* get_remote_image(bool throws = false);

protected:
	//- handle_message -----------------------
	virtual void handle_message (const adtb::Message& msg)
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
  Tango::DevState state_;
  std::string     status_;
  bool initialized_;

};


} // namespace

#if defined (__INLINE_IMPL__)
# include "BIATask.i"
#endif // __INLINE_IMPL__

#endif
