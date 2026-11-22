#include "DicomReaderInterfileCT.hh"
#include "DicomReaderCT.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"
#include "DICOM/DICOMBase/include/DicomVImage.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosReadDICOM/include/GmInterfile.hh"
#include "GamosCore/GamosReadDICOM/include/GmInterfileHeader.hh"

#include <fstream>
#include <map>

//-----------------------------------------------------------------------------
DicomReaderInterfileCT::DicomReaderInterfileCT(G4String fName, G4String name ) : DicomReaderInterfile( DRM_InterfileCT )
{
  theModality = DRM_InterfileCT;
  theFileName = fName;
  theHeaderFileName = fName;
  theName = name;
  //  if( name == "" ) theName = theFileName;
  thePatientPosition = "HFS";
  theModality = DRM_Interfile;

#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(infoVerb) ) G4cout << " DicomReaderInterfile::DicomReaderInterfile " << fName << " " << name << " " << DicomVReader::GetModalityStr(theModality) << G4endl;
#endif
  ReadHeaderAndPixels();

 }

//-----------------------------------------------------------------------------
void DicomReaderInterfileCT::ReadHeaderAndPixels(G4bool bReadPixelData)
{
#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(infoVerb) ) G4cout << " DicomReaderInterfileCT::ReadHeaderAndPixels " << G4endl;
#endif

  DicomReaderInterfile::ReadHeader();

  if( !bReadPixelData ) {
  //    G4cout << " bReadPixelData " << bReadPixelData << G4endl; //GDEB
    return;
  }

  
  ReadPixelData();
}


//----------------------------------------------------------------------------
void DicomReaderInterfileCT::ReadPixelData()
{
  GmInterfile* interfile = new GmInterfile();
  interfile->SetHeader(theInterfileHeader);
  interfile->Read(theDataFileName);

  size_t nVoxels = theInterfileHeader->GetNoVoxels();
  std::vector<float> DataRead = interfile->GetData();
  theVoxelData = new std::vector<G4double>;
  for( size_t ii = 0; ii < nVoxels; ii++ ) {
    theVoxelData->push_back(DataRead[ii]*theRescaleSlope + theRescaleIntercept);
     //    G4cout << " DicomReaderInterfile DATA " << ii << " : " << DataRead[ii]*theRescaleSlope + theRescaleIntercept << " " << DataRead[ii]<<"*"<<theRescaleSlope<<"+"<<theRescaleIntercept << G4endl; //GDEB
  }
  theDicomImage = CreateImage("InterfileCT", DIM_InterfileCT, true, true );

  //---- Create DicomReaderCT
  DicomReaderCT* theDicomReaderCT = new DicomReaderCT();
  theDicomReaderCT->SetVoxelData(theVoxelData);
  theDicomReaderCT->DICOMCreateImages();
  theDicomReaderCT->SetNoVoxelsX(theNoVoxelsX);
  theDicomReaderCT->SetNoVoxelsY(theNoVoxelsY);
  theDicomReaderCT->SetNoVoxelsZ(theNoVoxelsZ);
  theDicomReaderCT->SetMinX(theMinX);
  theDicomReaderCT->SetMaxX(theMaxX);
  theDicomReaderCT->SetMinY(theMinY);
  theDicomReaderCT->SetMaxY(theMaxY);
  theDicomReaderCT->SetMinZ(theMinZ);
  theDicomReaderCT->SetMaxZ(theMaxZ);
  
}

