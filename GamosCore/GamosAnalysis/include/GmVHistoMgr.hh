#ifndef GmVHistoMgr_HH
#define GmVHistoMgr_HH

#include "globals.hh"
#include <vector>
#include <set>
class GmVFilter;

class GmVHistoMgr 
{
public:
  GmVHistoMgr();

  void Initialise(const G4String& hname, const G4String& fname, std::vector<GmVFilter*>& filters );
  void BuildHistoName(const G4String& name, std::vector<GmVFilter*>& filters );
  void BuildHistoFileName(const G4String& name, std::vector<GmVFilter*>& filters );
  void BuildHistoNumber();

private:
  G4String theHistoName;
  G4String theHistoFileName;
  G4int theHistoNumber;
  static std::set<G4int> theHistoNumbers;

  GmAnalysisMgr* theAnaMgr;
};

#endif
