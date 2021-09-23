#------------------------------------------------------------------------------
# Module : Shielding
# Package: Shielding
#
#------------------------------------------------------------------------------
#
set(CMAKE_VERBOSE_MAKEFILE ON)
include_directories(${CMAKE_SOURCE_DIR}/source)
include_directories(${CMAKE_SOURCE_DIR}/include)
#
# Define the GEANT4 Module.
include(UseGamosAtGeant4)
#
GEANT4_DEFINE_MODULE(NAME Shielding
	 HEADERS
		SHPrintEMDataUA.hh
		SHDecayChainBuilder.hh
		SHActivationFromFluxUA.hh
		SHRadDecayChainSet.hh
		SHMetastableIsotMgr.hh
		ShieldingVerbosity.hh
		SHActivationUA.hh
		SHRadDecayScorerFromFlux.hh
		SHPlotPHPDBSpectraUA.hh
		SHPrintPHPChannelThickYieldUA.hh
		SHNthValueLayerUA.hh
		SHRadDecayChain.hh
		SHUtils.hh
		SHStudyDecayChannelsUA.hh
		SHDecayChains.hh
		GmRadDecayGammaAngleCorrelationUA.hh
		SHGetHadronicChannelXSUA.hh
		SHPrintPHPChannelXSUA.hh
		GmNoUseG4RadDecayTimeUA.hh
		GmNoUseG4RadDecayTimePrimaryUA.hh
		GmSetG4RadDecayTime0UA.hh
	 SOURCES
		plugin.cc
		SHActivationFromFluxUA.cc
		SHMetastableIsotMgr.cc
		SHUtils.cc
		SHGetHadronicChannelXSUA.cc
		SHStudyDecayChannelsUA.cc
		SHPlotPHPDBSpectraUA.cc
		SHActivationUA.cc
		SHNthValueLayerUA.cc
		SHRadDecayChainBuilder.cc
		SHRadDecayChain.cc
		GmNoUseG4RadDecayTimeUA.cc
		GmNoUseG4RadDecayTimePrimaryUA.cc
		SHRadDecayScorerFromFlux.cc
		SHPrintPHPChannelXSUA.cc
		SHRadDecayChainSet.cc
		SHPrintPHPChannelThickYieldUA.cc
		ShieldingVerbosity.cc
		GmRadDecayGammaAngleCorrelationUA.cc
		SHPrintEMDataUA.cc
		GmSetG4RadDecayTime0UA.cc	 

	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
		${Geant4_libs} 
		${ROOT_LIBRARIES} 
	GamosCore_GamosBase_Base
	GamosCore_GamosSD
	GamosCore_GamosUserActionMgr
	GamosCore_GamosUtils
	GamosCore_GamosReadDICOM
	GamosCore_GamosUtilsUA
	GamosCore_GamosScoring_Management
	GamosCore_GamosPhysics_Cuts
	GamosCore_GamosAnalysis
		${SEAL_LIBRARIES} 

	LINK_LIBRARIES
)
