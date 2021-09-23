#------------------------------------------------------------------------------
# Module : GamosToDICOM
# Package: DICOM_GamosToDICOM
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
include_directories(${CMAKE_SOURCE_DIR}/source)
include_directories(${CMAKE_SOURCE_DIR}/include)
#
# Define the GAMOS Module.
include(UseGamosAtGeant4)
#
GEANT4_DEFINE_MODULE(NAME GamosToDICOM
	 HEADERS
		DCMGamosToDICOM.hh
		DCMSqToRT.hh.old
	 SOURCES
		DCMGamosToDICOM.cc
	 
	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
		${Geant4_LIBRARIES} 
		${GAMOS_LIBRARIES} 
		${SEAL_LIBRARIES} 
		${ROOT_LIBRARIES} 
		${DCMTK_LIBRARIES} 
	DICOMBase
	DICOMReaders

	LINK_LIBRARIES
)
