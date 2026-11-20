# - sumProjdata module build definition

gamos_add_module(sumProjdata
  PUBLIC_HEADERS
    sumProjdata.hh
  SOURCES
    dummy.cc
)

# Program sumProjdata
gamos_add_executable(sumProjdata
  SOURCES
    sumProjdata.cc
  LINK_LIBRARIES
    sumProjdata
    GamosCore_GamosUtils
)
