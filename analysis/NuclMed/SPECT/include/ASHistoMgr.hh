#ifndef ASHistoMgr_hh
#define ASHistoMgr_hh
#include <set>
#include "GamosCore/GamosAnalysis/include/GmVHistoBuilder.hh"

#include "NuclMed/SPECT/include/SPECTIOMgr.hh"

class ASHistoMgr :   public GmVHistoBuilder
{
public:
  ASHistoMgr(G4String hname);
  ~ASHistoMgr(){};
  virtual void InitializeHistoNames();

  void FillHistos( SPECTOutput& sout );
  void FillHistos( SPECTOutput& sout, G4int index );

private:
  void BookHistos( G4int index );
  G4String theFileName;
  std::vector<GmVFilter*> theFilters;
  GmVClassifier* theClassifier;
  std::set<G4int> theHistos;
};

#endif
