#ifndef GmDepositedEnergyFilter_h
#define GmDepositedEnergyFilter_h 1

#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This filter accepts particles defined energy range.
//
///////////////////////////////////////////////////////////////////////////////

class GmDepositedEnergyFilter : public GmVFilter 
{

//-------
  public: // with description
      GmDepositedEnergyFilter(G4String name);
      // Constructor

     virtual ~GmDepositedEnergyFilter();

public: // with description
  virtual G4bool AcceptStep(const G4Step* aStep);
  virtual G4bool AcceptTrack(const G4Track* aTrack);

  void show();
  void SetParameters( std::vector<G4String>& params);
  
private:
  G4double fLowEnergy;
  G4double fHighEnergy;

  G4double theTrackEdepo;
  G4int theLastTrackID;
  G4int theLastEventID;
};

#endif
