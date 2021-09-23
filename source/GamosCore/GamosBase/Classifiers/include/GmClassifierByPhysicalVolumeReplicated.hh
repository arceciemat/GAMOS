#ifndef GmClassifierByPhysicalVolumeReplicated_hh
#define GmClassifierByPhysicalVolumeReplicated_hh 1

#include <vector>
#include <map>
#include "globals.hh"
class G4VPhysicalVolume;
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"

class GmClassifierByPhysicalVolumeReplicated  : public GmVClassifier
{
public: // with description
  GmClassifierByPhysicalVolumeReplicated(G4String);
  virtual ~GmClassifierByPhysicalVolumeReplicated();
  
  virtual int64_t GetIndexFromStep(const G4Step*);
  virtual int64_t GetIndexFromTrack(const G4Track*);

  virtual void SetParameters( std::vector<G4String>& param );

  virtual G4String GetIndexName(int64_t);

  virtual void SetIndices( std::vector<G4String> wl );

private:
  //  std::map<G4VPhysicalVolume*,int64_t> theIndexMap;
  std::map<G4String,int64_t> theIndexMap; // for divisions the copies share a unique PV pointer
};

#endif
