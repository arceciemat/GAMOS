#ifndef ROOT5
#include "GmBiasingOperatorFactory.hh"

GmBiasingOperatorFactory GmBiasingOperatorFactory::s_instance;

GmBiasingOperatorFactory::GmBiasingOperatorFactory (void)
    : seal::PluginFactory<GmVBiasingOperator*(G4String)> ("GmBiasingOperatorFactory")
{}

GmBiasingOperatorFactory *
GmBiasingOperatorFactory::get (void)
{ return &s_instance; }
#endif
