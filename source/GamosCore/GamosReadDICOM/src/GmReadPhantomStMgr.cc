#include "GmReadPhantomStMgr.hh"
#include "GmReadDICOMVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

#include <fstream>

GmReadPhantomStMgr* GmReadPhantomStMgr::theInstance = 0;

//---------------------------------------------------------------------------
GmReadPhantomStMgr::GmReadPhantomStMgr()
{
}

//---------------------------------------------------------------------------
GmReadPhantomStMgr* GmReadPhantomStMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new GmReadPhantomStMgr();
  }
  return theInstance;
}

//---------------------------------------------------------------------------
void GmReadPhantomStMgr::ReadStructEGS( GmFileIn& fin, G4int nVoxelX, G4int nVoxelY, G4int nVoxelZ )
{
  G4int theStNOD = GmParameterMgr::GetInstance()->GetNumericValue("GmReadPhantomEGSwithStGeometry:StNumberOfDigits",2);

  theStIDs = new G4String[nVoxelX*nVoxelY*nVoxelZ];
#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(-debugVerb) ) {
    G4cout << " GmReadPhantomStMgr::ReadStructEGS created theStIDs of size " << nVoxelX*nVoxelY*nVoxelZ << G4endl;
  }
#endif

  std::vector<G4String> wl;
  G4String stemp;
  size_t id;
  std::ifstream* finf = fin.GetIfstream();
  for( G4int ii = 0; ii < nVoxelY*nVoxelZ; ii++ ){
    *finf >> stemp;
    for( size_t jj = 0; jj < stemp.length(); jj += theStNOD ){
      id = GmGenUtils::GetInt(stemp.substr(jj,theStNOD));
      size_t copyNo = jj/theStNOD + ii*nVoxelX;
      //      G4cout << " GmReadPhantomStMgr copyNo " << copyNo << " jj " << jj << " ii " << ii << G4endl;
#ifndef GAMOS_NO_VERBOSE
      if( ReadDICOMVerb(debugVerb) ) {
	if( id != 0 ) G4cout << "GmReadPhantomStMgr::ReadStructEGS  ReadPhantomSt " << copyNo << " = " << id << " <- " << G4endl;
    }
#endif
      theStIDs[copyNo] = stemp;
    }
  }

  G4int tvnumber;
  G4String tvname;
  for(;;){
    if( !fin.GetWordsInLine(wl) ) return;
    tvnumber = GmGenUtils::GetInteger(wl[0]);
    tvname = wl[1];
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(debugVerb) ) G4cout << "GmReadPhantomStMgr::ReadStructEGS:  Reading tvnumber " << tvnumber << " tvname " << tvname << G4endl;
#endif
    if( fin.eof() ) break;
    theStructs[tvnumber] = tvname;
  }

}

//---------------------------------------------------------------------------
void GmReadPhantomStMgr::ReadStructG4( GmFileIn& fin, G4int nVoxelX, G4int nVoxelY, G4int nVoxelZ )
{
  size_t nVoxels = nVoxelX*nVoxelY*nVoxelZ;
  theStIDs = new G4String[nVoxels];
#ifndef GAMOS_NO_VERBOSE
 if( ReadDICOMVerb(debugVerb) ) {
   G4cout << " GmReadPhantomStMgr::ReadStructG4 created theStIDs of size " << nVoxels << G4endl;
 }
#endif
  G4String stemp;
  std::vector<G4String> wl;
  std::ifstream* finf = fin.GetIfstream();
  for( size_t ii = 0; ii < nVoxels; ii++ ){
    *finf >> stemp;
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(infoVerb) ) {
      if( ii% 1000000 == 1 ) G4cout<<"GmReadPhantomStMgr::ReadStructG4  ReadPhantomStn " << ii << " = " << stemp << G4endl;
    }
    if( ReadDICOMVerb(debugVerb) ) {
      if( stemp != 0 ) G4cout <<"GmReadPhantomStMgr::ReadStructG4  ReadPhantomSt " << ii << " = " << stemp << G4endl;
    }
#endif
    theStIDs[ii] = stemp;
  }
  /*  for( G4int iz = 0; iz < nVoxelZ; iz++ ) {
    for( G4int iy = 0; iy < nVoxelY; iy++ ) {
      for( G4int ix = 0; ix < nVoxelX; ix++ ) {
	fin >> id; 
	G4int nnew = ix + (iy)*nVoxelX + (iz)*nVoxelX*nVoxelY;
	//	G4cout << ix << " " << iy << " " << iz << " filling mateIDs " << nnew << " = " << atoi(cid)-1 << G4endl;
	theStIDs[nnew] = id;
      }
    }
    } */

  G4int tvnumber;
  G4String tvname;
  if( fin.eof() ) {
      G4Exception("GmReadPhantomStMgr::ReadStructG4",
		  "",
		  FatalErrorInArgument,
		  "End of file before list of structure names");
  }
  //  G4cout << "TO READ GmReadPhantomStMgr::ReadStructG4: " << G4endl; //GDEB
  for(;;){
    if( !fin.GetWordsInLine(wl) ) return;
    tvnumber = GmGenUtils::GetInteger(wl[0]);
    tvname = wl[1];;
    if( fin.eof() ) break;
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(debugVerb) ) {
      G4cout << "GmReadPhantomStMgr::ReadStructG4:  Reading tvnumber " << tvnumber << " tvname " << tvname << G4endl;
    }
