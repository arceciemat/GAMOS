#ifndef DetCClassifYZPosMin_H
#define DetCClassifYZPosMin_H

#include "DetVComptClassificationVariable.hh"

class DetCClassifYZPosMin : public DetVComptClassificationVariable
{

public:
  DetCClassifYZPosMin();
  virtual ~DetCClassifYZPosMin();

  void BookSpecificHistos(G4int histoNumber);

  virtual int64_t GetClassification( DetRecHitSet& prhs, G4int nh = -1 );
 
private:
};

#endif
