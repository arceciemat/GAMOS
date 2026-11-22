# - sumSqdoseDisp module build definition

gamos_add_module(sumSqdoseDisp
  PUBLIC_HEADERS
    Merge3ddoseUA.hh
  SOURCES
    dummy.cc
)

gamos_module_link_libraries(sumSqdoseDisp
  PUBLIC
    GamosCore_GamosReadDICOM
    GamosCore_GamosUserActionMgr
    GamosCore_GamosUtils
)

# Program sumSqdoseDisp
gamos_add_executable(sumSqdoseDisp
  SOURCES
    sumSqdoseDisp.cc
  LINK_LIBRARIES
    sumSqdoseDisp
    GamosCore_GamosReadDICOM
    GamosCore_GamosUserActionMgr
    GamosCore_GamosUtils
)
