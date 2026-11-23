#ifndef GmVPSPrinterSpectrum_hh
#define GmVPSPrinterSpectrum_hh 1

#include <vector>
#include "globals.hh"
#include "G4THitsMap.hh"
#include "GmVPSPrinter.hh"
class GmVPrimitiveScorer;
// class description:
//
// This is the base class of the GAMOS GmVPSPrinterSpectrum.
// It construct the GmVPSPrinterSpectrum object passing the corresponding arguments
typedef std::map<G4int,G4double> mid;

class GmVPSPrinterSpectrum : public GmVPSPrinter
{
public: // with description
  GmVPSPrinterSpectrum(G4String);
  virtual ~GmVPSPrinterSpectrum(){ };
  
  virtual void DumpAll( G4THitsMap<G4double>* , GmVPrimitiveScorer*  ){};

protected:

};

#endif
