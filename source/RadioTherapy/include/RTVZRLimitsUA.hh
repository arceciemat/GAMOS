#ifndef __RTVZRLimitsUA__
#define __RTVZRLimitsUA__

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTrackingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"

#include <map>
class G4Trajectory;
typedef std::map<G4int,G4Trajectory*> maptraj;
class G4LogicalVolume;

class RTVZRLimitsUA : public GmUserRunAction, public GmUserEventAction, public GmUserTrackingAction, public GmUserSteppingAction
{
public:
  struct XYdata{
    G4double xmin;
    G4double xmax;
    G4double ymin;
    G4double ymax;
  };
  
public:
  RTVZRLimitsUA();
  ~RTVZRLimitsUA(){}; 
  virtual void BeginOfRunAction( const G4Run* aRun );
  virtual void UserSteppingAction(const G4Step* aStep);
  virtual void EndOfRunAction( const G4Run* aRun );
 
protected:
  void GetWorld();
  virtual void BuildXYLimits() = 0;

protected:
  G4double theWorldX, theWorldY, theWorldZ;
  G4LogicalVolume* theWorldLogical;
  std::map<G4double,G4String> theXYvolName;
  std::map<G4double,XYdata> theXYlimits;
  std::map<G4double,G4int> theXYcounts;

  G4bool bRussRoul;
  G4double theRussRoulValue;

};

std::ostream& operator<<(std::ostream&, const RTVZRLimitsUA::XYdata&);

#endif
