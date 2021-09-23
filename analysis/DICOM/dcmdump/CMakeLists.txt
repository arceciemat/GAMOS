#------------------------------------------------------------------------------
# Module : dcmdump
# Package: DICOM_dcmdump
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
project(dcmdump)

include(UseGamosAtGeant4)

include_directories(${CMAKE_SOURCE_DIR}/../../)
include_directories(${PROJECT_SOURCE_DIR}/include/)
include_directories(${CMAKE_SOURCE_DIR}/source/GAMOS610.seal/analysis)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
add_executable(dcmdump dcmdump.cc ${sources} ${headers})
target_link_libraries(dcmdump 
	${Geant4_LIBRARIES} 
	${DCMTK_LIBRARIES} 
)

install(TARGETS dcmdump DESTINATION bin)

