# - root2csv module build definition

gamos_add_module(root2csv
  SOURCES
    touch.cc
)

gamos_module_link_libraries(root2csv
  PUBLIC
    GamosCore_GamosBase_Base
)

# Program root2csv
gamos_add_executable(root2csv
  SOURCES
    root2csv.cc
  LINK_LIBRARIES
    root2csv
    GamosCore_GamosBase_Base
)
