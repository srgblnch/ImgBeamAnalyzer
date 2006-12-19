// ============================================================================
//
// = CONTEXT
//    TANGO Project - ImgBeamAnalyzer DeviceServer - Data class
//
// = File
//    BIATask.i
//
// = AUTHOR
//    Julien Malik
//
// ============================================================================

namespace ImgBeamAnalyzer_ns
{

// ============================================================================
// BIATask::get_state_status
// ============================================================================
INLINE_IMPL
void
BIATask::get_state_status (Tango::DevState& state, std::string& status)
{
  adtb::DeviceMutexLock<> guard(this->state_status_mutex_);
  state = this->state_;
  status = this->status_;
}

// ============================================================================
// BIATask::set_state_status
// ============================================================================
INLINE_IMPL
void
BIATask::set_state_status (Tango::DevState state, const char* status)
{
  adtb::DeviceMutexLock<> guard(this->state_status_mutex_);
  this->state_ = state;
  this->status_ = status;
}

// ============================================================================
// BIATask::get_data
// ============================================================================
INLINE_IMPL
void
BIATask::get_data (BIAData*& data)
  throw (Tango::DevFailed)
{
  //- enter critical section
 adtb::DeviceMutexLock<> guard(this->data_mutex_);

  if (this->mode_ == MODE_CONTINUOUS && this->state_ == Tango::STANDBY)
    data = 0;
  else
    data = this->data_ ? this->data_->duplicate() : 0;
}

// ============================================================================
// BIATask::get_config
// ============================================================================
INLINE_IMPL
void
BIATask::get_config(BIAConfig& c)
{
  //- enter critical section
  adtb::DeviceMutexLock<> guard(this->config_mutex_);
  c = this->config_;
}

// ============================================================================
// BIATask::configure
// ============================================================================
INLINE_IMPL
void
BIATask::configure(const BIAConfig& config)
{
  adtb::DeviceMutexLock<> guard(this->config_mutex_);
  this->config_ = config;
  this->set_periodic_timeout(config.comput_period);
}

} // namespace
