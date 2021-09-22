#------------------------------------------------------------------------------
# Module : DICOMReaders
# Package: DICOM_DICOMReaders
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)

add_definitions(-DDICOMReaders_ALLOC_EXPORT)

include(Geant4MacroLibraryTargets)
if(GEANT4_BUILD_GRANULAR_LIBS)
    GEANT4_GRANULAR_LIBRARY_TARGET(COMPONENTS sources.cmake)
else()
   GEANT4_GLOBAL_LIBRARY_TARGET(COMPONENTS sources.cmake)
endif()
