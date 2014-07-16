#ifndef GmPrimaryFilter_h
#define GmPrimaryFilter_h 1

class G4NeutralDefinition;
#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This filter accepts primary particle tracks.
// 
///////////////////////////////////////////////////////////////////////////////

class GmPrimaryFilter : public GmVFilter 
{

  public: // with description
      GmPrimaryFilter(G4String name);
      virtual ~GmPrimaryFilter();

  public: // with description
      virtual G4bool AcceptTrack(const G4Track*);

};

#endif
