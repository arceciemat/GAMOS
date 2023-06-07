#ifndef ROOT5
#include "GmUserTimeStepActionFactory.hh"

GmUserTimeStepActionFactory GmUserTimeStepActionFactory::s_instance;

GmUserTimeStepActionFactory::GmUserTimeStepActionFactory (void)
    : seal::PluginFactory<G4UserTimeStepAction* ()> ("GmUserTimeStepActionFactory")
{}

GmUserTimeStepActionFactory *
GmUserTimeStepActionFactory::get (void)
{ return &s_instance; }

#endif
