#include "GmPositionG4Materials.hh"
#include "GmVGenerDistPositionVolumesAndSurfaces.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "G4VSolid.hh"

//------------------------------------------------------------------------
GmPositionG4Materials::GmPositionG4Materials()
{
}

//------------------------------------------------------------------------
std::vector<GVSTouchableInfo*> GmPositionG4Materials::AddTouchableInfos( const std::vector<G4String>& params ) const
{
  std::vector<GVSTouchableInfo*> tinfos;
  std::set<G4String> mateList;
  
  std::vector<GmTouchable*> touchs = GmGeometryUtils::GetInstance()->GetTouchables( "*", TRUE );
  for( std::vector<GmTouchable*>::const_iterator ite = touchs.begin(); ite != touchs.end(); ite++ ) {
    G4String mateName = (*ite)->GetMaterialName();
    mateList.insert(mateName);
    //    G4cout << " TEST pos " << mateName << " vol " << (*ite)->GetLongName() << " " << (*ite)->GetGlobalPos() << G4endl; //GDEB
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      if( GmGenUtils::AreWordsEquivalent( params[ii], mateName ) ) {
	 GVSTouchableInfo* tinfo = new GVSTouchableInfo;
	 tinfo->name = (*ite)->GetLongName();
	 tinfo->solid = (*ite )->GetSolid();
	 tinfo->pos = (*ite)->GetGlobalPos();
	 tinfo->rotmat = (*ite)->GetGlobalRotation();
	 //	 G4cout << tinfos.size() << " ADDING tinfo pos " <<  tinfo->pos << " " << mateName << " vol " << tinfo->name << G4endl; //GDEB

#ifndef GAMOS_NO_VERBOSE
	 if( GenerVerb(infoVerb) ) G4cout << tinfos.size() << " ADDING tinfo pos " <<  tinfo->pos << G4endl;
#endif
	 tinfos.push_back(tinfo);
      }
    }	
    delete *ite;
  }

  if( tinfos.size() == 0 ) {
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      std::cerr << " Material searched for: " << params[ii] << std::endl;
    }
    std::set<G4String>::const_iterator ites;
    for( ites = mateList.begin(); ites != mateList.end(); ites++ ) {
      std::cerr << " Material in geometry: " << *ites << std::endl;
    }
    G4Exception("GmGenerDistPositionInG4Materials::AddTouchableInfos",
		"Wrong argument",
		FatalErrorInArgument,
		"No material found in geometry, recheck the names " );
  }

  return tinfos;  
}
