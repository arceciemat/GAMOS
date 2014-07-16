#ifndef GmSSData_hh
#define GmSSData_hh

#include "GamosCore/GamosAnalysis/include/GmTrajStep.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"

#include <vector>
#include <iostream>

class GmSSData 
{
public:
  GmSSData( G4int showerID, const G4Step* aStep, TSPUse pointToUse );
  ~GmSSData(){};
  void AddStep( const G4Step* aStep );

  G4double GetTotalEnergy() const;  

  G4ThreeVector GetFirstTrackPos() const;
  G4ThreeVector GetFirstTrackDir() const;

  G4int GetShowerID() const {
    return theShowerID; 
  }
  G4int GetFirstTrackID() const {
    return theFirstTrackID;
  }
  G4int GetClassifierIndex() const {
    return theClassifierIndex;
  }
  std::vector<GmTrajStep*> GetSteps() const {
    return theSteps;
  }

  void SetClassifierIndex( G4int idx ){
    theClassifierIndex = idx;
  }

  void Dump( FILE* fout );
  void Dump( std::ostream& fout );

private:
  void DumpStepPre( GmTrajStep* step, std::ostream& fout );
  void DumpStepPost( GmTrajStep* step, std::ostream& fout );
  void DumpStepPre( GmTrajStep* step, FILE* fout );
  void DumpStepPost( GmTrajStep* step, FILE* fout );

private:
  TSPUse theTSPUse;
  G4int theShowerID;
  G4int theFirstTrackID;
  G4int theClassifierIndex;
  std::vector<GmTrajStep*> theSteps;
};

#endif
