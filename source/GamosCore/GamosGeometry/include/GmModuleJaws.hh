#ifndef GmModuleJaws_h
#define GmModuleJaws_h

// Class for Radiotherapy accelerator Jaws geometry module
// Pedro Arce
// 31.08.2009: Creation

#include "GmVModule.hh"

class GmModuleJaws : public GmVModule 
{ 

 public:
  GmModuleJaws( const std::vector<G4String>& par );
  ~GmModuleJaws();

  virtual void BuildObjects();

private: 

}; 

#endif
