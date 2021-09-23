#ifndef ROOT5
#include "GmFilterFactory.hh"

GmFilterFactory GmFilterFactory::s_instance;

GmFilterFactory::GmFilterFactory (void)
    : seal::PluginFactory<GmVFilter*(G4String)> ("GmFilterFactory")
{}

GmFilterFactory *
GmFilterFactory::get (void)
{ 
	return &s_instance;
}

#endif
