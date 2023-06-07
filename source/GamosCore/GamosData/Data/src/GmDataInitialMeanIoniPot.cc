#include "GmDataInitialMeanIoniPot.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"
#include "G4PVParameterised.hh"

//----------------------------------------------------------------
GmDataInitialMeanIoniPot::GmDataInitialMeanIoniPot()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 10.;
}

//----------------------------------------------------------------
GmDataInitialMeanIoniPot::~GmDataInitialMeanIoniPot()
{
}


//----------------------------------------------------------------
G4double GmDataInitialMeanIoniPot::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPreStepPoint()->GetMaterial()->GetIonisation()->GetMeanExcitationEnergy();
}

//----------------------------------------------------------------
G4double GmDataInitialMeanIoniPot::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  G4Material* mate = GetMateFromPV( GetPVFromPos(aTrack->GetVertexPosition()) );
  return mate->GetIonisation()->GetMeanExcitationEnergy();
}

//----------------------------------------------------------------
G4double GmDataInitialMeanIoniPot::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* , G4int )
{
  G4Material* mate = GetMateFromPV( GetPVFromPos(aTrack1->GetPosition()) );
  return mate->GetIonisation()->GetMeanExcitationEnergy();
}

//----------------------------------------------------------------
G4double GmDataInitialMeanIoniPot::GetValueFromEvent( const G4Event* anEvent, G4int )
{
  G4Material* mate = GetMateFromPV( GetPVFromPos(anEvent->GetPrimaryVertex(0)->GetPosition()) );
  return mate->GetIonisation()->GetMeanExcitationEnergy();

}


 //----------------------------------------------------------------
G4double GmDataInitialMeanIoniPot::GetValueFromStackedTrack( const G4Track* aTrack, G4int )
{
  G4Material* mate = GetMateFromPV( GetPVFromPos(aTrack->GetPosition()) );
  return mate->GetIonisation()->GetMeanExcitationEnergy();
}

