#------------------------------------------------------------------------------
# Module : Detector
# Package: Detector_PET_sumProjdata
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
project(sumProjdata)
 
include(UseGamosAtGeant4)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
add_executable(sumProjdata sumProjdata.cc ${sources} ${headers})
target_link_libraries(sumProjdata 
${Geant4_LIBRARIES} 
${Gamos_LIBRARIES}
${ROOT_LIBRARIES} 
${dlfcn_LIBRARIES}
${pcre_LIBRARIES} 
)

install(TARGETS sumProjdata DESTINATION bin)

