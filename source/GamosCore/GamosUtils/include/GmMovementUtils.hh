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

#ifdef WIN32
#if defined GmUtils_ALLOC_EXPORT
	G4DLLEXPORT static G4bool bUsingGmGenerator;
#else
	G4DLLIMPORT static G4bool bUsingGmGenerator;
#endif
#else
	static G4bool bUsingGmGenerator;
#endif

  static void SetbUsingGmGenerator( G4bool val ) {
    bUsingGmGenerator = val;
  }

};

#endif
