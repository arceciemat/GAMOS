#ifndef GmVPSRBEPhenom_h
#define GmVPSRBEPhenom_h 1

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "GmComputeDEDX.hh"
#include <map>
class G4Step;
class GmReadPhantomStMgr;

////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for scoring energy deposit.
// 
///////////////////////////////////////////////////////////////////////////////

class GmVPSRBEPhenom : public GmVPrimitiveScorer, public GmComputeDEDX
{
 
public: // with description
  GmVPSRBEPhenom(G4String name);
  virtual ~GmVPSRBEPhenom();
  
  G4double GetRBE(G4double dose, G4double LET, G4int idx ); // For EndOfEvent, it needs voxel copyNo

protected: // with description
  G4double GetRBE(G4Step* aStep );
  virtual G4double GetRBEMin( G4double LETd, G4double alphaBetaX ) = 0 ;
  virtual G4double GetRBEMax( G4double LETd, G4double alphaBetaX ) = 0;
  void SetOnlyEndOfEvent( G4bool boeoe ) {
    bOnlyEndOfEvent = boeoe;
  }

protected:
  G4bool bOnlyEndOfEvent;
  G4bool bIsOnlyAlpha;

private:
  void BuildAlphaBetaStructMap(G4String alphaBetaXFN);
  G4double GetAlphaBetaX( G4int copyNo );
  
private:
  G4double theAlphaBetaX;
  G4double theDefaultAlphaBetaX;
  std::map<G4int,G4double> theAlphaBetaStructIDMap;
  GmReadPhantomStMgr* theReadPhantomStMgr;
};
#endif
