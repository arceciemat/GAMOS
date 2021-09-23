#include "GmReadPhantomPSMgr.hh"
#include "GmReadDICOMVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

#include <fstream>

GmReadPhantomPSMgr* GmReadPhantomPSMgr::theInstance = 0;

//---------------------------------------------------------------------------
GmReadPhantomPSMgr* GmReadPhantomPSMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new GmReadPhantomPSMgr();
  }
  return theInstance;
}

//---------------------------------------------------------------------------
void GmReadPhantomPSMgr::ReadPSEGS( GmFileIn& fin, G4int nVoxelX, G4int nVoxelY, G4int nVoxelZ )
{
  //  std::vector<G4String> thePVSNames;
  //  thePVSNames = GmParameterMgr::GetInstance()->GetVStringValue("GmReadPhantomEGSwithStGeometry:PVSNames",thePVSNames);

  G4int thePSNOD = GmParameterMgr::GetInstance()->GetNumericValue("GmReadPhantomEGSwithStGeometry:PVNumberOfDigits",2);

  thePVSIDs = new size_t[nVoxelX*nVoxelY*nVoxelZ];
#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(debugVerb) ) {
    G4cout << " GmReadPhantomPSMgr::ReadPSEGS created thePVSIDs of size " << nVoxelX*nVoxelY*nVoxelZ << G4endl;
  }
#endif

  std::vector<G4String> wl;
  G4String stemp;
  G4int id;
  std::ifstream* finf = fin.GetIfstream();
  for( G4int ii = 0; ii < nVoxelY*nVoxelZ; ii++ ){
    *finf >> stemp;
    for( size_t jj = 0; jj < stemp.length(); jj += thePSNOD ){
      id = GmGenUtils::GetInt(stemp.substr(jj,thePSNOD));
      G4int copyNo = jj/thePSNOD + ii*nVoxelX;
      //      G4cout << " GmReadPhantomPSMgr copyNo " << copyNo << " jj " << jj << " ii " << ii << G4endl;
#ifndef GAMOS_NO_VERBOSE
      if( ReadDICOMVerb(debugVerb) ) {
	//	if( id != -1 ) G4cout << "GmReadPhantomPSMgr::ReadPSEGS  ReadPhantomPS " << copyNo << " = " << id << G4endl;
    }
#endif
      thePVSIDs[copyNo] = id;
    }
  }
  /*  for( G4int iz = 0; iz < nVoxelZ; iz++ ) {
    for( G4int iy = 0; iy < nVoxelY; iy++ ) {
      for( G4int ix = 0; ix < nVoxelX; ix++ ) {
	fin >> id; 
	G4int nnew = ix + (iy)*nVoxelX + (iz)*nVoxelX*nVoxelY;
	//	G4cout << ix << " " << iy << " " << iz << " filling mateIDs " << nnew << " = " << atoi(cid)-1 << G4endl;
	thePVSIDs[nnew] = id;
      }
    }
    } */

  G4int tvnumber;
  G4String tvname;
  for(;;){
    if( !fin.GetWordsInLine(wl) ) return;
    tvnumber = GmGenUtils::GetInteger(wl[0]);
    tvname = wl[1];
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(debugVerb) ) G4cout << "GmReadPhantomPSMgr::ReadPSEGS:  Reading tvnumber " << tvnumber << " tvname " << tvname << G4endl;
#endif
    if( fin.eof() ) break;
    thePVSs[tvnumber] = tvname;
    /*    G4bool bfound = FALSE;
    for( size_t ii = 0; ii < thePVSNames.size(); ii++ ){  
      if( tvname == thePVSNames[ii] ){
	thePVSNumbers.insert( tvnumber );
	bfound = TRUE;
      } 
    }
    if( !bfound ) {
      G4Exception("GmReadPhantomPSMgr::ReadPS",
		  "",
		  FatalErrorInArgument,
		  (" volume name not found " + tvname).c_str());
		  } */
  }

}

