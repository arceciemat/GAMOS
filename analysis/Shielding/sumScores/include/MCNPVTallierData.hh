#ifndef MCNPVTallierData_hh
#define MCNPVTallierData_hh

#include "ScorerData.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

class MCNPVTallierData : public ScorerData
{
public:
  MCNPVTallierData(std::vector<G4String>& wl, G4bool bMod = false);
  ~MCNPVTallierData(){};

  virtual G4String GetIndexVal( G4String scoreName, G4bool bLogX );

  virtual G4bool CheckData( std::vector<G4String>& wl ) = 0;

  void ReadData( std::vector<G4String>& wl, GmFileIn& fin );

  G4bool IsModified() const {
    return  bModified;
  }

  virtual G4String GetName() const{
    return theName;
  }
 G4int GetTallyType() const {
   return theTallyType; 
 }
 void SetTallyType(G4int tt ) {
   theTallyType = tt; 
 }
protected:
  void CreateTallys( std::vector<G4String> wl, std::vector<G4String> wl2 );
  void CreateTotalTally();

  virtual void Print( std::ofstream& fout ) = 0;

protected:
  G4bool bModified; // tally modifier used
  G4String theParticle;
  G4int theTallyType; // 1, 2, 3, 4, ...
};

#endif
