#ifndef GmClassifierByRegion_hh
#define GmClassifierByRegion_hh 1

#include <vector>
#include <map>
#include "globals.hh"
class G4Region;
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"

class GmClassifierByRegion  : public GmVClassifier
{
public: // with description
  GmClassifierByRegion(G4String);
  virtual ~GmClassifierByRegion();
  
  virtual G4int GetIndexFromStep(const G4Step*);
  virtual G4int GetIndexFromTrack(const G4Track*);

  virtual void SetParameters( std::vector<G4String>& param );

  virtual G4String GetIndexName(G4int);

  virtual void SetIndices( std::vector<G4String> wl );

private:
  std::map<G4Region*,G4int> theIndexMap;
};

#endif
