#------------------------------------------------------------------------------
# Module : Detector
# Package: Detector_sqdoseToInterfile
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
project(Detector)

include(UseGamosAtGeant4)

include_directories(${CMAKE_SOURCE_DIR}/../../)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
add_executable(Detector Detector.cc ${sources} ${headers})
target_link_libraries(Detector 
${Geant4_LIBRARIES} 
${Gamos_LIBRARIES}
${ROOT_LIBRARIES} 
${dlfcn_LIBRARIES}
${pcre_LIBRARIES}
)

install(TARGETS Detector DESTINATION bin)

