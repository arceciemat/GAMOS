#include "GmUniformElectricField.hh"

#include "G4UniformElectricField.hh"
#include "G4EqMagElectricField.hh"
#include "G4ClassicalRK4.hh"
#include "G4FieldManager.hh"
#include "G4MagIntegratorDriver.hh"
#include "G4ChordFinder.hh"
#include "G4TransportationManager.hh"

GmUniformElectricField::GmUniformElectricField(const G4ThreeVector FieldVector )
{
  G4UniformElectricField* fEMfield = new G4UniformElectricField( FieldVector );
  // Create an equation of motion for this field
  G4EqMagElectricField* fEquation = new G4EqMagElectricField(fEMfield); 
  G4int nvar = 8;
  G4ClassicalRK4* fStepper = new G4ClassicalRK4( fEquation, nvar ); 
  // Get the global field manager 
  G4FieldManager* fFieldManager= G4TransportationManager::GetTransportationManager()->GetFieldManager();
  // Set this field to the global field manager 
  fFieldManager->SetDetectorField(fEMfield);
  G4double fMinStep = 0.010*CLHEP::mm ; // minimal step of 10 microns
  G4MagInt_Driver* fIntgrDriver = new G4MagInt_Driver(fMinStep, fStepper, fStepper->GetNumberOfVariables() );
  G4ChordFinder* fChordFinder = new G4ChordFinder(fIntgrDriver);
  fFieldManager->SetChordFinder( fChordFinder );

}

