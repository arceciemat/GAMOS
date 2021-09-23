#ifndef GmGeneratorFromBinFile_HH
#define GmGeneratorFromBinFile_HH

class G4Event;

#include <vector>
#include "GmGeneratorFromFile.hh"
#include <fstream>

struct GenerBinFileData
{
  float posx, posy, posz, momx, momy, momz, time, weight;
};


class GmGeneratorFromBinFile: public GmGeneratorFromFile
{
public:
  GmGeneratorFromBinFile();
  ~GmGeneratorFromBinFile();
  void GeneratePrimaries(G4Event* evt);

private:
  G4bool CheckDataName( G4int ii, G4String dataName );
  void ReadHeader();
  void ReadHeaderCheck();

private:
  std::ifstream* theFileIn;

  std::vector<G4String> theExpectedDataNames;

  G4int theNExtraBytes;
  char* theExtraBytes;

  G4int theLastEventID;

  G4double theNEvents;

};

#endif
