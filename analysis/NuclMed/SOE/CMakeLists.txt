#------------------------------------------------------------------------------
# Module : Detector
# Package: Detector_SOE
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
project(SOE)

include(UseGamosAtGeant4)

include_directories(${CMAKE_SOURCE_DIR}/../../)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
add_executable(SOE SOE.cc ${sources} ${headers})
target_link_libraries(SOE 
${Geant4_LIBRARIES} 
${Gamos_LIBRARIES}
${ROOT_LIBRARIES} 
${dlfcn_LIBRARIES}
${pcre_LIBRARIES}
)

install(TARGETS SOE DESTINATION bin)

