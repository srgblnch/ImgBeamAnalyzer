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
Tango::DevState 
BIATask::state (void)
{
  return( this->state_ );
}


INLINE_IMPL
const std::string& 
BIATask::status(void)
{
  return( this->status_ );
}

} // namespace
