#ifndef GmGeneratorFromTextFile_HH
#define GmGeneratorFromTextFile_HH

class G4Event;
class GmParticleSource;
class GmIsotopeMgr;

#include "globals.hh"
#include <vector>
#include <iostream>
#include "G4VUserPrimaryGeneratorAction.hh"
#include <fstream>
#include "GmGeneratorFromFile.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

struct GenerTextFileData{
  G4int eventID;
  G4String partName;
  G4double posx, posy, posz, momx, momy, momz, time, weight;
};

class GmGeneratorFromTextFile: public GmGeneratorFromFile
{
public:
  GmGeneratorFromTextFile();
  ~GmGeneratorFromTextFile();
  void GeneratePrimaries(G4Event* evt);

private:
  void GenerateParticle( const GenerTextFileData& evtDat, G4Event* evt );

private:
  GmFileIn theFileIn;

  G4int thePrevEvent;
  GenerTextFileData thePrevData;
  
  G4int theNLinesRead;
};

#endif
