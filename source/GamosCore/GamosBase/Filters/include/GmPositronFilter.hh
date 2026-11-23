#ifndef GmPositronFilter_h
#define GmPositronFilter_h 1

class G4NeutralDefinition;
#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This filter accepts tracks that are positrons
// 
///////////////////////////////////////////////////////////////////////////////

class GmPositronFilter : public GmVFilter 
{

  public: // with description
      GmPositronFilter(G4String name);
      virtual ~GmPositronFilter();

  public: // with description
      virtual G4bool AcceptTrack(const G4Track*);

};

#endif
