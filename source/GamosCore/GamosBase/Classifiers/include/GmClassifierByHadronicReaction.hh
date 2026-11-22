#ifndef GmClassifierByHadronicReaction_hh
#define GmClassifierByHadronicReaction_hh 1

#include <vector>
#include <map>
#include <set>
#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
class G4ParticleDefinition;
class G4IonTable;

class GmClassifierByHadronicReaction  : public GmVClassifier
{
public: // with description
  GmClassifierByHadronicReaction(G4String);
  virtual ~GmClassifierByHadronicReaction();
  
  virtual int64_t GetIndexFromStep(const G4Step*);
  virtual int64_t GetIndexFromTrack(const G4Track*);

  virtual void SetParameters( std::vector<G4String>& param );

  virtual G4String GetIndexName(int64_t);

  virtual void SetIndices( std::vector<G4String> ){};

private:
  std::map<const G4String,int64_t> theIndexMap;
  //- std::set<G4String> theParticlesToExclude;

  G4IonTable* theIonTable;

};

#endif
