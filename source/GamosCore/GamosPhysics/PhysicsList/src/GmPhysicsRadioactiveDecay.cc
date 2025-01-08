#include "GmPhysicsRadioactiveDecay.hh"
#include "G4ProcessManager.hh"

#include "G4RadioactiveDecay.hh"
#include "G4GenericIon.hh"
#include "G4ParticleTable.hh"
#include "G4LossTableManager.hh"
#include "G4NuclearLevelData.hh"
#include "G4PhysicsListHelper.hh"
#include "G4UAtomicDeexcitation.hh"
#include "G4NuclideTable.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

//----------------------------------------------------------------------
GmPhysicsRadioactiveDecay::GmPhysicsRadioactiveDecay()
// : theRadioactiveDecay(0)
{
  G4DeexPrecoParameters* deex = G4NuclearLevelData::GetInstance()->GetParameters();
  deex->SetStoreICLevelData(true);
  deex->SetMaxLifeTime(G4NuclideTable::GetInstance()->GetThresholdOfHalfLife()
                       /std::log(2.));
  deex->SetIsomerProduction(true);
  deex->SetCorrelatedGamma(true);

}

//----------------------------------------------------------------------
GmPhysicsRadioactiveDecay::~GmPhysicsRadioactiveDecay()
{
  //  delete theRadioactiveDecay;
}
 
//----------------------------------------------------------------------
void GmPhysicsRadioactiveDecay::ConstructProcess()
{
  G4EmParameters::Instance()->SetAugerCascade(true);
  G4EmParameters::Instance()->SetDeexcitationIgnoreCut(true);

  G4LossTableManager* man = G4LossTableManager::Instance();
  G4VAtomDeexcitation* ad = man->AtomDeexcitation();
  if( ad == nullptr ) {
    ad = new G4UAtomicDeexcitation();
    man->SetAtomDeexcitation(ad);
    man->ResetParameters();
  }

  G4PhysicsListHelper::GetPhysicsListHelper()->
    RegisterProcess(new G4RadioactiveDecay(), G4GenericIon::GenericIon());

}


