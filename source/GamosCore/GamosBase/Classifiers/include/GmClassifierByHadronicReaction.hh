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
  
  virtual G4int GetIndexFromStep(const G4Step*);
  virtual G4int GetIndexFromTrack(const G4Track*);

  virtual void SetParameters( std::vector<G4String>& param );

  virtual G4String GetIndexName(G4int);

  virtual void SetIndices( std::vector<G4String> ){};

private:
  std::map<const G4String,G4int> theIndexMap;
  //- std::set<G4String> theParticlesToExclude;

  G4IonTable* theIonTable;

};

#endif
