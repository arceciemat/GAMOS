#ifndef RTPhaseSpaceHistos_hh
#define RTPhaseSpaceHistos_hh

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "iaea_record.hh"
#include <map>
#include <set>
#include <vector>
class RTPhaseSpaceUA;
class EGSPhspParticle;
class GmVClassifier;
class G4Step;

class RTPhaseSpaceHistos 
{
public:
  RTPhaseSpaceHistos(RTPhaseSpaceUA* ua);
  RTPhaseSpaceHistos(G4double zstop);
  ~RTPhaseSpaceHistos(){}; 

  void DefineHistoLimits();
  void BookHistos(G4String partName, G4int index);
  void FillHistos( iaea_record_type *p_iaea_record , G4double zstop, const G4Step* step);  
  void FillHistos( iaea_record_type *p_iaea_record, G4int hid, G4int zstopID, G4int index );
  void FillHistos( EGSPhspParticle *particle, G4double zstop );
  void FillHistos( EGSPhspParticle *particle, G4int hid, G4int zstopID, G4int index = 0);
  void FillHistos( G4double posX, G4double posY, G4double posR, G4double theta, G4double phi, G4double energy, G4double dirU, G4double dirV, G4double dirW, G4double weight, G4int histoNumber );
  G4int GetHistoID(G4String partName);
  G4int GetHistoID(float charge);

  G4String GetHistoName(G4int index);
  void SetClassifier( GmVClassifier* cl ){
    theClassifier = cl; }

private:
  void Initialise();

private:
  G4int theNbins;
  G4double theHisR;
  G4double theHisAng;
  G4double theHisE;
  std::map<G4String,G4int> theParticleHistoIDs;
  GmAnalysisMgr* theAnaMgr;
  std::vector<G4double> theZstops;

  std::set<G4int> theHistos;
  GmVClassifier* theClassifier;

  G4bool bHadrons;
};

#endif
