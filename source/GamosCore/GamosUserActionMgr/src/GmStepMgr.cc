#include "G4TouchableHandle.hh"
#include "G4TouchableHistory.hh"
#define private public
#include "G4Track.hh"

#include "GmStepMgr.hh"
#include "GmFutureFilterUA.hh"
#include "GmUserActionMgr.hh"
#include "GmFutureFilter.hh"

#include "GamosCore/GamosUserActionMgr/include/GmUAVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"

#include <set>

GmStepMgr* GmStepMgr::theInstance = 0;

//-----------------------------------------------------------------------
GmStepMgr::GmStepMgr()
{
  GmFutureFilterUA* ffUA = new GmFutureFilterUA;
  ffUA->SetName( "GmFutureFilterUA" );
  ffUA->SetApplyFiltersToStepping(FALSE);
  ffUA->SetStepMgr( this );
  //  GmUserActionMgr::GetInstance()->RegisterEventAction( ffUA ); 
  //  GmUserActionMgr::GetInstance()->RegisterSteppingAction( ffUA );
  GmUserActionMgr::GetInstance()->PutSteppingActionFirst( ffUA ); 
}

//------------------------------------------------------------------------
GmStepMgr* GmStepMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new GmStepMgr();
  }

  return theInstance;
}

//-----------------------------------------------------------------------
void GmStepMgr::SaveStep( const G4Step* aStep )
{
#ifndef GAMOS_NO_VERBOSE
  if( UAVerb(debugVerb) ) {
    G4cout << "GmStepMgr::SaveStep " << G4endl;
  }
#endif

  G4Track* newTrack = SaveTrack( aStep->GetTrack() );

  G4Step* newStep = new G4Step();
  G4cout << " NEW STEP " << newStep << G4endl;

  G4StepPoint* preSP = new G4StepPoint( *(aStep->GetPreStepPoint()) );
  theNewStepPoints.push_back( preSP );
  G4StepPoint* postSP = new G4StepPoint( *(aStep->GetPostStepPoint()) );
  theNewStepPoints.push_back( postSP );
  newStep->SetPreStepPoint( preSP );
  newStep->SetPostStepPoint( postSP );
  newStep->SetTrack(newTrack);
  newTrack->SetStep(newStep);

  // step length
  newStep->SetStepLength(aStep->GetStepLength());

   // total energy deposit 
  newStep->SetTotalEnergyDeposit(aStep->GetTotalEnergyDeposit());

  // total non-ionizing energy deposit 
  newStep->SetNonIonizingEnergyDeposit(aStep->GetNonIonizingEnergyDeposit());

  // control flag for stepping
  newStep->SetControlFlag(aStep->GetControlFlag());

  if(aStep->IsFirstStepInVolume() ) newStep->SetFirstStepFlag();
  if(aStep->IsLastStepInVolume() ) newStep->SetLastStepFlag();

  G4TrackVector* secondaries = new G4TrackVector( *(aStep->GetSecondary()) );
  newStep->SetSecondary( secondaries );

  theNewSteps.insert(mmis::value_type(aStep->GetTrack()->GetTrackID(), newStep));

  //---- Store this step as origin of the created secondary tracks 
  G4TrackVector secos = GmG4Utils::GetSecondariesOfCurrentStep();
  G4int currentTrackID;
  if( theCreatorStep.size() == 0 ) {
    currentTrackID = aStep->GetTrack()->GetTrackID(); // first primary track
  } else {
    std::map<G4int,G4Step*>::reverse_iterator rite = theCreatorStep.rbegin();
    currentTrackID = (*rite).first;
  }
  for( G4TrackVector::reverse_iterator ite = secos.rbegin(); ite != secos.rend(); ite++ ){
    theCreatorStep[++currentTrackID] = newStep;
#ifndef GAMOS_NO_VERBOSE
  if(UAVerb(debugVerb))  G4cout << " TRACK ID " << currentTrackID << " assigned to step " << G4endl;
#endif  
  }
}

