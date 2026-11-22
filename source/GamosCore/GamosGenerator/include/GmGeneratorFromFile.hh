#ifndef GmGeneratorFromFile_HH
#define GmGeneratorFromFile_HH

#include "globals.hh"
#include <map>
class G4Event;

#include "G4VUserPrimaryGeneratorAction.hh"

class GmGeneratorFromFile:public G4VUserPrimaryGeneratorAction
{
public:
  GmGeneratorFromFile();
  ~GmGeneratorFromFile(){};
  void GeneratePrimaries(G4Event* evt) = 0;

protected:
  void ReadIsotopeAtomicNumbers();

  std::map<G4String,G4int> theIsotNameZ; // map of isotope name - atomic number

  G4bool bInitialised;

  G4int theNEventsToSkip;
};

#endif
