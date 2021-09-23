#------------------------------------------------------------------------------
# Module : RTDose
# Package: RTDose_sqdoseTo3ddose
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
project(sqdoseTo3ddose)

include(UseGamosAtGeant4)

include_directories(${CMAKE_SOURCE_DIR}/../../)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
add_executable(sqdoseTo3ddose sqdoseTo3ddose.cc ${sources} ${headers})
target_link_libraries(sqdoseTo3ddose 
${Geant4_LIBRARIES} 
${Gamos_LIBRARIES}
${ROOT_LIBRARIES} 
${dlfcn_LIBRARIES}
${pcre_LIBRARIES}
)

install(TARGETS sqdoseTo3ddose DESTINATION bin)

