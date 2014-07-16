 #ifndef GmNumberOfSecondariesFilter_h
#define GmNumberOfSecondariesFilter_h 1

#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

#include <set>

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class filters steps by number of secondaries
// 
///////////////////////////////////////////////////////////////////////////////

class GmNumberOfSecondariesFilter : public GmVFilter 
{

  public: // with description
      GmNumberOfSecondariesFilter(G4String name);

      virtual ~GmNumberOfSecondariesFilter();

  public: // with description
      virtual G4bool AcceptStep(const G4Step*);

      //
      void show();
  void SetParameters( std::vector<G4String>& params);

  private:
  G4int theNSecoMin;
  G4int theNSecoMax;

};

#endif
