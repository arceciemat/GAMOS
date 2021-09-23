#ifndef ROOT5

#include "GmClassifierFactory.hh"

GmClassifierFactory GmClassifierFactory::s_instance;

GmClassifierFactory::GmClassifierFactory (void)
  : seal::PluginFactory<GmVClassifier *(G4String)> ("GmClassifierFactory")
{}

GmClassifierFactory *
GmClassifierFactory::get (void)
{ return &s_instance; }

#endif 
