#ifndef GmEventClassifierByGammaInteraction_H
#define GmEventClassifierByGammaInteraction_H

#include "globals.hh"
#include "GmVEventClassifier.hh"
#include <vector>
class GmTrajPoint;
class GmTrajStep;

class GmEventClassifierByGammaInteraction : public GmVEventClassifier
{
public:
  GmEventClassifierByGammaInteraction();
  virtual ~GmEventClassifierByGammaInteraction();

  int64_t Classify( const std::vector<GmTrajPoint*>& interactionList );
  int64_t Classify( const std::vector<GmTrajStep*>& interactionList );
  int64_t Classify1Interaction( const G4String& processName );

  virtual G4String GetClassificationName( int64_t ic );

private:
  void BuildProcessNames();

private:
  G4String thePhotoElecName, theComptonName, theRayleighName, theConversionName;
  G4bool bProcessNamesBuilt;
};

#endif
