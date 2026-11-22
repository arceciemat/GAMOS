#ifndef GmSDWithFilter_HH
#define GmSDWithFilter_HH

#include "GamosCore/GamosSD/include/GmSDSimple.hh"

#include <vector>

class GmVFilter;

class G4LogicalVolume;
class G4Step;
class GmDetUnitIDBuilderFromAncestors;

class GmSDWithFilter: public GmSDSimple
{

public:
  GmSDWithFilter(G4String type);
  ~GmSDWithFilter();

  virtual G4bool IsInterestingStep(G4Step*aStep);

  virtual void EndOfEvent(G4HCofThisEvent*HCE);

private:
  std::vector<GmVFilter*> theFilters;
};

#endif
