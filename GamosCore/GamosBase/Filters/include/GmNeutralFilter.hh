#ifndef GmNeutralFilter_h
#define GmNeutralFilter_h 1

class G4NeutralDefinition;
#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This filter accepts neutral tracks.
// 
///////////////////////////////////////////////////////////////////////////////

class GmNeutralFilter : public GmVFilter 
{

  public: // with description
      GmNeutralFilter(G4String name);
      virtual ~GmNeutralFilter();

  public: // with description
      virtual G4bool AcceptTrack(const G4Track*);

};

#endif
