#ifndef GmAODataSearch_hh
#define GmAODataSearch_hh

/*---------------------------------------------------------------------------   
ClassName:   GmAODataSearch    
Author:      P. Arce
Changes:     11/07: creation  
---------------------------------------------------------------------------*/ 
// Description  
//----------------------------------------------- 

#include <vector>
#include <map>
#include <set>
#include <fstream>

#include "globals.hh"

enum AOTreatment{ AOTPrint1, AOTPrintTogether, AOTSum, AOTMean, AOTStudenttTest };
enum AOProcessing{ AOPPrefix, AOPSuffix, AOPWordNumber, AOPNumberOfLines, AOPTreatment, AOPMessage, AOPNEvents};
enum AOttestType { AOttestFractionNEvents, AOttestFraction, AOttestValue};

class GmAOData;

//static G4int verbose;

//------------------------------------------------------------------------
class GmAODataSearch 
{ 
public:
  GmAODataSearch(const std::vector<G4String>& wl );

 public:
  ~GmAODataSearch();

  AOProcessing BuildProcessing(const G4String&) const;
  AOTreatment BuildTreatment(const G4String& str );
  G4bool CheckLine( std::vector<G4String>& wl, G4String fileName );
  void AddData( GmAOData* data ) {
    theData.push_back(data);
  }

  void ProcessData();
  void ProcessDataPrint1();
  void ProcessDataPrintTogether();
  void ProcessDataSumOrMean();
  void ProcessDataStudenttTest();

  void Print( std::ostream& out = G4cout, G4int wordn = -1);

  G4String GetType();

  AOTreatment GetTreatment() const {
    return theTreatment;
  }
  std::vector<G4int> GetWordNumbers() const {
    return theWordNumbers;
  }

  G4String GetMessage() const {
    return theMessage;
  }

  G4int GetNEvents() {
    return theNEvents;
  }

private:
  std::map<G4int,G4String> thePrefixes;
  std::map<G4int,G4String> theSuffixes;
  std::set<G4int> theNumberOfLines;
  std::vector<G4int> theWordNumbers;
  AOTreatment theTreatment;
  G4String theMessage;

  std::map<G4String,G4int> theLinesInFile; // how many times the line appeared in the file given by filename

  G4int theNEvents;
  std::vector<GmAOData*> theData;

  AOttestType theTTestType;

};

#endif
