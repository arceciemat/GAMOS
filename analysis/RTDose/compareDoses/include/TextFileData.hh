#ifndef TextFileData__HH
#define TextFileData__HH
#include <vector>
#include <fstream>
#include "globals.hh"

#include "FileData.hh"
#include "DoserData.hh"

class TextFileData : public FileData
{
public:
  TextFileData(G4String fileName, G4bool bInvertX );
  TextFileData(const FileData* fd2);
  // TextFileData(std::vector<G4String> wl);
  ~TextFileData(){};

public:
  
  virtual void Print( std::ofstream& fout );
  
private:
  G4String ExtractName( std::vector<G4String> wl);

};

#endif
