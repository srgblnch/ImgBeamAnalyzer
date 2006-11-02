// ============================================================================
//
// = CONTEXT
//    TANGO Project - ImgBeamAnalyzer DeviceServer - Data class
//
// = File
//    Data.i
//
// = AUTHOR
//    Julien Malik
//
// ============================================================================

namespace ImgBeamAnalyzer_ns
{

INLINE_IMPL
void
BIATask::get_state (Tango::DevState& state)
{
  adtb::DeviceMutexLock guard(this->state_status_mutex_);
  state = this->state_;
}


INLINE_IMPL
void 
BIATask::get_status(std::string& status)
{
  adtb::DeviceMutexLock guard(this->state_status_mutex_);
  status = this->status_;
}

} // namespace
