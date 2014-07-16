#ifndef GmDetUnitIDBuilderFromAncestors_HH
#define GmDetUnitIDBuilderFromAncestors_HH

#include "globals.hh"
#include "GmDetUnitIDBuilder.hh"

class GmDetUnitIDBuilderFromAncestors: public GmDetUnitIDBuilder 
{
public:
  GmDetUnitIDBuilderFromAncestors( G4String sdType );
  ~GmDetUnitIDBuilderFromAncestors();

  void SetNAncestors( G4int nance ) { theNAncestors = nance; }
  void SetNShift( G4int nshi ){ theNShift = nshi; }

public:
  virtual unsigned long long BuildID( G4Step* aStep );

private:
  G4int theNAncestors;
  G4int theNShift;

};

#endif
