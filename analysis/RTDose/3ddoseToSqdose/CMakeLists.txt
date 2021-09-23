#------------------------------------------------------------------------------
# Module : RTDose
# Package: RTDose_3ddoseToSqdose
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
project(3ddoseToSqdose)

include(UseGamosAtGeant4)

include_directories(${CMAKE_SOURCE_DIR}/../../)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
add_executable(3ddoseToSqdose 3ddoseToSqdose.cc ${sources} ${headers})
target_link_libraries(3ddoseToSqdose 
${Geant4_LIBRARIES} 
${Gamos_LIBRARIES}
${ROOT_LIBRARIES} 
${dlfcn_LIBRARIES}
${pcre_LIBRARIES}
)

install(TARGETS 3ddoseToSqdose DESTINATION bin)

