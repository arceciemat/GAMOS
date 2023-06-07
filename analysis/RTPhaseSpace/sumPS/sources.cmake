# - GAMOS_analysis_RTPhaseSpace_sumPS module build definition

include(UseGamosAtGeant4)

geant4_add_module(GAMOS_analysis_RTPhaseSpace_sumPS
  PUBLIC_HEADERS
    utilities.h
    egsclass.h
    histogram.h
    iaea_record.hh
    matrix.h
    iaea_utilities.hh
    iaea_config.hh
    iaea_header.hh
    general.h
    FILE2MEMORY.h
  SOURCES
    histogram.cc
    iaea_utilities.cc
    iaea_header.cc
    general.cc
    utilities.cc
    egsclass.cc
    FILE2MEMORY.cc
    iaea_record.cc
    matrix.cc
)
geant4_module_link_libraries(GAMOS_analysis_RTPhaseSpace_sumPS
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    GamosCore_GamosBase_Base
    ${SEAL_LIBRARIES} 
)
