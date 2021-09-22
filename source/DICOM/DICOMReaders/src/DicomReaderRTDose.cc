#include "DicomReaderRTDose.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"
#include "DICOM/DICOMBase/include/DicomVImage.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "dcmtk/dcmrt/drtdose.h"

#include <fstream>

//-----------------------------------------------------------------------------
DicomReaderRTDose::DicomReaderRTDose(DcmDataset* dset) : DicomVReaderImage(dset, DRM_RTDose)
{
  theDoseSqImage = 0;
  theDoseErrImage = 0;

  ReadHeaderAndPixels();
}

//-----------------------------------------------------------------------------
DicomReaderRTDose::DicomReaderRTDose(G4String fName) : DicomVReaderImage(DRM_RTDose)
{
  theFileName = fName;
  DcmFileFormat dfile;
  if( ! (dfile.loadFile(fName.c_str())).good() ) {
    G4Exception("DicomHandler::ReadFile",
                "",
                FatalErrorInArgument,
                ("Error reading file " + fName).c_str());
  }
  DcmDataset* dset = dfile.getDataset();
  theDataset = dset;

  ReadHeaderAndPixels();
}

//-----------------------------------------------------------------------------
DicomReaderRTDose::DicomReaderRTDose() : DicomVReaderImage(DRM_RTDose)
{
  InitialiseTo0();
}

//-----------------------------------------------------------------------------
void DicomReaderRTDose::ReadHeaderAndPixels(G4bool bReadPixelData)
{
  //-  DicomVReaderImage::ReadHeaderAndPixels(bReadPixelData); // false, first made some changes
  DicomVReaderImage::ReadHeaderAndPixels(false); // false, first made some changes

  //  G4cout << GetName() << " GetImage() " << GetImage() << G4endl; 
  //  G4cout << GetName() << " GetImage() " << GetImage() << " NDATA " << GetImage()->GetData()->size() << G4endl;

  DRTDoseIOD rtdose;
  OFCondition result = rtdose.read(*theDataset);
  G4int numberOfFrames = 1;
  std::vector<double> gridFrameOffsets;
  std::vector<double> numberOfFramesV = Read1Data(theDataset, DCM_NumberOfFrames,1);

  if( numberOfFramesV.size() == 0 ) {
    Sint32 fint;
    if( rtdose.getNumberOfFrames(fint) != EC_Normal ) {
      G4Exception("DicomReaderRTDoseImage::ReadHeaderAndPixels",
		  "",
		  FatalException,
		  "RTDose file has no NumberOfFrames, it cannot be read "); 
    }
    numberOfFrames = G4int(fint);
    theNoVoxelsZ = numberOfFrames;  
    //    numberOfFrames = Read1Data(theDataset, DCM_NumberOfFrames,1);
  }
  
  gridFrameOffsets = Read1Data(theDataset, DCM_GridFrameOffsetVector, numberOfFrames);
  if( gridFrameOffsets.size() > 1 && gridFrameOffsets[0] != 0 && gridFrameOffsets[0] != theVoxelDimZ ) {
    G4Exception("DicomReaderRTDoseImage::ReadHeaderAndPixels",
		"",
		FatalException,
		("RTDose file has gridFramOffsets = " + GmGenUtils::ftoa(gridFrameOffsets[0]) + " is different than slice thickness " + GmGenUtils::ftoa(theVoxelDimZ)).c_str()); 
  }
  if( G4int(gridFrameOffsets.size()) != numberOfFrames ) {
    G4Exception("DicomReaderRTDoseImage::ReadHeaderAndPixels",
		"",
		FatalException,
		("RTDose file does not have " + GmGenUtils::itoa(numberOfFrames) + " GridFrameOffsetVectors, it cannot be read ").c_str()); 
  }

  std::vector<double> dImagePositionPatient = Read1Data(theDataset, DCM_ImagePositionPatient,3);
  std::vector<double> dSliceThickness = Read1Data(theDataset, DCM_SliceThickness, 1);
  //  theMinZ = dImagePositionPatient[2]+gridFrameOffsets[0]; //?
  theMaxZ = theMinZ +gridFrameOffsets[gridFrameOffsets.size()-1]+dSliceThickness[0];
  G4cout << " RTDOSE MinZ " << theMinZ << " " << theMaxZ << G4endl; //GDEB
  
  if(bReadPixelData) ReadPixelData();

}

//-----------------------------------------------------------------------------
void DicomReaderRTDose::DICOMCreateImages()
{
  theDicomImage = CreateImage("RTDose", DIM_RTDose, true, false );
  ResizeImage();
}

