# - GamosCore_GamosBase_Base module build definition

include(UseGamosAtGeant4)
include(UseROOT)

geant4_add_module(GamosCore_GamosBase_Base
  PUBLIC_HEADERS
    GmFilterMgr.hh
    GmParameter.hh
    GmParameterList.icc
    GmDistributionMgr.hh
    GmAnalysisMessenger.hh
    GmIOtextMgr.hh
    GmHistoWriterGraphics.hh
    GmParameterListNumeric.hh
    GmUIcmdWithAString.hh
    GmHistoWriter.hh
    GmParameterMgr.hh
    GmFilterFactory.hh
    GmTrackInfo.hh
    GmHisto2.hh
    GmParameterListNumeric.icc
    GmVVerbosity.hh
    GmBaseVerbosity.hh
    GmChangeEnergies.hh
    GmHistoWriterTXT.hh
    GmHistoProfile2.hh
    GmClassifierMgr.hh
    GmIOMgr.hh
    GmAnalysisMgr.hh
    GmBaseMessenger.hh
    GmParameterListVNumeric.icc
    GmVisManager.hh
    GmHisto1.hh
    GmVClassifier.hh
    GmVerbosityFactory.hh
    GmConvergenceTester.hh
    GmParameterMessenger.hh
    GmDistributionFactory.hh
    GmVFilter.hh
    GmHistoWriterCSV.hh
    GmVDistribution.hh
    GmParameterList.hh
    GmParameterListVNumeric.hh
    GmHistoWriterROOT.hh
    GmGetParticleMgr.hh
    GmHistoProfile1.hh
    GmHistoReaderCSV.hh
    GmIObinMgr.hh
    GmClassifierFactory.hh
  SOURCES
    GmBaseMessenger.cc
    GmUIcmdWithAString.cc
    GmHistoProfile1.cc
    GmAnalysisMessenger.cc
    GmBaseVerbosity.cc
    GmIOtextMgr.cc
    GmClassifierMgr.cc
    GmVClassifier.cc
    GmAnalysisMgr.cc
    GmIObinMgr.cc
    GmFilterFactory.cc
    GmHistoWriterGraphics.cc
    GmVDistribution.cc
    GmTrackInfo.cc
    GmGetParticleMgr.cc
    plugin.cc
    GmParameterMessenger.cc
    GmDistributionFactory.cc
    GmHisto2.cc
    GmHisto1.cc
    GmFilterMgr.cc
    GmVFilter.cc
    GmHistoReaderCSV.cc
    GmHistoWriterCSV.cc
    GmConvergenceTester.cc
    GmParameterMgr.cc-
    GmParameterMgr.cc
    GmHistoProfile2.cc
    GmIOMgr.cc
    GmChangeEnergies.cc
    GmVerbosityFactory.cc
    GmHistoWriterTXT.cc
    GmVVerbosity.cc
    GmDistributionMgr.cc
    GmClassifierFactory.cc
    GmHistoWriterROOT.cc
)
# - Add path to generated header
geant4_module_include_directories(GamosCore_GamosBase_Base
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealBase/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealPlatform/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/PluginManager/include>
 PUBLIC  $<BUILD_INTERFACE:${ROOT_BASE_DIR}/include>)

geant4_module_link_libraries(GamosCore_GamosBase_Base
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    ${SEAL_LIBRARIES} 
    GamosCore_GamosUtils
)
