#include "DicomVImageStr.hh"
#include "DicomVerbosity.hh"
#include "DicomMgr.hh"
#include "DicomVLine.hh"
#include "DicomVLineList.hh"
#include "DicomVLineSet.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosReadDICOM/include/GmSqdose.hh"
#include "GamosCore/GamosReadDICOM/include/GmReadPhantomStMgr.hh"

//G4double DicomVImageStr::DP_PRECISION = 1.E-4;

//size_t DicomVImageStr::theStructureIdxShift = 100;
//-----------------------------------------------------------------------------
DicomVImageStr::DicomVImageStr(G4String name, DIModality mod ) :
  DicomVImage(name,mod)
{
}

//-----------------------------------------------------------------------------
DicomVImageStr::DicomVImageStr(DicomVImage* image, G4String name, DIModality mod, DIOrientation orient)
  : DicomVImage(image, name,mod,orient)
{
  //  G4cout << this << " DicomVImageStr2 " << name << " " << DicomVImageStr::GetModalityStr(modality) << G4endl; //GDEB
  theDataStr = 0;
  SetHeader( image );

}

//-----------------------------------------------------------------------------
DicomVImageStr::DicomVImageStr(const DicomVImageStr& rhs)
  : DicomVImage(rhs)
{
    //  G4cout << this << " DicomVImageStr2 " << name << " " << DicomVImageStr::GetModalityStr(modality) << G4endl; //GDEB
  theDataStr = 0;
  
  //----- Copy image data
  size_t nData = GetNoVoxels();
  std::vector<G4String>* data2 = rhs.GetDataStr();
  for( size_t ii = 0; ii < nData; ii++ ) {
    theDataStr->at(ii) = data2->at(ii);
  }

}

//-----------------------------------------------------------------------------
DicomVImageStr::~DicomVImageStr()
{
  theDataStr->clear();
  delete theDataStr;
}

//-----------------------------------------------------------------------------
void DicomVImageStr::SetHeader(const DicomVImage* image )
{
  DicomVImage::SetHeader(image);
  if( theDataStr ) theDataStr->clear();
  theDataStr = new std::vector<G4String>(theNoVoxelsX*theNoVoxelsY*theNoVoxelsZ);
  //  G4cout << GetName() << " IMAGE PRECISION " << thePrecision << " from " << DP_PRECISION << G4endl; //GDEB
}

//-----------------------------------------------------------------------------
DicomVImageStr DicomVImageStr::operator=( const DicomVImageStr& rhs )
{
 if (this != &rhs) {   //  CheckImageHeadersAreEqual( &rhs );
  SetHeader( &rhs );
  /*  theIsodosesXY = 0;
  theIsodosesXZ = 0;
  theIsodosesYZ = 0;
  */
  
  //----- Copy image data
  size_t nData = GetNoVoxels();
  std::vector<G4String>* data2 = rhs.GetDataStr();
  for( size_t ii = 0; ii < nData; ii++ ) {
    theDataStr->at(ii) = data2->at(ii);
  }
 }  
 *theRotation = (*(rhs.GetRotation()));
 
 return *this;
}

