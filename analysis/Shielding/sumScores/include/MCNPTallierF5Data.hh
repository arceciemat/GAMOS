#ifndef MCNPTallierF5Data_hh
#define MCNPTallierF5Data_hh

#include "MCNPVTallierData.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "G4ThreeVector.hh"

class MCNPTallierF5Data : public MCNPVTallierData
{
public:
  MCNPTallierF5Data( std::vector<G4String>& wl, GmFileIn& fin, G4bool bMod );
  ~MCNPTallierF5Data(){};

  virtual G4bool CheckData( std::vector<G4String>& wl );

  void SetPosition( G4ThreeVector pos ){
    thePosition = pos;
  }
  G4ThreeVector GetPosition() const {
    return thePosition;
  }
  virtual void Print( std::ofstream& fout );

private:
  G4ThreeVector thePosition;

};

#endif
