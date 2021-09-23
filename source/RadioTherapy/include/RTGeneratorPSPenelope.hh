#ifndef RTGeneratorPSPenelope_HH
#define RTGeneratorPSPenelope_HH

class G4Event;
class GmParticleSource;
class GmIsotopeMgr;
class iaea_record_type;
#include "globals.hh"
#include <iostream>
#include <fstream>
#include "GamosCore/GamosGenerator/include/GmGeneratorFromFile.hh"
class G4ParticleDefinition;
class RTPhaseSpaceHistos;

class RTGeneratorPSPenelope: public GmGeneratorFromFile
{
  enum MWR {NOMir, MirX, MirY, MirXY};
public:
  RTGeneratorPSPenelope();
  RTGeneratorPSPenelope(G4String);
  ~RTGeneratorPSPenelope();
  virtual void Init();
  void GeneratePrimaries(G4Event* evt);

protected:
  FILE* p_rfile;
  iaea_record_type *p_iaea_read;

  G4int theMaxNReuse, theNReused;
  G4int theMaxNRecycle, theNRecycled;
  G4String theFileName;
  G4double theNEventsInFile;
  G4bool bMaxNReuseAutomatic;
  G4ThreeVector theInitialDisp;
  MWR MirrorWhenReuse;

  G4double theInitialRotAngleX;
  G4double theInitialRotAngleY;
  G4double theInitialRotAngleZ;

  G4double theNEventsSkip;
  G4ParticleDefinition* theParticleDef;

  G4bool bHistos;
  RTPhaseSpaceHistos* theHistos;

  enum transType { TTDisplacement, TTRotateXYZ, TTRotateThetaPhiSelf } ;/// " D RXYZ, RTPS 
  
  struct transformation_struct{
    transType operation;
    G4double  opx;
    G4double  opy;
    G4double  opz;
  };
  
  std::vector<transformation_struct> theTransformations;

  G4double theNNewEvents;
  G4double theNParticlesRead;
  G4double theNOrigEvents;  

  std::ifstream theFileIn;
  char theLineRead[200];
  std::ifstream* theFileIn2;

};

#endif
