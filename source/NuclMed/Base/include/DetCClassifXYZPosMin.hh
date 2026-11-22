#ifndef DetCClassifXYZPosMin_H
#define DetCClassifXYZPosMin_H

#include "DetVComptClassificationVariable.hh"

class DetCClassifXYZPosMin : public DetVComptClassificationVariable
{

public:
  DetCClassifXYZPosMin();
  virtual ~DetCClassifXYZPosMin();

  void BookSpecificHistos(G4int histoNumber);

  virtual int64_t GetClassification( DetRecHitSet& prhs, G4int nh = -1 );
 
private:
};

#endif
