# - activationStudy module build definition

gamos_add_module(activationStudy
  SOURCES
    dummy.cc
)

gamos_module_link_libraries(activationStudy
  PUBLIC
    GamosCore_GamosUtils
)

# Program activationStudy
gamos_add_executable(activationStudy
  SOURCES
    activationStudy.cc
  LINK_LIBRARIES
    activationStudy
    GamosCore_GamosUtils
)
