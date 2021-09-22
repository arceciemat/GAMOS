#include "DicomReaderSqdose.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"
#include "DICOM/DICOMBase/include/DicomVImage.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosReadDICOM/include/GmSqdose.hh"
#include "GamosCore/GamosReadDICOM/include/GmSqdoseHeader.hh"
#include <fstream>

//-----------------------------------------------------------------------------
DicomReaderSqdose::DicomReaderSqdose(G4String fName, G4String name ) : DicomVReaderImage(DRM_Sqdose)
//  : theFileName(fName), theName(name)
{
  theFileName = fName;
  G4cout << "DicomReaderSqdose(G4String fName) " << theFileName << G4endl; //GDEB
  theName = name;
  if( name == "" ) theName = theFileName;
  thePatientPosition = "HFS";
  theModality = DRM_Sqdose;

#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(infoVerb) ) G4cout << " DicomReaderSqdose::DicomReaderSqdose " << fName << " " << name << " " << DicomVReader::GetModalityStr(theModality) << G4endl;
#endif
  ReadHeaderAndPixels();
}

//-----------------------------------------------------------------------------
void DicomReaderSqdose::ReadHeaderAndPixels(G4bool bReadPixelData)
{
#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(infoVerb) ) G4cout << " DicomReaderSqdose::ReadHeaderAndPixels " << G4endl;
#endif
  
  theSqdose = new GmSqdose();
  G4String path( getenv( "GAMOS_SEARCH_PATH" ) );  
  theFileName = GmGenUtils::FileInPath( path, theFileName );
  if( theFileName.substr(0,2) == "./" ) theFileName = theFileName.substr(2,theFileName.length());
  theSqdose->Read(theFileName);
  if( theSqdose->GetType() != SqTALL ) {
    G4Exception("DicomReaderSqdose::ReadHeaderAndPixels",
		"",
		FatalException,
		"Only sqdose files of type SqTALL (0) are supported");
  }

  GmSqdoseHeader* sqdh =theSqdose->GetHeader();
  theNoVoxelsX = sqdh->GetNoVoxelsX();
  theNoVoxelsY = sqdh->GetNoVoxelsY();
  theNoVoxelsZ = sqdh->GetNoVoxelsZ();
  theMinX = sqdh->GetMinX();
  theMinY = sqdh->GetMinY();
  theMinZ = sqdh->GetMinZ();
  theMaxX = sqdh->GetMaxX();
  theMaxY = sqdh->GetMaxY();
  theMaxZ = sqdh->GetMaxZ();
  size_t nVox = GetNoVoxels();

  if( !bReadPixelData ) return;
  
  std::vector<float> doses =theSqdose->GetDoses();
  theVoxelData = new std::vector<G4double>;
  for( size_t ii = 0; ii < nVox; ii++ ) {
    theVoxelData->push_back(doses[ii]);
  } 
  //----- Create sqdose image
  theDicomImage = CreateImage("Sqdose", DIM_Sqdose, true, true );

  //----- Create sqdose squared image
  std::vector<float> doseSqs =theSqdose->GetDoseSqs();
  theDoseSqImage = CreateImage("Sqdose_sq", DIM_Sqdose_sq, false, true );
  std::vector<G4double>* imgDataSq = new std::vector<G4double>;
  for( size_t ii = 0; ii < nVox; ii++ ) {
    imgDataSq->push_back(doseSqs[ii]);
  }
  theDoseSqImage->SetData(imgDataSq);
  
  //----- Create sqdose error image
  theSqdose->CalculateErrors();
  std::vector<float> doseErrors = theSqdose->GetDoseErrors();
  theDoseErrImage = CreateImage("Sqdose_err", DIM_Sqdose_err, false, true );
  std::vector<G4double>* imgDataErr = new std::vector<G4double>;
  for( size_t ii = 0; ii < nVox; ii++ ) {
    imgDataErr->push_back(doseErrors[ii]);
    //    if (  doseErrors[ii] != 0 )  std::cout << ii << " DOSE ERR " << doseErrors[ii] << " " << doses[ii] << " " << doseSqs[ii] << G4endl; //GDEB
  }
  theDoseErrImage->SetData(imgDataErr);

  GmParameterMgr* paramMgr = GmParameterMgr::GetInstance();
  G4String doseVar = paramMgr->GetStringValue("doseVar",""); 
  if( doseVar == "doseErrRel" ) {
    DicomVImage* doseErrRelImage = CreateImage("Sqdose_errRel", DIM_Sqdose_errRel, false, true );
    std::vector<G4double>* imgDataErrRel = new std::vector<G4double>;
    for( size_t ii = 0; ii < nVox; ii++ ) {
      G4double errRel = 0.;
      if( doses[ii] != 0. ) errRel = doseErrors[ii]/doses[ii];
      //      if( errRel != 0 ) G4cout << ii%theNoVoxelsX << " " << (ii/theNoVoxelsX)%theNoVoxelsY << " " << ii/theNoVoxelsX/theNoVoxelsY << " errRel " << errRel << " = " <<doseErrors[ii] << " / " <<doses[ii] << G4endl; //GDEB 
      imgDataErrRel->push_back(errRel);
    }
    //    if (  doseErrors[ii] != 0 )  std::cout << ii << " DOSE ERR " << doseErrors[ii] << " " << doses[ii] << " " << doseSqs[ii] << G4endl; //GDEB
    doseErrRelImage->SetData(imgDataErrRel);
  }
  
  /*#ifndef GAMOS_NO_VERBOSE
  G4double theWidthX = (theMaxX-theMinX)/theNoVoxelsX;
  G4double theWidthY = (theMaxY-theMinY)/theNoVoxelsY;
  G4double theWidthZ = (theMaxZ-theMinZ)/theNoVoxelsZ;
  if( DicomVerb(testVerb) ) {
    size_t nVoxelXY = theNoVoxelsX*theNoVoxelsY;
    for( size_t ii = 0; ii < nVox; ii++ ) {
      size_t ix = ii%theNoVoxelsX;
      size_t iy = (ii/theNoVoxelsX)%theNoVoxelsY;
      size_t iz = size_t(ii/nVoxelXY);
      //   if( iz != 33 ){ doses[ii] = ix;theDicomImage->SetData(ii,ix+0.1); }
      //if( iz == 33 ){ doses[ii] = ix;theVoxelData->at(ii) = ix; }
      //      if( doses[ii] != 0 )
      //      std::cout << ii << " = " << ix << " : " << iy << " : " << iz << " DOSE= " << " " << doses[ii] << " +- " << doseErrors[ii] << G4endl; 
      //      std::cout << ii << " =xyz " << theMinX+(ix+0.5)*theWidthX << " : " << theMinY+(iy+0.5)*theWidthY  << " : " << theMinZ+(iz+0.5)*theWidthZ << " DOSE= " << " " << doses[ii] << " +- " << doseErrors[ii] << G4endl; 
    }
  }
#endif
  */
  
  /*t
#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(testVerb) ) {
    size_t nVoxelXY = theNoVoxelsX*theNoVoxelsY;
    for( size_t ii = 0; ii < nVox; ii++ ) {
      std::cout << ii << " DOSE " << doses[ii] << " +- " << doseErrors[ii] << " Sqs " << doseSqs[ii] << G4endl; //GDEB
    }
  }
#endif
  */
  
}

