#------------------------------------------------------------------------------
# 
# Module : GamosCore
# Package: GamosScoring_Printers
#
#------------------------------------------------------------------------------
# List external includes needed.

# List external includes needed.
include(GamosSetup)
include(UseRoot)

#
# Define the GAMOS Module.
#
include(Geant4MacroDefineModule)
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosScoring_Printers

   HEADERS
       GmPSPrinterCSVFile.hh
       GmPSPrinterXYZ.hh
       GmPSPrinterSqdose.hh
       GmPSPrinterTextFile.hh
       GmPSPrinterBinFile.hh
       GmPSPrinterHistos.hh
       GmPSPrinter3ddose.hh
       GmPSPrinterCout.hh

   SOURCES
       plugin.cc
       GmPSPrinterTextFile.cc
       GmPSPrinterCout.cc
       GmPSPrinterCSVFile.cc
       GmPSPrinter3ddose.cc
       GmPSPrinterSqdose.cc
       GmPSPrinterHistos.cc
       GmPSPrinterXYZ.cc
       GmPSPrinterBinFile.cc
 
   GRANULAR_DEPENDENCIES

   GLOBAL_DEPENDENCIES
    ${Geant4_LIBRARIES}
    ${Root_LIBRARIES}
  GamosCore_GamosAnalysis
  GamosCore_GamosBase_Classifiers
  GamosCore_GamosGeometry
  GamosCore_GamosUtils
  GamosCore_GamosReadDICOM
  GamosCore_GamosScoring_Management
  GamosCore_GamosBase_Base

   LINK_LIBRARIES
)

# List any source specific properties here
