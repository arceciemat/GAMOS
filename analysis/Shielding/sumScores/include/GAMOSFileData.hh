#ifndef GAMOSFileData__HH
#define GAMOSFileData__HH
#include <vector>
#include <fstream>
#include "globals.hh"

#include "FileData.hh"
#include "ScorerData.hh"

class GAMOSFileData : public FileData
{
public:
  GAMOSFileData();
  GAMOSFileData(G4String fileName );
  GAMOSFileData(const FileData* fd2);
  // GAMOSFileData(std::vector<G4String> wl);
  ~GAMOSFileData(){};

public:
  
  virtual void Print( std::ofstream& fout );
  
private:
  G4String ExtractName( std::vector<G4String> wl);

};

#endif
