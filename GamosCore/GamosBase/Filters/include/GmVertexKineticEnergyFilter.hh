#ifndef GmVertexKineticEnergyFilter_h
#define GmVertexKineticEnergyFilter_h 1

#include "globals.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"

////////////////////////////////////////////////////////////////////////////////
// class description:
//
//  This filter accepts particles if the vertex energy is in a defined user range
//
///////////////////////////////////////////////////////////////////////////////

class GmVertexKineticEnergyFilter : public GmVFilter 
{
  
  //-------
public: // with description
  GmVertexKineticEnergyFilter(G4String name);
  // Constructor
  
  virtual ~GmVertexKineticEnergyFilter();
  
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
