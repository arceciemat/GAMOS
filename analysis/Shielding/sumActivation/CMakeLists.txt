#------------------------------------------------------------------------------
# Module : Shielding
# Package: Shielding_sumActivation
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
project(sumActivation)

include(UseGamosAtGeant4)

include_directories(${PROJECT_SOURCE_DIR}/include/)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
add_executable(sumActivation sumActivation.cc ${sources} ${headers})
target_link_libraries(sumActivation 
${Geant4_LIBRARIES} 
${Gamos_LIBRARIES}
${ROOT_LIBRARIES} 
${dlfcn_LIBRARIES}
${pcre_LIBRARIES}
Shielding
)

install(TARGETS sumActivation DESTINATION bin)