//-----------------------------------------------------------------------------
G4bool DicomVImageStr::ReadDataFromTextFile( std::ifstream& fin, G4bool bReadHeader )
{
  if( bReadHeader ) {
    fin >> theNoVoxelsX >> theNoVoxelsY >> theNoVoxelsZ;
    if( DicomVerb(debugVerb) ) G4cout << "DicomVImageStr " << GetName() << " " << GetModalityStr() << " nVoxel X/Y/Z " << theNoVoxelsX << " " << theNoVoxelsY << " " << theNoVoxelsZ << G4endl;
    theData = new std::vector<G4double>(theNoVoxelsX*theNoVoxelsY*theNoVoxelsZ);
    fin >> theMinX >> theMaxX;
    fin >> theMinY >> theMaxY;
    fin >> theMinZ >> theMaxZ;
    if( DicomVerb(debugVerb) ) G4cout << "DicomVImageStr " << GetName() << " " << GetModalityStr()
				      << " min/max X " << theMinX << " " << theMaxX 
				      << " min/max X " << theMinY << " " << theMaxY 
				      << " min/max X " << theMinZ << " " << theMaxZ << G4endl;
  }
  G4double voxelDimX = (theMaxX-theMinX)/theNoVoxelsX;
  G4double voxelDimY = (theMaxY-theMinY)/theNoVoxelsY;
  G4double voxelDimZ = (theMaxZ-theMinZ)/theNoVoxelsZ;
  G4String dataStr;

  size_t nVox = GetNoVoxels();
  for( size_t ii = 0; ii < nVox; ii++ ) {
    fin >> dataStr;
    // look for old way to define structures
    if( GmGenUtils::IsInteger(dataStr) && GmGenUtils::GetInteger(dataStr) > 1000 ) G4cout <<theName << " OLD WAY TO DEFINE STRUCTURES in .g4dcm: NOW THEY MUST BE SEPARATED BY '\"' " << dataStr << G4endl; //GDEB
    theDataStr->at(ii) = dataStr;
    //    std::set<G4int> dataID = GetIDList(dataStr);
    //    theDataIDs->at(ii) = dataID;
    size_t ix = ii%theNoVoxelsX;
    size_t iy = ii/theNoVoxelsX%theNoVoxelsY;
    size_t iz = ii/theNoVoxelsX/theNoVoxelsY;
    double posX = theMinX+(ix+0.5)*voxelDimX;
    double posY = theMinY+(iy+0.5)*voxelDimY;
    double posZ = theMinZ+(iz+0.5)*voxelDimZ;
    G4ThreeVector pos(posX,posY,posZ);
    if( DicomVerb(testVerb) && ii%1 == 0 ) G4cout << GetName() << " : " << ii << " " << ix << " " << iy << " " << iz << " = " << dataStr << G4endl; 
  }

  thePrecision = 1.e-4*std::min(std::min(GetVoxelDimX(),GetVoxelDimY()),GetVoxelDimZ());
  //  G4cout << this << " 1IMAGE PRECISION " << thePrecision << " from " << DP_PRECISION << " EOF " << fin.eof() << G4endl; //GDEB

  if( fin.eof() ) {
    return 1;
  } else {
    return 0;
  }
}

//-----------------------------------------------------------------------------
void DicomVImageStr::DumpDataToTextFile(std::ostream& fout, G4bool )
{  
  if( DicomVerb(warningVerb) ) G4cout << GetName() << " DumpDataToTextFile " << G4endl;
  size_t copyNo = 0;
  for( size_t iz = 0; iz < theNoVoxelsZ; iz++ ) {
    for( size_t iy = 0; iy < theNoVoxelsY; iy++ ) {
      //	  G4cout << ic+ir*fNoVoxelsX << " pixdata " << ir << " " << ic << " = " << pixData[ic+ir*fNoVoxelsX] << G4endl; //GDEB
      for( size_t ix = 0; ix < theNoVoxelsX; ix++ ) {
	//	  G4cout << ic+ir*fNoVoxelsX << " pixdata " << ir << " " << ic << " = " << pixData[ic+ir*fNoVoxelsX] << G4endl; //GDEB
	//	G4cout << ix+iy*theNoVoxelsX+iz*theNoVoxelsX*theNoVoxelsY << " : " << ix << " " << iy << " " << iz << " " << theMinX+GetVoxelDimX()*(ix+0.5) << " " << theMinY+GetVoxelDimY()*(iy+0.5) << " " << theMinZ+GetVoxelDimZ()*(iz+0.5) << " DATA_TO_TEXT " << theData->at(copyNo) << G4endl; //GDEB
	G4String data = theDataStr->at(copyNo);
	if( data == "" ) data = "0";
	fout << data << " ";
	/*	  G4ThreeVector pos = GetPosition(ix,iy,iz);
    G4double posZ = pos.z(); //GDEB
    G4double posY = pos.y(); //GDEB
    if( posZ < -867 && posZ > -871 && posY < -100 && posY > -140 )
      G4cout << GetName() << " " << pos << " WRITE DATA " << data << G4endl; //GDEB
    if( data == "66" ) G4cout << GetName() << " " << pos << " 66WRITE DATA " << data << G4endl; //GDEB
	*/
	copyNo++;
	//-       	if( !bFloat ) fout << " " << copyNo << " D=" << *data++ << G4endl;
	//	if( ix != theNoVoxelsX-1) fout << " ";
      }
      fout << G4endl;
    }
  }
}  


//---------------------------------------------------------------------------
std::set<G4int> DicomVImageStr::GetIDList( G4int id ) 
{
  G4String voxelDataStr = theDataStr->at(id);
  std::set<G4int> idList;

  std::vector<G4String> idListV = GmGenUtils::StringSplit( voxelDataStr, ":" );
  for( size_t ii = 0; ii < idListV.size(); ii++ ) {
    idList.insert(GmGenUtils::GetInt(idListV[ii]));
  }

  //  G4cout << " DicomVImageStr::GetIDList( voxel " << voxelID << " id " << id << " sfhit=" << theShift << " nIDS " << nIDs << G4endl; //GDEB
  return idList;

}


//----------------------------------------------------------------------
void DicomVImageStr::ClearData()
{
  if( theDataStr ) theDataStr->clear();
}


