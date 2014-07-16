#ifndef GmClassifierByAncestors_hh
#define GmClassifierByAncestors_hh 1

#include <vector>
#include "globals.hh"

#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"

class GmClassifierByAncestors  : public GmVClassifier
{
public: // with description
  GmClassifierByAncestors(G4String);
  virtual ~GmClassifierByAncestors(){ };
  
  virtual G4int GetIndexFromStep(const G4Step*);
  virtual G4int GetIndexFromTrack(const G4Track*);

  virtual G4String GetIndexName(G4int index);

  virtual void SetParameters( std::vector<G4String>& param );

  virtual void SetIndices( std::vector<G4String> wl );

private:
  G4int theNAncestors;
  G4int theNShift;
  G4int theMaxIndex;

  std::map<G4int,G4String> theIndexNames;
  std::map<G4String,G4int> theIndexMap;
};

#endif
