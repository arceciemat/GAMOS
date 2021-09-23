#include "GmVDistribution.hh"

//---------------------------------------------------------------------
GmVDistribution::GmVDistribution()
{
  theName = "";
  theData = 0;
  G4cout << " GmVDistribution::GmVDistribution" << G4endl; //GDEB
}

//---------------------------------------------------------------------
GmVDistribution::GmVDistribution(G4String name)
:theName(name)
{
  theData = 0;
}

