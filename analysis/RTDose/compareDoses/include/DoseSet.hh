#ifndef DoseSet__HH
#define DoseSet__HH
#include <vector>
#include "globals.hh"

typedef std::pair<G4String,G4String> pss;
typedef std::vector<pss> vpss;
typedef std::pair<vpss*,G4bool> pvpssb;
struct DoseInfo {
  G4String fileName;
  G4String doseName;
  G4double multFactor;
  G4double dispFactor;
  G4int iColor;
  G4double dispYFactor;
};

class DoseSet 
{
public:
  DoseSet(std::vector<G4String> wl, G4bool bLogXDef, G4bool bLogYDef );
  ~DoseSet(){};

  void AddDoser(G4String fileName, G4String doseName, G4double multFactor, G4double dispFactor, G4int iColor, G4double dispYFactor = 0.);

  G4bool IsNormalized() const 
  {
    return bNormalized;
  }
  G4String GetHisFileName() const 
  {
    return theHisFileName;
  }
  std::vector<DoseInfo> GetDoseInfos() const {
    return theDoseInfos;
  }

/*  vpss GetFileDoseNames() const 
{
    return theFileDoseNames;
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
  std::vector<DoseInfo> theDoseInfos;
G4bool bLogX, bLogY;
//  vpss theFileDoseNames; // vector of fileName - doseName
  G4bool bNormalized;
  G4String theHisFileName;
  G4bool bPlotDivided;
};

#endif
