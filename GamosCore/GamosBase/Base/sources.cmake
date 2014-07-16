#------------------------------------------------------------------------------
# 
# Module : GamosCore
# Package: GamosBase_Base
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
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosBase_Base

   HEADERS
       GmIObinMgr.hh
       GmHisto1.hh
       GmParameterMessenger.hh
       GmParameter.hh
       GmGetParticleMgr.hh
       GmAnalysisMessenger.hh
       GmParameterListNumeric.hh
       GmFilterMgr.hh
       GmHistoWriterGraphics.hh
       GmVClassifier.hh
       GmIOtextMgr.hh
       GmVFilter.hh
       GmParameterMgr.hh
       GmHistoWriter.hh
       GmAnalysisMgr.hh
       GmDistributionMgr.hh
       GmBaseMessenger.hh
       GmParameterList.hh
       GmUIcmdWithAString.hh
       GmChangeEnergies.hh
       GmHistoWriterROOT.hh
       GmVDistribution.hh
       GmVisManager.hh
       GmHistoProfile1.hh
       GmHistoWriterTXT.hh
       GmTrackInfo.hh
       GmClassifierMgr.hh
       GmHistoProfile2.hh
       GmVVerbosity.hh
       GmHistoReaderCSV.hh
       GmHistoWriterCSV.hh
       GmBaseVerbosity.hh
       GmIOMgr.hh
       GmConvergenceTester.hh
       GmParameterListVNumeric.hh
       GmHisto2.hh

   SOURCES
       plugin.cc
       GmAnalysisMgr.cc
       GmHistoProfile1.cc
       GmConvergenceTester.cc
       GmHistoWriterCSV.cc
       GmHisto2.cc
       GmVVerbosity.cc
       GmVFilter.cc
       GmBaseMessenger.cc
       GmHistoProfile2.cc
       GmBaseVerbosity.cc
       GmFilterMgr.cc
       GmDistributionMgr.cc
       GmHistoWriterTXT.cc
       GmUIcmdWithAString.cc
       GmChangeEnergies.cc
       GmHistoReaderCSV.cc
       GmTrackInfo.cc
       GmAnalysisMessenger.cc
       GmVClassifier.cc
       GmIObinMgr.cc
       GmClassifierMgr.cc
       GmVDistribution.cc
       GmHisto1.cc
       GmHistoWriterROOT.cc
       GmParameterMessenger.cc
       GmParameterMgr.cc
       GmIOMgr.cc
       GmIOtextMgr.cc
       GmHistoWriterGraphics.cc
       GmGetParticleMgr.cc
 
   GRANULAR_DEPENDENCIES

   GLOBAL_DEPENDENCIES
    ${Geant4_LIBRARIES}
    ${Root_LIBRARIES}
  GamosCore_GamosUtils

   LINK_LIBRARIES
)

# List any source specific properties here
