#ifndef GmPDSScore_hh
#define GmPDSScore_hh

#include "GmPDSProcessHelper.hh"

#include <vector>
#include <set>
#include <map>
#include "G4ThreeVector.hh"

class GmConvergenceTester;
class GmPDSDetector;

class GmPDSScore {

public: 
  GmPDSScore( G4String name, G4int detID, std::set<G4double>& energies ); 

  ~GmPDSScore();

  void FillEnergy( G4double wei, const Flux2Dose& flux2Dose, G4int enerID );
  void UpdateFluxErrorsThisEvent();
  void PrintResults( std::ostream& out, GmPDSDetector* det );
  G4int GetID() const {
    return theID; }

  G4int GetDetID() const {
    return theDetID; }

  static void SetbHstar( G4bool val ){
    bHstar = val; 
  }
  static void SetbHp0( G4bool val ){
    bHp0 = val; 
  }
  static void SetbHp15( G4bool val ){
    bHp15 = val; 
  }
  static void SetbHp30( G4bool val ){
    bHp30 = val; 
  }
  static void SetbHp45( G4bool val ){
    bHp45 = val; 
  }
  static void SetbHp60( G4bool val ){
    bHp60 = val; 
  }
  static void SetbHp75( G4bool val ){
    bHp75 = val; 
  }

private:
  static G4bool bHstar, bHp0, bHp15, bHp30, bHp45, bHp60, bHp75;

protected:
  G4String theName; 
  G4int theID;
  G4int theDetID;
  std::set<G4double> theEnergies;
  std::vector<G4double> theFluxes;
  std::vector<G4double> theFluxesPrevEvent;
  std::vector<G4double> theFluxes2;
  std::vector<G4double> theFluxes3;
  std::vector<G4double> theFluxes4;
  std::vector<int> theFluxCounts;
  G4double theHstar;
  G4double theHp0;
  G4double theHp15;
  G4double theHp30;
  G4double theHp45;
  G4double theHp60;
  G4double theHp75;

  static G4int theNumberOfScores;

  GmConvergenceTester* theConvergenceTester;

};

#endif
