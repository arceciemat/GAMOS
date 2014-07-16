#ifndef GmVHistoBuilder_HH
#define GmVHistoBuilder_HH

#include "globals.hh"
#include <vector>
#include <set>
class GmVFilter;
class GmVClassifier;
class GmAnalysisMgr;

class GmVHistoBuilder 
{
public:
  GmVHistoBuilder();

  void SetHistoNameAndNumber(const G4String& fname, std::vector<GmVFilter*>& filters, GmVClassifier* classifier );
  void BuildHistoName(const G4String& name, std::vector<GmVFilter*>& filters, GmVClassifier* classifier );
  void BuildHistoFileName(const G4String& name, std::vector<GmVFilter*>& filters, GmVClassifier* classifier );
  void BuildHistoNumber();

  void PrintHisto( G4int hn );

protected:
  G4String theHistoName;
  G4String theHistoFileName;
  G4int theHistoNumber;
  static std::set<G4int> theHistoNumbers;

  GmAnalysisMgr* theAnaMgr;

  G4bool bPrintHistoStats;
};

#endif