//-----------------------------------------------------------------------
G4Track* GmStepMgr::SaveTrack( const G4Track* aTrack )
{
  G4Track* newTrack = new G4Track( *aTrack );
  
  newTrack->SetTrackID(aTrack->GetTrackID());
  newTrack->SetParentID(aTrack->GetParentID());
  newTrack->fCurrentStepNumber = aTrack->GetCurrentStepNumber();
  newTrack->SetCreatorProcess( aTrack->GetCreatorProcess());
  newTrack->SetUserInformation( aTrack->GetUserInformation());
  newTrack->SetWeight( aTrack->GetWeight() );
  newTrack->SetStepLength( aTrack->GetStepLength() );

  G4TouchableHistory* newTouchable = new G4TouchableHistory();
  newTouchable->UpdateYourself( aTrack->GetTouchable()->GetVolume(), aTrack->GetTouchable()->GetHistory() );
  newTrack->SetTouchableHandle( newTouchable );
  G4TouchableHistory* newNextTouchable = new G4TouchableHistory();
  newNextTouchable->UpdateYourself( aTrack->GetNextTouchable()->GetVolume(), aTrack->GetNextTouchable()->GetHistory() );
  newTrack->SetNextTouchableHandle( newNextTouchable );
  theNewTouchables.push_back( newNextTouchable );
  if( aTrack->GetOriginTouchable() ) {
    G4TouchableHistory* newOriginTouchable = new G4TouchableHistory();
    newOriginTouchable->UpdateYourself( aTrack->GetOriginTouchable()->GetVolume(), aTrack->GetOriginTouchable()->GetHistory() );
    newTrack->SetOriginTouchableHandle( newOriginTouchable );     
    theNewTouchables.push_back( newOriginTouchable );
  }

  theNewTracks[aTrack->GetTrackID()] = newTrack;

  return newTrack;
 }

//-----------------------------------------------------------------------
std::vector<G4Step*> GmStepMgr::GetSteps( GmFutureFilter* filter, const G4Step* currentStep )
{
  std::vector<G4Step*> acceptedSteps;
  
#ifndef GAMOS_NO_VERBOSE
  if(  UAVerb(debugVerb)) G4cout << " GmStepMgr::GetSteps " << filter->GetName() << G4endl;
#endif
  if( !filter->IsFilterWithChildren() ) { // look only to current track
    mmis::const_iterator ites;
    for( ites = theNewSteps.begin(); ites != theNewSteps.end(); ites++ ){
      if( currentStep->GetTrack()->GetTrackID() == (*ites).first 
	  && filter->AcceptPastStep( (*ites).second ) ) acceptedSteps.push_back( (*ites).second );
#ifndef GAMOS_NO_VERBOSE
      if(  UAVerb(debugVerb)) G4cout << " GmStepMgr::GetSteps Filter without children currentStep TrackID " << currentStep->GetTrack()->GetTrackID() << " =? step TrackID " << (*ites).first << " NacceptedSetps " << acceptedSteps.size() << G4endl;
#endif

    }
  } else { // futureFilter is on children, so look to all ancestors too
    // build list of step ancestors of current step
    std::map<G4int,G4Track*>::const_iterator itet;
    std::set<G4Step*> ancestorSteps; 
    G4int trkID = currentStep->GetTrack()->GetTrackID();
    // get track curresponding to step
    G4Track* trk = 0;
    for( itet = theNewTracks.begin(); itet != theNewTracks.end(); itet++ ){
      if( (*itet).second->GetTrackID() != (*itet).first ) { 
	G4Exception("ERROR",
		    "ERROR",
		    FatalException,
		    "TRACK ID WRONG ");
      }
      if( (*itet).second->GetTrackID() == trkID ) { 
	trk = (*itet).second;
	break;
      }
    }
    itet = theNewTracks.find(trkID);
    //    if( !trk ) {
    if( itet == theNewTracks.end() ) {
      G4Exception("GmStepMgr::GetSteps",
		  "Track does not exist",
		  FatalException,
		  "Please contact GAMOS authors");
    }

    // Now look for parent steps
    G4Step* ancestorStep = const_cast<G4Step*>(currentStep);
    ancestorSteps.insert( ancestorStep );
    for(;;) {

      //--- Loop to steps of parent track, the one closer in position
      std::pair<mmis::const_iterator,mmis::const_iterator> pite = theNewSteps.equal_range(ancestorStep->GetTrack()->GetParentID());
      mmis::const_iterator ite;
      G4double dist = DBL_MAX;
      G4ThreeVector trackVtxPos = ancestorStep->GetTrack()->GetVertexPosition();
      for( ite = pite.first; ite != pite.second; ite++ ) {
	G4double distNew = ((*ite).second->GetPostStepPoint()->GetPosition() - trackVtxPos).mag();
	//	G4cout << " DISTNEW " << distNew << " " << dist << " POS THIS STEP " << (*ite).second->GetPostStepPoint()->GetPosition() << " POS TRACK VERTEX " << trackVtxPos << G4endl; 
	if( distNew < dist ) {
	  dist = distNew;
	  ancestorStep = (*ite).second;	 
	}
      }
      if( dist == DBL_MAX ) {
	G4Exception("GmStepMgr::GetSteps",
		    "Ancestor step not found",
		    FatalException,
		    "Please contact GAMOS authors");
      }

      ancestorSteps.insert( ancestorStep );
      G4Track* trk = ancestorStep->GetTrack();
#ifndef GAMOS_NO_VERBOSE
      if( UAVerb(debugVerb) ) {
	G4cout << "GmStepMgr::GetSteps insert ancestor step track " << trk->GetTrackID() << " parent " << trk->GetParentID() << G4endl;
      }
#endif  
      if( trk->GetParentID() == 0 ) break;
    }
  
    // accept if current step is in the list of ancestors
    std::set<G4Step*>::const_iterator ites;
    for( ites = ancestorSteps.begin(); ites != ancestorSteps.end(); ites++ ){
      if( filter->AcceptPastStep( *ites ) ) acceptedSteps.push_back( *ites );
    } 
    
  } 
#ifndef GAMOS_NO_VERBOSE
  if( UAVerb(debugVerb) ) {
    G4cout << "GmStepMgr::GetSteps " << acceptedSteps.size() << G4endl;
  }
#endif  
  
  return acceptedSteps;

}

