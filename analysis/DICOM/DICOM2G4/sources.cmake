# - DICOM2G4 module build definition

gamos_add_module(DICOM2G4
  PUBLIC_HEADERS
    Dicom2G4FileMgr.hh
    DicomBeam.hh
    DicomBeamBlock.hh
    DicomBeamCompensator.hh
    DicomBeamDevice.hh
    DicomBeamDeviceBLD.hh
    DicomBeamDevicePos.hh
    DicomBeamRTControlPoint.hh
    DicomBeamRTIonControlPoint.hh
    DicomBeamWedge.hh
    DicomReaderRTIonPlan.hh
    DicomReaderRTPlan.hh
    DicomVBeamDevice.hh
    DicomVBeamRTControlPoint.hh
  SOURCES
    Dicom2G4FileMgr.cc
    DicomBeam.cc
    DicomBeamBlock.cc
    DicomBeamCompensator.cc
    DicomBeamDevice.cc
    DicomBeamDeviceBLD.cc
    DicomBeamDevicePos.cc
    DicomBeamRTControlPoint.cc
    DicomBeamRTIonControlPoint.cc
    DicomBeamWedge.cc
    DicomReaderRTIonPlan.cc
    DicomReaderRTPlan.cc
    DicomVBeamDevice.cc
    DicomVBeamRTControlPoint.cc
)

gamos_module_link_libraries(DICOM2G4
  PUBLIC
    DICOMBase
    DICOMReaders
    GamosCore_GamosUtils
    ${GAMOS_DCMTK_LIBRARIES}
)

# Program DICOM2G4
gamos_add_executable(DICOM2G4
  SOURCES
    DICOM2G4.cc
  LINK_LIBRARIES
    DICOM2G4
    DICOMBase
    GamosCore_GamosBase_Base
    GamosCore_GamosUtils
)
