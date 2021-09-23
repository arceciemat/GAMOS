#ifndef GmDumpMateG4dcmUA__HH
#define GmDumpMateG4dcmUA__HH

#include "GamosCore/GamosUserActionMgr/include/GmUserRunAction.hh"
#include "GamosCore/GamosBase/Base/include/GmIOtextMgr.hh"
#include <vector>
#include <fstream>
#include "G4PhantomParameterisation.hh"
#include "G4ThreeVector.hh"
class G4VPhysicalVolume;
class G4PhantomParameterisation;
class GmAnalysisMgr;

class GmDumpMateG4dcmUA : public GmUserRunAction,   public GmIOtextMgr
{
public:
  GmDumpMateG4dcmUA();
  ~GmDumpMateG4dcmUA(){};
  virtual void BeginOfRunAction( const G4Run* );

private:
  //  void BuildMaterialList();  
  void PrintImage();

private:

  G4VPhysicalVolume* thePhantomStructure;

  G4PhantomParameterisation* theRegularParam;

  //  std::map< G4Material*, size_t > theMaterials;
};

#endif
