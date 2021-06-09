#ifndef GmClassifierByCreatorProcess_hh
#define GmClassifierByCreatorProcess_hh 1

#include <vector>
#include <map>
#include "globals.hh"
class G4VProcess;
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"

class GmClassifierByCreatorProcess  : public GmVClassifier
{
public: // with description
  GmClassifierByCreatorProcess(G4String);
  virtual ~GmClassifierByCreatorProcess();
  
  virtual G4int GetIndexFromStep(const G4Step*);
  virtual G4int GetIndexFromTrack(const G4Track*);

  virtual void SetParameters( std::vector<G4String>& param );

  virtual G4String GetIndexName(G4int);

  virtual void SetIndices( std::vector<G4String> wl );

private:
  std::map<const G4String,G4int> theIndexMap;
};

#endif
