#include "GmUniformEMField.hh"
#include "G4EqMagElectricField.hh"
#include "G4ClassicalRK4.hh"
#include "G4FieldManager.hh"
#include "G4MagIntegratorDriver.hh"
#include "G4ChordFinder.hh"

GmUniformEMField::GmUniformEMField( G4ThreeVector MagFieldVector,G4ThreeVector ElecFieldVector ) 
{ 
  // Create an equation of motion for this field
  G4EqMagElectricField* fEquation = new G4EqMagElectricField(this); 
  G4int nvar = 8;
  G4ClassicalRK4* fStepper = new G4ClassicalRK4( fEquation, nvar ); 
  // Get the global field manager 
  G4FieldManager* fFieldManager= G4TransportationManager::GetTransportationManager()->GetFieldManager();
  // Set this field to the global field manager 
  fFieldManager->SetDetectorField(this);
  G4double fMinStep = 0.010*CLHEP::mm ; // minimal step of 10 microns
  G4MagInt_Driver* fIntgrDriver = new G4MagInt_Driver(fMinStep, fStepper, fStepper->GetNumberOfVariables() );
  G4ChordFinder* fChordFinder = new G4ChordFinder(fIntgrDriver);
  fFieldManager->SetChordFinder( fChordFinder );

  theField = new double(6);
  theField[0] = MagFieldVector.x();
  theField[1] = MagFieldVector.y();
  theField[2] = MagFieldVector.z();
  theField[3] = ElecFieldVector.x();
  theField[4] = ElecFieldVector.y();
  theField[5] = ElecFieldVector.z();
}

void GmUniformEMField::GetFieldValue(const double*, double *Bfield ) const
{ 
  Bfield = theField;  
  G4cout << " GmUniformEMField::GetFieldValue " << Bfield[0] << " " 
	 << Bfield[1] << " " 
	 << Bfield[2] << " " 
	 << Bfield[3] << " " 
	 << Bfield[4] << " " 
	 << Bfield[5] << G4endl;
}

