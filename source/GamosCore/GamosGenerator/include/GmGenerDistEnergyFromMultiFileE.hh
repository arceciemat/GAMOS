#ifndef GmGenerDistEnergyFromMultiFileE_h
#define GmGenerDistEnergyFromMultiFileE_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

class G4Event;
class GmAnalysisMgr;
#include <map>

#include "GamosCore/GamosGenerator/include/GmVGenerDistEnergy.hh"
class GmParticleSource;
class GmGenerDistEnergyFromFile;

class GmGenerDistEnergyFromMultiFileE : public GmVGenerDistEnergy
{
public:
  GmGenerDistEnergyFromMultiFileE();
  virtual ~GmGenerDistEnergyFromMultiFileE(){};

  virtual G4double GenerateEnergy( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

  void BuildEnergyDists();

private:
  std::map<G4double,GmGenerDistEnergyFromFile*> theEnergyDists;
  std::map<G4double,G4String> theEnergyFileNames;
  GmGenerDistEnergyFromFile* theCurrentDist;
  G4String theInterpolationType;
};

#endif
