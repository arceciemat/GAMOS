#------------------------------------------------------------------------------
# 
# Module : GamosCore
# Package: GamosUtils
#
#------------------------------------------------------------------------------
include(Geant4MacroLibraryTargets)

# Add allocation export symbol for this category
add_definitions(-DGmUtils_ALLOC_EXPORT)

if(GAMOS_BUILD_GRANULAR_LIBS)
   GAMOS_GRANULAR_LIBRARY_TARGET(COMPONENT sources.cmake)
else()
   GEANT4_GLOBAL_LIBRARY_TARGET(COMPONENTS sources.cmake)
endif()
