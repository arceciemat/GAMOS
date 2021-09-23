#ifndef GmMultiIsotopesSource_H
#define GmMultiIsotopesSource_H
//
#include "GmFromTextFileSource.hh"
#include "G4ParticleDefinition.hh"
#include "GmGenerTextFileData.hh"
#include "GmGeneratorMultiIsotopes.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

class GmMultiIsotopesSource : public GmFromTextFileSource
{
public:
  GmMultiIsotopesSource( const G4String& name );
  ~GmMultiIsotopesSource();
  virtual void Initialize();

  void SetIsotopeData(GmGenerIsotopeData isotd );
  void BuildGenerTimeConstantChangeRandom(G4double prevTime);
  void BuildGenerTimeDecayProd(G4double prevTime );
  G4bool BuildGenerTimeDecay(G4double prevProdTime, G4double prevDecayTime );
  virtual G4double GenerateTime();
  void SetFileIn( GmFileIn fin ) {
    theFileIn = fin;
  }
  
private:

  GmGenerIsotopeData theIsotopeData;
  G4double thePreviousTime;
  G4double theProductionTime;
  G4double theMinimumActivity;
};

#endif // GmMultiIsotopesSource_H
