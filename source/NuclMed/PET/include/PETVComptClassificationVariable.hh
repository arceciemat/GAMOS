#ifndef PETVComptClassificationVariable_H
#define PETVComptClassificationVariable_H

#include "globals.hh"
#include "DetRecHitSet.hh"
#include <set>
class GmAnalysisMgr;

class PETVComptClassificationVariable 
{

public:
  PETVComptClassificationVariable();
  virtual ~PETVComptClassificationVariable(){};
  void InitVariables();

  virtual void BookHistos(G4int histoNumber);
  virtual void BookGeneralHistos(G4int histoNumber);
  virtual void BookSpecificHistos(G4int histoNumber) = 0;
  virtual int64_t GetClassification( DetRecHitSet& prhs, G4int nh = -1 ) = 0;
  virtual void SetCurrentClassificationAndFillHistos( DetRecHitSet& prhs, G4int nh );
  void FillHisto( G4int hn, G4double val);
  int64_t GetIndexFromValue( G4double val );

protected:
  G4String theName;
  std::vector<G4double> thes;
  G4double theMin;
  G4double theMax;
  G4double theStep;
  std::set<G4double> theIntervalLimits;
  int64_t theCurrentIndex;
  GmAnalysisMgr* theAnaMgr;
};

#endif
