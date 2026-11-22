#include "GmDataTrackPointer.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"
#include <sstream>

//----------------------------------------------------------------
GmDataTrackPointer::GmDataTrackPointer()
{
  bInitial = true;
  theNChar = 25;
  theNBytes = theNChar;

  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataTrackPointer::~GmDataTrackPointer()
{
}

//----------------------------------------------------------------
G4String GmDataTrackPointer::GetStringValueFromStep( const G4Step* aStep )
{
  std::stringstream fout;
  fout << "GmDATA G4TRACK "<< aStep->GetTrack();
  return fout.str();
}

//----------------------------------------------------------------
G4String GmDataTrackPointer::GetStringValueFromTrack( const G4Track* aTrack )
{ 
  std::stringstream fout;
  fout << "GmDATA G4TRACK "<< aTrack;
  return fout.str();
}

//----------------------------------------------------------------
G4String GmDataTrackPointer::GetStringValueFromSecoTrack( const G4Track* , const G4Track* aTrack2)
{
  std::stringstream fout;
  fout << "GmDATA G4TRACK "<< aTrack2;
  return fout.str();
}