//---------------------------------------------------------------------------
void GmReadPhantomPSMgr::ReadPSG4( GmFileIn& fin, G4int nVoxelX, G4int nVoxelY, G4int nVoxelZ )
{
  //  std::vector<G4String> thePVSNames;
  //  thePVSNames = GmParameterMgr::GetInstance()->GetVStringValue("GmReadPhantomEGSwithStGeometry:PVSNames",thePVSNames);

  G4int nVoxels = nVoxelX*nVoxelY*nVoxelZ;
thePVSIDs = new size_t[nVoxels];
#ifndef GAMOS_NO_VERBOSE
 if( ReadDICOMVerb(debugVerb) ) {
   G4cout << " GmReadPhantomPSMgr::ReadPSG4 created thePVSIDs of size " << nVoxelX*nVoxelY*nVoxelZ << G4endl;
 }
#endif
  G4String stemp;
  G4int id;
  std::vector<G4String> wl;
  std::ifstream* finf = fin.GetIfstream();
  for( G4int ii = 0; ii < nVoxels; ii++ ){
    *finf >> id;
      //      G4cout << " GmReadPhantomPSMgr copyNo " << copyNo << " jj " << jj << " ii " << ii << G4endl;
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(debugVerb) ) {
       if( id != -1 ) G4cout << "GmReadPhantomPSMgr::ReadPSG4  ReadPhantomPV " << ii << " = " << id << G4endl;
}
#endif
    thePVSIDs[ii] = id;
  }
  /*  for( G4int iz = 0; iz < nVoxelZ; iz++ ) {
    for( G4int iy = 0; iy < nVoxelY; iy++ ) {
      for( G4int ix = 0; ix < nVoxelX; ix++ ) {
	fin >> id; 
	G4int nnew = ix + (iy)*nVoxelX + (iz)*nVoxelX*nVoxelY;
	//	G4cout << ix << " " << iy << " " << iz << " filling mateIDs " << nnew << " = " << atoi(cid)-1 << G4endl;
	thePVSIDs[nnew] = id;
      }
    }
    } */

  G4int tvnumber;
  G4String tvname;
  for(;;){
    if( !fin.GetWordsInLine(wl) ) return;
    tvnumber = GmGenUtils::GetInteger(wl[0]);
    tvname = wl[1];;
    if( fin.eof() ) break;
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(debugVerb) ) {
      G4cout << "GmReadPhantomPSMgr::ReadPSG4:  Reading tvnumber " << tvnumber << " tvname " << tvname << G4endl;
    }
#endif
    thePVSs[tvnumber] = tvname;
    /*    G4bool bfound = FALSE;
    for( size_t ii = 0; ii < thePVSNames.size(); ii++ ){  
      if( tvname == thePVSNames[ii] ){
	thePVSNumbers.insert( tvnumber );
	bfound = TRUE;
      } 
    }
    if( !bfound ) {
      G4Exception("GmReadPhantomPSMgr::ReadPS",
		  "",
		  FatalErrorInArgument,
		  (" volume name not found " + tvname).c_str());
		  } */
  }

}

//---------------------------------------------------------------------------
G4int GmReadPhantomPSMgr::GetPVSID( size_t voxelID )
{
  /*  //  size_t vid = *(thePVSIDs+voxelID*sizeof(size_t));
  size_t vid = thePVSIDs[voxelID];
  std::map<G4int,G4String>::const_iterator ite = thePVSs.find(vid);
  if( ite != thePVS.end() ){
    return vid;
  }else {
    return -1;
  }
*/

  return thePVSIDs[voxelID];

}

//---------------------------------------------------------------------------     
G4int GmReadPhantomPSMgr::GetPVSIDFromPVSName( G4String pvName )
{
  std::map<G4int,G4String>::const_iterator ite;
  for( ite = thePVSs.begin(); ite != thePVSs.end(); ite++ ) {
    if( (*ite).second == pvName ) {
      return (*ite).first;      
    }
  }

  for( ite = thePVSs.begin(); ite != thePVSs.end(); ite++ ) {
    G4cerr << " GmReadPhantomPSMgr::GetPVSIDFromPVSName PVS " << (*ite).first << " : " << (*ite).second << G4endl;
  }
  G4Exception("GmReadPhantomPSMgr::GetPVSIDFromPVSName",
	      "",
	      FatalException,
	      ("PVS name not found " + pvName).c_str());

  return -1;
}
