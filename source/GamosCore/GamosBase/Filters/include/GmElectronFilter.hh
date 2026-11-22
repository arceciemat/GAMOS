#ifndef GmElectronFilter_h
#define GmElectronFilter_h 1

class G4NeutralDefinition;
#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This filter accepts tracks that are electrons
// 
///////////////////////////////////////////////////////////////////////////////

class GmElectronFilter : public GmVFilter 
{

  public: // with description
      GmElectronFilter(G4String name);
      virtual ~GmElectronFilter();

  public: // with description
      virtual G4bool AcceptTrack(const G4Track*);

};

#endif
