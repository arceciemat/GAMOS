#ifndef GmPDSGammaProcess_h
#define GmPDSGammaProcess_h

#include "GmPDSVProcess.hh"

class G4SteppingManager;
class GmPDSInteractionAngleManager;

class GmPDSGammaProcess : 
  public GmPDSVProcess
{
public:
  
  GmPDSGammaProcess(const G4String&, G4SteppingManager* );
  
  virtual ~GmPDSGammaProcess();


public :

  virtual void AddHelper( GmPDSProcessHelper* helper );

};
#endif
