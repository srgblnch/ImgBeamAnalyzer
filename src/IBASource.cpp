//*******************************************************************************
//* Copyright (c) 2004-2014 Synchrotron SOLEIL
//* All rights reserved. This program and the accompanying materials
//* are made available under the terms of the GNU Lesser Public License v3
//* which accompanies this distribution, and is available at
//* http://www.gnu.org/licenses/lgpl.html
//******************************************************************************

#include "IBASource.h"
#include "ImgBeamAnalyzer.h"

// I got rid of this as I default to IBASourceTango as default factory.
// -- /*static*/ IBASourceFactory* IBASourceFactory::s_first_ = 0;

void IBASource::process(ImageAndInfo &imginf)
{
  ObserverType* obs = this->observer_;
  if (obs)
    obs->just_process(imginf);
}

/*static*/ void IBASourceFactory::register_factory(IBASourceFactory* builder)
{
  if (builder->next_ || IBASourceFactory::s_first_ == builder)
    // It's already registered, as this is the only way to set next_
    return;

  builder->next_ = IBASourceFactory::s_first_;
  IBASourceFactory::s_first_ = builder;
}

/*static*/ IBASource* IBASourceFactory::create(const std::string & name)
{
  if (!IBASourceFactory::s_first_)
    THROW_YAT_ERROR(
        "NO_FACTORY",
        "No IBASource factory has been defined, so it is not possible to create"
        " IBASource objects -> Can't connect to a source of images.",
        "IBASourceFactory::create");
  return IBASourceFactory::s_first_->create_object(name);
}

IBASource* IBASourceFactory::create_next(const std::string & name)
{
  if (!this->next_)
    THROW_YAT_ERROR(
        "NO_NEXT_FACTORY",
        "No IBASource factory has been been able to create"
        " a IBASource object -> Can't connect to a source of images.",
        "IBASourceFactory::create_next");

  return this->next_->create_object(name);
}
