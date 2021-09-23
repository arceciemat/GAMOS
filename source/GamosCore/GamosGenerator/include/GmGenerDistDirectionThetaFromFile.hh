#ifndef GmGenerDistDirectionThetaFromFile_h
#define GmGenerDistDirectionThetaFromFile_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

class G4ParticleGun;
class G4Event;
class GmAnalysisMgr;
#include <map>

#include "GamosCore/GamosGenerator/include/GmVGenerDistDirection.hh"
class GmParticleSource;

class GmGenerDistDirectionThetaFromFile : public GmVGenerDistDirection
{
public:
  GmGenerDistDirectionThetaFromFile();
  virtual ~GmGenerDistDirectionThetaFromFile(){};

  virtual G4ThreeVector GenerateDirection( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

private:
  G4String theFileName;
  
  void ReadThetaDist();

  G4double theThetaUnit;

  std::multimap<G4double,G4double> theProbaccumTheta; // multimap <added up probability, energy>
  std::map<G4double,G4double> theThetaProb; // map <energy, probability>

  G4double theHBin;

  EFFCalcType theCalculationType;

  G4double theUnit;
  G4ThreeVector theInitialDir;
  G4ThreeVector thePerpDir;

};

#endif
