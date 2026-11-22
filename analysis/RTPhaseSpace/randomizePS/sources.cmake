# - randomizePS module build definition

gamos_add_module(randomizePS
  SOURCES
    dummy.cc
)

gamos_module_link_libraries(randomizePS
  PUBLIC
    GamosCore_GamosBase_Base
    GamosCore_GamosRunManager
    GamosCore_GamosUtils
    RadioTherapy
)

# Program randomizePS
gamos_add_executable(randomizePS
  SOURCES
    randomizePS.cc
  LINK_LIBRARIES
    randomizePS
    GamosCore_GamosBase_Base
    GamosCore_GamosRunManager
    GamosCore_GamosUtils
    RadioTherapy
)
