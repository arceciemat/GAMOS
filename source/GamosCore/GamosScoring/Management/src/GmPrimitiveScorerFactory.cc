#ifndef ROOT5
#include "GmPrimitiveScorerFactory.hh"

GmPrimitiveScorerFactory GmPrimitiveScorerFactory::s_instance;

GmPrimitiveScorerFactory::GmPrimitiveScorerFactory (void)
  : seal::PluginFactory<GmVPrimitiveScorer *(G4String)> ("GmPrimitiveScorerFactory")
{}

GmPrimitiveScorerFactory *
GmPrimitiveScorerFactory::get (void)
{ return &s_instance; }
#endif
