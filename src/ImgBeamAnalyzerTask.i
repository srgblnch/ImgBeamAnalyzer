// ============================================================================
//
// = CONTEXT
//    TANGO Project - ImgBeamAnalyzer DeviceServer - Data class
//
// = File
//    ImgBeamAnalyzerTask.i
//
// = AUTHOR
//    Julien Malik
//
// ============================================================================

namespace ImgBeamAnalyzer_ns
{

// ============================================================================
// ImgBeamAnalyzerTask::get_state_status
// ============================================================================
YAT_INLINE
void
ImgBeamAnalyzerTask::get_state_status (State& state, std::string& status)
{
  yat::MutexLock guard(this->state_status_mutex_);
  state = this->state_;
  status = this->status_;
}

// ============================================================================
// ImgBeamAnalyzerTask::set_state_status
// ============================================================================
YAT_INLINE
void
ImgBeamAnalyzerTask::set_state_status (State state, std::string status)
{
  yat::MutexLock guard(this->state_status_mutex_);
  this->state_ = state;
  this->status_ = status;
}

// ============================================================================
// ImgBeamAnalyzerTask::get_data
// ============================================================================
YAT_INLINE
void
ImgBeamAnalyzerTask::get_data (BIAData*& data)
  throw (yat::Exception)
{
  //- enter critical section
  yat::MutexLock guard(this->data_mutex_);

  if (this->mode_ == MODE_CONTINUOUS && this->state_ == STANDBY)
    data = 0;
  else
    data = this->data_ ? this->data_->duplicate() : 0;
}

// ============================================================================
// ImgBeamAnalyzerTask::get_config
// ============================================================================
YAT_INLINE
void
ImgBeamAnalyzerTask::get_config(BIAConfig& c)
{
  //- enter critical section
  yat::MutexLock guard(this->config_mutex_);
  c = this->config_;
}

// ============================================================================
// ImgBeamAnalyzerTask::configure
// ============================================================================
YAT_INLINE
void
ImgBeamAnalyzerTask::configure(const BIAConfig& config)
{
  yat::MutexLock guard(this->config_mutex_);
  this->config_ = config;
  this->set_periodic_msg_period(config.comput_period);
}

} // namespace
