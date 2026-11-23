#ifndef GmVDataUserHistos_hh
#define GmVDataUserHistos_hh

#include "GamosCore/GamosAnalysis/include/GmVHistoBuilder.hh"
#include "GmVDataUser.hh"

#include <set>

class GmVDataUserHistos : public GmVHistoBuilder,
			  public GmVDataUser
{
public:
  GmVDataUserHistos();
  ~GmVDataUserHistos(){};
  virtual void EndOfRun();

protected:
  virtual void BookHistos(G4int index, GmVClassifier* theClassifier);

  std::set<G4int> theHistos;

  G4String theOutputType;
};

#endif
