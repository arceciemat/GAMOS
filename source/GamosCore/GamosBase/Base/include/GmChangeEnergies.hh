#ifndef GmChangeEnergies_h
#define GmChangeEnergies_h

#include <vector>
#include "globals.hh"

class GmChangeEnergies 
{

public:
  GmChangeEnergies();
  ~GmChangeEnergies(){};

  void GetEnergyParameters(G4String name);

  G4double GetMinE() const {
    return theMinE;
  }
  G4double GetMaxE() const {
    return theMaxE;
  }
  G4double GetStepE() const {
    return theStepE;
  }
  unsigned int GetNStepsE() const {
    return theNStepsE;
  }

private:
  void BuildEnergies();

protected:
  G4double GetEnergy(G4int ii);

  G4double theMinE;
  G4double theMaxE;
  unsigned int theNStepsE;
  G4double theStepE;
  G4bool bLogE;

  std::vector<G4double> theEnergies;
  G4String theEnergyFileName;
  G4bool bEnergyFromFile;

};
#endif
