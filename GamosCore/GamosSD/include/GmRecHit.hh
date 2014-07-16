#ifndef GmRecHit_HH
#define GmRecHit_HH

#include "globals.hh"
#include "GamosCore/GamosSD/include/GmEDepo.hh"
#include "G4ThreeVector.hh"
#include <vector>
#include <map>
#include <iostream>

class GmHit;
struct GmRecHitPrint {
  float energy;
  unsigned long long timeMin;
  unsigned long long timeMax;
  float posX;
  float posY;
  float posZ;
  unsigned int nhits;
};

class GmRecHit
{

public:
  
  GmRecHit(){};
  GmRecHit( GmHit* hit );
  ~GmRecHit(){};

  void AddHit( GmHit* hit );

  void Draw();
  void Print(std::ostream& os = G4cout, G4bool bVerbose = true );
  void Print(FILE* fout, G4bool bVerbose = true );

  G4double GetEnergy() const { return theEnergy;}
  G4double GetTime() const;
  G4double GetTimeMin() const { return theTimeMin;}
  G4double GetTimeMax() const { return theTimeMax;}
  G4ThreeVector GetPosition() const { return thePosition;}
  G4ThreeVector GetPositionOfMaxEnergy() const;
  std::vector<GmHit*> GetHits() const { return theHits; }
  G4int GetNHits() const { return theHits.size(); }
  G4String GetSDType() const { return theSDType; }

  void SetPosition( G4ThreeVector pos ) { thePosition = pos;}
  void SetEnergy( G4double ener ) { theEnergy = ener;}
  void SetEnergySmeared( G4double ener ) {
    theEnergyNoSmear = theEnergy;
    theEnergy = ener;
  }
  void SetTime( G4double tim ) { theTimeMin = tim;}
  void SetTimeMin( G4double tim ) { theTimeMin = tim;}
  void SetTimeMinSmeared( G4double tim ) { 
    theTimeMinNoSmear = theTimeMin;
    theTimeMin = tim;
  }
  void SetTimeMax( G4double tim ) { theTimeMax = tim;}
  void SetHits( std::vector<GmHit*> hits ) { theHits = hits; }
  void SetSDType( G4String sdt ) { theSDType = sdt; }


protected:
  G4double theEnergy;
  G4double theEnergyNoSmear;
  G4double theTimeMin;
  G4double theTimeMinNoSmear;
  G4double theTimeMax;
  G4ThreeVector thePosition;
  std::vector<GmHit*> theHits;
  G4String theSDType;

  G4double theMaxSimHitEnergy; 
  G4bool bPosAtBarycentre;

  G4int theHitsTimeType;
};

typedef std::map< G4String,  std::vector<GmRecHit*> > gamosSDRecHitMap;
typedef std::vector<GmRecHit*> gamosRecHitList;

std::ostream& operator<<(std::ostream& os, const GmRecHit& );

#endif














