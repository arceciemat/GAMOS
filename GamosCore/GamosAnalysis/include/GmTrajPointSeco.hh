#ifndef GmTrajPointSeco_H
#define GmTrajPointSeco_H

#include <vector>
#include "globals.hh"
#include "G4StepPoint.hh"

class GmTrajPointSeco
{
public:
  GmTrajPointSeco( const G4StepPoint* );
  virtual ~GmTrajPointSeco();

  G4int GetNumberOfSecondaries() const { return theNumberOfSecondaries;}
  G4double GetEnergyOfSecondaries() const { return theEnergyOfSecondaries; }

private:
  G4int theNumberOfSecondaries;
  G4double theEnergyOfSecondaries;

};

#endif
