///////////////////////////////////////////////////////////////////////////////
// File : GmScoringUA.h
// Author: P.Arce 
// Description:
// Modifications:
//  
///////////////////////////////////////////////////////////////////////////////

#ifndef GmScoringUA_hh
#define GmScoringUA_hh 1

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"

#include "globals.hh"

class GmScoringUA : public GmUserRunAction,  public GmUserEventAction
{

 public:
  GmScoringUA();
  ~GmScoringUA();

 public:
  virtual G4Run* GenerateRun();
  virtual void EndOfRunAction(const G4Run* );
  virtual void BeginOfEventAction( const G4Event* );
  virtual void EndOfEventAction( const G4Event* );

};

#endif
