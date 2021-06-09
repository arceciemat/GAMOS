#ifndef GmRegionData_h
#define GmRegionData_h

/*---------------------------------------------------------------------------   
ClassName:   GmRegionData    
Author:      P. Arce
Changes:      
---------------------------------------------------------------------------*/ 
// Description  
//----------------------------------------------- 

#include <vector>
#include "globals.hh"

//------------------------------------------------------------------------
class GmRegionData 
{ 
 public:
  GmRegionData( const std::vector<G4String>& rd );

  ~GmRegionData();

  void SetCutsData( const std::vector<G4String>& cu );

  //--- Get methods
  G4String GetRegionName() const {
    return theRegionName; }

  std::vector<G4String> GetLVNames(){
    return theLVNames; }

  G4double GetGammaCut() const {
    return theGammaCut; }
  G4double GetElectronCut() const {
    return theElectronCut; }
  G4double GetPositronCut() const {
    return thePositronCut; }
  G4bool CutsAreSet() const {
    return bCutsSet; }

private:
  G4String theRegionName;

  std::vector<G4String> theLVNames;
  G4double theGammaCut, theElectronCut, thePositronCut;
  G4bool bCutsSet;
};

#endif
