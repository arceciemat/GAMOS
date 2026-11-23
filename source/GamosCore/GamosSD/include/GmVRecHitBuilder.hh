#ifndef GmVRecHitBuilderizer_HH
#define GmVRecHitBuilderizer_HH

#include <vector>
#include "globals.hh"

class GmRecHit;
class GmVNumericDistribution;
class GmHitsEventMgr;

enum MEBehaviour{MEAcceptAll, MEDeleteSmall, MEAcceptIf1Big, MEDeleteIf1Small};

class GmVRecHitBuilder
{

public:
  
  GmVRecHitBuilder();
  virtual ~GmVRecHitBuilder(){};

  void SetSDType(const G4String& sdtype);
  void SetName( const G4String& name ){
    theName = name; }
  void SetParams();

  virtual void CheckRecHitsMinEnergy(std::vector<GmRecHit*>& recHits);
  virtual void CheckEnergyEfficiency(std::vector<GmRecHit*>& recHits);

  virtual void SmearRecHitsEnergy(std::vector<GmRecHit*>& recHits);
  virtual void SmearRecHitsTime(std::vector<GmRecHit*>& recHits);

protected:

  G4String theName;
  G4String theSDType;

  MEBehaviour theMEBehaviour;
  G4double theMinHitEnergy;
  G4double theMinRecHitEnergy;
  GmHitsEventMgr* theHitsEventMgr;
  GmVNumericDistribution* theEfficiencyDistribution;

  G4double theEnergyResol;
  G4double theEnergyResolFluct;
  G4double theEnergyResolInstr;
  G4double theTimeResol;

};

#endif
