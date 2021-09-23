#ifndef ROOT5
#include "GmGeneratorDistributionFactories.hh"

// Generator energy distribution factory
GmGenerDistEnergyFactory GmGenerDistEnergyFactory::s_instance;

GmGenerDistEnergyFactory::GmGenerDistEnergyFactory (void)
    : seal::PluginFactory<GmVGenerDistEnergy*()> ("GmGenerDistEnergyFactory")
{}

GmGenerDistEnergyFactory *
GmGenerDistEnergyFactory::get (void)
{ return &s_instance; }


// Generator time distribution factory
GmGenerDistTimeFactory GmGenerDistTimeFactory::s_instance;

GmGenerDistTimeFactory::GmGenerDistTimeFactory (void)
    : seal::PluginFactory<GmVGenerDistTime*()> ("GmGenerDistTimeFactory")
{}

GmGenerDistTimeFactory *
GmGenerDistTimeFactory::get (void)
{ return &s_instance; }


// Generator position distribution factory
GmGenerDistPositionFactory GmGenerDistPositionFactory::s_instance;

GmGenerDistPositionFactory::GmGenerDistPositionFactory (void)
    : seal::PluginFactory<GmVGenerDistPosition*()> ("GmGenerDistPositionFactory")
{}

GmGenerDistPositionFactory *
GmGenerDistPositionFactory::get (void)
{ return &s_instance; }


// Generator direction distribution factory
GmGenerDistDirectionFactory GmGenerDistDirectionFactory::s_instance;

GmGenerDistDirectionFactory::GmGenerDistDirectionFactory (void)
    : seal::PluginFactory<GmVGenerDistDirection*()> ("GmGenerDistDirectionFactory")
{}

GmGenerDistDirectionFactory *
GmGenerDistDirectionFactory::get (void)
{ return &s_instance; }
#endif
