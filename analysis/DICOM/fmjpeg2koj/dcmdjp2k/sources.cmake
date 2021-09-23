#------------------------------------------------------------------------------
# Module : dcmdjp2k
# Package: DICOM_fmjpeg2koj_dcmdjp2k
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
include_directories(${CMAKE_SOURCE_DIR}/source)
include_directories(${CMAKE_SOURCE_DIR}/include)
#
# Define the GAMOS Module.
include(UseGamosAtGeant4)
#
GEANT4_DEFINE_MODULE(NAME dcmdjp2k
	 HEADERS
	 SOURCES
		djdecode.cc
		djcodecd.cc
		djencode.cc
		djrparam.cc
		djutils.cc
		djcparam.cc
		memory_file.cc
		djcodece.cc
	 
	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
		${Geant4_LIBRARIES} 
		${GAMOS_LIBRARIES} 
		${SEAL_LIBRARIES} 
		${ROOT_LIBRARIES} 
		${DCMTK_LIBRARIES} 
	XTRALIBS
	tiff

	LINK_LIBRARIES
)
