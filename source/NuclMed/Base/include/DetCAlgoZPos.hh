#ifndef DetCAlgoZPos_H
#define DetCAlgoZPos_H

#include "DetVComptAlgorithmVariable.hh"

class DetCAlgoZPos : public DetVComptAlgorithmVariable
{

public:
  DetCAlgoZPos();
  virtual ~DetCAlgoZPos(){};

  virtual void FillHistosInteractions( std::pair<mmits::iterator,mmits::iterator> itemmp, G4int nh );
  virtual void FillHistosRHits( const std::vector<std::pair<GmTrajStep*,GmRecHit*> >& recHitOrdered, G4int nh );

protected:
};

#endif
