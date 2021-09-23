#ifndef GmClassifierCrossPhantom_hh
#define GmClassifierCrossPhantom_hh 1

#include <vector>
#include "globals.hh"

#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
class G4TouchableHistory;

class GmClassifierCrossPhantom  : public GmVClassifier
{
public: // with description
  GmClassifierCrossPhantom(G4String);
  virtual ~GmClassifierCrossPhantom(){ };
  
  virtual int64_t GetIndexFromStep(const G4Step*);
  virtual int64_t GetIndexFromTrack(const G4Track*);

  virtual void SetParameters( std::vector<G4String>& param );

  virtual void SetIndices( std::vector<G4String> wl );

private:
  int64_t GetIndex(G4TouchableHistory* th);

  std::map<int64_t,G4String> theIndexNames;
  std::map<G4String,int64_t> theIndexMap;

  G4int theNVoxelsX;
  G4int theNVoxelsY;
  G4int theNVoxelsZ;
  G4int theNVoxelsXY;
  G4int theYPhantomNVoxels;
  
};

#endif
