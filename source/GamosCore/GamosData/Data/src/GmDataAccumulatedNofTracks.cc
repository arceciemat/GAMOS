#include "GmDataAccumulatedNofTracks.hh"
#include "GamosCore/GamosData/Management/include/GmDataVerbosity.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataAccumulatedNofTracks::GmDataAccumulatedNofTracks()
{
  bInitial = false;
  theHMax = 1.;

  theExcludedTypes.clear(); // delete DTRun
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTStep);
  theExcludedTypes.insert(DTRun);

}

//----------------------------------------------------------------
GmDataAccumulatedNofTracks::~GmDataAccumulatedNofTracks()
{
}

//----------------------------------------------------------------
void GmDataAccumulatedNofTracks::Accumulate( const G4Step* aStep, G4int index )
{
  theTracks[index].insert(aStep->GetTrack()->GetTrackID());
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << theName << "  GmVDataAccumulated::Accumulate " << index << " : " << theTracks[index].size() << G4endl;
#endif 
#endif

}

//----------------------------------------------------------------
G4double GmDataAccumulatedNofTracks::GetValueFromTrack( const G4Track*, G4int index )
{
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << theName << "  GmVDataAccumulated::GetValueFromTrack " << index << " = 1 " << G4endl;
#endif 
#endif
  return 1;
}

//----------------------------------------------------------------
G4double GmDataAccumulatedNofTracks::GetValueFromEvent( const G4Event*, G4int index )
{
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << theName << "  GmVDataAccumulated::GetValueFromEvent " << index << " = " << theTracks[index].size() << G4endl;
#endif 
#endif
  return theTracks[index].size();
}

//----------------------------------------------------------------
void GmDataAccumulatedNofTracks::Initialise()
{
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
    if( DataVerb(debugVerb) ) G4cout << " GmVDataAccumulated::Initialise data " << theName << G4endl; 
#endif
#endif
	theTracks.clear();
}

