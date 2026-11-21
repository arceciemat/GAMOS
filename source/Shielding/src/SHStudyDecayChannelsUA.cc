#include "G4Threading.hh"
#include "G4AutoLock.hh"
#include "SHStudyDecayChannelsUA.hh"
#include "ShieldingVerbosity.hh"
#include "SHMetastableIsotMgr.hh"
#include "SHRadDecayChainSet.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "G4Track.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"

#include "G4DecayTable.hh"
#include "G4VDecayChannel.hh"

//-----------------------------------------------------------------
SHStudyDecayChannelsUA::SHStudyDecayChannelsUA()
{
  theName = "SHStudyDecayChannelsUA";
  //--- Multithreading: particles found by the workers are added to the master instance, which prints their decay tables
  SetRunOnMaster(true);

}


//-----------------------------------------------------------------
SHStudyDecayChannelsUA::~SHStudyDecayChannelsUA() 
{
}

//------------------------------------------------------------------
void SHStudyDecayChannelsUA::BeginOfRunAction( const G4Run* )
{

}

//------------------------------------------------------------------
void SHStudyDecayChannelsUA::UserSteppingAction(const G4Step* aStep)
{
  const G4VProcess* proc = aStep->GetPostStepPoint()->GetProcessDefinedStep();
  if( proc && proc->GetProcessName() == "RadioactiveDecay") {
    std::vector<G4Track*> secos = GmG4Utils::GetSecondariesOfCurrentStep();
    G4cout << aStep->GetTrack()->GetDefinition()->GetParticleName() << " DECAY: ";
    for( size_t ii = 0; ii < secos.size(); ii++ ){
      G4cout << secos[ii]->GetDefinition()->GetParticleName() << " " << secos[ii]->GetKineticEnergy() << " ";
    }
    G4cout << G4endl;
  }
}



//-----------------------------------------------------------------
void SHStudyDecayChannelsUA::PostUserTrackingAction(const G4Track* aTrack)
{
  G4ParticleDefinition* particle = aTrack->GetDefinition();

  G4DecayTable* decayTab = GmG4Utils::FindOrBuildDecayTable(particle);
  //  G4cout << particle->GetParticleName() << " stable? " << particle->GetPDGStable() << " lifetime " << particle->GetPDGLifeTime() << G4endl; //GDEB
  if( decayTab ) {
    theParticles.insert(particle);
    //    G4cout << " decayTab " << particle->GetParticleName() << G4endl; //GDEB
    G4int nDec = decayTab->entries();
    for( G4int ii = 0; ii < nDec; ii++ ) {
      G4VDecayChannel* decayChan = decayTab->GetDecayChannel(ii);
      decayChan->DumpInfo();
    }
  } else {
    if( !particle->GetPDGStable() )  G4cout << " Unstable and NO decayTab " << particle->GetParticleName() << G4endl;
  }

}


//-----------------------------------------------------------------
void SHStudyDecayChannelsUA::EndOfRunAction(const G4Run* )
{
  if( G4Threading::IsMultithreadedApplication() && !G4Threading::IsMasterThread() ) {
    SHStudyDecayChannelsUA* master = dynamic_cast<SHStudyDecayChannelsUA*>(GmUserAction::GetMasterUserAction(GetName()));
    if( master ) {
      G4AutoLock l(&GmUserAction::GetMergeMutex());
      master->theParticles.insert( theParticles.begin(), theParticles.end() );
      theParticles.clear();
    }
    return;
  }
  std::set<G4ParticleDefinition*>::const_iterator ite;
  for( ite = theParticles.begin(); ite != theParticles.end(); ite++ ){
    G4ParticleDefinition* particle = *ite;
    G4DecayTable* decayTab = particle->GetDecayTable();
    G4cout << "%%" << theName << " DECAT_TABLE " << particle->GetParticleName() << G4endl;
    G4int nDec = decayTab->entries();
    for( G4int ii = 0; ii < nDec; ii++ ) {
      G4VDecayChannel* decayChan = decayTab->GetDecayChannel(ii);
      decayChan->DumpInfo();
    }
  }
}


