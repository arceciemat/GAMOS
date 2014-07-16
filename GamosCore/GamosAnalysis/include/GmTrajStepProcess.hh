#ifndef GmTrajStepProcess_H
#define GmTrajStepProcess_H

#include <vector>
#include "globals.hh"
#include "GmVTrajStep.hh"
class G4Step;
class G4VProcess;

class GmTrajStepProcess : public GmVTrajStep
{
public:
  GmTrajStepProcess( const G4Step* , TSPUse pointToUse = TSPUPrePost);
  virtual ~GmTrajStepProcess(){};

  G4String GetProcessName() const { return theProcessName;}
  const G4VProcess* GetProcess() const { return theProcess;}

private:
  G4String theProcessName;
  const G4VProcess* theProcess;

};

#endif
