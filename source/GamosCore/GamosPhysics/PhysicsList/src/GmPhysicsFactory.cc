#ifndef ROOT5
#include "GmPhysicsFactory.hh"

GmPhysicsFactory GmPhysicsFactory::s_instance;

GmPhysicsFactory::GmPhysicsFactory (void)
    : seal::PluginFactory<G4VUserPhysicsList*()> ("GmPhysicsFactory")
{}

GmPhysicsFactory *
GmPhysicsFactory::get (void)
{ return &s_instance; }
#endif
