#include "GmReadPhantomSVMgr.hh"
#include "GmReadDICOMVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

#include <fstream>

GmReadPhantomSVMgr* GmReadPhantomSVMgr::theInstance = 0;

//---------------------------------------------------------------------------
GmReadPhantomSVMgr* GmReadPhantomSVMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new GmReadPhantomSVMgr();
  }
  return theInstance;
}

//---------------------------------------------------------------------------
void GmReadPhantomSVMgr::ReadPVEGS( std::ifstream& fin, G4int nVoxelX, G4int nVoxelY, G4int nVoxelZ )
{
  //  std::vector<G4String> thePVNames;
  //  thePVNames = GmParameterMgr::GetInstance()->GetVStringValue("GmReadPhantomEGSwithPSGeometry:PVNames",thePVNames);

  G4int thePVNOD = GmParameterMgr::GetInstance()->GetNumericValue("GmReadPhantomEGSwithPSGeometry:PVNumberOfDigits",2);

  thePVIDs = new size_t[nVoxelX*nVoxelY*nVoxelZ];
#ifndef GAMOS_NO_VERBOSE
  if( ReadDICOMVerb(debugVerb) ) {
    G4cout << " GmReadPhantomSVMgr::ReadPV created thePVIDs of size " << nVoxelX*nVoxelY*nVoxelZ << G4endl;
  }
#endif

  G4String stemp;
  G4int id;
  for( G4int ii = 0; ii < nVoxelY*nVoxelZ; ii++ ){
    fin >> stemp;
    for( size_t jj = 0; jj < stemp.length(); jj += thePVNOD ){
      id = GmGenUtils::GetInt(stemp.substr(jj,thePVNOD));
      G4int copyNo = jj/thePVNOD + ii*nVoxelX;
      //      G4cout << " GmReadPhantomSVMgr copyNo " << copyNo << " jj " << jj << " ii " << ii << G4endl;
#ifndef GAMOS_NO_VERBOSE
      if( ReadDICOMVerb(debugVerb) ) {
	if( id != -1 ) G4cout << "GmReadPhantomSVMgr::ReadPV  ReadPhantomPV " << copyNo << " = " << id << G4endl;
    }
#endif
      thePVIDs[copyNo] = id;
    }
  }
  /*  for( G4int iz = 0; iz < nVoxelZ; iz++ ) {
    for( G4int iy = 0; iy < nVoxelY; iy++ ) {
      for( G4int ix = 0; ix < nVoxelX; ix++ ) {
	fin >> id; 
	G4int nnew = ix + (iy)*nVoxelX + (iz)*nVoxelX*nVoxelY;
	//	G4cout << ix << " " << iy << " " << iz << " filling mateIDs " << nnew << " = " << atoi(cid)-1 << G4endl;
	thePVIDs[nnew] = id;
      }
    }
    } */

  G4int tvnumber;
  G4String tvname;
  for(;;){
    fin >> tvnumber >> tvname;
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(debugVerb) ) G4cout << "GmReadPhantomSVMgr::ReadPV:  Reading tvnumber " << tvnumber << " tvname " << tvname << G4endl;
#endif
    if( fin.eof() ) break;
    thePVs[tvnumber] = tvname;
    /*    G4bool bfound = FALSE;
    for( size_t ii = 0; ii < thePVNames.size(); ii++ ){  
      if( tvname == thePVNames[ii] ){
	thePVNumbers.insert( tvnumber );
	bfound = TRUE;
      } 
    }
    if( !bfound ) {
      G4Exception("GmReadPhantomSVMgr::ReadPV",
		  "",
		  FatalErrorInArgument,
		  (" volume name not found " + tvname).c_str());
		  } */
  }

}

