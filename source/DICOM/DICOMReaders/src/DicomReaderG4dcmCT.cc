#include "DicomReaderG4dcmCT.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"
#include "DICOM/DICOMBase/include/DicomVImage.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include <fstream>

#include "DicomReaderMgr.hh" //GDEB
//-----------------------------------------------------------------------------
DicomReaderG4dcmCT::DicomReaderG4dcmCT(G4String fName, G4String name) : DicomVReaderImage(DRM_G4dcmCT)
{
  theFileName = fName;
  theName = name;
  if( name == "" ) theName = theFileName;
  thePatientPosition = "HFS";
  theStructIDImage = 0;
  theModality = DRM_G4dcmCT;

  ReadHeaderAndPixels();

}

//-----------------------------------------------------------------------------
void DicomReaderG4dcmCT::ReadHeaderAndPixels( G4bool bReadPixelData )
{
  if( DicomVerb(debugVerb) ) G4cout << " DicomReaderG4dcmCT::ReadHeaderAndPixels theFileName " << theFileName << G4endl; 
  theFileName = GmGenUtils::FileInPath( theFileName );
  if( theFileName.substr(0,2) == "./" ) theFileName = theFileName.substr(2,theFileName.length());
  std::vector<G4String> wl;
  GmFileIn fing = GmFileIn::GetInstance(theFileName,true);
  std::ifstream* fin = fing.GetIfstream();
  G4int nMaterials;
  if( !fing.GetWordsInLine(wl) ) return;
  nMaterials = GmGenUtils::GetInteger(wl[0]);
  G4int imate;
  if( DicomVerb(debugVerb) ) G4cout << " DicomReaderG4dcmCT::ReadHeaderAndPixels NMATE " << nMaterials << G4endl; 
  for( G4int ii = 0; ii < nMaterials; ii++ ){
    if( !fing.GetWordsInLine(wl) ) return;
    imate = GmGenUtils::GetInteger(wl[0]);
    if( DicomVerb(debugVerb) ) G4cout << "DicomReaderG4dcmCT::ReadHeaderAndPixels reading nmate " << ii << " = " << imate << " mate " << wl[1] << G4endl;
    if( ii != imate ) G4Exception("DicomReaderG4dcmCT::ReadHeaderAndPixels",
				  "Wrong argument",
				  FatalErrorInArgument,
				  "Material number should be in increasing order: wrong material number ");
    theMaterialNames[ii] = wl[1];
    //    G4cout <<  this << " " << ii << " theMaterialNames " << theMaterialNames[ii] << " = " << wl[1] << G4endl; //GDEB
  }
  *fin >> thePatientPosition;
  //  G4cout << "DicomReaderG4dcmCT::ReadHeaderAndPixels thePatientPosition " << thePatientPosition << G4endl; //GDEB
  if( GmGenUtils::IsNumber( thePatientPosition ) ) {
    G4Exception("DicomReaderG4dcmCT::ReadHeaderAndPixels",
		"Wrong argument",
		//		FatalErrorInArgument,
		JustWarning,
		("No PatientPosition in G4dcmCT file :"+thePatientPosition+" It will be set to HFS").c_str());
    thePatientPosition = "HFS";
    //--- go back to the position before (to read again NVoxels in theMateIDImage::ReadDataFromTextFile)
    fin->seekg (0, std::ios::beg);
    fing.GetWordsInLine(wl);
    for( G4int ii = 0; ii < nMaterials; ii++ ){
      fing.GetWordsInLine(wl);
    }
  }

  if( !bReadPixelData ) return;
  theMateIDImage = new DicomVImage("G4dcmCT_MateID",DIM_G4dcmCT_MateID);
  theMateIDImage->ReadDataFromTextFile( *fin, true );
  if( DicomVerb(testVerb) ) G4cout << " DicomReaderG4dcmCT::ReadHeaderAndPixels theMateIDImage  NDATA " << theMateIDImage->GetNoVoxels() << G4endl; 

  theMateDensImage = new DicomVImage(theMateIDImage,"G4dcmCT_MateDens",DIM_G4dcmCT_MateDens);
  G4bool bEOF = theMateDensImage->ReadDataFromTextFile( *fin, false );
  if( DicomVerb(testVerb) ) G4cout << " DicomReaderG4dcmCT::ReadHeaderAndPixels theMateDensImage NDATA " << theMateDensImage->GetNoVoxels() << G4endl; 
  if( DicomVerb(testVerb) ) G4cout << " DicomReaderG4dcmCT::ReadHeaderAndPixels bEOF " << bEOF << G4endl; 

  // }
  //-- read structure names
  
  if( !bEOF ) {
    //    G4cout << theStructIDImage << " TO READ STRUCt " << G4endl;  //GDEB
    theStructIDImage = new DicomVImageStr(theMateIDImage,"G4dcmCT_StructID",DIM_G4dcmCT_StructID);
    //    G4cout << theStructIDImage << " TO READ STRUCT READ " << theStructIDImage->GetNoVoxels() << G4endl;  //GDEB
    theStructIDImage->ReadDataFromTextFile( *fin, false );

  size_t nVoxelX = theStructIDImage->GetNoVoxelsX();
  size_t nVoxelY = theStructIDImage->GetNoVoxelsY();
  size_t nVoxelXY = nVoxelX*nVoxelY;
  size_t nVoxelZ = theStructIDImage->GetNoVoxelsZ();
  size_t nVoxelXYZ = nVoxelXY*nVoxelZ;
  //  float fWidthX = theStructIDImage->GetVoxelDimX();
  float fWidthY = theStructIDImage->GetVoxelDimY();
  //  float fWidthZ = theStructIDImage->GetVoxelDimZ();
  //  G4double fMinX = theStructIDImage->GetMinX();
  G4double fMinY = theStructIDImage->GetMinY();
  //  G4double fMinZ = theStructIDImage->GetMinZ();
  //  std::map<int,double> stMinY; // GDEB
  std::set<G4int> ists;
  /*  for( size_t ii = 0; ii < nVoxelXYZ; ii++ ){
    //    G4int ix = ii%nVoxelX;
        G4int iy = (ii/nVoxelX)%nVoxelY;
    //    G4int iz = ii/nVoxelXY;
    //    G4ThreeVector voxelCentre( fMinX+(ix+0.5)*fWidthX, fMinY+(iy+0.5)*fWidthY, fMinZ+(iz+0.5)*fWidthZ );
    G4double posY = fMinY+(iy+0.5)*fWidthY;
    ists = theStructIDImage->GetIDList( ii );
    for( std::set<G4int>::const_iterator itest = ists.begin(); itest != ists.end(); itest++ ) {
      size_t ist = *itest;
      if( stMinY.find(ist) == stMinY.end() ) stMinY[ist] = -DBL_MIN;
      stMinY[ist] = std::min( stMinY[ist], posY );
      //      G4cout << ix <<":"<<iy<<":"<<iz << " " << ist << " stMinY " << stMinY[ist] << " " << posY << " " << fMinY <<"+("<<iy+0.5<<")*"<<fWidthY<< G4endl; //GDEB
      }
  }
    for( std::map<int,double>::iterator ite = stMinY.begin(); ite !=  stMinY.end(); ite++ ) {
    std::cout << " MINY " << ite->first << " = " << ite->second << std::endl; //GDEB
    }*/

    G4int ist;
    G4String stName;
    for( ;; )  {
      if( !fing.GetWordsInLine(wl) ) return;
      ist = GmGenUtils::GetInteger(wl[0]);
      stName = wl[1];      
      //--- AVOID REPEATED NAMES
      for( std::map<G4int,G4String>::const_iterator itest =  theStructNames.begin(); itest != theStructNames.end(); itest++ ) {
	if( (*itest).second == stName ) {
	  G4Exception("DicomReaderG4dcmCT::ReadHeaderAndPixels",
		      "",
		      JustWarning,
		      ("Repeated Structure Name " + stName + " ,  a '_2' will be added").c_str());
	  stName = stName+"_2";
	}
      }
      theStructNames[ist] = stName;
      if( DicomVerb(debugVerb) ) G4cout << this << " DicomReaderG4dcmCT::ReadHeaderAndPixels ADD Structure " << ist << " : " << stName << " NSts " << theStructNames.size() << G4endl; 
      //      G4cout << " DicomReaderG4dcmCT ADD StructureMINY " << ist << " : " << stName << "  = " << stMinY[ist] << G4endl; //GDEB
      //      if( fin->eof() ) break; 
    }
  }

  //  for( size_t ii = 0; ii < theStructIDImage->GetNoVoxels(); ii++ ) {
  //   if( ii%1000 == 0 ) G4cout << theStructIDImage << " " << ii << " READ STRUCt " << theStructIDImage->GetData(ii) << G4endl;  //GDEB
  // G4cout << this << " READER G4DCMCT theStructIDImage " << theStructIDImage << G4endl; //GDEB

}

