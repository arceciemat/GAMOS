#ifndef RTPlanBeam_H
#define RTPlanBeam_H
#include "RTVPlanBeam.hh"

class RTPlanBeam : public RTVPlanBeam
{
public: 
  RTPlanBeam( const G4String& fileName, const G4String cpFileName );
  ~RTPlanBeam(){}

private:
  virtual void ReorderControlPoints();

};

#endif // RTPlanBeam_H
