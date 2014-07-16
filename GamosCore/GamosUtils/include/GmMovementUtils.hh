#ifndef GmMovementUtils_HH
#define GmMovementUtils_HH
//
#include "globals.hh"

class GmMovementUtils
{
private:
  GmMovementUtils(){};
  ~GmMovementUtils(){};

public:
  static G4bool bUsingGmGenerator;

private:
  static void SetbUsingGmGenerator( G4bool val ) {
    bUsingGmGenerator = val;
  }

};

#endif
