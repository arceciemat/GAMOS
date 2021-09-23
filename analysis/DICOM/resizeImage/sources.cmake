#------------------------------------------------------------------------------
# Module : resizeImage
# Package: DICOM_resizeImage
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
include_directories(${CMAKE_SOURCE_DIR}/source)
include_directories(${CMAKE_SOURCE_DIR}/include)
#
# Define the GAMOS Module.
include(UseGamosAtGeant4)
#
GEANT4_DEFINE_MODULE(NAME resizeImage
	 HEADERS
		DCMResizeImage.hh
	 SOURCES
		DCMResizeImage.cc
	 
	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
		${Geant4_LIBRARIES} 
		${GAMOS_LIBRARIES} 
		${SEAL_LIBRARIES} 
		${ROOT_LIBRARIES} 
		${DCMTK_LIBRARIES} 
	DICOMBase
	DICOMReaders
	DICOM2G4

	LINK_LIBRARIES
)
