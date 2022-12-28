#ifndef GmDataInitialPhantomStruct_hh
#define GmDataInitialPhantomStruct_hh

#include "GamosCore/GamosData/Management/include/GmVDataString.hh"

class GmGeometryUtils;
class GmReadPhantomStMgr;
class G4VTouchable;

class GmDataInitialPhantomStruct : public GmVDataString
{
public:
  GmDataInitialPhantomStruct();
  ~GmDataInitialPhantomStruct();

  virtual G4String GetStringValueFromStep( const G4Step* aStep );
  virtual G4String GetStringValueFromTrack( const G4Track* aTrack );
  virtual G4String GetStringValueFromSecoTrack( const G4Track* , const G4Track* );
  virtual G4String GetStringValueFromEvent( const G4Event* aEvent );
  virtual G4String GetStringValueFromStackedTrack( const G4Track* aTrack );

private:
  G4String GetStructNames(const G4VTouchable* touch);
  GmGeometryUtils* theGeomUtils;
  GmReadPhantomStMgr* theStructMgr;

};
#endif
