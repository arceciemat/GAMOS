#ifndef RTGeneratorPhaseSpace_EGS_HH
#define RTGeneratorPhaseSpace_EGS_HH

#include "RTGeneratorPhaseSpace.hh"
class EGSPhspParticle;

class RTGeneratorPhaseSpace_EGS: public RTGeneratorPhaseSpace
{
public:
  RTGeneratorPhaseSpace_EGS();
  ~RTGeneratorPhaseSpace_EGS();
  void GeneratePrimaries(G4Event* evt);

  virtual void Init();

private:
  FILE* thePhspfile;
  EGSPhspParticle* theEGSParticle;
  G4double theFilePosZ;

};

#endif
