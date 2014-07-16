#ifndef GmDifferentLogicalVolumeFilter_h
#define GmDifferentLogicalVolumeFilter_h 1

class G4NeutralDefinition;
#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This filter accepts primary particle tracks.
// 
///////////////////////////////////////////////////////////////////////////////

class GmDifferentLogicalVolumeFilter : public GmVFilter 
{

  public: // with description
      GmDifferentLogicalVolumeFilter(G4String name);
      virtual ~GmDifferentLogicalVolumeFilter();

  public: // with description
      virtual G4bool AcceptStep(const G4Step*);
      virtual G4bool AcceptTrack(const G4Track*);

};

#endif
