#ifndef ScoreData__HH
#define ScoreData__HH

// Windows: static data used from other DLLs/programs must be dllexport/dllimport
#ifdef _WIN32
# ifdef sumScores_EXPORTS
#  define SUMSCORES_DATA_API __declspec(dllexport)
# else
#  define SUMSCORES_DATA_API __declspec(dllimport)
# endif
#else
# define SUMSCORES_DATA_API
#endif
#include "globals.hh"
#include <cstdlib>
#include <vector>

enum ScoreType{ST_GAMOS, ST_MCNP_F5, ST_MCNP_FN, ST_TEXT};
enum ScoreIndexValType{SIVT_LOWER, SIVT_MIDDLE, SIVT_UPPER};

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

  long double GetScore() const;
  long double GetScoreSq() const;
  long double GetScoreError() const;
  
  virtual void Print( std::ofstream& ){
    G4cerr << " ERROR Print" << G4endl;
    exit(1);
  };
  void SetScorerData( ScorerData* sd );
  //  void MultiplyByNEvents();

  G4String GetName() const {
    return theName;
  }
  G4double GetNEvents() const;

  G4bool IsTotal() const {
    return bTotal;
  }

  G4bool SelfCheck() const;

  SUMSCORES_DATA_API static int verbosity;

  SUMSCORES_DATA_API static ScoreIndexValType theIndexValType;

protected:
  long double theScoreDivideError;
  long double theInvErrorExp2;
  G4int theNumberOfFiles;
  long double theScore;
  long double theScoreSq;
  long double theScoreError;
  G4String theName;

  G4bool bTotal;
  ScorerData* theScorerData;

};

#endif
