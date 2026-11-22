#ifndef PETVComptAlgorithmVariable_H
#define PETVComptAlgorithmVariable_H

#include "globals.hh"
#include "DetRecHitSet.hh"
#include <set>
class GmAnalysisMgr;
class GmTrajStep;
typedef std::multimap<G4int,GmTrajStep*> mmits;

class PETVComptAlgorithmVariable 
{

public:
  PETVComptAlgorithmVariable();
  virtual ~PETVComptAlgorithmVariable(){};
  void InitVariables();

  virtual void BookHistos(G4int histoNumber);

  virtual void FillHistosFromValues(std::vector<G4double>& vars, G4int nh);
  virtual void FillHistosInteractions( std::pair<mmits::iterator,mmits::iterator> itemmp, G4int nh ) = 0;
  virtual void FillHistosRHits( const std::vector<std::pair<GmTrajStep*,GmRecHit*> >& recHitOrdered, G4int nh ) = 0;

protected:
  G4String theName;
  G4double theValueMin;
  G4double theValueMax;
  GmAnalysisMgr* theAnaMgr;

  G4int theGroupingNumber;

};

#endif
