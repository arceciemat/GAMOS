#ifndef GmGenerDistEnergyFromFile_h
#define GmGenerDistEnergyFromFile_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

class G4Event;
class GmAnalysisMgr;
#include <map>
typedef std::map<G4double,G4double> mdd;
typedef std::multimap<G4double,G4double> mmdd;

#include "GamosCore/GamosGenerator/include/GmVGenerDistEnergy.hh"
class GmParticleSource;

class GmGenerDistEnergyFromFile : public GmVGenerDistEnergy
{
public:
  GmGenerDistEnergyFromFile();
  virtual ~GmGenerDistEnergyFromFile(){};

  virtual G4double GenerateEnergy( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );
  
  G4String GetFileName() const {
    return theFileName;
  }
  void SetEnerProb( mdd );
  void SetCalculationType( G4String type );
  
private:
  G4String theFileName;
  
  G4double theEnergyUnit;

  std::multimap<G4double,G4double> theProbaccumEner; // multimap <added up probability, energy>
  std::map<G4double,G4double> theEnerProb; // map <energy, probability>

  G4double theHBin;

  EFFCalcType theCalculationType;
  G4String theCalculationTypeOld;

  G4double theUnit;

  void ReadEnergyDist();
  void BuildAccumProbEnergy();

};

#endif
