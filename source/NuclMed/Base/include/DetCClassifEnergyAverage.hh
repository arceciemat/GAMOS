#ifndef DetCClassifEnergyAverage_H
#define DetCClassifEnergyAverage_H

#include "DetVComptClassificationVariable.hh"

class DetCClassifEnergyAverage : public DetVComptClassificationVariable
{

public:
  DetCClassifEnergyAverage();
  virtual ~DetCClassifEnergyAverage();

  void BookSpecificHistos(G4int histoNumber);

  virtual int64_t GetClassification( DetRecHitSet& prhs, G4int nh = -1 );
 
private:
};

#endif
