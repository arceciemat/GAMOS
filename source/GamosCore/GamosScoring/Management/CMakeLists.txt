#------------------------------------------------------------------------------
# Module : Management
# Package: GamosCore_GamosScoring_Management
#
#------------------------------------------------------------------------------
# 
add_definitions(-DGmScoringMgt_ALLOC_EXPORT)
set(CMAKE_VERBOSE_MAKEFILE ON)
include(Geant4MacroLibraryTargets)
if(GEANT4_BUILD_GRANULAR_LIBS)
    GEANT4_GRANULAR_LIBRARY_TARGET(COMPONENTS sources.cmake)
else()
   GEANT4_GLOBAL_LIBRARY_TARGET(COMPONENTS sources.cmake)
endif()
