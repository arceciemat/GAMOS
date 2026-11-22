#include "DicomReaderG4dcmNM.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"
#include "DICOM/DICOMBase/include/DicomVImage.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include <fstream>

//-----------------------------------------------------------------------------
DicomReaderG4dcmNM::DicomReaderG4dcmNM(G4String fName, G4String name) : DicomVReaderImage(DRM_G4dcmNM)
{
  theFileName = fName;
  theName = name;
  if( name == "" ) theName = theFileName;
  thePatientPosition = "HFS";
  theModality = DRM_G4dcmNM;

  theDicomImage = new DicomVImage("G4dcmNM",DIM_G4dcmNM);

  ReadHeaderAndPixels();
}

//-----------------------------------------------------------------------------
void DicomReaderG4dcmNM::ReadHeaderAndPixels(G4bool bReadPixelData )
{
  if( DicomVerb(debugVerb) ) G4cout << " DicomReaderG4dcmNM::ReadHeaderAndPixels theFileName " << theFileName << G4endl; 
  theFileName = GmGenUtils::FileInPath( theFileName );
  if( theFileName.substr(0,2) == "./" ) theFileName = theFileName.substr(2,theFileName.length());
  std::vector<G4String> wl;
  GmFileIn fing = GmFileIn::GetInstance(theFileName, true);
  std::ifstream* fin = fing.GetIfstream();
  *fin >> thePatientPosition;
  //  G4cout << "DicomReaderG4dcmNM::ReadHeaderAndPixels thePatientPosition " << thePatientPosition << G4endl; //GDEB
  if( GmGenUtils::IsNumber( thePatientPosition ) ) {
    G4Exception("DicomReaderG4dcmNM::ReadHeaderAndPixels",
		"Wrong argument",
		//		FatalErrorInArgument,
		JustWarning,
		("No PatientPosition in G4dcmNM file :"+thePatientPosition+" It will be set to HFS").c_str());
    thePatientPosition = "HFS";
    //--- go back to the position before (to read again NVoxels in theDicomImage::ReadDataFromTextFile)
    fin->seekg (0, std::ios::beg);
  }

  if( !bReadPixelData ) return;
  theDicomImage->ReadDataFromTextFile( *fin, true );
  if( DicomVerb(testVerb) ) G4cout << " DicomReaderG4dcmNM::ReadHeaderAndPixels theDicomImage  NDATA " << theDicomImage->GetNoVoxels() << G4endl; 

}

