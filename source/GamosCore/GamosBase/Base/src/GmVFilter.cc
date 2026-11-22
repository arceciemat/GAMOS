#include "GmVFilter.hh"

GmVFilter::GmVFilter(G4String name)
:theFilterName(name)
{
  bFutureFilter = FALSE;
}

GmVFilter::~GmVFilter()
{;}
