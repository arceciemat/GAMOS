#ifndef GmTimeStudyUA_hh
#define GmTimeStudyUA_hh 1

#include <map>
#include "G4SliceTimer.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"

class GmTimeStudyUA : public GmUserSteppingAction, 
		      public GmUserRunAction
{
  typedef  std::map<unsigned int, G4SliceTimer*> mut;

public:   

  GmTimeStudyUA();
  virtual ~GmTimeStudyUA();
  
  void StartTimer(const G4Step*);
  void StopTimer(const G4Step*);

  void Report();

  virtual void BeginOfRunAction( const G4Run* aRun );
  virtual void UserSteppingAction( const G4Step* aStep );
  virtual void EndOfRunAction( const G4Run* aRun );

protected:
  mut fTimers;
private:
  unsigned int theCurrentTimerID;

};


#endif
