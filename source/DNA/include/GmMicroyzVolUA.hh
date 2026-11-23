#ifndef GmMicroyzVolUA_h
#define GmMicroyzVolUA_h 1

#include "G4VSensitiveDetector.hh"

#include "GmMicroyzHit.hh"
#include "GmVMicroyzUA.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/// The hits are accounted in hits in SteppingAction() method, and processed by EndOfEventAction()

class GmMicroyzVolUA : public GmVMicroyzUA
{
public:
  GmMicroyzVolUA();
  virtual ~GmMicroyzVolUA();
  
  // methods from base class
  virtual void UserSteppingAction(const G4Step* aStep);
  virtual void EndOfEventAction(const G4Event* );

private: 
  virtual void BookHistos();
  
private:
  G4String theVolumeName;
  G4double theSphereRadius;
  G4double theVolumeChord;
  G4double theVolumeMass;

  G4int theNHistosV; //N histos declared in GmVMicroyzUA
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
