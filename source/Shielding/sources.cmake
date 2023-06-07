# - Shielding module build definition

include(UseGamosAtGeant4)
include(UseROOT)

geant4_add_module(Shielding
  PUBLIC_HEADERS
    SHRadDecayChain.hh
    SHPlotPHPDBSpectraUA.hh
    GmNoUseG4RadDecayTimeUA.hh
    SHGetHadronicChannelXSUA.hh
    SHDecayChains.hh
    SHActivationFromFluxUA.hh
    SHMetastableIsotMgr.hh
    SHPrintPHPChannelXSUA.hh
    SHRadDecayChainSet.hh
    SHRadDecayScorerFromFlux.hh
    SHNthValueLayerUA.hh
    SHActivationUA.hh
    ShieldingVerbosity.hh
    GmNoUseG4RadDecayTimePrimaryUA.hh
    GmRadDecayGammaAngleCorrelationUA.hh
    GmSetG4RadDecayTime0UA.hh
    SHPrintPHPChannelThickYieldUA.hh
    SHUtils.hh
    SHDecayChainBuilder.hh
    SHPrintEMDataUA.hh
    SHStudyDecayChannelsUA.hh
  SOURCES
    SHPrintPHPChannelXSUA.cc
    SHRadDecayScorerFromFlux.cc
    SHRadDecayChainSet.cc
    SHUtils.cc
    ShieldingVerbosity.cc
    SHRadDecayChainBuilder.cc
    SHGetHadronicChannelXSUA.cc
    GmNoUseG4RadDecayTimeUA.cc
    plugin.cc
    SHActivationUA.cc
    SHStudyDecayChannelsUA.cc
    GmRadDecayGammaAngleCorrelationUA.cc
    SHPrintEMDataUA.cc
    SHPlotPHPDBSpectraUA.cc
    GmSetG4RadDecayTime0UA.cc
    GmNoUseG4RadDecayTimePrimaryUA.cc
    SHRadDecayChain.cc
    SHMetastableIsotMgr.cc
    SHActivationFromFluxUA.cc
    SHNthValueLayerUA.cc
    SHPrintPHPChannelThickYieldUA.cc
)
# - Add path to generated header
geant4_module_include_directories(Shielding
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealBase/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealPlatform/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/PluginManager/include>
 PUBLIC  $<BUILD_INTERFACE:${ROOT_BASE_DIR}/include>)

geant4_module_link_libraries(Shielding
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    ${SEAL_LIBRARIES} 
    GamosCore_GamosBase_Base
    GamosCore_GamosSD
    GamosCore_GamosUserActionMgr
    GamosCore_GamosUtils
    GamosCore_GamosReadDICOM
    GamosCore_GamosUtilsUA
    GamosCore_GamosScoring_Management
    GamosCore_GamosPhysics_Cuts
    GamosCore_GamosAnalysis
)
