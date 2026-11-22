#ifndef GmModuleMLC_h
#define GmModuleMLC_h

// Class for Radiotherapy accelerator MLC geometry module
// JI Lagares and Pedro Arce 
// 31.08.2009: Creation

#include "GmVModule.hh"

class GmModuleMLC : public GmVModule 
{ 

 public:
  GmModuleMLC( const std::vector<G4String>& par );
  ~GmModuleMLC();

  virtual void BuildObjects();

private: 

}; 

#endif
