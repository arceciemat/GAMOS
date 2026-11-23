 #ifndef GmStepNumberFilter_h
#define GmStepNumberFilter_h 1

#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

#include <set>

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class filters steps by step numbers
// 
///////////////////////////////////////////////////////////////////////////////

class GmStepNumberFilter : public GmVFilter 
{

  public: // with description
      GmStepNumberFilter(G4String name);

      virtual ~GmStepNumberFilter();

  public: // with description
      virtual G4bool AcceptStep(const G4Step*);

      //
      void show();
  void SetParameters( std::vector<G4String>& params);

  private:
  G4int theNStepMin;
  G4int theNStepMax;

};

#endif
