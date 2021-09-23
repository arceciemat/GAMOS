#------------------------------------------------------------------------------
# Module : printValueAlongLine
# Package: DICOM_printValueAlongLine
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
project(printValueAlongLine)

include(UseGamosAtGeant4)

include_directories(${CMAKE_SOURCE_DIR}/../../)
include_directories(${PROJECT_SOURCE_DIR}/include/)
include_directories(${CMAKE_SOURCE_DIR}/source/GAMOS610.seal/analysis)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
add_executable(printValueAlongLine printValueAlongLine.cc ${sources} ${headers})
target_link_libraries(printValueAlongLine 
	${Geant4_LIBRARIES} 
	${Gamos_LIBRARIES}
	${ROOT_LIBRARIES} 
	DICOMBase
	DICOMReaders
)

install(TARGETS printValueAlongLine DESTINATION bin)

