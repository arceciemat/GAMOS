#ifndef ROOT5
#include "GmDataFactory.hh"


GmDataFactory GmDataFactory::s_instance;


GmDataFactory::GmDataFactory (void)
    : seal::PluginFactory<GmVData*()> ("GmDataFactory")
{}

GmDataFactory *
GmDataFactory::get (void)
{ return &s_instance; }
#endif
