#------------------------------------------------------------------------------
# 
# Module : GamosCore
# Package: GamosData_Users
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
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosData_Users

   HEADERS
       GmStackDataBinFileUA.hh
       GmStepDataTextFileUA.hh
       GmVDataUser.hh
       GmSecondaryTrackDataBinFileUA.hh
       GmRunDataHistosUA.hh
       GmVDataUserHistos.hh
       GmTrackDataTextFileUA.hh
       GmStepDataCoutUA.hh
       GmRunDataCoutUA.hh
       GmEventDataHistosUA.hh
       GmStepDataHistosUA.hh
       GmStepDataBinFileUA.hh
       GmClassifierByNumericData.hh
       GmSecondaryTrackDataHistosUA.hh
       GmClassifierByDistribution.hh
       GmStringDataFilter.hh
       GmClassifierByStringData.hh
       GmEventDataTextFileUA.hh
       GmSecondaryTrackDataTextFileUA.hh
       GmTrackDataCoutUA.hh
       GmSecondaryTrackDataCoutUA.hh
       GmVDataUserTextFile.hh
       GmEventDataBinFileUA.hh
       GmEventDataCoutUA.hh
       GmDataTTreeUA.hh
       GmStackDataTextFileUA.hh
       GmStackDataHistosUA.hh
       GmVDataUserBinFile.hh
       GmTrackDataHistosUA.hh
       GmNumericDataFilter.hh
       GmRunDataTextFileUA.hh
       GmStackDataCoutUA.hh
       GmRunDataBinFileUA.hh
       GmTrackDataBinFileUA.hh

   SOURCES
       plugin.cc
       GmStepDataTextFileUA.cc
       GmTrackDataTextFileUA.cc
       GmEventDataBinFileUA.cc
       GmTrackDataBinFileUA.cc
       GmTrackDataHistosUA.cc
       GmVDataUserBinFile.cc
       GmEventDataCoutUA.cc
       GmClassifierByStringData.cc
       GmStringDataFilter.cc
       GmStepDataBinFileUA.cc
       GmClassifierByDistribution.cc
       GmRunDataBinFileUA.cc
       GmNumericDataFilter.cc
       GmSecondaryTrackDataHistosUA.cc
       GmVDataUserTextFile.cc
       GmClassifierByNumericData.cc
       GmStackDataHistosUA.cc
       GmStackDataBinFileUA.cc
       GmEventDataHistosUA.cc
       GmSecondaryTrackDataBinFileUA.cc
       GmVDataUserHistos.cc
       GmStepDataHistosUA.cc
       GmVDataUser.cc
       GmDataTTreeUA.cc
       GmSecondaryTrackDataCoutUA.cc
       GmStepDataCoutUA.cc
       GmSecondaryTrackDataTextFileUA.cc
       GmEventDataTextFileUA.cc
       GmStackDataTextFileUA.cc
       GmRunDataTextFileUA.cc
       GmRunDataCoutUA.cc
       GmRunDataHistosUA.cc
       GmStackDataCoutUA.cc
       GmTrackDataCoutUA.cc
 
   GRANULAR_DEPENDENCIES

   GLOBAL_DEPENDENCIES
    ${Geant4_LIBRARIES}
    ${Root_LIBRARIES}
  GamosCore_GamosAnalysis
  GamosCore_GamosGeometry
  GamosCore_GamosData_Distributions
  GamosCore_GamosUtils
  GamosCore_GamosUserActionMgr
  GamosCore_GamosBase_Base
  GamosCore_GamosData_Management

   LINK_LIBRARIES
)

# List any source specific properties here
