# - analysis

add_subdirectory(analyseOutput)
add_subdirectory(NuclMed)
add_subdirectory(RTDose)
add_subdirectory(RTPhaseSpace)
if(GAMOS_USE_DCMTK)
  add_subdirectory(DICOM)
endif()
add_subdirectory(Shielding)
if(GAMOS_USE_ROOT)
  add_subdirectory(ROOTUtilities)
endif()
