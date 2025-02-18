#------------------------------------------------------------------------------
# Module : DICOMBase
# Package: DICOM_DICOMBase
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
include_directories(${CMAKE_SOURCE_DIR}/source)
include_directories(${CMAKE_SOURCE_DIR}/include)
#
# Define the GAMOS Module.
include(UseGamosAtGeant4)
#
GEANT4_DEFINE_MODULE(NAME DICOMBase
	 HEADERS
		Dicom2DOperAddError.hh
		DicomOperLogN.hh
		DicomLineDrawingOptions.hh
		DicomOperIntersectStruct.hh
		DicomPolygonSet.hh
		DicomOperLog.hh
		DicomOperMultByNumber.hh
		DicomVOperator.hh
		DicomPolygon.hh
		DicomDrawerROOT.hh
		DicomOperPrintMax.hh
		DicomVImage.hh
		DicomLine.hh
		DicomNDOperArithm.hh
		DicomParameterMgr.hh
		DicomVLine.hh
		DicomMgr.hh
		DicomVLineList.hh
		DicomJobUtils.hh
		DicomOperPrintValueAtPoint.hh
		DicomVDrawer.hh
		DicomVObjectWithParams.hh
		DicomPolygonList.hh
		DicomOperRotateXY180.hh
		DicomLineSet.hh
		DicomVLineSet.hh
		DicomVerbosity.hh
		DicomOperLog10.hh
		DicomOperMirrorY.hh
		DicomOperCompressXY.hh
		DicomLineList.hh
		DicomOperExtendAir.hh
	 SOURCES
		DicomLineList.cc
		DicomNDOperArithm.cc
		DicomVerbosity.cc
		plugin.cc
		DicomParameterMgr.cc
		DicomVLine.cc
		DicomOperPrintMax.cc
		DicomVOperator.cc
		DicomPolygonSet.cc
		DicomVImage.cc
		DicomLineDrawingOptions.cc
		DicomOperLog.cc
		DicomOperExtendAir.cc
		DicomVLineList.cc
		DicomVObjectWithParams.cc
		DicomVLineSet.cc
		DicomJobUtils.cc
		DicomMgr.cc
		DicomPolygon.cc
		DicomLine.cc
		DicomOperCompressXY.cc
		DicomOperMirrorY.cc
		DicomPolygonList.cc
		DicomOperPrintValueAtPoint.cc
		DicomVDrawer.cc
		DicomOperLog10.cc
		Dicom2DOperAddError.cc
		DicomOperLogN.cc
		DicomOperIntersectStruct.cc
		DicomDrawerROOT.cc
		DicomLineSet.cc
		DicomOperRotateXY180.cc
		DicomOperMultByNumber.cc
		
	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
		${Geant4_LIBRARIES} 
		${GAMOS_LIBRARIES} 
		${SEAL_LIBRARIES} 
		${ROOT_LIBRARIES} 
		${DCMTK_LIBRARIES} 

	LINK_LIBRARIES
)
