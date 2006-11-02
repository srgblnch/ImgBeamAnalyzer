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
BIAProcessor::get_data (BIAData*& data)
{
  adtb::DeviceMutexLock(this->data_mutex_);
  data = this->data_ ? this->data_->duplicate() : 0;
}


INLINE_IMPL
void
BIAProcessor::get_config (BIAConfig& c)
{
  adtb::DeviceMutexLock(this->config_mutex_);
  c = this->config_;
}

INLINE_IMPL
void
BIAProcessor::configure(const BIAConfig& config)
{
  adtb::DeviceMutexLock(this->config_mutex_);
  this->config_ = config;
}

} // namespace
