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
#if defined GamosCore_GamosUtils_EXPORTS
	__declspec(dllexport) static G4bool bUsingGmGenerator;
#else
	__declspec(dllimport) static G4bool bUsingGmGenerator;
#endif
#else
	static G4bool bUsingGmGenerator;
#endif

  static void SetbUsingGmGenerator( G4bool val ) {
    bUsingGmGenerator = val;
  }

};

#endif
