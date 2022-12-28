#ifndef GmDataFinalPhantomStruct_hh
#define GmDataFinalPhantomStruct_hh

#include "GamosCore/GamosData/Management/include/GmVDataString.hh"

class GmGeometryUtils;
class GmReadPhantomStMgr;
class G4VTouchable;

class GmDataFinalPhantomStruct : public GmVDataString
{
public:
  GmDataFinalPhantomStruct();
  ~GmDataFinalPhantomStruct();

  virtual G4String GetStringValueFromStep( const G4Step* aStep );
  virtual G4String GetStringValueFromTrack( const G4Track* aTrack );

private:
  G4String GetStructNames(const G4VTouchable* touch);
  GmGeometryUtils* theGeomUtils;
  GmReadPhantomStMgr* theStructMgr;

};
#endif
