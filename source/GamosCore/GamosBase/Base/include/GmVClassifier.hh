#ifndef GmVClassifier_hh
#define GmVClassifier_hh 1

#include <vector>
#include <set>
#include "globals.hh"
#include "G4Threading.hh"
#include "G4AutoLock.hh"
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

  //--- Multithreading: classifiers created on the master are registered by name; worker classifiers that assign
  //    indices in order of appearance ask the master classifier of the same name, so that the indices are the same
  //    in all threads (results are merged by index) and the master knows the names of all indices
  static void RegisterMasterClassifier( GmVClassifier* cls );
  static GmVClassifier* GetMasterClassifier( const G4String& name );
  static G4Mutex& GetIndexMutex();

protected:
  // Assign the index of a new key: in sequential mode / master as before (size+1+theMaxIndex),
  // in a worker thread the index is assigned by the master classifier (under mutex) and cached in this one
  template<class CLS, class MAP, class KEY>
  int64_t AssignNewIndex( CLS* cls, MAP CLS::* mapMember, const KEY& key, int64_t add = 1, G4bool bAddMaxIndex = true ) {
    MAP& localMap = cls->*mapMember;
    if( !G4Threading::IsMultithreadedApplication() || G4Threading::IsMasterThread() ) {
      int64_t index = localMap.size()+add+(bAddMaxIndex ? theMaxIndex : 0);
      localMap[key] = index;
      return index;
    }
    G4AutoLock l(&GetIndexMutex());
    CLS* master = dynamic_cast<CLS*>(GetMasterClassifier(theName));
    int64_t index;
    if( !master ) {
      index = localMap.size()+add+(bAddMaxIndex ? theMaxIndex : 0);
    } else {
      MAP& masterMap = master->*mapMember;
      typename MAP::const_iterator ite = masterMap.find(key);
      if( ite == masterMap.end() ) {
	index = masterMap.size()+add+(bAddMaxIndex ? master->theMaxIndex : 0);
	masterMap[key] = index;
      } else {
	index = ite->second;
      }
    }
    localMap[key] = index;
    return index;
  }

protected:
  G4String theName;
  G4String theClass;

  int64_t theMaxIndex;

  G4bool bIsCompound;

  G4bool bOnSecondary;
};

#endif
