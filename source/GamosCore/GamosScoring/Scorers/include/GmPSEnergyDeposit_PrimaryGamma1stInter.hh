#ifndef GmPSEnergyDeposit_PrimaryGamma1stInter_h
#define GmPSEnergyDeposit_PrimaryGamma1stInter_h 1

#include "GmPSEnergyDeposit_VPrimaryGamma1stInter.hh"

class GmPSEnergyDeposit_PrimaryGamma1stInter : public GmPSEnergyDeposit_VPrimaryGamma1stInter
{
 
 public: // with description
      GmPSEnergyDeposit_PrimaryGamma1stInter(G4String name);
      virtual ~GmPSEnergyDeposit_PrimaryGamma1stInter();

 protected: // with description
      virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  
public: 
  
};
#endif
