#------------------------------------------------------------------------------
# 
# Module : GamosCore
# Package: GamosData_Management
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
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosData_Management

   HEADERS
       GmVDataNon1DHisto.hh
       GmVDataString.hh
       GmVDataProf2DHisto.hh
       GmVDataAccumulated.hh
       GmVDataProf1DHisto.hh
       GmDataVerbosity.hh
       GmVDataInt.hh
       GmVData2DHisto.hh
       GmDataMgr.hh
       GmVData.hh

   SOURCES
       plugin.cc
       GmDataMgr.cc
       GmDataVerbosity.cc
       GmVDataNon1DHisto.cc
       GmVDataString.cc
       GmVDataAccumulated.cc
       GmVDataProf2DHisto.cc
       GmVData.cc
       GmVData2DHisto.cc
       GmVDataProf1DHisto.cc
       GmVDataInt.cc
 
   GRANULAR_DEPENDENCIES

   GLOBAL_DEPENDENCIES
    ${Geant4_LIBRARIES}
    ${Root_LIBRARIES}
  GamosCore_GamosBase_Base
  GamosCore_GamosUtils

   LINK_LIBRARIES
)

# List any source specific properties here
