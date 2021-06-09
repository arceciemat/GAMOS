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

  virtual void CleanRecHits() = 0; //cannot be done here, because there is no info about GmRecHit

  virtual void DeleteHit( GmRecHit* rhit ) = 0; //cannot be done here, because there is no info about GmRecHit

  virtual void CheckRecHitsMinEnergy();
  virtual void CheckEnergyEfficiency();

  virtual void SmearRecHitsEnergy();
  virtual void SmearRecHitsTime();

protected:
  std::vector<GmRecHit*> theRecHits;

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
