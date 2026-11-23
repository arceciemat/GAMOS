#ifndef DetCAlgoXZPos_H
#define DetCAlgoXZPos_H

#include "DetVComptAlgorithmVariable.hh"

class DetCAlgoXZPos : public DetVComptAlgorithmVariable
{

public:
  DetCAlgoXZPos();
  virtual ~DetCAlgoXZPos(){};

  virtual void FillHistosInteractions( std::pair<mmits::iterator,mmits::iterator> itemmp, G4int nh );
  virtual void FillHistosRHits( const std::vector<std::pair<GmTrajStep*,GmRecHit*> >& recHitOrdered, G4int nh );

protected:
};

#endif
