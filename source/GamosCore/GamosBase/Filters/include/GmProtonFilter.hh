#ifndef GmProtonFilter_h
#define GmProtonFilter_h 1

class G4NeutralDefinition;
#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This filter accepts tracks that are gammas
// 
///////////////////////////////////////////////////////////////////////////////

class GmProtonFilter : public GmVFilter 
{

  public: // with description
      GmProtonFilter(G4String name);
      virtual ~GmProtonFilter();

  public: // with description
      virtual G4bool AcceptTrack(const G4Track*);

};

#endif
