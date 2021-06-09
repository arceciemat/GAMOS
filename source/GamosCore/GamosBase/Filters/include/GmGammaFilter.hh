#ifndef GmGammaFilter_h
#define GmGammaFilter_h 1

class G4NeutralDefinition;
#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This filter accepts tracks that are gammas
// 
///////////////////////////////////////////////////////////////////////////////

class GmGammaFilter : public GmVFilter 
{

  public: // with description
      GmGammaFilter(G4String name);
      virtual ~GmGammaFilter();

  public: // with description
      virtual G4bool AcceptTrack(const G4Track*);

};

#endif
