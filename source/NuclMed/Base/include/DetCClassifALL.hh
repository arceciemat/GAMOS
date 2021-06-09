#ifndef DetCClassifALL_H
#define DetCClassifALL_H

#include "DetVComptClassificationVariable.hh"

class DetCClassifALL : public DetVComptClassificationVariable
{

public:
  DetCClassifALL();
  virtual ~DetCClassifALL();

  void BookSpecificHistos(G4int histoNumber);

  virtual int64_t GetClassification( DetRecHitSet& prhs, G4int nh = -1 );
 
private:
};

#endif
