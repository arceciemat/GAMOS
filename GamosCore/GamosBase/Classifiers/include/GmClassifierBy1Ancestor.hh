#ifndef GmClassifierBy1Ancestor_hh
#define GmClassifierBy1Ancestor_hh 1

#include <vector>
#include <map>
#include "globals.hh"

#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"

class GmClassifierBy1Ancestor  : public GmVClassifier
{
public: // with description
  GmClassifierBy1Ancestor(G4String);
  virtual ~GmClassifierBy1Ancestor(){ };
  
  virtual G4int GetIndexFromStep(const G4Step*);
  virtual G4int GetIndexFromTrack(const G4Track*);

  virtual void SetParameters( std::vector<G4String>& param );

  virtual void SetIndices( std::vector<G4String> wl );

private:
  G4int theIndexDepth;
  std::map<G4int,G4int> theIndexMap;
};

#endif
