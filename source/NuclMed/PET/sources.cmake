#------------------------------------------------------------------------------
# Module : PET
# Package: NuclMed_PET
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
GEANT4_DEFINE_MODULE(NAME NuclMed_PET
	 HEADERS
		PETVComptClassificationVariable.hh
		PETIOMgr.hh
		PETVerbosity.hh
		PETProjDataMgr.hh
		PETIOMgrTOF.hh
		PETEventClassifierUA.hh
		PETPrintPositron.hh
		PETVComptAlgorithmVariable.hh
		PETHistosPositron.hh
		PETEventClassifierTOFUA.hh
	 SOURCES
		PETEventClassifierTOFUA.cc
		plugin.cc
		PETPrintPositron.cc
		PETVerbosity.cc
		PETEventClassifierUA.cc
		PETHistosPositron.cc
		PETProjDataMgr.cc
		PETIOMgrTOF.cc
		PETIOMgr.cc
	 
	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
		${Geant4_libs} 
		${ROOT_LIBRARIES} 
		${SEAL_LIBRARIES} 
		${Gamos_LIBRARIES} 
		NuclMed_Base
	
	LINK_LIBRARIES
)
