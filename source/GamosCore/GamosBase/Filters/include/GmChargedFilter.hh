#ifndef GmChargedFilter_h
#define GmChargedFilter_h 1

class G4ChargedDefinition;
#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This filter accepts charged particles.
//
///////////////////////////////////////////////////////////////////////////////

class GmChargedFilter : public GmVFilter 
{

  public: // with description
      GmChargedFilter(G4String name);
      virtual ~GmChargedFilter();

  public: // with description
      virtual G4bool AcceptTrack(const G4Track*);

};
#endif
