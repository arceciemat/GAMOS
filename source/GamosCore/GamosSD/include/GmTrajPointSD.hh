#ifndef GmTrajPointSD_H
#define GmTrajPointSD_H

#include <vector>
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "GamosCore/GamosSD/include/GmVSD.hh"
#include "GamosCore/GamosAnalysis/include/GmTrajPoint.hh"

class GmTrajPointSD : public GmTrajPoint
{
public:
  GmTrajPointSD( const G4StepPoint* );
  virtual ~GmTrajPointSD();

  //  SDType GetSDType() const { return theSDType; }

private:
  //  SDType theSDType;

};

#endif
