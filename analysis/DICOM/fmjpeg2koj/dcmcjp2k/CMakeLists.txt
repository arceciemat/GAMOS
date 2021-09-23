#------------------------------------------------------------------------------
# Module : dcmcjp2k
# Package: DICOM_fmjpeg2koj_dcmcjp2k
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
project(dcmcjp2k)

include(UseGamosAtGeant4)

include_directories(${CMAKE_SOURCE_DIR}/../../)
include_directories(${PROJECT_SOURCE_DIR}/include/)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
add_executable(dcmcjp2k dcmcjp2k.cc ${sources} ${headers})
target_link_libraries(dcmcjp2k 
	${Geant4_LIBRARIES} 
	${Gamos_LIBRARIES}
	${ROOT_LIBRARIES} 
	XTRALIBS
	tiff
)

install(TARGETS dcmcjp2k DESTINATION bin)

