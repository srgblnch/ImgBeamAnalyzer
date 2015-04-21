// ============================================================================
//
// = CONTEXT
//    TANGO Project - ImgBeamAnalyzer DeviceServer - Data class
//
// = File
//    BIAData.i
//
// = AUTHOR
//    Julien Malik
//
// ============================================================================

namespace ImgBeamAnalyzer_ns
{

// ============================================================================
// Data::duplicate
// ============================================================================
YAT_INLINE BIAData *BIAData::duplicate (void)
{
  return reinterpret_cast < BIAData * >(SharedObject::duplicate ());
}

// ============================================================================
// Data::release
// ============================================================================
YAT_INLINE void BIAData::release (void)
{
  SharedObject::release ();
}

} // namespace
