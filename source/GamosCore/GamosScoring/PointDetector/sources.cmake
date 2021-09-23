#------------------------------------------------------------------------------
# Module : PointDetector
# Package: GamosCore_GamosScoring_PointDetector
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
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosScoring_PointDetector
	 HEADERS
		GmPDSInvertGeantinoStackUA.hh
		GmPDSDetector.hh
		GmPDSOpticalPhotonProcess.hh
		GmPDSCreateAngleTablesUA.hh
		GmPDSNeutronProcess.hh
		GmPDSInteractionAngleManager.hh
		GmPDSGeantinoProcess.hh
		GmPDSVProcess.hh
		GmPDSUtils.hh
		GmPDSScoringTrackInfo.hh
		GmPDSProcessHelper.hh
		GmPDSUA.hh
		GmPDSGammaProcess.hh
		GmPDSScore.hh
	 SOURCES
		plugin.cc
		GmPDSProcessHelper.cc
		GmPDSUA.cc
		GmPDSGeantinoProcess.cc
		GmPDSUtils.cc
		GmPDSVProcess.cc
		GmPDSNeutronProcess.cc
		GmPDSCreateAngleTablesUA.cc
		GmPDSGammaProcess.cc
		GmPDSInvertGeantinoStackUA.cc
		GmPDSOpticalPhotonProcess.cc
		GmPDSInteractionAngleManager.cc
		GmPDSScore.cc
		GmPDSDetector.cc
	 
	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
		${Geant4_libs} 
		${ROOT_LIBRARIES} 
		${SEAL_LIBRARIES} 
		GamosCore_GamosUtils
		GamosCore_GamosBase_Base
		GamosCore_GamosGeometry
		GamosCore_GamosReadDICOM
		GamosCore_GamosGenerator
		GamosCore_GamosData_Management
		GamosCore_GamosData_Users
		GamosCore_GamosScoring_Management

	LINK_LIBRARIES
)
