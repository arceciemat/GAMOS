#ifndef GmClassifierByPhantomStructure_hh
#define GmClassifierByPhantomStructure_hh 1

#include <vector>
#include "globals.hh"

#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
class GmReadPhantomStMgr;

class GmClassifierByPhantomStructure  : public GmVClassifier
{
public: // with description
  GmClassifierByPhantomStructure(G4String);
  virtual ~GmClassifierByPhantomStructure(){ };
  
  virtual int64_t GetIndexFromStep(const G4Step*);
  virtual int64_t GetIndexFromTrack(const G4Track*);

  virtual void SetParameters( std::vector<G4String>& param );

private:
 virtual void SetIndices( std::vector<G4String> wl );

private:
  G4int theIndexDepth;
  std::map<int64_t,int64_t> theIndexMap;

  GmReadPhantomStMgr *theReadPhantomStMgr;

};

#endif
