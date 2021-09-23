#ifndef GmPSEnergyDeposit_PrimaryGamma1stInterAttenuation_h
#define GmPSEnergyDeposit_PrimaryGamma1stInterAttenuation_h 1

#include "GmPSEnergyDeposit_VPrimaryGamma1stInter.hh"

class GmPSEnergyDeposit_PrimaryGamma1stInterAttenuation : public GmPSEnergyDeposit_VPrimaryGamma1stInter
{
 
public: // with description
  GmPSEnergyDeposit_PrimaryGamma1stInterAttenuation(G4String name);
  virtual ~GmPSEnergyDeposit_PrimaryGamma1stInterAttenuation();
  
  //  virtual void UserSteppingAction( const G4Step* aStep );

  //  virtual void SetParameters( const std::vector<G4String>& params );  
  
protected: // with description
  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
 
private:
  std::set<G4VPhysicalVolume*> theChamberPVs;
  std::set<G4VPhysicalVolume*> theCavityPVs;
  
};
#endif
