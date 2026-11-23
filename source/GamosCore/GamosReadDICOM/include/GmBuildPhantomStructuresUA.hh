#ifndef GmBuildPhantomStructuresUA__HH
#define GmBuildPhantomStructuresUA__HH

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include <vector>
#include <fstream>
#include "G4PhantomParameterisation.hh"
#include "G4ThreeVector.hh"
class G4VPhysicalVolume;
class G4PhantomParameterisation;
class GmAnalysisMgr;

struct PStPlaneData {
  std::vector< std::vector<G4double>* > thePointV; // 3 sets of points (x,y,z)
};
struct PStructureData {
  G4String theName;
  G4int theNumber;
  std::vector<PStPlaneData*>* thePlanes;
};

class GmBuildPhantomStructuresUA : public GmUserRunAction
{
public:
  GmBuildPhantomStructuresUA();
  ~GmBuildPhantomStructuresUA(){};
  virtual void BeginOfRunAction( const G4Run* );

private:
  void InitialiseDoseCounters();
  void Write3ddose();
  void FillDoseHistos();

private:
  GmAnalysisMgr* theAnaMgr;

  G4VPhysicalVolume* thePhantomStructure;

  std::ofstream* theDoseFile;

  G4PhantomParameterisation* theRegularParam;

  G4ThreeVector thePhantomMinusCorner;
};

#endif
