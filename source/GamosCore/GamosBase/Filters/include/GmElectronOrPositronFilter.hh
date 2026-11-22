#ifndef GmElectronOrPositronFilter_h
#define GmElectronOrPositronFilter_h 1

class G4NeutralDefinition;
#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This filter accepts tracks that are electrons or positrons
// 
///////////////////////////////////////////////////////////////////////////////

class GmElectronOrPositronFilter : public GmVFilter 
{

  public: // with description
      GmElectronOrPositronFilter(G4String name);
      virtual ~GmElectronOrPositronFilter();

  public: // with description
      virtual G4bool AcceptTrack(const G4Track*);

};

#endif
