#ifndef GmGeneratorMgr_h
#define GmGeneratorMgr_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class GmGenerator;

#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
class G4Ions;

class GmGeneratorMgr: public G4UImessenger {
private:
  GmGeneratorMgr();

public:
  
  static GmGeneratorMgr* GetInstance();
  ~GmGeneratorMgr();
  
private:
  static GmGeneratorMgr* theInstance;
  
};

#endif
