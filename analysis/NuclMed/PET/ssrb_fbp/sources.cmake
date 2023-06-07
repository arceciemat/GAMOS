# - GAMOS_analysis_NuclMed_PET_ssrb_fbp module build definition

include(UseGamosAtGeant4)

geant4_add_module(GAMOS_analysis_NuclMed_PET_ssrb_fbp
  PUBLIC_HEADERS
    VIP_lm2pd.hh
    VIP_ssrb_fbp.hh
    ssrb_fbp.hh
  SOURCES
    extra_TXT2FBP_outfile.cxx
    VIP_lm2pd.cxx
    dummy.cc
    VIP_ssrb_fbp.cxx
)
geant4_module_link_libraries(GAMOS_analysis_NuclMed_PET_ssrb_fbp
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    fftw3
    ${SEAL_LIBRARIES} 
)
