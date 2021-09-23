#------------------------------------------------------------------------------
# Module : DICOM2G4
# Package: DICOM_DICOM2G4
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
include_directories(${CMAKE_SOURCE_DIR}/source)
include_directories(${CMAKE_SOURCE_DIR}/include)
#
#
# Define the GAMOS Module.
include(UseGamosAtGeant4)
#
GEANT4_DEFINE_MODULE(NAME DICOM2G4
	 HEADERS
		DicomBeamDevice.hh
		DicomBeamWedge.hh
		DicomBeamDeviceBLD.hh
		DicomBeamCompensator.hh
		DicomReaderRTIonPlan.hh
		Dicom2G4FileMgr.hh
		DicomBeamDevicePos.hh
		DicomBeam.hh
		DicomBeamBlock.hh
		DicomBeamRTIonControlPoint.hh
		DicomVBeamRTControlPoint.hh
		DicomVBeamDevice.hh
		DicomBeamRTControlPoint.hh
		DicomReaderRTPlan.hh
	 SOURCES
		DicomReaderRTIonPlan.cc
		DicomBeam.cc
		Dicom2G4FileMgr.cc
		DicomBeamBlock.cc
		DicomBeamDevice.cc
		DicomReaderNM.cc.old
		DicomVBeamRTControlPoint.cc
		DicomBeamDevicePos.cc
		DicomBeamDeviceBLD.cc
		DicomBeamRTControlPoint.cc
		DicomVBeamDevice.cc
		DicomBeamCompensator.cc
		DicomReaderRTPlan.cc
		DicomBeamWedge.cc
		DicomBeamRTIonControlPoint.cc
	 
	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
		${Geant4_LIBRARIES} 
		${GAMOS_LIBRARIES} 
		${SEAL_LIBRARIES} 
		${ROOT_LIBRARIES} 
		${DCMTK_LIBRARIES} 
	DICOMReaders
	DICOMBase

	LINK_LIBRARIES
)