//-----------------------------------------------------------------------
void GmStepMgr::ClearObjects()
{
#ifndef GAMOS_NO_VERBOSE
  if( UAVerb(debugVerb) ) {
    G4cout << "GmStepMgr::ClearObjects " << G4endl;
  }
#endif

  // --- Delete all new objects   
  mmis::const_iterator ites;
  for( ites = theNewSteps.begin(); ites != theNewSteps.end(); ites++ ){
    G4cout << " DELETE STEP  " << (*ites).second << G4endl;
    delete (*ites).second;
  }
  theNewSteps.clear();

  std::map<G4int,G4Track*>::const_iterator itet;
  for( itet = theNewTracks.begin(); itet != theNewTracks.end(); itet++ ){
    delete (*itet).second;
  }
  theNewTracks.clear();

  /* Deleted by G4Step::~G4Step()
  std::vector<G4StepPoint*>::const_iterator itesp;
  for( itesp = theNewStepPoints.begin(); itesp != theNewStepPoints.end(); itesp++ ){
    G4cout << " DELETE Step point " << *itesp << G4endl;
    delete (*itesp);
  }
  theNewStepPoints.clear();
  */

  std::vector<G4TouchableHistory*>::const_iterator iteto;
  for( iteto = theNewTouchables.begin(); iteto != theNewTouchables.end(); iteto++ ){
    //    delete (*iteto);
  }
  theNewTouchables.clear();

  theCreatorStep.clear();
}

//-----------------------------------------------------------------------
G4Step* GmStepMgr::GetAncestorStep( const G4Step* currentStep ) 
{
  // Now look for parent steps
  G4Step* ancestorStep = const_cast<G4Step*>(currentStep);
  for(;;) {
    std::map<G4int,G4Step*>::const_iterator ite = theCreatorStep.find(ancestorStep->GetTrack()->GetTrackID());
    if( ite == theCreatorStep.end() ){
      for( ite = theCreatorStep.begin(); ite != theCreatorStep.end(); ite++ ) {
	G4cout << currentStep->GetTrack()->GetTrackID() << " STEP " << (*ite).first << " " << (*ite).second->GetPostStepPoint()->GetPosition().z() << G4endl;
      }
      G4Exception("GmStepMgr::GetAncestorStep",
		  "Ancestor step not found",
		  FatalException,
		  "Please contact GAMOS authors");
    }
    ancestorStep = (*ite).second;
    
    /*    for( ite = theCreatorStep.begin(); ite != theCreatorStep.end(); ite++ ) {
      G4cout << ancestorStep->GetTrack()->GetTrackID() << " STEP " << (*ite).first << G4endl; //DEBUG
      } */

    G4Track* trk = ancestorStep->GetTrack();
#ifndef GAMOS_NO_VERBOSE
    if( UAVerb(debugVerb) ) {
      G4cout << "GmStepMgr::GetAncestorSteps insert ancestor step track " << trk->GetTrackID() << " parent " << trk->GetParentID() << G4endl;
    }
#endif  
    if( trk->GetParentID() == 0 ) break;
  }
  
  return ancestorStep;

}
