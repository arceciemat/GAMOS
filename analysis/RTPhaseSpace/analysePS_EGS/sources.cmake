#------------------------------------------------------------------------------
# Module : analysePS_EGS
# Package: RTPhaseSpace_analysePS_EGS
#
#------------------------------------------------------------------------------
# 
set(CMAKE_VERBOSE_MAKEFILE ON)
include_directories(${CMAKE_SOURCE_DIR}/source)
include_directories(${CMAKE_SOURCE_DIR}/include)
#
# Define the GAMOS Module.
include(UseGamosAtGeant4)
#
GEANT4_DEFINE_MODULE(NAME analysePS_EGS
         HEADERS
         SOURCES
                dummy.cc
         
         GRANULAR_DEPENDENCIES
         GLOBAL_DEPENDENCIES
                ${Geant4_LIBRARIES} 
                ${GAMOS_LIBRARIES} 
                ${SEAL_LIBRARIES} 
                ${ROOT_LIBRARIES} 
        RadioTherapy
        gamos

        LINK_LIBRARIES
)
