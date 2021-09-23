#ifndef GmPSData_h
#define GmPSData_h 1

#include "GamosCore/GamosScoring/Management/include/GmVPrimitiveScorer.hh"
class GmVData;

class GmPSData : public GmVPrimitiveScorer
{
 
 public: // with description
  GmPSData(G4String name);
  virtual ~GmPSData();
    
protected: // with description
  virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
  
public: 
  virtual G4double GetUnit() const;
  virtual G4String GetUnitName() const;
  
  void SetParameters( const std::vector<G4String>& params);
  
private:
  GmVData* theData;
};


#endif

