#include "GmVPSPrinterSpectrum.hh"
#include "GmScoringVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//-------------------------------------------------------------------
GmVPSPrinterSpectrum::GmVPSPrinterSpectrum(G4String name) : GmVPSPrinter( name )
{
  theNewUnit = -1;
}


