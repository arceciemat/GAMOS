#ifndef DetCAlgoXPos_H
#define DetCAlgoXPos_H

#include "DetVComptAlgorithmVariable.hh"

class DetCAlgoXPos : public DetVComptAlgorithmVariable
{

public:
  DetCAlgoXPos();
  virtual ~DetCAlgoXPos(){};

  virtual void FillHistosInteractions( std::pair<mmits::iterator,mmits::iterator> itemmp, G4int nh );
  virtual void FillHistosRHits( const std::vector<std::pair<GmTrajStep*,GmRecHit*> >& recHitOrdered, G4int nh );

protected:
};

#endif
