#------------------------------------------------------------------------------
# Module : Detector
# Package: Detector_PET_ssrb_fbp
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
project(ssrb_fbp)

include(UseGamosAtGeant4)
include(UseFFTW)

include_directories(${CMAKE_SOURCE_DIR}/../../)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
add_executable(ssrb_fbp ssrb_fbp.cc ${sources} ${headers})
target_link_libraries(ssrb_fbp 
${Geant4_LIBRARIES} 
${Gamos_LIBRARIES}
${ROOT_LIBRARIES} 
${dlfcn_LIBRARIES}
${pcre_LIBRARIES}
${FFTW_LIBRARIES}
)

install(TARGETS ssrb_fbp DESTINATION bin)

