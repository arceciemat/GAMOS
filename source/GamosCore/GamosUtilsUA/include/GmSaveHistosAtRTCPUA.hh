#ifndef __GmSaveHistosAtRTCPUA__
#define __GmSaveHistosAtRTCPUA__

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventAction.hh"

class G4Run;
class G4Event;
class G4Track;
class RTPlanMgr;
class GmParameterMgr;

class GmSaveHistosAtRTCPUA : public GmUserRunAction, 
			public GmUserEventAction
{
public:
  GmSaveHistosAtRTCPUA();
  ~GmSaveHistosAtRTCPUA(){};
  virtual void BeginOfRunAction(const G4Run* );
  virtual void EndOfEventAction(const G4Event* );

private:
  G4int theEachNEvent;
  G4int theFirstEvent;

  G4bool bChangeAtBeam;
  G4bool bChangeAtControlPoint;
  G4bool bChangeAtBeamState;
  G4bool bSaveEvent;
  G4bool bDeleteHistos;
  RTPlanMgr* theRTMgr;
  G4int theBeam;
  G4int theControlPoint;
  G4int theBeamState;
  G4String theOriginalSuffix;
  GmParameterMgr* theParamMgr;
};

#endif
