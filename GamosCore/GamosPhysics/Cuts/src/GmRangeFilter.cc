#include "GmRangeFilter.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh"

//-----------------------------------------------------------
GmRangeFilter::GmRangeFilter(G4String name)
  :GmInitialRangeFilter(name)
{
}

//-----------------------------------------------------------
GmRangeFilter::~GmRangeFilter()
{;}

//-----------------------------------------------------------
G4bool GmRangeFilter::AcceptStep(const G4Step* aStep)
{
  const G4Track* aTrack = aStep->GetTrack();
  return AcceptTrack(aTrack);
}
