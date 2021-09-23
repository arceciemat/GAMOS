#------------------------------------------------------------------------------
# Module : getGammaIndex
# Package: DICOM_getGammaIndex
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
include_directories(${CMAKE_SOURCE_DIR}/source)
include_directories(${CMAKE_SOURCE_DIR}/include)
#
# Define the GAMOS Module.
include(UseGamosAtGeant4)
#
GEANT4_DEFINE_MODULE(NAME getGammaIndex
	 HEADERS
		DCMGetGammaIndex.hh
		DCM2DOperGammaIndex.hh
	 SOURCES
		DCM2DOperGammaIndex.cc
		DCMGetGammaIndex.cc
	 
	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
		${Geant4_LIBRARIES} 
		${GAMOS_LIBRARIES} 
		${SEAL_LIBRARIES} 
		${ROOT_LIBRARIES} 
		${DCMTK_LIBRARIES} 
	DICOMBase
	DICOM2G4
	DICOMReaders

	LINK_LIBRARIES
)
