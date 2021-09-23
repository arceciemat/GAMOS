#------------------------------------------------------------------------------
# Module : dcmdjp2k
# Package: DICOM_fmjpeg2koj_dcmdjp2k
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
project(dcmdjp2k)

include(UseGamosAtGeant4)

include_directories(${CMAKE_SOURCE_DIR}/../../)
include_directories(${PROJECT_SOURCE_DIR}/include/)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
add_executable(dcmdjp2k dcmdjp2k.cc ${sources} ${headers})
target_link_libraries(dcmdjp2k 
	${Geant4_LIBRARIES} 
	${Gamos_LIBRARIES}
	${ROOT_LIBRARIES} 
	XTRALIBS
	tiff
)

install(TARGETS dcmdjp2k DESTINATION bin)

