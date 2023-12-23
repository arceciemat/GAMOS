#include "GmPSPrinterCout.hh"
#include "GmPSPrinterCoutContIndex.hh"
#include "GmPSPrinterBinFile.hh"
#include "GmPSPrinterTextFile.hh"
#include "GmPSPrinter3ddose.hh"
#include "GmPSPrinter3ddoseSplitZ.hh"
#include "GmPSPrinterSqdose.hh"
#include "GmPSPrinterInterfile.hh"
#include "GmPSPrinterHistos.hh"
#include "GmPSPrinterCSVFile.hh"

#include "GmPSPrinterCoutSpectrum.hh"
#include "GmPSPrinterHistosSpectrum.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"

PLUGINSVC_FACTORY(GmPSPrinterCout,GmVPSPrinter*(G4String))
PLUGINSVC_FACTORY(GmPSPrinterCoutContIndex,GmVPSPrinter*(G4String))
PLUGINSVC_FACTORY(GmPSPrinter3ddose,GmVPSPrinter*(G4String))
PLUGINSVC_FACTORY(GmPSPrinter3ddoseSplitZ,GmVPSPrinter*(G4String))
PLUGINSVC_FACTORY(GmPSPrinterSqdose,GmVPSPrinter*(G4String))
PLUGINSVC_FACTORY(GmPSPrinterInterfile,GmVPSPrinter*(G4String))
PLUGINSVC_FACTORY(GmPSPrinterBinFile,GmVPSPrinter*(G4String))
PLUGINSVC_FACTORY(GmPSPrinterTextFile,GmVPSPrinter*(G4String))
PLUGINSVC_FACTORY(GmPSPrinterHistos,GmVPSPrinter*(G4String))
PLUGINSVC_FACTORY(GmPSPrinterCSVFile,GmVPSPrinter*(G4String))

#else

#include "GamosCore/GamosScoring/Management/include/GmPSPrinterFactory.hh"

DEFINE_SEAL_MODULE ();

DEFINE_GAMOS_SCORER_PRINTER(GmPSPrinterCout);
DEFINE_GAMOS_SCORER_PRINTER(GmPSPrinterCoutContIndex);
DEFINE_GAMOS_SCORER_PRINTER(GmPSPrinter3ddose);
DEFINE_GAMOS_SCORER_PRINTER(GmPSPrinter3ddoseSplitZ);
DEFINE_GAMOS_SCORER_PRINTER(GmPSPrinterSqdose);
DEFINE_GAMOS_SCORER_PRINTER(GmPSPrinterInterfile);
DEFINE_GAMOS_SCORER_PRINTER(GmPSPrinterBinFile);
DEFINE_GAMOS_SCORER_PRINTER(GmPSPrinterTextFile);
DEFINE_GAMOS_SCORER_PRINTER(GmPSPrinterHistos);
DEFINE_GAMOS_SCORER_PRINTER(GmPSPrinterCSVFile);

#include "GamosCore/GamosScoring/Management/include/GmPSPrinterSpectrumFactory.hh"
DEFINE_GAMOS_SCORER_SPECTRUM_PRINTER(GmPSPrinterCoutSpectrum);
DEFINE_GAMOS_SCORER_SPECTRUM_PRINTER(GmPSPrinterHistosSpectrum);

#endif
