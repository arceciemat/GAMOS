#ifndef ScoreSet__HH
#define ScoreSet__HH
#include <vector>
#include "globals.hh"

typedef std::pair<G4String,G4String> pss;
typedef std::vector<pss> vpss;
typedef std::pair<vpss*,G4bool> pvpssb;
struct ScoreInfo {
  G4String fileName;
  G4String doseName;
  G4double multFactor;
  G4double dispFactor;
  G4int iColor;
  G4double dispYFactor;
};

class ScoreSet 
{
public:
  ScoreSet(std::vector<G4String> wl, G4bool bLogXDef, G4bool bLogYDef );
  ~ScoreSet(){};

  void AddScorer(G4String fileName, G4String doseName, G4double multFactor, G4double dispFactor, G4int iColor, G4double dispYFactor = 0.);

  G4bool IsNormalized() const 
  {
    return bNormalized;
  }
  G4String GetHisFileName() const 
  {
    return theHisFileName;
  }
  std::vector<ScoreInfo> GetScoreInfos() const {
    return theScoreInfos;
  }

/*  vpss GetFileScoreNames() const 
{
    return theFileScoreNames;
    } */
  
  G4bool PlotDivided() const 
{
    return bPlotDivided;
  }
  G4bool IsLogX() const 
{
    return bLogX;
  }
  G4bool IsLogY() const 
  {
    return bLogY;
  }

private:
  std::vector<ScoreInfo> theScoreInfos;
G4bool bLogX, bLogY;
//  vpss theFileScoreNames; // vector of fileName - doseName
  G4bool bNormalized;
  G4String theHisFileName;
  G4bool bPlotDivided;
};

#endif
