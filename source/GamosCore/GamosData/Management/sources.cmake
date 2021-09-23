#------------------------------------------------------------------------------
# Module : Management
# Package: GamosCore_GamosData_Management
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
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosData_Management
	 HEADERS
		GmDataAncestorStepStringData.hh
		GmVData.hh
		GmVDataProf1DHisto.hh
		GmDataFactory.hh
		GmDataAncestorStepNumericData.hh
		GmVDataProf2DHisto.hh
		GmDataMgr.hh
		GmDataVerbosity.hh
		GmVDataNon1DHisto.hh
		GmVDataString.hh
		GmVData2DHisto.hh
		GmVDataAccumulated.hh
		GmVDataInt.hh
	 SOURCES
		GmVDataNon1DHisto.cc
		GmVDataString.cc
		plugin.cc
		GmVDataInt.cc
		GmDataMgr.cc
		GmVData.cc
		GmDataVerbosity.cc
		GmVData2DHisto.cc
		GmDataAncestorStepNumericData.cc
		GmVDataProf2DHisto.cc
		GmVDataAccumulated.cc
		GmVDataProf1DHisto.cc
		GmDataAncestorStepStringData.cc
		GmDataFactory.cc
	 
	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
		${Geant4_libs} 
		${ROOT_LIBRARIES} 
	GamosCore_GamosBase_Base
	GamosCore_GamosUserActionMgr
		${SEAL_LIBRARIES} 

	LINK_LIBRARIES
)
