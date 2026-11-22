
#ifndef GmPSDummyRBEScorer_h
#define GmPSDummyRBEScorer_h 1

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "G4THitsMap.hh"


//////////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a dummy primitive scorer class for use with GmPSPrinterRBE 
// 
///////////////////////////////////////////////////////////////////////////////

class GmPSDummyRBEScorer : public GmVPrimitiveScorer
{
 
 public: // with description
      GmPSDummyRBEScorer(G4String name);

  protected: // with description
      virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);

  public:
      virtual ~GmPSDummyRBEScorer();

};



#endif
