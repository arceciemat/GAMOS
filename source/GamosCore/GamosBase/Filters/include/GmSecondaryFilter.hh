#ifndef GmSecondaryFilter_h
#define GmSecondaryFilter_h 1

class G4Track;
class G4NeutralDefinition;
#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This filter accepts secondary particle tracks.
// 
///////////////////////////////////////////////////////////////////////////////

class GmSecondaryFilter : public GmVFilter 
{

  public: // with description
      GmSecondaryFilter(G4String name);
      virtual ~GmSecondaryFilter();

  public: // with description
      virtual G4bool AcceptTrack(const G4Track*);

};

#endif
