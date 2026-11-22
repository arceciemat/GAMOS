#ifndef GmDigit_HH
#define GmDigit_HH

#include "globals.hh"
#include "GamosCore/GamosSD/include/GmEDepo.hh"
#include "G4ThreeVector.hh"
#include <vector>
#include <map>
#include <iostream>

class GmHit;

class GmDigit
{

public:
  
  GmDigit( GmHit* hit );
  ~GmDigit(){};

  void AddHit( GmHit* hit );

  void Draw();
  void Print(std::ostream& os = G4cout );

  G4double GetEnergy() const { return theEnergy;}
  G4double GetTime() const;
  G4double GetTimeMin() const { return theTimeMin;}
  G4double GetTimeMax() const { return theTimeMax;}
  G4ThreeVector GetPosition() const { return thePosition;}
  std::vector<GmHit*> GetHits() const { return theHits; }
  G4int GetNHits() const { return theHits.size(); }
  G4String GetSDType() const { return theSDType; }

  void SetEnergy( G4double ener ) { theEnergy = ener;}
  void SetTimeMin( G4double tim ) { theTimeMin = tim;}
  void SetTimeMax( G4double tim ) { theTimeMax = tim;}
  void SetPosition( G4ThreeVector pos ) { thePosition = pos;}
  void SetSDType( G4String sdtyp )  { theSDType = sdtyp; }


protected:
  G4double theEnergy;
  G4double theTimeMin;
  G4double theTimeMax;
  G4ThreeVector thePosition;
  std::vector<GmHit*> theHits;
  G4String theSDType;
  G4int theHitsTimeType;

};

typedef std::map< G4String, std::vector<GmDigit*> > gamosSDDigitMap;
typedef std::vector<GmDigit*> gamosDigitList;

std::ostream& operator<<(std::ostream& os, const GmDigit& );

#endif
