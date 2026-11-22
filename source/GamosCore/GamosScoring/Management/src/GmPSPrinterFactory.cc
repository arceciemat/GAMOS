#ifndef ROOT5
#include "GmPSPrinterFactory.hh"

GmPSPrinterFactory GmPSPrinterFactory::s_instance;

GmPSPrinterFactory::GmPSPrinterFactory (void)
  : seal::PluginFactory<GmVPSPrinter *(G4String)> ("GmPSPrinterFactory")
{}

GmPSPrinterFactory *
GmPSPrinterFactory::get (void)
{ return &s_instance; }
#endif
