#ifndef GmPDSInvertGeantinoStackUA_hh
#define GmPDSInvertGeantinoStackUA_hh

#include "GamosCore/GamosUserActionMgr/include/GmUserStackingAction.hh"

class GmPDSInvertGeantinoStackUA :  public GmUserStackingAction
{
private:
  GmPDSInvertGeantinoStackUA();
public:
  static GmPDSInvertGeantinoStackUA* GetInstance();
  ~GmPDSInvertGeantinoStackUA(){};
  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack );
  virtual void NewStage();

  void ChangeGeantinoWeight( G4double weinew, G4int parentID, G4int detID );

private:
  static GmPDSInvertGeantinoStackUA* theInstance;

};

#endif
