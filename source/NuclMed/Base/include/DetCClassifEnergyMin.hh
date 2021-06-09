#ifndef DetCClassifEnergyMin_H
#define DetCClassifEnergyMin_H

#include "DetVComptClassificationVariable.hh"

class DetCClassifEnergyMin : public DetVComptClassificationVariable
{

public:
  DetCClassifEnergyMin();
  virtual ~DetCClassifEnergyMin();

  void BookSpecificHistos(G4int histoNumber);

  virtual int64_t GetClassification( DetRecHitSet& prhs, G4int nh = -1 );
 
private:
};

#endif
