# - dcmdump module build definition

gamos_add_module(dcmdump
  SOURCES
    dummy.cc
)

gamos_module_link_libraries(dcmdump
  PUBLIC
    ${GAMOS_DCMTK_LIBRARIES}
)

# Program dcmdump
gamos_add_executable(dcmdump
  SOURCES
    dcmdump.cc
  LINK_LIBRARIES
    dcmdump
)
