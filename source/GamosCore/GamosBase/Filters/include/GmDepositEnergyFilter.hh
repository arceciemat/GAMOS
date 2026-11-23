#ifndef GmDepositEnergyFilter_h
#define GmDepositEnergyFilter_h 1

#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This filter accepts particles defined energy range.
//
///////////////////////////////////////////////////////////////////////////////

class GmDepositEnergyFilter : public GmVFilter 
{

//-------
  public: // with description
      GmDepositEnergyFilter(G4String name);
      // Constructor

     virtual ~GmDepositEnergyFilter();

public: // with description
  virtual G4bool AcceptStep(const G4Step* aStep);
  virtual G4bool AcceptTrack(const G4Track* aTrack);
  virtual G4bool AcceptStackedTrack(const G4Track* aTrack);

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
