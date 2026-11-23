#ifndef GmCompoundClassifier_hh
#define GmCompoundClassifier_hh 1

#include <vector>
#include "globals.hh"
//
class G4Step;
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"

class GmCompoundClassifier  : public GmVClassifier
{
public: // with description
  GmCompoundClassifier(G4String);
  virtual ~GmCompoundClassifier();
  
  virtual int64_t GetIndexFromStep(const G4Step*);
  virtual int64_t GetIndexFromTrack(const G4Track*);
  virtual int64_t GetIndexFromSecoTrack(const G4Track* aTrack1, const G4Track* aTrack2);
  
  virtual G4String GetIndexName(int64_t index);

  virtual void SetParameters( std::vector<G4String>& param );

  virtual void SetIndices( std::vector<G4String> wl );

  G4int GetNShift() const {
    return theNShift; }

private:
  G4int theNShift;
  std::vector<GmVClassifier*> theClassifiers;
};

#endif
