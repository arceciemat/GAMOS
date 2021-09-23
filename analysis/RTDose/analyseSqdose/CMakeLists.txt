#------------------------------------------------------------------------------
# Module : analyseSqdose
# Package: RTPhaseSpace_analyseSqdose
#
#
set(CMAKE_VERBOSE_MAKEFILE ON)
project(analyseSqdose)
 
include(UseGamosAtGeant4)

include_directories(${CMAKE_SOURCE_DIR}/../../)
include_directories(${PROJECT_SOURCE_DIR}/include/)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
add_executable(analyseSqdose analyseSqdose.cc ${sources} ${headers})
target_link_libraries(analyseSqdose 
${Geant4_LIBRARIES} 
${Gamos_LIBRARIES}
${ROOT_LIBRARIES} 
${dlfcn_LIBRARIES}
${pcre_LIBRARIES} 
RadioTherapy
)

install(TARGETS analyseSqdose DESTINATION bin)

