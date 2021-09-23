#ifndef ROOT5
#include "GmDistributionFactory.hh"

GmDistributionFactory GmDistributionFactory::s_instance;

GmDistributionFactory::GmDistributionFactory (void)
    : seal::PluginFactory<GmVDistribution*(G4String)> ("GmDistributionFactory")
{}

GmDistributionFactory *
GmDistributionFactory::get (void)
{ return &s_instance; }

#endif
