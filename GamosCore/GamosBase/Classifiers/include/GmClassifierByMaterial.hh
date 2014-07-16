#ifndef GmClassifierByMaterial_hh
#define GmClassifierByMaterial_hh 1

#include <vector>
#include <map>
#include "globals.hh"
class G4Material;
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"

class GmClassifierByMaterial  : public GmVClassifier
{
public: // with description
  GmClassifierByMaterial(G4String);
  virtual ~GmClassifierByMaterial();
  
  virtual G4int GetIndexFromStep(const G4Step*);
  virtual G4int GetIndexFromTrack(const G4Track*);

  virtual void SetParameters( std::vector<G4String>& param );

  virtual G4String GetIndexName(G4int);

  virtual void SetIndices( std::vector<G4String> wl );

private:
  std::map<G4Material*,G4int> theIndexMap;
};

#endif
