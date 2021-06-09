#ifndef GmModuleRangeModulator_h
#define GmModuleRangeModulator_h

// Class for Hadrontherapy Range Modulate geometry module
// Juan Ignacio Lagares
// 19.10.2010: Creation

#include "GmVModule.hh"

class GmModuleRangeModulator : public GmVModule 
{ 

 public:
  GmModuleRangeModulator( const std::vector<G4String>& par );
  ~GmModuleRangeModulator();

  virtual void BuildObjects();

private: 

}; 

#endif
