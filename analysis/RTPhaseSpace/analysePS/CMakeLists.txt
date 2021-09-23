#------------------------------------------------------------------------------
# Module : analysePS
# Package: RTPhaseSpace_analysePS
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
project(analysePS)

include(UseGamosAtGeant4)

include_directories(${CMAKE_SOURCE_DIR}/../../)
include_directories(${PROJECT_SOURCE_DIR}/include/)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
add_executable(analysePS analysePS.cc ${sources} ${headers})
target_link_libraries(analysePS 
        ${Geant4_LIBRARIES} 
        ${Gamos_LIBRARIES}
        ${ROOT_LIBRARIES} 
		RadioTherapy
)

install(TARGETS analysePS DESTINATION bin)
