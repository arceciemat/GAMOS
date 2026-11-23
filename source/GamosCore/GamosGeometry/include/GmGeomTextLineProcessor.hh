//------------------------------------------------------------------
//
// Description: 
//  This class is a G4tgrFileReader that adds the definition of regions
//
// Author:      Pedro Arce
// History:
//     Create class   May 2007
//
//------------------------------------------------------------------
#ifndef GmGeomTextLineProcessor_H
#define GmGeomTextLineProcessor_H 1

using namespace std;
#include "globals.hh"
#include <vector>

class G4tgrVolume;


#include "G4tgrLineProcessor.hh"

class GmGeomTextLineProcessor : public G4tgrLineProcessor
{

public:

  GmGeomTextLineProcessor();
  virtual ~GmGeomTextLineProcessor(){ };

  virtual G4bool ProcessLine( const std::vector<G4String>& wl );

private:

};

#endif 
