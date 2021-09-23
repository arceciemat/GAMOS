#ifndef GmPDSOpticalPhotonProcess_h
#define GmPDSOpticalPhotonProcess_h

#include "GmPDSVProcess.hh"

class G4SteppingManager;
class GmPDSInteractionAngleManager;

class GmPDSOpticalPhotonProcess : 
  public GmPDSVProcess
{
public:
  
  GmPDSOpticalPhotonProcess(const G4String&, G4SteppingManager* );
  
  virtual ~GmPDSOpticalPhotonProcess();


public :

  virtual void AddHelper( GmPDSProcessHelper* helper );

};
#endif
