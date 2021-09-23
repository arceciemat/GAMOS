#ifndef ScoreData__HH
#define ScoreData__HH
#include "globals.hh"
#include <vector>

enum DoseType{ST_ROOT, ST_TEXT};
enum DoseIndexValType{SIVT_LOWER, SIVT_MIDDLE, SIVT_UPPER};

class ScorerData;

class ScoreData 
{
public:
  ScoreData(){};
  ~ScoreData(){};

  ScoreData operator+=( const ScoreData& id2 );
  ScoreData & operator*=( G4double wei );
  bool operator!=( const ScoreData& id2 );
  bool operator==( const ScoreData& id2 );

  long double GetDose() const;
  long double GetDoseSq() const;
  long double GetDoseError() const;
  
  virtual void Print( std::ofstream& ){
    G4cerr << " ERROR Print" << G4endl;
    std::exception();
  };
  void SetScorerData( ScorerData* sd );
  //  void MultiplyByNEvents();

  G4String GetName() const {
    return theName;
  }
  void SetName( G4String name ) {
    theName = name;
  }
  G4double GetNEvents() const;

  G4bool IsTotal() const {
    return bTotal;
  }

  G4bool SelfCheck() const;

  static int verbosity;

  static DoseIndexValType theIndexValType;

protected:
  long double theDoseDivideError;
  long double theInvErrorExp2;
  G4int theNumberOfFiles;
  long double theDose;
  long double theDoseSq;
  long double theDoseError;
  G4String theName;

  G4bool bTotal;
  ScorerData* theScorerData;

};

#endif
