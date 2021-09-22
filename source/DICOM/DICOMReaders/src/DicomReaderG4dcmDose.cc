#include "DicomReaderG4dcmDose.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"
#include "DICOM/DICOMBase/include/DicomVImage.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include <fstream>

//-----------------------------------------------------------------------------
DicomReaderG4dcmDose::DicomReaderG4dcmDose(G4String fName, G4String name) : DicomVReaderImage(DRM_G4dcmDose)
{
  theFileName = fName;
  theName = name;
  if( name == "" ) theName = theFileName;
  thePatientPosition = "HFS";
  theModality = DRM_G4dcmDose;

  theDicomImage = new DicomVImage("G4dcmDose",DIM_G4dcmDose);

  ReadHeaderAndPixels();
}

//-----------------------------------------------------------------------------
void DicomReaderG4dcmDose::ReadHeaderAndPixels(G4bool bReadPixelData)
{
  if( DicomVerb(debugVerb) ) G4cout << " DicomReaderG4dcmDose::ReadHeaderAndPixels theFileName " << theFileName << G4endl; 
  theFileName = GmGenUtils::FileInPath( theFileName );
  if( theFileName.substr(0,2) == "./" ) theFileName = theFileName.substr(2,theFileName.length());
  std::vector<G4String> wl;
  GmFileIn fing = GmFileIn::GetInstance(theFileName,true);
  std::ifstream* fin = fing.GetIfstream();
  *fin >> thePatientPosition;
  //  G4cout << "DicomReaderG4dcmDose::ReadHeaderAndPixels thePatientPosition " << thePatientPosition << G4endl; //GDEB
  if( GmGenUtils::IsNumber( thePatientPosition ) ) {
    G4Exception("DicomReaderG4dcmDose::ReadHeaderAndPixels",
		"Wrong argument",
		//		FatalErrorInArgument,
		JustWarning,
		("No PatientPosition in G4dcmDose file :"+thePatientPosition+" It will be set to HFS").c_str());
    thePatientPosition = "HFS";
    //--- go back to the position before (to read again NVoxels in theDicomImage::ReadDataFromTextFile)
    fin->seekg (0, std::ios::beg);
  }

  if( !bReadPixelData ) return;
  theDicomImage->ReadDataFromTextFile( *fin, true );
  if( DicomVerb(testVerb) ) G4cout << " DicomReaderG4dcmDose::ReadHeaderAndPixels theDicomImage  NDATA " << theDicomImage->GetNoVoxels() << G4endl; 

}

