#ifndef GmVPSPrinter_hh
#define GmVPSPrinter_hh 1

#include <vector>
#include "globals.hh"
#include "G4THitsMap.hh"
class GmVPrimitiveScorer;
class GmVPrimitiveScorerVector;
// class description:
//
// This is the base class of the GAMOS GmVPSPrinter.
// It construct the GmVPSPrinter object passing the corresponding arguments

class GmVPSPrinter 
{
public: // with description
  GmVPSPrinter(G4String);
  virtual ~GmVPSPrinter(){ };
  
  virtual void DumpAll( G4THitsMap<G4double>* RunMap, GmVPrimitiveScorer* theScorer ) = 0;
  virtual void DumpAllVector( std::vector<G4double>*, GmVPrimitiveScorerVector* ){};

  G4String GetName() const {
    return theName; }

  virtual void SetParameters( const std::vector<G4String>& );

  void SetUnit(GmVPrimitiveScorer* theScorer );
  
protected:

  G4String theName;

  G4bool bPrintSumV2;

  G4String theUnitName;
  G4double theUnit;
  G4double theNewUnit;
  G4double theUnitRatio;
  G4double theUnitRatio2;
};

#endif
