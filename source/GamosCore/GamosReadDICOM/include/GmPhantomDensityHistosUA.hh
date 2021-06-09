#ifndef GmPhantomDensityHistosUA__HH
#define GmPhantomDensityHistosUA__HH

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include <vector>
#include <fstream>
#include "G4PhantomParameterisation.hh"
#include "G4ThreeVector.hh"
class G4VPhysicalVolume;
class G4PhantomParameterisation;
class GmAnalysisMgr;

class GmPhantomDensityHistosUA : public GmUserRunAction
{
public:
  GmPhantomDensityHistosUA();
  ~GmPhantomDensityHistosUA(){};
  virtual void BeginOfRunAction( const G4Run* );

private:
  void BookHistos();  
  void FillHistos();

private:
  GmAnalysisMgr* theAnaMgr;

  G4VPhysicalVolume* thePhantomStructure;

  G4PhantomParameterisation* theRegularParam;

  G4ThreeVector thePhantomMinusCorner;
};

#endif
