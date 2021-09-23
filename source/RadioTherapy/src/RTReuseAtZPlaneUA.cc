#include "RTReuseAtZPlaneUA.hh"
#include "RTReuseAtZPlaneMessenger.hh"
#include "RTReuseAtZPlaneTrackInfo.hh"

//---------------------------------------------------------------------
RTReuseAtZPlaneUA::RTReuseAtZPlaneUA()
{
  new RTReuseAtZPlaneMessenger();

}

//---------------------------------------------------------------------
RTReuseAtZPlaneUA::~RTReuseAtZPlaneUA()
{

}

//---------------------------------------------------------------------
void RTReuseAtZPlaneUA::UserSteppingAction(const G4Step* aStep)
{
  G4VUserTrackInformation* trkInfo = aStep->GetTrack()->GetUserInformation();
  //--- ONLY REUSE ONCE
  if( trkInfo ) {
    RTReuseAtZPlaneTrackInfo* trkInfoS = (RTReuseAtZPlaneTrackInfo*)trkInfo;
    if( trkInfoS->IsReuse() ){
      G4TrackVector secondariesTrack = GetStepSecondaries();
      G4TrackVector::const_iterator ite;
      for( ite = secondariesTrack.begin(); ite != secondariesTrack.end(); ite++ ){
	if( (*ite)->GetUserInformation() ) {
	  delete  (*ite)->GetUserInformation();
	}
	RTReuseAtZPlaneTrackInfo * ti = new RTReuseAtZPlaneTrackInfo; 
 	(*ite)->SetUserInformation( ti );
      }
    }
  }
}
