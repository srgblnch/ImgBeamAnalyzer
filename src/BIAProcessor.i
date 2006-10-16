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
BIAData*
BIAProcessor::get_data (void)
{
  return( this->data_ ? this->data_->duplicate() : 0);
}


INLINE_IMPL
const BIAConfig&
BIAProcessor::get_config (void)
{
  return( this->config_ );
}

INLINE_IMPL
void
BIAProcessor::configure(const BIAConfig& config)
{
  this->config_ = config;
}

} // namespace
