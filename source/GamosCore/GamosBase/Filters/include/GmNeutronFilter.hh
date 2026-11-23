#ifndef GmNeutronFilter_h
#define GmNeutronFilter_h 1

class G4NeutralDefinition;
#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This filter accepts tracks that are gammas
// 
///////////////////////////////////////////////////////////////////////////////

class GmNeutronFilter : public GmVFilter 
{

  public: // with description
      GmNeutronFilter(G4String name);
      virtual ~GmNeutronFilter();

  public: // with description
      virtual G4bool AcceptTrack(const G4Track*);

};

#endif
