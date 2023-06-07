# - GAMOS_analysis_ROOTUtilities_ghadd module build definition

include(UseGamosAtGeant4)

geant4_add_module(GAMOS_analysis_ROOTUtilities_ghadd
  PUBLIC_HEADERS
    TFileMerger.hh
  SOURCES
    TFileMerger.cc
    dummy.cc
)
geant4_module_link_libraries(GAMOS_analysis_ROOTUtilities_ghadd
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    ${SEAL_LIBRARIES} 
)
