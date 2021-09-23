#ifndef GmClassifierByKineticEnergy_hh
#define GmClassifierByKineticEnergy_hh 1

#include <vector>
#include "globals.hh"
class G4LogicalVolume;
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"

class GmClassifierByKineticEnergy  : public GmVClassifier
{
public: // with description
  GmClassifierByKineticEnergy(G4String);
  virtual ~GmClassifierByKineticEnergy();
  
  virtual int64_t GetIndexFromStep(const G4Step*);
  virtual int64_t GetIndexFromTrack(const G4Track*);

  virtual void SetParameters( std::vector<G4String>& param );

  virtual G4String GetIndexName(int64_t);

  virtual void SetIndices( std::vector<G4String> wl );

private:
  std::map<G4double,int64_t> theIndexMap;
  G4double theMin, theMax, theStep;
};

#endif
