# - SealModuleSymCheck module build definition

gamos_add_module(SealModuleSymCheck
  SOURCES
    SealModuleLoadCheck.cc
    dummy.cc
)

gamos_module_link_libraries(SealModuleSymCheck
  PUBLIC
    lcg_SealBase
)

# Program SealModuleSymCheck
gamos_add_executable(SealModuleSymCheck
  SOURCES
    SealModuleSymCheck.cc
  LINK_LIBRARIES
    SealModuleSymCheck
    lcg_SealBase
)
