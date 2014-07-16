#ifndef GmClassifierByPhysicalVolume_hh
#define GmClassifierByPhysicalVolume_hh 1

#include <vector>
#include <map>
#include "globals.hh"
class G4VPhysicalVolume;
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"

class GmClassifierByPhysicalVolume  : public GmVClassifier
{
public: // with description
  GmClassifierByPhysicalVolume(G4String);
  virtual ~GmClassifierByPhysicalVolume();
  
  virtual G4int GetIndexFromStep(const G4Step*);
  virtual G4int GetIndexFromTrack(const G4Track*);

  virtual void SetParameters( std::vector<G4String>& param );

  virtual G4String GetIndexName(G4int);

  virtual void SetIndices( std::vector<G4String> wl );

private:
  std::map<G4VPhysicalVolume*,G4int> theIndexMap;
};

#endif
