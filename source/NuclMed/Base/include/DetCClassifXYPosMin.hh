#ifndef DetCClassifXYPosMin_H
#define DetCClassifXYPosMin_H

#include "DetVComptClassificationVariable.hh"

class DetCClassifXYPosMin : public DetVComptClassificationVariable
{

public:
  DetCClassifXYPosMin();
  virtual ~DetCClassifXYPosMin();

  void BookSpecificHistos(G4int histoNumber);

  virtual int64_t GetClassification( DetRecHitSet& prhs, G4int nh = -1 );
 
private:
};

#endif
