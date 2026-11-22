#ifndef ScoreSet__HH
#define ScoreSet__HH
#include <vector>
#include "globals.hh"

typedef std::pair<G4String,G4String> pss;
typedef std::vector<pss> vpss;
typedef std::pair<vpss*,G4bool> pvpssb;

class ScoreSet 
{
public:
  ScoreSet(std::vector<G4String> wl);
  ~ScoreSet(){};

  void AddScorer(G4String fileName, G4String scorerName, G4double weight);

  G4String GetOutFileName() const {
    return theOutFileName;
  }
  G4String GetDetectorName() const {
    return theDetectorName;
  }
  G4String GetScorerName() const {
    return theScorerName;
  }

  vpss GetFileScorerNames() const {
    return theFileScorerNames;
  }
  std::vector<G4double> GetScoreWeights() const {
    return theScoreWeights;
  }
 
private:
  vpss theFileScorerNames; // vector of fileName - scorerName
  std::vector<G4double> theScoreWeights; 
  G4String theOutFileName;
  G4String theDetectorName;
  G4String theScorerName;
};

#endif
