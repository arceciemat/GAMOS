# - dcmcjp2k module build definition

gamos_add_module(dcmcjp2k
  SOURCES
    djcodecd.cc
    djcodece.cc
    djcparam.cc
    djdecode.cc
    djencode.cc
    djrparam.cc
    djutils.cc
    memory_file.cc
)

gamos_module_link_libraries(dcmcjp2k
  PUBLIC
    ${GAMOS_DCMTK_LIBRARIES}
)

# Program dcmcjp2k
gamos_add_executable(dcmcjp2k
  SOURCES
    dcmcjp2k.cc
  LINK_LIBRARIES
    dcmcjp2k
)
