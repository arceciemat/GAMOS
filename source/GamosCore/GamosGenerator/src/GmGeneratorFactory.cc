#ifndef ROOT5
#include "GmGeneratorFactory.hh"

GmGeneratorFactory GmGeneratorFactory::s_instance;

GmGeneratorFactory::GmGeneratorFactory (void)
    : seal::PluginFactory<G4VUserPrimaryGeneratorAction*()> ("GmGeneratorFactory")
{}

GmGeneratorFactory *
GmGeneratorFactory::get (void)
{ return &s_instance; }
#endif
