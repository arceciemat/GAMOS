#ifndef GmTrajPoint_H
#define GmTrajPoint_H

#include <vector>
#include "globals.hh"
#include "G4ThreeVector.hh"

#include "GmTrajPointProcess.hh"
#include "GmTrajPointPosMom.hh"
#include "GmTrajPointSeco.hh"

class GmTrajPoint: 
  public GmTrajPointProcess, 
  public GmTrajPointPosMom,
  public GmTrajPointSeco
{
public:
  GmTrajPoint( const G4StepPoint* );
  virtual ~GmTrajPoint();

private:
};

#endif
