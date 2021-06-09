#ifndef GmPostKineticEnergyFilter_h
#define GmPostKineticEnergyFilter_h 1

#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This filter accepts particles if energy is in a defined user range. It uses PostStepPoint energy in AcceptStep
//
///////////////////////////////////////////////////////////////////////////////

class GmPostKineticEnergyFilter : public GmVFilter 
{
  
  //-------
public: // with description
  GmPostKineticEnergyFilter(G4String name);
  // Constructor
  
  virtual ~GmPostKineticEnergyFilter();
  
public: // with description
  virtual G4bool AcceptTrack(const G4Track*);
  virtual G4bool AcceptStep(const G4Step*);
  
  void show();
  void SetParameters( std::vector<G4String>& params);

private:
  G4double fLowEnergy;
  G4double fHighEnergy;

};

#endif
