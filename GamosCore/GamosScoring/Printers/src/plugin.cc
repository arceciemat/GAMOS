#include "Reflex/PluginService.h"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

#include "GmPSPrinterG4cout.hh"
#include "GmPSPrinterCout.hh"
#include "GmPSPrinterBinFile.hh"
#include "GmPSPrinterTextFile.hh"
#include "GmPSPrinter3ddose.hh"
#include "GmPSPrinterSqdose.hh"
#include "GmPSPrinterHistos.hh"
#include "GmPSPrinterCSVFile.hh"

PLUGINSVC_FACTORY(GmPSPrinterG4cout,GmVPSPrinter*(G4String))
PLUGINSVC_FACTORY(GmPSPrinterCout,GmVPSPrinter*(G4String))
PLUGINSVC_FACTORY(GmPSPrinter3ddose,GmVPSPrinter*(G4String))
PLUGINSVC_FACTORY(GmPSPrinterSqdose,GmVPSPrinter*(G4String))
PLUGINSVC_FACTORY(GmPSPrinterBinFile,GmVPSPrinter*(G4String))
PLUGINSVC_FACTORY(GmPSPrinterTextFile,GmVPSPrinter*(G4String))
PLUGINSVC_FACTORY(GmPSPrinterHistos,GmVPSPrinter*(G4String))
PLUGINSVC_FACTORY(GmPSPrinterCSVFile,GmVPSPrinter*(G4String))
