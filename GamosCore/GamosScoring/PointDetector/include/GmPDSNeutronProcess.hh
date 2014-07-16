#ifndef GmPDSNeutronProcess_h
#define GmPDSNeutronProcess_h

#include "GmPDSVProcess.hh"

class G4SteppingManager;
class GmPDSInteractionAngleManager;

class GmPDSNeutronProcess : 
  public GmPDSVProcess
{
public:
  
  GmPDSNeutronProcess(const G4String&, G4SteppingManager* );
  
  virtual ~GmPDSNeutronProcess();
  
public :
  virtual void AddHelper( GmPDSProcessHelper* helper );

};
#endif
