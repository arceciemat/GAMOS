#ifndef GmClassifierByPrimaryParticle_hh
#define GmClassifierByPrimaryParticle_hh 1

#include <vector>
#include <map>
#include "globals.hh"
class G4ParticleDefinition;
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"

class GmClassifierByPrimaryParticle  : public GmVClassifier
{
public: // with description
  GmClassifierByPrimaryParticle(G4String);
  virtual ~GmClassifierByPrimaryParticle();
  
  virtual int64_t GetIndexFromStep(const G4Step*);
  virtual int64_t GetIndexFromTrack(const G4Track*);

  virtual void SetParameters( std::vector<G4String>& param );

  virtual G4String GetIndexName(int64_t);

  virtual void SetIndices( std::vector<G4String> wl );

private:
  std::map<G4ParticleDefinition*,int64_t> theIndexMap;
  G4ParticleDefinition* thePrimaryParticle;
};

#endif
