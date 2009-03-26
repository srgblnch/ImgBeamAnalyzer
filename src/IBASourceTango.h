
#pragma once

#ifdef WIN32
#pragma warning( push )
#pragma warning( disable: 4786 ) // 
#pragma warning( disable: 4267 ) // 'var' : conversion from 'size_t' to 'type', possible loss of data 
#pragma warning( disable: 4311 ) // 'variable' : pointer truncation from 'type' to 'type' 
#pragma warning( disable: 4312 ) // 'operation' : conversion from 'type1' to 'type2' of greater size 
#include <tango.h>
#include "D:\dev\soleil-root\tango-soleil-win32\sw-support\Tango\include\event.h"
#pragma warning( pop )
#else
#include <tango.h>
#endif

#include "IBASource.h"

class IBASourceTango : public IBASource, public Tango::CallBack
{
  Tango::DeviceProxy* dev_proxy_;
  int eventId_;
  std::string bit_depth_attribute_name_;
public:
  IBASourceTango(const std::string & deviceName);
  ~IBASourceTango();

  /// @todo Shouldn't it be somewhere more publicly available?
  class TangoYATException : public yat::Exception
  {
  public:
    TangoYATException( Tango::DevFailed& );
  };

  virtual /*from Tango::Callback*/
    void push_event(Tango::EventData *ed);

  virtual /*from IBASource*/
    void get_image(ImageAndInfo & imginf) throw (yat::Exception);

  virtual /*from IBASource*/
    void set_observed_attribute(const std::string & attrName);
};

class IBA_DLLDIR IBASourceFactoryTango : public IBASourceFactory
{
  static IBASourceFactoryTango s_singleton;
  friend class IBASourceFactory;

protected:
  virtual IBASource* create_object(const std::string & name)
  { return new IBASourceTango(name); }

public:
  /// Never used now, as we "register" it by default, see first lines of IBASourceTango.cpp
  static void register_factory()
  { IBASourceFactory::register_factory(&IBASourceFactoryTango::s_singleton); }
};
