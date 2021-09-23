#ifndef ROOTFileData__HH
#define ROOTFileData__HH
#include <vector>
#include <fstream>
#include "globals.hh"

#include "FileData.hh"
#include "DoserData.hh"

class ROOTFileData : public FileData
{
public:
  ROOTFileData(G4String fileName, G4bool bInvertX);
  ROOTFileData(const FileData* fd2);
  // ROOTFileData(std::vector<G4String> wl);
  ~ROOTFileData(){};

public:
  
  virtual void Print( std::ofstream& fout );
  
private:
  G4String ExtractName( std::vector<G4String> wl);

};

#endif
