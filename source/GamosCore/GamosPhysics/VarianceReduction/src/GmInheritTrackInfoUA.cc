#include "GmInheritTrackInfoUA.hh"
#include "GamosCore/GamosBase/Base/include/GmTrackInfo.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Track.hh"
#include "G4EventManager.hh"
#include "G4TrackingManager.hh"
#include "G4SteppingManager.hh"


//----------------------------------------------------------------
GmInheritTrackInfoUA::GmInheritTrackInfoUA()
{
  bSomethingToInherit = false;
}


//----------------------------------------------------------------
void GmInheritTrackInfoUA::BeginOfRunAction( const G4Run* )
{

  theSteppingManager = G4EventManager::GetEventManager()->GetTrackingManager()->GetSteppingManager();

}


//----------------------------------------------------------------
void GmInheritTrackInfoUA::UserSteppingAction(const G4Step* aStep )
{
  
  G4VUserTrackInformation* trkInfo = aStep->GetTrack()->GetUserInformation();
  if( !trkInfo ) return;     
  if( !bSomethingToInherit ) return;

  GmTrackInfo* gmTrkInfo = static_cast<GmTrackInfo*>(trkInfo);
  G4String trkInfoType = gmTrkInfo->GetType();

  std::vector<G4String>::const_iterator itevb, itevi, itevd, itevs, itevtv;

  G4TrackVector secoTracks = GmG4Utils::GetSecondariesOfCurrentStep(theSteppingManager);

  G4TrackVector::iterator ite;
  for( ite = secoTracks.begin(); ite != secoTracks.end(); ite++ ){
    G4VUserTrackInformation* secoTrkInfo = (*ite)->GetUserInformation();
    GmTrackInfo* gmSecoTrkInfo;
    if( secoTrkInfo ) {
      gmSecoTrkInfo = static_cast<GmTrackInfo*>(secoTrkInfo);
    } else {
      gmSecoTrkInfo = new GmTrackInfo(trkInfoType);
      (*ite)->SetUserInformation( gmSecoTrkInfo );
      //      G4cout << " GmInheritTrackInfoUA::PostUserSteppingAction adding TrackInfo to child without it" << G4endl;
    }

    for( itevb = theBoolValueNames.begin(); itevb != theBoolValueNames.end(); itevb++ ) {
      std::string vname = *itevb;
      if( gmSecoTrkInfo->BoolValueExists(vname)// value already set, do not touch it
	  || !gmTrkInfo->BoolValueExists(vname) // parent does not have value, do not set it
	  ) continue;
      gmSecoTrkInfo->SetBoolValue(vname,gmTrkInfo->GetBoolValue(vname));
    }
    
    for( itevb = theIntValueNames.begin(); itevb != theIntValueNames.end(); itevb++ ) {
      std::string vname = *itevb;
      if( gmSecoTrkInfo->IntValueExists(vname)// value already set, do not touch it
	  || !gmTrkInfo->IntValueExists(vname) // parent does not have value, do not set it
	  ) continue;
      
      gmSecoTrkInfo->SetIntValue(vname,gmTrkInfo->GetIntValue(vname));
      /*      G4cout << " GmInheritTrackInfoUA::PostUserSteppingAction adding INT TrackInfo " 
	     << vname << " : " 
	     << gmTrkInfo->GetIntValue(vname) << " = " << gmSecoTrkInfo->GetIntValue(vname) 
	     << G4endl; */

    }
    for( itevb = theDoubleValueNames.begin(); itevb != theDoubleValueNames.end(); itevb++ ) {
      std::string vname = *itevb;
      if( gmSecoTrkInfo->DoubleValueExists(vname)// value already set, do not touch it
	  || !gmTrkInfo->DoubleValueExists(vname) // parent does not have value, do not set it
	  ) continue;
      
      gmSecoTrkInfo->SetDoubleValue(vname,gmTrkInfo->GetDoubleValue(vname));
    }
    
    for( itevb = theStringValueNames.begin(); itevb != theStringValueNames.end(); itevb++ ) {
      std::string vname = *itevb;
      if( gmSecoTrkInfo->StringValueExists(vname)// value already set, do not touch it
	  || !gmTrkInfo->StringValueExists(vname) // parent does not have value, do not set it
	  ) continue;
      
      gmSecoTrkInfo->SetStringValue(vname,gmTrkInfo->GetStringValue(vname));
    }
    
    for( itevb = theThreeVectorValueNames.begin(); itevb != theThreeVectorValueNames.end(); itevb++ ) {
      std::string vname = *itevb;
      if( gmSecoTrkInfo->ThreeVectorValueExists(vname)// value already set, do not touch it
	  || !gmTrkInfo->ThreeVectorValueExists(vname) // parent does not have value, do not set it
	  ) continue;
      
      gmSecoTrkInfo->SetThreeVectorValue(vname,gmTrkInfo->GetThreeVectorValue(vname));
    }

  }

}



//  LocalWords:  GetSteppingManage
