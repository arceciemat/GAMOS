#ifndef GmPDSUtils_HH
#define GmPDSUtils_HH
//
#include "globals.hh"
#include "G4Track.hh"

G4double GetWeight( const G4Track* aTrack, G4bool bWeightMustExist = TRUE );
void SetWeight( G4Track* aTrack, G4double wei );

G4double GetPDSNumericParameter(const G4String& param, G4String partName, G4double defVal);
G4String GetPDSStringParameter(const G4String& param, G4String partName, G4String defVal);

#endif
