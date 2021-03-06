#ifndef GmClassifierByLogicalVolume_hh
#define GmClassifierByLogicalVolume_hh 1

#include <vector>
#include <map>
#include "globals.hh"
class G4LogicalVolume;
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"

class GmClassifierByLogicalVolume  : public GmVClassifier
{
public: // with description
  GmClassifierByLogicalVolume(G4String);
  virtual ~GmClassifierByLogicalVolume();
  
  virtual G4int GetIndexFromStep(const G4Step*);
  virtual G4int GetIndexFromTrack(const G4Track*);

  virtual void SetParameters( std::vector<G4String>& param );

  virtual G4String GetIndexName(G4int);

  virtual void SetIndices( std::vector<G4String> wl );

private:
  std::map<G4LogicalVolume*,G4int> theIndexMap;
};

#endif
