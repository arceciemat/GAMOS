
#ifndef Exer6SD_h
#define Exer6SD_h 1

#include "G4VSensitiveDetector.hh"
#include "Exer6Hit.hh"

class G4Step;
class G4HCofThisEvent;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class Exer6SD : public G4VSensitiveDetector
{
  public:
      Exer6SD(G4String);
     ~Exer6SD();

      void Initialize(G4HCofThisEvent*);
      G4bool ProcessHits(G4Step*, G4TouchableHistory*);
      void EndOfEvent(G4HCofThisEvent*);

  private:
      Exer6HitsCollection* calorCollection;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

