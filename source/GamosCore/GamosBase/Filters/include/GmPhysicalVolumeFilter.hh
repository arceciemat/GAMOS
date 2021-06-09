#ifndef GmPhysicalVolumeFilter_h
#define GmPhysicalVolumeFilter_h 1

class G4VPhysicalVolmue;
#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

#include <set>

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This class filters steps by physical volume
// 
///////////////////////////////////////////////////////////////////////////////

class GmPhysicalVolumeFilter : public GmVFilter 
{

  public: // with description
      GmPhysicalVolumeFilter(G4String name);


      virtual ~GmPhysicalVolumeFilter();

  public: // with description
  virtual G4bool AcceptStep(const G4Step* aStep);

      //
      void show();
  void SetParameters( std::vector<G4String>& params);

  private:
      std::set<G4VPhysicalVolume*> theVPhysicalVolumes;

};

#endif
