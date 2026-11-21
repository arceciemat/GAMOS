# - dcmdjp2k module build definition

gamos_add_module(dcmdjp2k
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

gamos_module_link_libraries(dcmdjp2k
  PUBLIC
    ${GAMOS_DCMTK_LIBRARIES}
)

# Program dcmdjp2k
gamos_add_executable(dcmdjp2k
  SOURCES
    dcmdjp2k.cc
  LINK_LIBRARIES
    dcmdjp2k
)