//-----------------------------------------------------------------------------
void DicomReaderG4dcmCT::AddMaterialName(G4int mateID, G4String mateName)
{
  std::map<G4int,G4String>::iterator ite;
  for( ite = theMaterialNames.begin(); ite != theMaterialNames.end(); ite++ ) {
    if( (*ite).first == mateID ) { 
      G4Exception("DicomReaderG4dcmCT::AddMaterialName",
		  "",
		  FatalException,
		  ("Material ID already exists " + GmGenUtils::itoa(mateID)).c_str());
    }
    if( (*ite).second == mateName ) { 
      G4Exception("DicomReaderG4dcmCT::AddMaterialName",
		  "",
		  FatalException,
		  ("Material name already exists " + mateName).c_str());
    }
  }

  theMaterialNames[mateID] = mateName;
}

//-----------------------------------------------------------------------------
void DicomReaderG4dcmCT::DumpHeaderToTextFile(std::ofstream& fout)
{
  std::map<G4int,G4String>::iterator ite;
  fout << theMaterialNames.size() << G4endl;
  G4cout << " DumpHeaderToTextFile theMaterialNames " << theMaterialNames.size() << G4endl; //GDEB
  for( ite = theMaterialNames.begin(); ite != theMaterialNames.end(); ite++ ) {
    G4cout << " DumpHeaderToTextFile MaterialName " << ite->first << " " << ite->second << G4endl; //GDEB
    fout << ite->first << " \"" << ite->second << "\"" << std::endl;
  }
  fout << thePatientPosition << G4endl;
         
}

