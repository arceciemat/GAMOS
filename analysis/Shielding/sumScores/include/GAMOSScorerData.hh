#ifndef GAMOSScorerData__HH
#define GAMOSScorerData__HH
#include "ScorerData.hh"

class GAMOSScorerData : public ScorerData
{
public:
  GAMOSScorerData();
  ~GAMOSScorerData(){};

public:

  GAMOSScorerData operator+=( const GAMOSScorerData& sd2 );
  bool operator!=( const GAMOSScorerData& sd2 );
  virtual void Print( std::ofstream& fout );

  void SetDetectorName(G4String name ) {
    theDetectorName = name;
  }
  G4String GetDetectorName() const {
    return theDetectorName;
  }
  G4String GetScorerName() const {
    return theScorerName;
  }
  void SetScorerName(G4String name) {
    theScorerName = name;
  }

  virtual G4String GetName() const{
    return theDetectorName + "_" + theScorerName;
  }

  virtual G4String GetIndexVal( G4String scoreName, G4bool bLogX );
  
private:
  G4String theDetectorName;
  G4String theScorerName;
};

#endif
