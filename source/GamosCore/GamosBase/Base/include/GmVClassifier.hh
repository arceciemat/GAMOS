#ifndef GmVClassifier_hh
#define GmVClassifier_hh 1

#include <vector>
#include <set>
#include "globals.hh"
#include "G4THitsMap.hh"
class G4Step;
class G4Track;
// class description:
//
// This is the base class of the GmVClassifier.


class GmVClassifier 
{
public: // with description
  GmVClassifier(G4String );
  virtual ~GmVClassifier(){ 
  }
  
  virtual int64_t GetIndexFromStep(const G4Step* aStep ) = 0;

  virtual int64_t GetIndexFromTrack(const G4Track*);

  virtual int64_t GetIndexFromSecoTrack(const G4Track* aTrack1, const G4Track* aTrack2);

  virtual G4String GetIndexName(int64_t);

  virtual void SetParameters( std::vector<G4String>& ){};

  virtual std::set<int64_t> GetIndexSet() const{
    std::set<int64_t> si;
    return si; 
  }

  virtual void SetIndices( std::vector<G4String> wl ) = 0;

  void CheckNumberOfIndices( std::vector<G4String> wl );

  G4String GetName() const {
    return theName; 
  }
  inline G4String GetClass() const {
    return theClass; }
  inline void SetClass( G4String cl ) {
    theClass = cl;  }

  G4bool IsCompound() const {
    return bIsCompound; }

  G4bool IsOnSecondary() {
    return bOnSecondary; }

protected:
  G4String theName;
  G4String theClass;

  int64_t theMaxIndex;

  G4bool bIsCompound;

  G4bool bOnSecondary;
};

#endif
