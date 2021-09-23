#ifndef ROOT5
#include "GmGeometryFactory.hh"

GmGeometryFactory GmGeometryFactory::s_instance;

GmGeometryFactory::GmGeometryFactory (void)
    : seal::PluginFactory<G4VUserDetectorConstruction*()> ("GmGeometryFactory")
{}

GmGeometryFactory *
GmGeometryFactory::get (void)
{ return &s_instance; }
#endif
