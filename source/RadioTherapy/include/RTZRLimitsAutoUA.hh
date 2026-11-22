#ifndef __RTZRLimitsAutoUA__
#define __RTZRLimitsAutoUA__

#include "RTVZRLimitsUA.hh"

class RTZRLimitsAutoUA : public RTVZRLimitsUA
{
public:
  RTZRLimitsAutoUA();
  ~RTZRLimitsAutoUA(){}; 
 
protected:
  virtual void BuildXYLimits();

private:
  void BuildVolumesInWorld();
  void BuildXYSizes();
  void XYSizes2Limits();

protected:
  std::vector<const G4VPhysicalVolume*> theVolumesInWorld;
  std::map<G4double,XYdata> theXYsizes;

};

#endif
