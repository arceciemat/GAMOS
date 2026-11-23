# - ghadd module build definition

gamos_add_module(ghadd
  PUBLIC_HEADERS
    TFileMerger.hh
  SOURCES
    TFileMerger.cc
    dummy.cc
)

# Program ghadd
gamos_add_executable(ghadd
  SOURCES
    ghadd.cc
  LINK_LIBRARIES
    ghadd
)
