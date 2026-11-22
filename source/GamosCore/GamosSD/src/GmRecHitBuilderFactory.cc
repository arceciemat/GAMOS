#ifndef ROOT5

#include "GmRecHitBuilderFactory.hh"


GmRecHitBuilderFactory GmRecHitBuilderFactory::s_instance;


GmRecHitBuilderFactory::GmRecHitBuilderFactory (void)
    : seal::PluginFactory<GmVRecHitBuilder*()> ("GmRecHitBuilderFactory")
{}

GmRecHitBuilderFactory *
GmRecHitBuilderFactory::get (void)
{ return &s_instance; }
#endif
