#ifndef ScorerData__HH
#define ScorerData__HH
#include "globals.hh"
#include <vector>
#include <map>
#include <fstream>
#include "ScoreData.hh"

typedef std::pair<double,double> pdd; // value, error
typedef std::map <std::string,pdd > mspdd;  // energy - value, error
typedef std::map <std::string,mspdd* > msmspdd; 

class ScorerData 
{
public:
  ScorerData();
  ~ScorerData(){};

public:

  void AddScoreData(ScoreData*);
  ScorerData operator=( const ScorerData& sd2 );
  ScorerData operator+=( const ScorerData& sd2 );
  ScorerData & operator*=( G4double wei );
  bool operator!=( const ScorerData& sd2 );

  virtual mspdd* GetPlotData( G4bool );  

  virtual void Print( std::ofstream& ){
    G4cerr << " ERROR Print" << G4endl;
    std::exception();
  };

  ScoreData* GetScoreData(unsigned int) const;

  void SetNEvents(G4double nev);
  G4double GetNEvents() const {
    return theNEvents; 
  }

  void SetNScores(G4double evts) {
    theNScores = evts;
  }
  G4double GetNScores() const {
    return theNScores; // counts all scores except TOTAL
  }
  G4int GetScoreDataSize() const {
    return theScoreData.size(); // counts all scores
   }

  void SetName( G4String name ) {
    theName = name;
  }
  virtual G4String GetName() const{
    G4cerr << "!!! ScorerData ERROR GetName() " << G4endl;
    std::exception();
    return "";
  }

  G4int GetNumberOfFiles() const { 
    return theNumberOfFiles;
  }
  ScoreType GetScoreType() const {
    return theScoreType;
  }

  
  virtual G4String GetIndexVal( G4String, G4bool ){
    G4cerr << "!!! ERROR GetIndexVal() " << G4endl;
    std::exception();
    return "0.";
  } 

protected:
  std::vector<ScoreData*> theScoreData;
  G4String theName;
  G4double theNEvents;
  G4double theNScores; // not counting the 'TOTAL' score
  G4int theNumberOfFiles;
  ScoreType theScoreType;
};

#endif
