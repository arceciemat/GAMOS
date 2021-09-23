#ifndef GmGenerDistPositionDiscFromFile_h
#define GmGenerDistPositionDiscFromFile_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

class G4ParticleGun;
class G4Event;
class GmAnalysisMgr;
#include <map>

#include "GmVGenerDistPosition2D.hh"
class GmParticleSource;

class GmGenerDistPositionDiscFromFile : public GmVGenerDistPosition2D
{
public:
  GmGenerDistPositionDiscFromFile();
  virtual ~GmGenerDistPositionDiscFromFile(){};

  virtual G4ThreeVector GeneratePosition( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

private:
  G4String theFileName;
  
  void ReadPositionDist();

  G4double thePositionUnit;

  std::multimap<G4double,G4double> theProbaccumRadius; // multimap <added up probability, radius>
  std::map<G4double,G4double> theRadiusProb; // map <energy, probability>

  G4double theHBin;

  EFFCalcType theCalculationType;

  G4double theUnit;

};

#endif
