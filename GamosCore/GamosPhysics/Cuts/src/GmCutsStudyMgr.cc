#include "GmCutsStudyMgr.hh"
#include "GmCSTrackInfo.hh"
#include "GmCSTrackStepInfo.hh"
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------------
GmCutsStudyMgr* GmCutsStudyMgr::theInstance = 0;

//-----------------------------------------------------------------------
GmCutsStudyMgr::GmCutsStudyMgr()
{
}

//----------------------------------------------------------------------
GmCutsStudyMgr* GmCutsStudyMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new GmCutsStudyMgr;
  }

  return theInstance;

}

//-----------------------------------------------------------------------
GmCutsStudyMgr::~GmCutsStudyMgr()
{
}

//-----------------------------------------------------------------------
void GmCutsStudyMgr::AddTrackInfo( GmCSTrackInfo* ti )
{
  theTrackInfos.insert(mmrcs::value_type( ti->GetRegion(), ti ) );
}
 
//-----------------------------------------------------------------------
GmCSTrackInfo* GmCutsStudyMgr::GetTrackInfo( G4int id )
{
  GmCSTrackInfo* ti = 0;
  G4int nti = 0;
  mmrcs::const_iterator ite;
  for( ite = theTrackInfos.begin(); ite != theTrackInfos.end(); ite++ ){
    if( (*ite).second->GetTrackID() == id ) {
     ti = (*ite).second;
#ifndef GAMOS_NO_VERBOSE
      if( PhysicsVerb(debugVerb) ) G4cout << " GmCutsStudyMgr::GetTrackInfo ";
#endif
      if( PhysicsVerb(debugVerb) ) ti->Print();
     if( nti != 0 ){
       G4Exception("GmCutsStudyMgr::GetTrackInfo",
		   " Two track infos with same track ID ",JustWarning,G4String("Track ID = "+ GmGenUtils::itoa(id)).c_str());
     }
     nti++;
    }
  }
  
  return ti;
}


//-----------------------------------------------------------------------
GmCSTrackStepInfo* GmCutsStudyMgr::GetTrackStepInfo( G4int id, G4ThreeVector pos )
{
  GmCSTrackStepInfo* ti = 0;
  //  G4int nti = 0;
  mmrcs::reverse_iterator ite;
  // Found one case of e+ where eIoni takes all energy, and then next step is annihil, at the same point. Take always last
  //  G4cout << " GetTrackStepInfo N ti " << theTrackInfos.size() << G4endl;
  for( ite = theTrackInfos.rbegin(); ite != theTrackInfos.rend(); ite++ ){
    GmCSTrackStepInfo* tsit = (GmCSTrackStepInfo*)((*ite).second);
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << "GmCutsStudyMgr::GetTrackStepInfo " << tsit->GetTrackID() << " =? " << id << " pos " << tsit->GetPosition()<< " =? " << pos << " " << (tsit->GetPosition()-pos).mag() << G4endl;
#endif
    if( tsit->GetTrackID() == id && (tsit->GetPosition()-pos).mag() < 1.e-9 ) {
      ti = tsit;
#ifndef GAMOS_NO_VERBOSE
      if( PhysicsVerb(debugVerb) ) G4cout << " GmCutsStudyMgr::GetTrackStepInfo found ";   if( PhysicsVerb(debugVerb) ) ti->Print();
#endif
      break;
      /*      if( nti != 0 ){
	G4Exception("GmCutsStudyMgr::GetTrackStepInfo",
	" Two track infos with same track ID ",JustWarning,G4String("Track ID = "+ GmGenUtils::itoa(id)).c_str());
      }
      nti++; */
    }
  }
  
  return ti;
}

//-----------------------------------------------------------------------
std::vector<GmCSTrackStepInfo*> GmCutsStudyMgr::GetTrackStepInfosBefore( G4int id, G4double trklen )
{
  std::vector<GmCSTrackStepInfo*> tis;

  mmrcs::reverse_iterator ite;
  //  G4cout << " GetTrackStepInfosBefore N ti " << theTrackInfos.size() << G4endl;
  GmCSTrackStepInfo* tsitPrev = 0;
  for( ite = theTrackInfos.rbegin(); ite != theTrackInfos.rend(); ite++ ){
    GmCSTrackStepInfo* tsit = (GmCSTrackStepInfo*)((*ite).second);
    //---- check that previous step is not on the same region but with bigger energy
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << " GetTrackStepInfo " << tsit->GetTrackID() << "=? " << id << " trklen " << tsit->GetTrackLength()<< " <=? " << trklen << " energy " << tsit->GetEnergy() << G4endl;
#endif
    if( tsit->GetTrackID() == id && tsit->GetTrackLength() <= trklen ) { //includes the proper trkInfo
      if( tsitPrev && tsit->GetTrackID() == tsitPrev->GetTrackID() 
	  && tsit->GetRegion() == tsitPrev->GetRegion() ){
	//	G4cout << " ENER " << tsit->GetEnergy() << " prev " << tsitPrev->GetEnergy() << G4endl;
	if( tsit->GetEnergy() >= tsitPrev->GetEnergy() ) continue;
      }
      tis.push_back(tsit);
#ifndef GAMOS_NO_VERBOSE
      if( PhysicsVerb(debugVerb) ) G4cout << " GmCutsStudyMgr::GetTrackStepInfo ";   if( PhysicsVerb(debugVerb) ) tsit->Print();
#endif
      tsitPrev = tsit;
    }
  }
  
  return tis;
}



//-----------------------------------------------------------------------
void GmCutsStudyMgr::CleanTrackInfos()
{
  mmrcs::const_iterator ite;
  for( ite = theTrackInfos.begin(); ite != theTrackInfos.end(); ite++ ){
    delete (*ite).second;
  }
  theTrackInfos.clear();

}