//-----------------------------------------------------------------------------
void DicomReaderG4dcmCT::DumpStructureList(std::ofstream& fout)
{
  std::map<G4int,G4String>::iterator ite;  
  for( ite = theStructNames.begin(); ite != theStructNames.end(); ite++ ) {
    fout << ite->first << " \"" << ite->second << "\"" << std::endl;
  }
         
}

//-----------------------------------------------------------------------------
G4String DicomReaderG4dcmCT::GetMaterialName(G4int id) const
{
  std::map<G4int,G4String>::const_iterator ite = theMaterialNames.find(id);
  if( ite == theMaterialNames.end() ) {
    for( ite = theMaterialNames.begin(); ite != theMaterialNames.end(); ite++ ) {
      G4cerr << " MATERIAL " << ite->first << " : " << ite->second << G4endl;
    }
    G4Exception("DicomReaderG4dcmCT::GetMaterialName",
		"",
		FatalException,
		("Material ID not found " + GmGenUtils::itoa(id)).c_str());
  }

  return ite->second;
}

//-----------------------------------------------------------------------------
G4String DicomReaderG4dcmCT::GetStructName(G4int id) const
{
  std::map<G4int,G4String>::const_iterator ite = theStructNames.find(id);
  if( ite == theStructNames.end() ) {
    for( ite = theStructNames.begin(); ite != theStructNames.end(); ite++ ) {
      G4cerr << " MATERIAL " << ite->first << " : " << ite->second << G4endl;
    }
    G4Exception("DicomReaderG4dcmCT::GetStructName",
		"",
		FatalException,
		("Struct ID not found " + GmGenUtils::itoa(id)).c_str());
  }

  return ite->second;
}

//-----------------------------------------------------------------------------
G4int DicomReaderG4dcmCT::GetStructID( G4String stName )
{
  for( std::map<G4int,G4String>::const_iterator ite = theStructNames.begin(); ite != theStructNames.end(); ite++ ) {
    if( GmGenUtils::AreWordsEquivalent(stName, ite->second ) ) {
      return ite->first;
    }
  }
  G4Exception("DicomReaderG4dcmCT::GetStructID",
		"",
		FatalException,
		("Struct Name not found " + stName).c_str());

  return -1;
}



