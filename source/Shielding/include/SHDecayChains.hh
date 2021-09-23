#ifndef SHDecayChains_h
#define SHDecayChains_h 1

#include <map>
#include <vector>
#include "G4Ions.hh"

typedef std::vector<G4Ions*> vi;
class G4Track;

class SHDecayChains
{

 public:
  SHDecayChains();
  ~SHDecayChains();

 public:
  void StoreIon(const G4Track*);
  void StoreEventChains();
  void PrintChains( std::ostream& out );
  G4String GetChainName( vi* chain );
  std::map<G4String, vi* > GetChains() const{
    return theChains; 
  }
  void Clean();

 private:
  void AddChain( vi* );

private:
  std::map<G4int,G4Ions*> theIons;
  std::map<G4int, G4int> theParentChildren;
  std::map<G4String, vi* > theChains;
};

#endif
