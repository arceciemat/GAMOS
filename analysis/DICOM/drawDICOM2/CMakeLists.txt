#------------------------------------------------------------------------------
# Module : drawDICOM
# Package: DICOM_drawDICOM
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
project(drawDICOM) 

include(UseGamosAtGeant4)

include_directories(${CMAKE_SOURCE_DIR}/../../)
include_directories(${PROJECT_SOURCE_DIR}/include/)
include_directories(${CMAKE_SOURCE_DIR}/source/GAMOS610.seal/analysis)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
add_executable(drawDICOM drawDICOM.cc ${sources} ${headers})
target_link_libraries(drawDICOM 
	${Geant4_LIBRARIES} 
	${Gamos_LIBRARIES}
	${ROOT_LIBRARIES} 
	DICOMBase
	DICOMReaders
)

install(TARGETS drawDICOM DESTINATION bin)

