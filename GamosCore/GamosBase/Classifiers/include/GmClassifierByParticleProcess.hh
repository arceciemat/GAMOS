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
  
  virtual G4int GetIndexFromStep(const G4Step*);
  virtual G4int GetIndexFromTrack(const G4Track*);

  virtual void SetParameters( std::vector<G4String>& param );

  virtual G4String GetIndexName(G4int);

  virtual void CheckNumberOfIndices( std::vector<G4String> wl );

  virtual void SetIndices( std::vector<G4String> wl );


private:
  std::map<const G4VProcess*,G4int> theIndexMap;
};

#endif
