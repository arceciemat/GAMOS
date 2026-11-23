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
  
  virtual int64_t GetIndexFromStep(const G4Step*);
  virtual int64_t GetIndexFromTrack(const G4Track*);

  virtual G4String GetIndexName(int64_t index);

  virtual void SetParameters( std::vector<G4String>& param );

  virtual void SetIndices( std::vector<G4String> wl );

private:
  G4int theNAncestors;
  G4int theNShift;
  int64_t theMaxIndex;

  std::map<int64_t,G4String> theIndexNames;
  std::map<G4String,int64_t> theIndexMap;
};

#endif
