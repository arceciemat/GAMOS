#include "GmDataFinalCSDARange.hh"

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
GmDataFinalCSDARange::GmDataFinalCSDARange()
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
GmDataFinalCSDARange::~GmDataFinalCSDARange()
{
}


//----------------------------------------------------------------
G4double GmDataFinalCSDARange::GetValueFromStep( const G4Step* aStep, G4int )
{
  //  G4cout << " GmDataFinalCSDARange::GetValueFromStep " << aStep->GetPreStepPoint()->GetKineticEnergy() << G4endl;
  G4double kinE = aStep->GetPostStepPoint()->GetKineticEnergy();
  G4ParticleDefinition* part = aStep->GetTrack()->GetDefinition();
  G4Material* mate = 0;
  G4Region* region = 0;
  G4VPhysicalVolume* PV = aStep->GetPostStepPoint()->GetPhysicalVolume();
  if( PV ) { 
    mate = aStep->GetPostStepPoint()->GetMaterial();
    region = PV->GetLogicalVolume()->GetRegion();
  } else {
    return 0.;
  }

  return theG4EmCalculator->GetCSDARange( kinE, part, mate, region );
}

//----------------------------------------------------------------
G4double GmDataFinalCSDARange::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  //  G4cout << " GmDataFinalCSDARange::GetValueFromTrack " << aTrack->GetVertexKineticEnergy() << G4endl;
  G4double kinE = aTrack->GetVertexKineticEnergy();
  G4ParticleDefinition* part = aTrack->GetDefinition();
  G4Material* mate = 0;
  G4Region* region = 0;
  if( aTrack->GetNextMaterial() ) {
    mate = aTrack->GetNextMaterial();
    region = aTrack->GetNextVolume()->GetLogicalVolume()->GetRegion();
  } else {
    return 0.;
  }
   
  return theG4EmCalculator->GetCSDARange( kinE, part, mate, region );
  
}


