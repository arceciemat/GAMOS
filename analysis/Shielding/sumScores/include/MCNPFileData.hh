#ifndef MCNPFileData_hh
#define MCNPFileData_hh

#include "FileData.hh"
#include "MCNPTallyData.hh"
#include <map>

class MCNPFileData : public FileData
{
public:
  MCNPFileData( const G4String fileName );
  ~MCNPFileData(){};

  //  void BuildHStar( G4String particle, G4String fNameDose, G4int interp);
  //  void PrintHStar( G4String particle, std::ofstream& fout );
  
  //  MCNPTallyData* GetTally( G4String tallyName ) const;
  //  std::vector<MCNPTallyData*> GetTallies() const {
  //    return theTallies;
  // }

  virtual void Print( std::ofstream& fout );
  
private:
};

#endif
