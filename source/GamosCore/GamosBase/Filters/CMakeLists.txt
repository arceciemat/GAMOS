#------------------------------------------------------------------------------
# Module : Filters
# Package: GamosCore_GamosBase_Filters
#
#------------------------------------------------------------------------------
# 
add_definitions(-DGmBaseFilters_ALLOC_EXPORT)
#add_definitions(-DGAMOS_NO_ROOT)

set(CMAKE_VERBOSE_MAKEFILE ON)
include(Geant4MacroLibraryTargets)
if(GEANT4_BUILD_GRANULAR_LIBS)
    GEANT4_GRANULAR_LIBRARY_TARGET(COMPONENTS sources.cmake)
else()
   GEANT4_GLOBAL_LIBRARY_TARGET(COMPONENTS sources.cmake)
endif()
