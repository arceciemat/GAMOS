#ifndef RTGeneratorPhaseSpace_HH
#define RTGeneratorPhaseSpace_HH

#include "globals.hh"
#include <iostream>
#include <fstream>
#include "GamosCore/GamosGenerator/include/GmGeneratorFromFile.hh"
#include "iaea_header.hh"

class G4Event;
class GmParticleSource;
class GmIsotopeMgr;
class iaea_record_type;
class G4ParticleDefinition;
class RTPhaseSpaceHistos;
class RTExtraInfoMgr;

class RTGeneratorPhaseSpace: public GmGeneratorFromFile
{
protected:
  enum MWR {NOMir, MirX, MirY, MirXY};

public:
  RTGeneratorPhaseSpace();
  RTGeneratorPhaseSpace(G4String);
  ~RTGeneratorPhaseSpace();
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

  RTExtraInfoMgr* theExtraInfoMgr;

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

  G4double theMinPosX, theMaxPosX;
  iaea_header_type *p_iaea_header;
};

#endif
