#ifndef GmRegularParamUtils__HH
#define GmRegularParamUtils__HH

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4PhantomParameterisation;
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include <vector>
class GmGeometryUtils;

class GmRegularParamUtils
{
public:
  static GmRegularParamUtils* GetInstance();

  ~GmRegularParamUtils(){};

  G4PhantomParameterisation* GetPhantomParam( G4VPhysicalVolume* pv, G4bool bMustExist = true );
  G4LogicalVolume* GetPhantomMotherVolume( G4bool bMustExist = true );

  G4ThreeVector GetPhantomMotherTranslation( G4bool bMustExist = true );
  G4RotationMatrix GetPhantomMotherRotation( G4bool bMustExist = true );


private:
  GmRegularParamUtils(){};

  static GmRegularParamUtils* theInstance;
  GmGeometryUtils* theGeomUtils;
};

#endif
