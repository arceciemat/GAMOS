#ifndef ROOT5
#include "GmSensDetFactory.hh"

GmSensDetFactory GmSensDetFactory::s_instance;

GmSensDetFactory::GmSensDetFactory (void)
    : seal::PluginFactory<G4VSensitiveDetector*(const G4String&)> ("GmSensDetFactory")
{}

GmSensDetFactory *
GmSensDetFactory::get (void)
{ return &s_instance; }
#endif
