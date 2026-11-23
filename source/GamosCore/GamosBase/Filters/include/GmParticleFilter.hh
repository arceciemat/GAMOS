#ifndef GmParticleFilter_h
#define GmParticleFilter_h 1

class G4ParticleDefinition;
#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

#include <set>

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class filters steps by particle definition.
// 
///////////////////////////////////////////////////////////////////////////////

class GmParticleFilter : public GmVFilter 
{

  public: // with description
      GmParticleFilter(G4String name);


      virtual ~GmParticleFilter();

  public: // with description
      virtual G4bool AcceptTrack(const G4Track*);

      //
      void show();
  void SetParameters( std::vector<G4String>& params);

  private:
      std::set<G4ParticleDefinition*> theParticles;

};

#endif
