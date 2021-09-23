#ifndef RTIonPlanBeam_H
#define RTIonPlanBeam_H
#include "RTVPlanBeam.hh"

class RTIonPlanBeam : public RTVPlanBeam
{
public: 
  RTIonPlanBeam( const G4String& fileName, const G4String cpFileName );
  ~RTIonPlanBeam(){}

  G4double GetBeamPosZ() const {
    return theBeamPosZ;
  }
  void SetBeamPosZ( G4double bpz ) {
    theBeamPosZ = bpz;
  }

private:
  virtual void ReorderControlPoints();

    G4double theBeamPosZ;

};

#endif // RTIonPlanBeam_H
