#ifndef GmVMicroyzUA_h
#define GmVMicroyzUA_h 1

#include "G4VSensitiveDetector.hh"

#include "GmMicroyzHit.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
class GmParameterMgr;
class GmMicroyzHit;

class GmVMicroyzUA :  public GmUserRunAction,
		      public GmUserEventAction, 
		      public GmUserSteppingAction
{
public:
  GmVMicroyzUA();
  virtual ~GmVMicroyzUA();

  virtual void BeginOfRunAction(const G4Run* );
  virtual void EndOfRunAction(const G4Run* );
  virtual void UserSteppingAction(const G4Step* aStep) = 0;
  virtual void EndOfEventAction(const G4Event* );

  G4double GetSumY() const {
    return theSumY;
  }
  G4double GetSumY2() const {
    return theSumY2;
  }
  G4double GetSumY4() const {
    return theSumY4;
  }
  G4double GetSumZ() const {
    return theSumZ;
  }
  G4double GetSumZ2() const {
    return theSumZ2;
  }
  G4double GetSumZ4() const {
    return theSumZ4;
  }
  G4double GetSumWei() const {
    return theSumWei;
  }
  G4double GetSumWei2() const {
    return theSumWei2;
  }

  std::vector<GmMicroyzHit*> GetHits() const {
    return theHits;
  }
  
protected:
  virtual void BookHistos();
  // add the y, z of one event with its weight to all the sums used for the means and their errors
  void AddEntry( G4double y, G4double z, G4double wei );

protected:
  G4bool bTextFile;
  G4bool bBinFile;
  std::ofstream *theFileOut;
  G4bool bHistos;
  G4int theHistoNumber;
  GmAnalysisMgr* theAnaMgr;
  GmParameterMgr* theParamMgr;

  G4double theSumY;
  G4double theSumY2;
  G4double theSumY4;
  G4double theSumZ;
  G4double theSumZ2;
  G4double theSumZ4;
  G4double theSumWei;
  G4double theSumWei2;
  // sums for the errors (delta method for the ratios of sums yF=Sum(w*y)/Sum(w), yD=Sum(w*y^2)/Sum(w*y))
  G4double theSumW2Y, theSumW2Y2, theSumW2Y3, theSumW2Y4;
  G4double theSumW2Z, theSumW2Z2, theSumW2Z3, theSumW2Z4;
  G4double theNEntries;
  std::vector<GmMicroyzHit*> theHits;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
