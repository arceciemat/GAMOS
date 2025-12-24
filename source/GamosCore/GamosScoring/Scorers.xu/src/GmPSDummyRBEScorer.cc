// GmPSDummyRBEScorer
#include "GmPSDummyRBEScorer.hh"

///////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring only terminated tracks inside
//  the cell.
//
///////////////////////////////////////////////////////////////////////////////

GmPSDummyRBEScorer::GmPSDummyRBEScorer(G4String name)
  :GmVPrimitiveScorer(name)
{
}

GmPSDummyRBEScorer::~GmPSDummyRBEScorer()
{;}

G4bool GmPSDummyRBEScorer::ProcessHits(G4Step* ,G4TouchableHistory*)
{
  return TRUE;
}

