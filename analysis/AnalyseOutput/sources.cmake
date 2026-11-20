# - analyseOutput module build definition

gamos_add_module(analyseOutput
  PUBLIC_HEADERS
    GmAOData.hh
    GmAODataSearch.hh
    GmAOParam.hh
  SOURCES
    GmAOData.cc
    GmAODataSearch.cc
)

gamos_module_link_libraries(analyseOutput
  PUBLIC
    GamosCore_GamosUtils
)

# Program analyseOutput
gamos_add_executable(analyseOutput
  SOURCES
    analyseOutput.cc
  LINK_LIBRARIES
    analyseOutput
    GamosCore_GamosUtils
)
