#ifndef GmPhantomDoseUA__HH
#define GmPhantomDoseUA__HH

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include <vector>
#include <fstream>
#include "G4PhantomParameterisation.hh"
#include "G4ThreeVector.hh"
class G4VPhysicalVolume;
class G4PhantomParameterisation;
class GmAnalysisMgr;

class GmPhantomDoseUA : public GmUserRunAction, public GmUserEventAction, public GmUserTrackingAction, public GmUserSteppingAction
{
public:
  GmPhantomDoseUA();
  ~GmPhantomDoseUA(){};
  virtual void BeginOfRunAction( const G4Run* );
  virtual void EndOfRunAction( const G4Run* );
  virtual void BeginOfEventAction( const G4Event* );
  virtual void EndOfEventAction( const G4Event* );
  virtual void PreUserTrackingAction(const G4Track* aTrack);
  virtual void PostUserTrackingAction(const G4Track* aTrack);
  virtual void UserSteppingAction(const G4Step* aStep);

private:
  void BookHistos();  
  G4bool IsInterestingStep(const G4Step* aStep);

  void Write3ddoseHeader();
  void InitialiseDoseCounters();
  void Write3ddose();
  void FillDoseHistos();

private:
  GmAnalysisMgr* theAnaMgr;

  G4VPhysicalVolume* thePhantomStructure;

  std::vector<float> theDose;
  std::vector<float> theDose2;
  std::vector<size_t> theDoseCounts;

  std::ofstream* theDoseFile;

  G4PhantomParameterisation* theRegularParam;

  G4ThreeVector thePhantomMinusCorner;
};

#endif
