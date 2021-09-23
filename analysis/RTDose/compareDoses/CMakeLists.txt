#------------------------------------------------------------------------------
# Module : RTDose
# Package: RTDose_compareDoses
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
project(compareDoses)

include(UseGamosAtGeant4)

include_directories(${CMAKE_SOURCE_DIR}/../../)
include_directories(${PROJECT_SOURCE_DIR}/include/)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
add_executable(compareDoses compareDoses.cc ${sources} ${headers})
target_link_libraries(compareDoses 
${Geant4_LIBRARIES} 
${Gamos_LIBRARIES}
${ROOT_LIBRARIES} 
${dlfcn_LIBRARIES}
${pcre_LIBRARIES}
)

install(TARGETS compareDoses DESTINATION bin)

