#------------------------------------------------------------------------------
# Module : OtherGmG4EmDNAPhysicss
# Package: GamosCore_GamosPhysics_OtherGmG4EmDNAPhysicss
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
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosPhysics_OtherGmG4EmDNAPhysicss
	 HEADERS
		GmQGSP_BIC_HP_EMopt3.icc
		PhysListEmStandardSS.hh
		GmDummyPhysics.hh
		GmEmDNAPhysicsMessenger.hh
		GmG4EmDNAPhysics.hh
		GmEmDNAPhysics.hh
		GmQGSP_BIC_HP_EMopt3.hh
	 SOURCES
		plugin.cc
		GmEmDNAPhysics.cc
		PhysListEmStandardSS.cc
		GmEmDNAPhysicsMessenger.cc
		GmDummyPhysics.cc
		GmG4EmDNAPhysics.cc
	 
	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
		${Geant4_libs} 
		${ROOT_LIBRARIES} 
	GamosCore_GamosUtils
	GamosCore_GamosPhysics_GmG4EmDNAPhysics
		${SEAL_LIBRARIES} 

	LINK_LIBRARIES
)
