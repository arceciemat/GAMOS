#ifndef ROOT5
#include "GmDigitizerFactory.hh"


GmDigitizerFactory GmDigitizerFactory::s_instance;


GmDigitizerFactory::GmDigitizerFactory (void)
    : seal::PluginFactory<GmVDigitizer*()> ("GmDigitizerFactory")
{}

GmDigitizerFactory *
GmDigitizerFactory::get (void)
{ return &s_instance; }
#endif
