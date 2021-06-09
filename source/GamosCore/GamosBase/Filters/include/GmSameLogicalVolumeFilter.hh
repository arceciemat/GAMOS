#ifndef GmSameLogicalVolumeFilter_h
#define GmSameLogicalVolumeFilter_h 1

class G4NeutralDefinition;
#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This filter accepts primary particle tracks.
// 
///////////////////////////////////////////////////////////////////////////////

class GmSameLogicalVolumeFilter : public GmVFilter 
{

  public: // with description
      GmSameLogicalVolumeFilter(G4String name);
      virtual ~GmSameLogicalVolumeFilter();

  public: // with description
      virtual G4bool AcceptStep(const G4Step*);
      virtual G4bool AcceptTrack(const G4Track*);

};

#endif
