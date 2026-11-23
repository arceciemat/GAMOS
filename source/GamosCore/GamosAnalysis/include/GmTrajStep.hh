#ifndef GmTrajStep_H
#define GmTrajStep_H

#include "GmTrajStepPosMom.hh"
#include "GmTrajStepEner.hh"
#include "GmTrajStepProcess.hh"

class GmTrajStep: 
  public GmTrajStepPosMom,
  public GmTrajStepEner,
  public GmTrajStepProcess
{
public:
  GmTrajStep( const G4Step*, TSPUse pointToUse = TSPUPrePost );
  virtual ~GmTrajStep();

private:
};

#endif
