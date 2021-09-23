#ifndef RTGeneratorPhaseSpaceRTPlan_HH
#define RTGeneratorPhaseSpaceRTPlan_HH

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
class RTExtraInfoMgr;
class G4EventManager;
class RTPlanSource;

class RTGeneratorPhaseSpaceRTPlan: public GmGeneratorFromFile
{
protected:
  enum MWR {NOMir, MirX, MirY, MirXY};
public:
  RTGeneratorPhaseSpaceRTPlan();
  RTGeneratorPhaseSpaceRTPlan(G4String);
  ~RTGeneratorPhaseSpaceRTPlan();
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
  MWR MirrorWhenReuse;

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

  G4ThreeVector theOriginalBeamDisplacement;
  G4double theOriginalBeamRotAngleX;
  G4double theOriginalBeamRotAngleY;
  G4double theOriginalBeamRotAngleZ;
  G4bool bOriginalBeamInvert;
  
  RTPlanSource* theRTPlanSource;
  G4bool bEventSkipInitialised;
};

#endif
