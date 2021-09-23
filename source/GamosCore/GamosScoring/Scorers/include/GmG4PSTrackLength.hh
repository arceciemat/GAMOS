#ifndef GmG4PSTrackLength_h
#define GmG4PSTrackLength_h 1

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
#include "G4THitsMap.hh"


////////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring track length.
// 
///////////////////////////////////////////////////////////////////////////////

class GmG4PSTrackLength : public GmVPrimitiveScorer
{
 
 public: // with description
  GmG4PSTrackLength(G4String name);
  virtual ~GmG4PSTrackLength();
  
  inline void MultiplyKineticEnergy(G4bool flg=true) { multiplyKinE = flg; }
  // Multiply Kinetic Energy
  
  inline void DivideByVelocity(G4bool flg=true) { divideByVelocity = flg; }
  // Divide by velocity
  
  
protected: // with description
  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  
public: 
  virtual G4double GetUnit() const;
  virtual G4String GetUnitName() const;
  
  void SetParameters( const std::vector<G4String>& params);
  
private:
  G4bool multiplyKinE;
  G4bool divideByVelocity;
};


#endif

