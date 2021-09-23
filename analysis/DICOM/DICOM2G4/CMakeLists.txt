#------------------------------------------------------------------------------
# Module : DICOM2G4
# Package: DICOM_DICOM2G4
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
project(DICOM2G4) 

include(UseGamosAtGeant4)

include_directories(${CMAKE_SOURCE_DIR}/../../)
include_directories(${PROJECT_SOURCE_DIR}/include/)
include_directories(${CMAKE_SOURCE_DIR}/source/GAMOS610.seal/analysis)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
add_executable(DICOM2G4 DICOM2G4.cc ${sources} ${headers})

target_link_libraries(DICOM2G4 
	${Geant4_LIBRARIES} 
	${Gamos_LIBRARIES}
	${ROOT_LIBRARIES} 
	DICOMReaders
	DICOMBase
)

install(TARGETS DICOM2G4 DESTINATION bin)

