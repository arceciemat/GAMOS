# - ssrb_fbp module build definition

gamos_add_module(ssrb_fbp
  PUBLIC_HEADERS
    VIP_lm2pd.hh
    VIP_ssrb_fbp.hh
    ssrb_fbp.hh
  SOURCES
    dummy.cc
)

# Program ssrb_fbp
gamos_add_executable(ssrb_fbp
  SOURCES
    ssrb_fbp.cc
  LINK_LIBRARIES
    ssrb_fbp
)
