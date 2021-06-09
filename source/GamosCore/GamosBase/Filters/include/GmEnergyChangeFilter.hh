#ifndef GmEnergyChangeFilter_h
#define GmEnergyChangeFilter_h 1

#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This filter accepts particles if energy is in a defined user range. It uses PreStepPoint energy in AcceptStep
//
///////////////////////////////////////////////////////////////////////////////

class GmEnergyChangeFilter : public GmVFilter 
{
  
  //-------
public: // with description
  GmEnergyChangeFilter(G4String name);
  // Constructor
  
  virtual ~GmEnergyChangeFilter();
  
public: // with description
  virtual G4bool AcceptTrack(const G4Track*);
  virtual G4bool AcceptStep(const G4Step*);
  
  void show();
  void SetParameters( std::vector<G4String>& params);

private:
  G4double fLowEnergy;
  G4double fHighEnergy;

  G4double theTrackElost;
  G4int theLastTrackID;
  G4int theLastEventID;

};

#endif
