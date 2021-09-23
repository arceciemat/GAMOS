#ifndef GmGenerator_HH
#define GmGenerator_HH

class G4Event;
class GmParticleSource;
class GmIsotopeMgr;

#include "globals.hh"
#include <vector>
#include <iostream>
#include "G4VUserPrimaryGeneratorAction.hh"

class GmGenerator:public G4VUserPrimaryGeneratorAction
{
public:
  GmGenerator();
  ~GmGenerator();
  void GeneratePrimaries(G4Event* evt);

  void AddIsotopeSource( const G4String& newValues );
  void AddSingleParticleSource( const G4String& newValues );
  void AddDoubleBackToBackParticleSource( const G4String& newValues );
  void AddRTPlanSource( const G4String& newValues );
  void AddRTIonPlanScanSpotSource( const G4String& newValues );
  void AddFromTextFileSource( const G4String& newValues );

  GmParticleSource* FindSource( const G4String& name, G4bool mustExist = TRUE );
  void SetDistribution( const G4String& distType, std::vector<G4String>& wl );

  void ListSources( std::ostream& out = G4cout );

  void DeleteSources();

  void ReInitialiseDistributions();

  G4double GetLastEventTime() const {
   return theLastEventTime; 
  }

  void AddBiasDistribution( const G4String& sourceName, const G4String& varName, const G4String& biasDistName );
  void BiasTime(GmParticleSource* source, G4double& time);

  std::vector<GmParticleSource*> GetSources() const {
    return theSources; }

  void CheckSourceIsNotRepeated( std::string sName );
  
protected:
  std::vector<GmParticleSource*> theSources;
  GmIsotopeMgr* theIsotopeMgr;
  G4double theLastEventTime;

  G4bool bBiasDistributions;
};

#endif
