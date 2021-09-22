#------------------------------------------------------------------------------
# Module : DICOMReaders
# Package: DICOM_DICOMReaders
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
include_directories(${CMAKE_SOURCE_DIR}/source)
include_directories(${CMAKE_SOURCE_DIR}/include)

include_directories(G:/GAMOSwin/external/dmctk/dcmtk-3.18.2/include)

#
# Define the GAMOS Module.
include(UseGamosAtGeant4)
#
GEANT4_DEFINE_MODULE(NAME DICOMReaders
	 HEADERS
		DicomReaderG4dcmDose.hh
		DicomReaderRTDose.hh
		DicomReaderTextLines.hh
		DicomVReaderImage.hh
		DicomReaderLine.hh
		DicomReaderG4dcmNM.hh
		DicomROIContour.hh
		DicomReaderSqdose.hh
		DicomReaderNM.hh
		DicomVReader.hh
		DicomReaderRTStruct.hh
		DicomReaderMgr.hh
		DicomUIDInfo.hh
		DicomReaderG4dcmCT.hh
		DicomReaderCT.hh
		DicomROI.hh
	 SOURCES
		DicomReaderLine.cc
		DicomReaderG4dcmCT.cc
		DicomVReader.cc
		DicomReaderRTStruct.cc
		DicomUIDInfo.cc
		DicomReaderG4dcmNM.cc
		DicomReaderNM.cc
		DicomVReaderImage.cc
		DicomROIContour.cc
		DicomReaderMgr.cc
		DicomROI.cc
		DicomReaderTextLines.cc
		DicomReaderG4dcmDose.cc
		DicomReaderCT.cc
		DicomReaderRTDose.cc
		DicomReaderSqdose.cc
	 
	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
		${Geant4_LIBRARIES} 
		${GAMOS_LIBRARIES} 
		${SEAL_LIBRARIES} 
		${ROOT_LIBRARIES} 
		${DCMTK_LIBRARIES} 
		DICOMBase

	LINK_LIBRARIES
)
