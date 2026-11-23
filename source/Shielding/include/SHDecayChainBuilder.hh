#ifndef SHDecayChainBuilder_h
#define SHDecayChainBuilder_h 1

#include <map>
#include <vector>
#include "G4Ions.hh"

typedef std::vector<G4Ions*> vi;
typedef std::map<G4ParticleDefinition*, G4double> mpd;
typedef std::map<G4ParticleDefinition*, mpd* > mpmpd;

class G4Track;

class SHDecayChainBuilder
{

 public:
  SHDecayChainBuilder();
  ~SHDecayChainBuilder();

 public:
  void StoreIon(const G4Track*, G4bool bAlsoLifeTime0 = false);
  std::vector< vi* > StoreEventChains();
  void PrintChains( std::ostream& out = G4cout );
  static G4String GetChainName( vi* chain );
  std::map<G4String, vi* > GetChains() const{
    return theChains; 
  }
  vi GetChainFromTrack( G4int trkID );
  void Clean();

  G4double GetDecayBR( G4ParticleDefinition* parent, G4ParticleDefinition* daughter, G4bool bMustExist = true );

  void StoreDecayBranchingRatios( const G4Track* aTrack );
  void BuildPossibleDecayChains();

private:
  void AddChain( vi* );
  void AddDaughters( vi* chain );
  
private:
  std::map<G4Ions*,G4int> theIons; // start of decay chain
  std::map<G4int, G4int> theParentChildren;
  //  std::map<G4String, vi* > theChains;
  std::vector< vi* > theChains;

  mpmpd theBranchingRatios;

};

#endif
