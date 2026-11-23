#ifndef GmEMParticleFilter_h
#define GmEMParticleFilter_h 1

class G4NeutralDefinition;
#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This filter accepts tracks that are gammas, electrons or positrons
// 
///////////////////////////////////////////////////////////////////////////////

class GmEMParticleFilter : public GmVFilter 
{

  public: // with description
      GmEMParticleFilter(G4String name);
      virtual ~GmEMParticleFilter();

  public: // with description
      virtual G4bool AcceptTrack(const G4Track*);

};

#endif
