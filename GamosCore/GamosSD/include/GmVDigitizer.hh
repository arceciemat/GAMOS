#ifndef GmVDigitizer_HH
#define GmVDigitizer_HH

#include <vector>
#include "globals.hh"

class GmDigit;
class GmHit;

class GmVDigitizer
{

public:
  
  GmVDigitizer(){};
  virtual ~GmVDigitizer(){};

  void SetName( const G4String& name ){
    theName = name; }
  void SetParams(){};

  virtual std::vector<GmDigit*> DigitizeHits(const std::vector<GmHit*>*) = 0;

  virtual void CleanDigits() = 0; //cannot be done here, because there is no info about GmDigi

  void SetSDType( G4String sdtyp ) {
    theSDType = sdtyp;
  }

protected:
  std::vector<GmDigit*> theDigits;
  G4String theName;
  G4String theSDType;
};

#endif
