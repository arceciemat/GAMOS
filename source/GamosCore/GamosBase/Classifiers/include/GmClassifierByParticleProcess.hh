#ifndef GmClassifierByParticleProcess_hh
#define GmClassifierByParticleProcess_hh 1

#include <vector>
#include <map>
#include "globals.hh"
class G4VProcess;
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"

class GmClassifierByParticleProcess  : public GmVClassifier
{
public: // with description
  GmClassifierByParticleProcess(G4String);
  virtual ~GmClassifierByParticleProcess();
  
  virtual int64_t GetIndexFromStep(const G4Step*);
  virtual int64_t GetIndexFromTrack(const G4Track*);

  virtual void SetParameters( std::vector<G4String>& param );

  virtual G4String GetIndexName(int64_t);

  virtual void CheckNumberOfIndices( std::vector<G4String> wl );

  virtual void SetIndices( std::vector<G4String> wl );


private:
  std::map<const G4VProcess*,int64_t> theIndexMap;
};

#endif
