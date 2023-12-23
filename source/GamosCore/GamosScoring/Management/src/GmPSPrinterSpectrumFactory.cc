#ifndef ROOT5
#include "GmPSPrinterSpectrumFactory.hh"

GmPSPrinterSpectrumFactory GmPSPrinterSpectrumFactory::s_instance;

GmPSPrinterSpectrumFactory::GmPSPrinterSpectrumFactory (void)
  : seal::PluginFactory<GmVPSPrinterSpectrum *(G4String)> ("GmPSPrinterSpectrumFactory")
{}

GmPSPrinterSpectrumFactory *
GmPSPrinterSpectrumFactory::get (void)
{ return &s_instance; }
#endif
