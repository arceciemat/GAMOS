///////////////////////////////////////////////////////////////////////////////
// File : GmCountTracksAndStepsUA.h
// Description:
// Modifications:
//  
///////////////////////////////////////////////////////////////////////////////

#ifndef GmCountTracksAndStepsUA_h
#define GmCountTracksAndStepsUA_h 1

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingAction.hh"
#include "GamosCore/GamosAnalysis/include/GmVHistoBuilder.hh"

#include "globals.hh"

#include <map>
#include <set>
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"

class GmCountTracksAndStepsUA : public GmUserRunAction, 
				public GmUserSteppingAction
{

 public:
  GmCountTracksAndStepsUA();
  ~GmCountTracksAndStepsUA();

 public:
  virtual void BeginOfRunAction(const G4Run* );
  virtual void EndOfRunAction(const G4Run* );
  virtual void UserSteppingAction(const G4Step* aStep);

 private:
  std::map< G4int,G4int> theNSteps;
  std::map< G4int,G4int> theNTracks;

};

#endif
