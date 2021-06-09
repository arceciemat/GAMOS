#ifndef GmTrajPointPosMom_H
#define GmTrajPointPosMom_H

#include <vector>
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "GmVTrajPoint.hh"
class G4StepPoint;

class GmTrajPointPosMom : public GmVTrajPoint
{
public:
  GmTrajPointPosMom(){};
  GmTrajPointPosMom( const G4StepPoint* );
  virtual ~GmTrajPointPosMom();

  G4ThreeVector GetPosition() const { return thePos;}
  G4ThreeVector GetMomentum() const { return theMom; }
  G4ThreeVector GetMomentumDirection() const;
  G4double GetKineticEnergy() const { return  theKineticEnergy;}
  G4double GetTime() const { return theTime;}

private:
  G4ThreeVector thePos;
  G4ThreeVector theMom;
  G4double theKineticEnergy;
  G4double theTime;

};

std::ostream & operator<<(std::ostream & os, const GmTrajPointPosMom &);

#endif
