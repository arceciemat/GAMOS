#include "GmDataInitialMaterial.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"
#include "G4PVParameterised.hh"

//----------------------------------------------------------------
GmDataInitialMaterial::GmDataInitialMaterial()
{
  bInitial = true;
  theNChar = 25;
  theNBytes = theNChar;

  //  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataInitialMaterial::~GmDataInitialMaterial()
{
}

//----------------------------------------------------------------
G4String GmDataInitialMaterial::GetStringValueFromStep( const G4Step* aStep )
{ 
  return aStep->GetPreStepPoint()->GetMaterial()->GetName();
}

//----------------------------------------------------------------
G4String GmDataInitialMaterial::GetStringValueFromTrack( const G4Track* aTrack )
{
  G4Material* mate = GetMateFromPV( GetPVFromPos(aTrack->GetVertexPosition()) );
  return mate->GetName();
}

//----------------------------------------------------------------
G4String GmDataInitialMaterial::GetStringValueFromSecoTrack( const G4Track* aTrack1, const G4Track* )
{
  G4Material* mate = GetMateFromPV( GetPVFromPos(aTrack1->GetPosition()) );
  return mate->GetName();
}

//----------------------------------------------------------------
G4String GmDataInitialMaterial::GetStringValueFromEvent( const G4Event* anEvent )
{
  G4Material* mate = GetMateFromPV( GetPVFromPos(anEvent->GetPrimaryVertex(0)->GetPosition()) );
  return mate->GetName();

}

//----------------------------------------------------------------
G4String GmDataInitialMaterial::GetStringValueFromStackedTrack( const G4Track* aTrack )
{
  G4Material* mate = GetMateFromPV( GetPVFromPos(aTrack->GetPosition()) );
  return mate->GetName();
}



