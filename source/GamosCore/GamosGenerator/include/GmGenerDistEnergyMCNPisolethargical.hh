#ifndef GmGenerDistEnergyMCNPisolethargical_h
#define GmGenerDistEnergyMCNPisolethargical_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

class G4ParticleGun;
class G4Event;
class GmAnalysisMgr;
#include <map>

#include "GamosCore/GamosGenerator/include/GmVGenerDistEnergy.hh"
class GmParticleSource;

class GmGenerDistEnergyMCNPisolethargical : public GmVGenerDistEnergy
{
public:
  GmGenerDistEnergyMCNPisolethargical();
  virtual ~GmGenerDistEnergyMCNPisolethargical(){};

  virtual G4double GenerateEnergy( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

private:
  G4String theFileName;
  
  void ReadEnergyDist();

  G4double theEnergyUnit;

  std::map<G4double,G4double> theProbEner; // map <added up probability, energy>

  G4double theHBin;

  G4double theUnit;

};

#endif
