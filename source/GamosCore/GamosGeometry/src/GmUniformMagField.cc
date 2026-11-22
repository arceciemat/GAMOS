#include "GmUniformMagField.hh"

#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4ClassicalRK4.hh"
#include "G4FieldManager.hh"
#include "G4MagIntegratorDriver.hh"
#include "G4ChordFinder.hh"
#include "G4TransportationManager.hh"

//----------------------------------------------------------

GmUniformMagField::GmUniformMagField(G4ThreeVector FieldVector) 
{
  G4UniformMagField* fEMfield = new G4UniformMagField( FieldVector );
  // Create an equation of motion for this field
  G4Mag_UsualEqRhs* fEquation = new  G4Mag_UsualEqRhs(fEMfield); 
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




