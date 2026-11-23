#ifndef GmPrintEMParamsUA_h
#define GmPrintEMParamsUA_h 1

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include <map>
typedef std::map<G4String, std::map<G4String,double>* > msmsd;
typedef std::map<G4String, std::map<G4String,G4String>* > msmss;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GmPrintEMParamsUA : public GmUserRunAction
{
public:
    GmPrintEMParamsUA();
   ~GmPrintEMParamsUA();

  public:
  virtual void BeginOfRunAction(const G4Run*);

private:
  msmsd GetIoniParams();
  msmss GetBremsParams();
  msmsd GetMscParams();

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

