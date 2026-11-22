#include "GmDataInitialCSDARange.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4EmCalculator.hh"
#include "G4EmParameters.hh"
#include "G4Region.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"

//----------------------------------------------------------------
GmDataInitialCSDARange::GmDataInitialCSDARange()
{
  bInitial = true;
  theHMax = 1.;
  theG4EmCalculator = new G4EmCalculator();
  G4EmParameters* emParam = G4EmParameters::Instance();
  if( !emParam->BuildCSDARange() )  {
    G4Exception("GmDataFinalCSDARange::GmDataFinalCSDARange",
		"",
		FatalException,
		"Use command  /process/eLoss/CSDARange true  in your script, before /run/initialize");
  }
}

//----------------------------------------------------------------
GmDataInitialCSDARange::~GmDataInitialCSDARange()
{
}


//----------------------------------------------------------------
G4double GmDataInitialCSDARange::GetValueFromStep( const G4Step* aStep, G4int )
{
  //  G4cout << " GmDataInitialCSDARange::GetValueFromStep " << aStep->GetPreStepPoint()->GetKineticEnergy() << G4endl;
  G4double kinE = aStep->GetPreStepPoint()->GetKineticEnergy();
  G4ParticleDefinition* part = aStep->GetTrack()->GetDefinition();
  G4Material* mate = aStep->GetPreStepPoint()->GetMaterial();
  G4Region* region = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetRegion();

  return theG4EmCalculator->GetCSDARange( kinE, part, mate, region );
}

//----------------------------------------------------------------
G4double GmDataInitialCSDARange::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  //  G4cout << " GmDataInitialCSDARange::GetValueFromTrack " << aTrack->GetVertexKineticEnergy() << G4endl;
  G4double kinE = aTrack->GetVertexKineticEnergy();
  G4ParticleDefinition* part = aTrack->GetDefinition();
  G4Material* mate = GetMateFromPV( GetPVFromPos(aTrack->GetVertexPosition()) );
  G4Region* region = aTrack->GetLogicalVolumeAtVertex()->GetRegion();
   
  return theG4EmCalculator->GetCSDARange( kinE, part, mate, region );
  
}

//----------------------------------------------------------------
G4double GmDataInitialCSDARange::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* aTrack2, G4int )
{
  //  G4cout << " GmDataInitialCSDARange::GetValueFromSecoTrack " << aTrack2->GetKineticEnergy() << G4endl;
  G4double kinE = aTrack2->GetKineticEnergy();
  G4ParticleDefinition* part = aTrack2->GetDefinition();
  G4Material* mate = GetMateFromPV( GetPVFromPos(aTrack1->GetPosition()) );
  G4Region* region = aTrack1->GetVolume()->GetLogicalVolume()->GetRegion();
   
  return theG4EmCalculator->GetCSDARange( kinE, part, mate, region );
}

 //----------------------------------------------------------------
G4double GmDataInitialCSDARange::GetValueFromStackedTrack( const G4Track* aTrack, G4int ) 
{
  G4double kinE = aTrack->GetKineticEnergy();
  G4ParticleDefinition* part = aTrack->GetDefinition();
  G4VPhysicalVolume* PV = GetPVFromPos(aTrack->GetPosition());
  G4Material* mate = GetMateFromPV( PV );
  G4Region* region = PV->GetLogicalVolume()->GetRegion();

  return theG4EmCalculator->GetCSDARange( kinE, part, mate, region );
}
