#ifndef GmCellyzUA_h
#define GmCellyzUA_h 1

#include "G4VSensitiveDetector.hh"

#include "GmMicroyzHit.hh"
#include "GmVMicroyzUA.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/// The hits are accounted in hits in SteppingAction() method, and processed by EndOfEventAction()

class GmCellyzUA : public GmVMicroyzUA
{
public:
  GmCellyzUA();
  virtual ~GmCellyzUA();
  
  // methods from base class
  virtual void UserSteppingAction(const G4Step* aStep);
  virtual void EndOfEventAction(const G4Event* );

private: 
  virtual void BookHistos();
  
private:
  G4double theSphereRadius;
  G4double theVolumeChord;
  G4double theVolumeMass;

  G4int theNHistosV; //N histos declared in GmVMicroyzUA
  
  G4int theNSpheresX;
  G4int theNSpheresY;
  G4int theNSpheresZ;
  G4int theNSpheresXY;
  G4double theStepX;
  G4double theStepY;
  G4double theOffsetX;
  G4double theOffsetY;
  G4double theCentreX;
  G4double theCentreY;
  G4double theCentreZ;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
