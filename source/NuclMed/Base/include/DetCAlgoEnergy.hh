#ifndef DetCAlgoEnergy_H
#define DetCAlgoEnergy_H

#include "DetVComptAlgorithmVariable.hh"

class DetCAlgoEnergy : public DetVComptAlgorithmVariable
{

public:
  DetCAlgoEnergy();
  virtual ~DetCAlgoEnergy(){};

  virtual void FillHistosInteractions( std::pair<mmits::iterator,mmits::iterator> itemmp, G4int nh );
  virtual void FillHistosRHits( const std::vector<std::pair<GmTrajStep*,GmRecHit*> >& recHitOrdered, G4int nh );

protected:
};

#endif