//---------------------------------------------------------------------------
void GmReadPhantomSVMgr::ReadPVG4( std::ifstream& fin, G4int nVoxelX, G4int nVoxelY, G4int nVoxelZ )
{
  //  std::vector<G4String> thePVNames;
  //  thePVNames = GmParameterMgr::GetInstance()->GetVStringValue("GmReadPhantomEGSwithPSGeometry:PVNames",thePVNames);

  G4int nVoxels = nVoxelX*nVoxelY*nVoxelZ;
thePVIDs = new size_t[nVoxels];
#ifndef GAMOS_NO_VERBOSE
 if( ReadDICOMVerb(debugVerb) ) {
   G4cout << " GmReadPhantomSVMgr::ReadPV created thePVIDs of size " << nVoxelX*nVoxelY*nVoxelZ << G4endl;
 }
#endif
  G4String stemp;
  G4int id;
  for( G4int ii = 0; ii < nVoxels; ii++ ){
    fin >> id;
      //      G4cout << " GmReadPhantomSVMgr copyNo " << copyNo << " jj " << jj << " ii " << ii << G4endl;
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(debugVerb) ) {
       if( id != -1 ) G4cout << "GmReadPhantomSVMgr::ReadPV  ReadPhantomPV " << ii << " = " << id << G4endl;
}
#endif
    thePVIDs[ii] = id;
  }
  /*  for( G4int iz = 0; iz < nVoxelZ; iz++ ) {
    for( G4int iy = 0; iy < nVoxelY; iy++ ) {
      for( G4int ix = 0; ix < nVoxelX; ix++ ) {
	fin >> id; 
	G4int nnew = ix + (iy)*nVoxelX + (iz)*nVoxelX*nVoxelY;
	//	G4cout << ix << " " << iy << " " << iz << " filling mateIDs " << nnew << " = " << atoi(cid)-1 << G4endl;
	thePVIDs[nnew] = id;
      }
    }
    } */

  G4int tvnumber;
  G4String tvname;
  for(;;){
    fin >> tvnumber >> tvname;
    if( fin.eof() ) break;
#ifndef GAMOS_NO_VERBOSE
    if( ReadDICOMVerb(debugVerb) ) {
      G4cout << "GmReadPhantomSVMgr::ReadPV:  Reading tvnumber " << tvnumber << " tvname " << tvname << G4endl;
    }
#endif
    thePVs[tvnumber] = tvname;
    /*    G4bool bfound = FALSE;
    for( size_t ii = 0; ii < thePVNames.size(); ii++ ){  
      if( tvname == thePVNames[ii] ){
	thePVNumbers.insert( tvnumber );
	bfound = TRUE;
      } 
    }
    if( !bfound ) {
      G4Exception("GmReadPhantomSVMgr::ReadPV",
		  "",
		  FatalErrorInArgument,
		  (" volume name not found " + tvname).c_str());
		  } */
  }

}

//---------------------------------------------------------------------------
G4int GmReadPhantomSVMgr::GetPVID( size_t voxelID )
{
  /*  //  size_t vid = *(thePVIDs+voxelID*sizeof(size_t));
  size_t vid = thePVIDs[voxelID];
  std::map<G4int,G4String>::const_iterator ite = thePVs.find(vid);
  if( ite != thePV.end() ){
    return vid;
  }else {
    return -1;
  }
*/

  return thePVIDs[voxelID];

}

//---------------------------------------------------------------------------     
G4int GmReadPhantomSVMgr::GetPVIDFromPVName( G4String pvName )
{
  std::map<G4int,G4String>::const_iterator ite;
  for( ite = thePVs.begin(); ite != thePVs.end(); ite++ ) {
    if( (*ite).second == pvName ) {
      return (*ite).first;      
    }
  }

  for( ite = thePVs.begin(); ite != thePVs.end(); ite++ ) {
    G4cerr << " GmReadPhantomSVMgr::GetPVIDFromPVName PV " << (*ite).first << " : " << (*ite).second << G4endl;
  }
  G4Exception("GmReadPhantomSVMgr::GetPVIDFromPVName",
	      "",
	      FatalException,
	      ("PV name not found " + pvName).c_str());

  return -1;
}
