# - lm2pd module build definition

gamos_add_module(lm2pd
  PUBLIC_HEADERS
    lm2pd.hh
  SOURCES
    dummy.cc
)

# Program lm2pd
gamos_add_executable(lm2pd
  SOURCES
    lm2pd.cc
  LINK_LIBRARIES
    lm2pd
)
