#------------------------------------------------------------------------------
# Module : resizeImage
# Package: DICOM_resizeImage
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
project(resizeImage)

include(UseGamosAtGeant4)

include_directories(${CMAKE_SOURCE_DIR}/../../)
include_directories(${PROJECT_SOURCE_DIR}/include/)
include_directories(${CMAKE_SOURCE_DIR}/source/GAMOS610.seal/analysis)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
add_executable(resizeImage resizeImage.cc ${sources} ${headers})
target_link_libraries(resizeImage 
	${Geant4_LIBRARIES} 
	${Gamos_LIBRARIES}
	${ROOT_LIBRARIES} 
	DICOMBase
	DICOMReaders
)

install(TARGETS resizeImage DESTINATION bin)

