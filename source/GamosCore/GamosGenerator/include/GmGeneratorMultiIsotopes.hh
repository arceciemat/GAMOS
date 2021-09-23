#ifndef GmGeneratorMultiIsotopes_HH
#define GmGeneratorMultiIsotopes_HH

class G4Event;
class GmParticleSource;
class GmIsotopeMgr;
class GmMultiIsotopesSource;

#include "globals.hh"
#include <vector>
#include <iostream>
#include "G4VUserPrimaryGeneratorAction.hh"
#include <fstream>
#include "GmGenerator.hh"
#include "GmGenerTextFileData.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

struct GmGenerIsotopeData {
  G4String name;
  G4double lifeTime;
  G4double prodPerTime;
};
  
class GmGeneratorMultiIsotopes: public GmGenerator
{
public:
  GmGeneratorMultiIsotopes();
  ~GmGeneratorMultiIsotopes();
  void GeneratePrimaries(G4Event* evt);

private:
  void GenerateParticle( const GenerTextFileData& evtDat, G4Event* evt );
  void BuildIsotopeList( G4String fname );
  void BuildIsotopeSources( G4String fnamesfl );
  void BuildGammaSource( G4String fName );
    
  
private:
  std::vector<GmMultiIsotopesSource*> theMISources;
  std::map<G4String,GmGenerIsotopeData> theIsotopeDatae;
  G4double theProtonProduction;
  G4double thePreviousProductionTime; // time of isotope production before first event
  G4double thePreviousDecayTime; // time of isotope decay before first event
  G4bool bFirstEvent;
  G4bool bFirstGammaAsPositrons;
  G4bool bPrintEventTime;
  G4double theFirstEventTime;
  G4bool bOnlyDecay;
  G4bool bSkipRandomNumberOfLines;
  GmMultiIsotopesSource* theGammaSource;
  
};

#endif
