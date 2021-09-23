#------------------------------------------------------------------------------
# Module : sumPS
# Package: RTPhaseSpace_sumPS
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
project(sumPS)

include(UseGamosAtGeant4)

include_directories(${CMAKE_SOURCE_DIR}/../../)
include_directories(${PROJECT_SOURCE_DIR}/include/)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
add_executable(sumPS sumPS.cc ${sources} ${headers})
target_link_libraries(sumPS 
        ${Geant4_LIBRARIES} 
        ${Gamos_LIBRARIES}
        ${ROOT_LIBRARIES} 
		RadioTherapy
)

install(TARGETS sumPS DESTINATION bin)

