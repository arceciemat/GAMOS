#------------------------------------------------------------------------------
# Module : sumPS
# Package: RTPhaseSpace_sumPS
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
GEANT4_DEFINE_MODULE(NAME sumPS
         HEADERS
                iaea_config.hh
                FILE2MEMORY.h
                histogram.h
                iaea_utilities.hh
                egsclass.h
                general.h
                utilities.h
                iaea_header.hh
                iaea_record.hh
                matrix.h
         SOURCES
               histogram.cc
                iaea_record.cc
                utilities.cc
                egsclass.cc
                #egsclass.cc#
                iaea_header.cc
                iaea_utilities.cc
                general.cc
                FILE2MEMORY.cc
                matrix.cc
         
         GRANULAR_DEPENDENCIES
         GLOBAL_DEPENDENCIES
                ${Geant4_LIBRARIES} 
                ${GAMOS_LIBRARIES} 
                ${SEAL_LIBRARIES} 
                ${ROOT_LIBRARIES} 
        GamosCore_GamosBase_Base

        LINK_LIBRARIES
)
