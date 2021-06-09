#ifndef DetRecHitSet_HH
#define DetRecHitSet_HH

#include "globals.hh"
#include "G4ThreeVector.hh"
#include <set>
#include <map>
#include <vector>
#include <iostream>
class GmRecHit;


class DetRecHitSet : public std::set<GmRecHit*> 
{

public:
  DetRecHitSet();
  
  virtual ~DetRecHitSet(){};

  void Initialise();

  void AddHit( GmRecHit* rhit );

  G4double GetEnergy() const{
    return theEnergy; 
  }
  G4ThreeVector GetPosition() const{
    return thePosition; 
  }

  void SetEnergy( G4double ener ) {
    theEnergy = ener; 
  }
  void SetPosition( G4ThreeVector pos ) {
    thePosition = pos;
  }

  G4double GetTime() const;
  G4double GetTimeMin() const { return theTimeMin;}
  G4double GetTimeMax() const { return theTimeMax;}
 
  GmRecHit* GetRecHitByEnergy( G4int nEnergy );
  void Print( std::ostream& os = G4cout );

private:
  GmRecHit* GetRecHitByX( G4int nEnergy, std::map<G4double,GmRecHit*>& recHitMap, G4bool bStartByLast );

private:
  //  std::set<GmRecHit*> theRecHits;
  G4double theEnergy;
  G4ThreeVector thePosition;

  G4double theTimeMin;
  G4double theTimeMax;
};

typedef std::vector<DetRecHitSet> DetRecHitSetList;

#endif
