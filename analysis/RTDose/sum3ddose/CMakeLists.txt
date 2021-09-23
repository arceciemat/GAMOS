#------------------------------------------------------------------------------
# Module : RTDose
# Package: RTDose_sum3ddose
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
project(sum3ddose)

include(UseGamosAtGeant4)

include_directories(${CMAKE_SOURCE_DIR}/../../)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
add_executable(sum3ddose sum3ddose.cc ${sources} ${headers})
target_link_libraries(sum3ddose 
${Geant4_LIBRARIES} 
${Gamos_LIBRARIES}
${ROOT_LIBRARIES} 
${dlfcn_LIBRARIES}
${pcre_LIBRARIES}
)

install(TARGETS sum3ddose DESTINATION bin)

