#------------------------------------------------------------------------------
# Module : NuclMed
# Package: NuclMed_analyseSPECT
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
project(analyseSPECT)

include(UseGamosAtGeant4)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/../../)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
add_executable(analyseSPECT analyseSPECT.cc ${sources} ${headers})
target_link_libraries(analyseSPECT 
${Geant4_LIBRARIES} 
${Gamos_LIBRARIES}
${ROOT_LIBRARIES} 
${dlfcn_LIBRARIES}
${pcre_LIBRARIES}
NuclMed_SPECT
)

install(TARGETS analyseSPECT DESTINATION bin)