#endif
    theStructs[tvnumber] = tvname;
  }

}

//---------------------------------------------------------------------------
G4String GmReadPhantomStMgr::GetStID( size_t voxelID )
{
  return theStIDs[voxelID];

}

//---------------------------------------------------------------------------
std::set<size_t> GmReadPhantomStMgr::GetStIDList( size_t voxelID )
{
  G4cout << voxelID << G4endl; //GDEB
  G4String stid1 = theStIDs[voxelID];
  std::vector<G4String> stidlistStr = GmGenUtils::StringSplit(stid1,":");
  G4cout << voxelID << " stid1 " << stid1 << " " << stidlistStr.size() << G4endl; //GDEB
  std::set<size_t> stIDList;
  for( size_t ii = 0; ii < stidlistStr.size(); ii++ ) {
    G4cout << voxelID << " " << ii << " stidlistStr " << stidlistStr[ii] << G4endl; //GDEB
    stIDList.insert( GmGenUtils::GetInt(stidlistStr[ii]) );
  }
  
  return stIDList;

}

//---------------------------------------------------------------------------   
std::vector<G4int> GmReadPhantomStMgr::GetStIDFromPhysVolName( G4String pvName )
{
  std::vector<G4int> stids;
  std::map<G4int,G4String>::const_iterator ite;
  for( ite = theStructs.begin(); ite != theStructs.end(); ite++ ) {
    if( GmGenUtils::AreWordsEquivalent(pvName,(*ite).second) ) {
      stids.push_back((*ite).first);     
    }
  }

  if( stids.size() == 0 ){
    for( ite = theStructs.begin(); ite != theStructs.end(); ite++ ) {
      G4cerr << " GmReadPhantomStMgr::GetStIDFromPhysVolName Struct " << (*ite).first << " : " << (*ite).second << G4endl;
    }
    G4Exception("GmReadPhantomStMgr::GetStIDFromPhysVolName",
		"",
		FatalException,
	      ("Struct name not found " + pvName).c_str());
    
  }
  
  return stids;
}

//---------------------------------------------------------------------------   
G4String GmReadPhantomStMgr::GetStName( size_t stID )
{
  G4String stName = "NOT_FOUND";
  for( std::map<G4int,G4String>::const_iterator ite = theStructs.begin(); ite != theStructs.end(); ite++ ) {
    if( ite->first == int(stID) ) {
      return ite->second;
    }
  }

  return stName;
}

//---------------------------------------------------------------------------   
G4String GmReadPhantomStMgr::GetStNameList( size_t voxelID )
{
  G4String stName = "NO_STRUCT";
  std::set<size_t> stIDList = GetStIDList(voxelID);
  for( std::set<size_t>::const_iterator ite = stIDList.begin(); ite != stIDList.end(); ite++ ) {
    size_t stID = *ite;
    if( stID != 0 ) {
      if( stName == "NO_STRUCT" ) {
	stName = GetStName(stID);
      } else {
	stName += ":"+ GetStName(stID);
      }
    }
  }

  return stName;

}
