#include "GmFutureWithChildrenFilter.hh"

//------------------------------------------------------------------------
GmFutureWithChildrenFilter::GmFutureWithChildrenFilter(G4String name)
: GmFutureFilter(name)
{
  bFilterWithChildren = TRUE;
}

//------------------------------------------------------------------------
GmFutureWithChildrenFilter::~GmFutureWithChildrenFilter()
{;}
