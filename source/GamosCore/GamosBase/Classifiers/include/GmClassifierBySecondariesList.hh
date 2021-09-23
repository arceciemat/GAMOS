#ifndef GmClassifierBySecondariesList_hh
#define GmClassifierBySecondariesList_hh 1

#include <vector>
#include <map>
#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"

class GmClassifierBySecondariesList  : public GmVClassifier
{
public: // with description
  GmClassifierBySecondariesList(G4String);
  virtual ~GmClassifierBySecondariesList();
  
  virtual int64_t GetIndexFromStep(const G4Step*);
  virtual int64_t GetIndexFromTrack(const G4Track*);

  virtual void SetParameters( std::vector<G4String>& param );

  virtual G4String GetIndexName(int64_t);

  virtual void SetIndices( std::vector<G4String> ){};

private:
  std::map<const G4String,int64_t> theIndexMap;
};

#endif
