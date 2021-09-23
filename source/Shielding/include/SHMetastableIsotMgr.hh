#ifndef SHMetastableIsotMgr_h
#define SHMetastableIsotMgr_h

#include "globals.hh"
#include <vector>
#include <map>
class G4ParticleDefinition;
class G4Ions;

typedef std::pair<G4double,G4double> pdd;
typedef std::pair<G4ParticleDefinition*,pdd> ppdd;

struct MetastableData {
  G4String name;
  G4double energy;
  G4double halflife;
  G4double branchingRatio;
};    //Sc44m 270.95*keV 58.61*hour 0.988

//------------------------------------------------------------------------
class SHMetastableIsotMgr 
{ 
 private:
  SHMetastableIsotMgr();

 public:
  ~SHMetastableIsotMgr();

  //! Get the only instance 
  static SHMetastableIsotMgr* GetInstance();
  
  void ReadData();
  std::pair<G4Ions*,G4double> FindIsomere( G4ParticleDefinition* nucleus, std::vector<ppdd> tracks ); // return isomere and its weight

private:

  static SHMetastableIsotMgr* theInstance;
  std::map<G4String, G4Ions* > theMetastableIsotopes;
  std::map<G4String, MetastableData > theMetastableIsotopesData;

};
#endif
