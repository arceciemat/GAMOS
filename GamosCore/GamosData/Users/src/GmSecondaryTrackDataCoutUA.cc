#include "GmSecondaryTrackDataCoutUA.hh"
#include "GamosCore/GamosData/Management/include/GmVData.hh"
#include "GamosCore/GamosData/Management/include/GmDataVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmSecondaryTrackDataCoutUA::GmSecondaryTrackDataCoutUA()
{
  theDataType = DTSeco;
}

//----------------------------------------------------------------
void GmSecondaryTrackDataCoutUA::BeginOfRunAction( const G4Run* )
{
  SetUserName( theName );

  theDataList.clear(); 
  theDataList.push_back("EventID");
  theDataList.push_back("TrackID");
  theDataList.push_back("InitialPrimMinusSecoKineticEnergy");
  theDataList.push_back("FinalPrimMinusSecoKineticEnergy");
  theDataList.push_back("SecoDividedInitialPrimKineticEnergy");  
  theDataList.push_back("SecoKineticEnergy");
  theDataList.push_back("FinalPrimSecoAngle");        
  theDataList.push_back("InitialPrimKineticEnergy");
  theDataList.push_back("FinalPrimKineticEnergy");

  BuildData();
 
}

//----------------------------------------------------------------
void GmSecondaryTrackDataCoutUA::UserSteppingAction(const G4Step* aStep )
{
  const G4Track* aTrack = aStep->GetTrack();
  G4TrackVector secondaries = GetStepSecondaries();
  unsigned int nSeco = secondaries.size();

  std::vector<GmVData*>::const_iterator ite;
  std::vector<GmVFilter*>::const_iterator itef;

  // pass GmOnSecondaryFilter to each secondary track
  for(unsigned int ii = 0; ii < nSeco; ii++ ){
    G4Track* secoTrack = secondaries[ii];
    G4bool bSecoAccepted = true;
    for( itef = theFilters.begin(); itef != theFilters.end(); itef++ ) {
      if( (*itef)->GetClass() == "GmOnSecondaryFilter" ) {
	//       	G4cout << " GmOnSecondaryFilter " << (*itef)->GetName() << " = " << (*itef)->AcceptTrack(secoTrack)  << G4endl;
	if( !(*itef)->AcceptTrack(secoTrack) ) {
	  bSecoAccepted = false; 
	  break;
	}
      }
    }
    if( !bSecoAccepted ) continue;
    for( ite = theData.begin(); ite != theData.end(); ite++ ) {
      (*ite)->WriteCout( aTrack, secoTrack, ite == theData.begin() );
    }
  }
  if( nSeco != 0 ) G4cout << G4endl;  

}


//----------------------------------------------------------------
void GmSecondaryTrackDataCoutUA::EndOfRunAction( const G4Run* )
{
}
