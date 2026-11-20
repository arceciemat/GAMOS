# - source

add_subdirectory(SEAL_Foundation)
add_subdirectory(MagFieldManager)
add_subdirectory(GamosCore)
add_subdirectory(RadioTherapy)
add_subdirectory(NuclMed)
add_subdirectory(Shielding)
if(GAMOS_USE_DCMTK)
  add_subdirectory(DICOM)
endif()
add_subdirectory(DNA)
