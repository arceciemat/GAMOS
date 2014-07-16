#include "GmCountTracksAndStepsUA.hh"
#include "G4Track.hh"
#include "G4Run.hh"
#include "G4Step.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"

//-----------------------------------------------------------------
GmCountTracksAndStepsUA::GmCountTracksAndStepsUA()
{
}


//-----------------------------------------------------------------
GmCountTracksAndStepsUA::~GmCountTracksAndStepsUA() 
{
}

//-----------------------------------------------------------------
void GmCountTracksAndStepsUA::BeginOfRunAction(const G4Run* )
{
  theNSteps.clear();
  theNTracks.clear();
}

//-----------------------------------------------------------------
void GmCountTracksAndStepsUA::UserSteppingAction(const G4Step* aStep )
{
  G4int index;
  if (theClassifier ) {
    index = theClassifier->GetIndexFromStep(aStep);
  } else {
    index = 0;
  }
  theNSteps[index]++;  

  //  G4cout << " GmCountTracksAndStepsUA NSteps " << index << " " << theNSteps[index] << G4endl;
  if( aStep->GetTrack()->GetCurrentStepNumber() == 1) theNTracks[index]++;  

}


//-----------------------------------------------------------------
void GmCountTracksAndStepsUA::EndOfRunAction(const G4Run* )
{
  std::map<G4int,G4int>::const_iterator itei;

  G4String Name = "%%% COUNT_TRACKS_AND_STEPS: ";
  unsigned int ii;
  for( ii = 0; ii < theFilters.size(); ii++ ){
    if( ii != 0 ) Name += "_";
    Name += theFilters[ii]->GetName();
  }
  if( theClassifier ) {
    if( ii != 0 ) Name += "_";
    Name += theClassifier->GetName();
  }
  G4cout << Name << G4endl;

  G4int ntrkTOTAL = 0;
  for( itei =theNTracks.begin(); itei != theNTracks.end(); itei++ ){
    G4int index = (*itei).first;
    G4cout << "%%% COUNT_TRACKS: ";
    if( theClassifier ) {
      G4cout << theClassifier->GetIndexName(index);
    } else {
      G4cout << index;
    }
    G4cout << " = " << (*itei).second << G4endl;
    ntrkTOTAL += (*itei).second;
  }
  G4cout << "%%% COUNT_TRACKS: ALL = " << ntrkTOTAL << G4endl;
  
  G4int nstepTOTAL = 0;
  for( itei =theNSteps.begin(); itei != theNSteps.end(); itei++ ){
    G4int index = (*itei).first;
    G4cout << "%%% COUNT_STEPS: ";
    if( theClassifier ) {
      G4cout << theClassifier->GetIndexName(index);
    } else {
      G4cout << index;
    }
    G4cout << " = " << (*itei).second << G4endl;
    nstepTOTAL += (*itei).second;
  }
  G4cout << "%%% COUNT_STEPS: ALL = " << nstepTOTAL << G4endl;

}
