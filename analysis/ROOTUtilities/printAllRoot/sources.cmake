# - printAllRoot module build definition

gamos_add_module(printAllRoot
  SOURCES
    dummy.cc
)

gamos_module_link_libraries(printAllRoot
  PUBLIC
    GamosCore_GamosUtils
)

# Program printAllRoot
gamos_add_executable(printAllRoot
  SOURCES
    printAllRoot.cc
  LINK_LIBRARIES
    printAllRoot
    GamosCore_GamosUtils
)
