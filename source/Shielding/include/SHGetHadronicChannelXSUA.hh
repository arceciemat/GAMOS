#ifndef SHGetHadronicChannelXSUA_h
#define SHGetHadronicChannelXSUA_h

class GmAnalysisMgr;
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GamosCore/GamosAnalysis/include/GmVHistoBuilder.hh"

#include <map>
#include <vector>

class G4PVPlacement;
class G4HadronicProcess;
class G4Material;
#include "G4ElementVector.hh"

typedef  std::map< const G4HadronicProcess*, std::map< std::map<G4String,G4int>, G4int > > mhmmsi;
typedef  std::map< std::map<G4String,G4int>, G4int > mmsi;

class SHGetHadronicChannelXSUA : public GmUserRunAction,
				 public GmUserEventAction,
				 public GmUserSteppingAction, 
				 public GmVHistoBuilder
{
  public:
  SHGetHadronicChannelXSUA();
  ~SHGetHadronicChannelXSUA(){};
  virtual void BeginOfRunAction(const G4Run*);
  virtual void BeginOfEventAction(const G4Event*);
  virtual void UserSteppingAction(const G4Step* aStep);
  virtual void EndOfRunAction(const G4Run*);

private:
  //  void BookHistos();
  void InitialiseProcessSecoParticles();
  //  G4String GetShortName( G4String name );

private:
  mhmmsi theProcessSecoParticles;

  std::map<const G4HadronicProcess*,G4double> theCrossSections;
  G4bool bFirstEventInRun;
  G4bool bInitialised;
  G4double theNEventsInRun;
  G4double thePrimaryKineticEnergy;
  G4Material* theCurrentMaterial;
  G4ElementVector* theCurrentElementVector;
  G4double* theCurrentAtomNumDensityVector;
  const G4ParticleDefinition* thePrimaryParticle;
  G4String thePrimaryParticleName;
  std::map<G4String,G4int> theNEvents; // n events per target nucleus
  G4bool bOnlyIons;
  G4bool bNotGammas;
  G4bool bParticleYields;
  G4bool bHistos;
  G4bool bLogE;
  G4double theMinE;
  G4double theMaxE;
  G4int theNStepsE;
  G4int theMateIdxMax;
  G4int theChanIdxMax;
  G4int theTargIdxMax;
  G4int theProdIdxMax;
  std::map<G4Material*,G4int> theMaterials;
  std::map<G4String,G4int> theChannels;
  std::map<G4String,G4int> theTargetNuclei;
  std::map<G4String,G4int> theProducts;

};
#endif
