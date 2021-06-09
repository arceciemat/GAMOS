#ifndef DetCClassifEnergyMax_H
#define DetCClassifEnergyMax_H

#include "DetVComptClassificationVariable.hh"

class DetCClassifEnergyMax : public DetVComptClassificationVariable
{

public:
  DetCClassifEnergyMax();
  virtual ~DetCClassifEnergyMax();

  void BookSpecificHistos(G4int histoNumber);

  virtual int64_t GetClassification( DetRecHitSet& prhs, G4int nh = -1 );
 
private:
};

#endif
