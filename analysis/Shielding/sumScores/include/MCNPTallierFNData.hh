#ifndef MCNPTallierFNData_hh
#define MCNPTallierFNData_hh

#include "MCNPVTallierData.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

class MCNPTallierFNData : public MCNPVTallierData
{
public:
  MCNPTallierFNData( std::vector<G4String>& wl, GmFileIn& fin, G4bool bMod );
  ~MCNPTallierFNData(){};

  virtual G4bool CheckData( std::vector<G4String>& wl );

  virtual void Print( std::ofstream& fout );

  G4int GetNonZeroHist() const {
    return theNonZeroHist;
  }
  void SetNonZeroHist( G4int nzh ){
    theNonZeroHist = nzh;
  }
  G4int GetCell() const {
    return theCell;
  }
  void SetCell( G4int cell ){
    theCell = cell;
  }

private:
  G4int theCell;
  G4int theNonZeroHist;

};

#endif
