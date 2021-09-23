#------------------------------------------------------------------------------
# Module : ghadd
# Package: ROOTUtilities_ghadd
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
project(ghadd)

include(UseGamosAtGeant4)

include_directories(${CMAKE_SOURCE_DIR}/../../)
include_directories(${PROJECT_SOURCE_DIR}/include/)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
add_executable(ghadd ghadd.cc ${sources} ${headers})
target_link_libraries(ghadd 
	${Geant4_LIBRARIES} 
	${Gamos_LIBRARIES}
	${ROOT_LIBRARIES} 
)
)

install(TARGETS ghadd DESTINATION bin)

