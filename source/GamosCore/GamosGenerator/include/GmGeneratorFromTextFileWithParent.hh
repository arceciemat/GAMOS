#ifndef GmGeneratorFromTextFileWithParent_HH
#define GmGeneratorFromTextFileWithParent_HH

class G4Event;
class GmParticleSource;
class GmIsotopeMgr;

#include "globals.hh"
#include <vector>
#include <iostream>
#include "G4VUserPrimaryGeneratorAction.hh"
#include <fstream>
#include "GamosCore/GamosGenerator/include/GmGeneratorFromFile.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

struct GenerTextFileDataWP{
  G4int eventID;
  G4String partName;
  G4double posx, posy, posz, momx, momy, momz, time, weight;
};

class GmGeneratorFromTextFileWithParent: public GmGeneratorFromFile
{
public:
  GmGeneratorFromTextFileWithParent();
  ~GmGeneratorFromTextFileWithParent();
  void GeneratePrimaries(G4Event* evt);

private:
  void GenerateParticle( const GenerTextFileDataWP& evtDat, G4Event* evt );

private:
  GmFileIn theFileIn;

  G4int thePrevEvent;
  GenerTextFileDataWP thePrevData;
  
  G4int theNLinesRead;

  G4String theGoodParticle;
};

#endif
