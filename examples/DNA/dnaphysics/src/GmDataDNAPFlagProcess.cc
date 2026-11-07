#include "GmDataDNAPFlagProcess.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Proton.hh"
#include "G4Alpha.hh"
#include "G4VProcess.hh"

#include "G4DNAGenericIonsManager.hh"

//----------------------------------------------------------------
GmDataDNAPFlagProcess::GmDataDNAPFlagProcess()
{
  bInitial = true;
  theHMin = -1.;
  theHMax = 1.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
  theExcludedTypes.insert(DTTrack);
}

//----------------------------------------------------------------
GmDataDNAPFlagProcess::~GmDataDNAPFlagProcess()
{
}


//----------------------------------------------------------------
G4double GmDataDNAPFlagProcess::GetValueFromStep( const G4Step* step, G4int )
{

  G4double flagParticle=-1.;
  G4double flagProcess=-1.;

  G4ParticleDefinition * partDef = step->GetTrack()->GetDynamicParticle()->GetDefinition();

  if (partDef == G4Gamma::GammaDefinition())
     flagParticle = 0; 

  if (partDef == G4Electron::ElectronDefinition())
     flagParticle = 1; 

  if (partDef == G4Proton::ProtonDefinition())
     flagParticle = 2; 

  if (partDef == G4Alpha::AlphaDefinition())
     flagParticle = 4; 

  G4DNAGenericIonsManager *instance = G4DNAGenericIonsManager::Instance();
  if (partDef == instance->GetIon("hydrogen"))
     flagParticle = 3; 

  if (partDef == instance->GetIon("alpha+"))
     flagParticle = 5; 

  if (partDef == instance->GetIon("helium"))
     flagParticle = 6; 

  // Alternative method (based on string comparison)
  /*
  const G4String& particleName = step->GetTrack()->GetDynamicParticle()->
      GetDefinition()->GetParticleName();
      
  if (particleName == "gamma")         flagParticle = 0;
  else if (particleName == "e-")       flagParticle = 1;
  else if (particleName == "proton")   flagParticle = 2;
  else if (particleName == "hydrogen") flagParticle = 3;
  else if (particleName == "alpha")    flagParticle = 4;
  else if (particleName == "alpha+")   flagParticle = 5;
  else if (particleName == "helium")   flagParticle = 6;
  */

  // Process identification

  // Process sub-types are listed in G4DNAPPhysicsListHelper.cc
  // or in Geant4-DNA process class implementation files (*.cc)

  G4StepPoint * preStep = step->GetPreStepPoint();
  G4StepPoint * postStep = step->GetPostStepPoint();
  G4int procID = postStep->GetProcessDefinedStep()->GetProcessSubType();

  const G4String& processName = postStep->GetProcessDefinedStep()->GetProcessName();

  if (processName=="Capture") flagProcess =1;
  // (no subType and procID exists at the moment for this process)

  else if (flagParticle == 1)

  {
    if      (procID==58) flagProcess =10;
    else if (procID==51) flagProcess =11;
    else if (procID==52) flagProcess =12;
    else if (procID==53) flagProcess =13;
    else if (procID==55) flagProcess =14;
    else if (procID==54) flagProcess =15;
    else if (procID==10) flagProcess =110;
    else if (procID==1)  flagProcess =120;
    else if (procID==2)  flagProcess =130;
  }
        
  else if (flagParticle == 2)

  {
    if      (procID==51) flagProcess =21;
    else if (procID==52) flagProcess =22;
    else if (procID==53) flagProcess =23;
    else if (procID==56) flagProcess =24;
    else if (procID==10) flagProcess =210;
    else if (procID==1)  flagProcess =220;
    else if (procID==2)  flagProcess =230;
    else if (procID==8)  flagProcess =240;
  }
        
  else if (flagParticle == 3)

  {
    if      (procID==51) flagProcess =31;
    else if (procID==52) flagProcess =32;
    else if (procID==53) flagProcess =33;
    else if (procID==57) flagProcess =35;
  }
        
  else if (flagParticle == 4)

  {
    if      (procID==51) flagProcess =41;
    else if (procID==52) flagProcess =42;
    else if (procID==53) flagProcess =43;
    else if (procID==56) flagProcess =44;
    else if (procID==10) flagProcess =410;
    else if (procID==1)  flagProcess =420;
    else if (procID==2)  flagProcess =430;
    else if (procID==8)  flagProcess =440;
  }
        
  else if (flagParticle == 5)

  {
    if      (procID==51) flagProcess =51;
    else if (procID==52) flagProcess =52;
    else if (procID==53) flagProcess =53;
    else if (procID==56) flagProcess =54;
    else if (procID==57) flagProcess =55;
    else if (procID==10) flagProcess =510;
    else if (procID==1)  flagProcess =520;
    else if (procID==2)  flagProcess =530;
    else if (procID==8)  flagProcess =540;
  }
        
  else if (flagParticle == 6)

  {
    if      (procID==51) flagProcess =61;
    else if (procID==52) flagProcess =62;
    else if (procID==53) flagProcess =63;
    else if (procID==57) flagProcess =65;

  }

  else if (processName=="GenericIon_G4DNAIonisation") flagProcess =73;
  else if (processName=="msc")                        flagProcess =710;
  else if (processName=="CoulombScat")                flagProcess =720;
  else if (processName=="ionIoni")                    flagProcess =730;
  else if (processName=="nuclearStopping")            flagProcess =740;
  // (for all GenericIons)
      
  // Alternatively, using process names

  /*
  else if (processName=="e-_G4DNAElectronSolvation")    flagProcess =10;
  else if (processName=="e-_G4DNAElastic")              flagProcess =11;
  else if (processName=="e-_G4DNAExcitation")           flagProcess =12;
  else if (processName=="e-_G4DNAIonisation")           flagProcess =13;
  else if (processName=="e-_G4DNAAttachment")           flagProcess =14;
  else if (processName=="e-_G4DNAVibExcitation")        flagProcess =15;

  else if (processName=="proton_G4DNAElastic")          flagProcess =21;
  else if (processName=="proton_G4DNAExcitation")       flagProcess =22;
  else if (processName=="proton_G4DNAIonisation")       flagProcess =23;
  else if (processName=="proton_G4DNAChargeDecrease")   flagProcess =24;

  else if (processName=="hydrogen_G4DNAElastic")        flagProcess =31;
  else if (processName=="hydrogen_G4DNAExcitation")     flagProcess =32;
  else if (processName=="hydrogen_G4DNAIonisation")     flagProcess =33;
  else if (processName=="hydrogen_G4DNAChargeIncrease") flagProcess =35;

  else if (processName=="alpha_G4DNAElastic")           flagProcess =41;
  else if (processName=="alpha_G4DNAExcitation")        flagProcess =42;
  else if (processName=="alpha_G4DNAIonisation")        flagProcess =43;
  else if (processName=="alpha_G4DNAChargeDecrease")    flagProcess =44;

  else if (processName=="alpha+_G4DNAElastic")          flagProcess =51;
  else if (processName=="alpha+_G4DNAExcitation")       flagProcess =52;
  else if (processName=="alpha+_G4DNAIonisation")       flagProcess =53;
  else if (processName=="alpha+_G4DNAChargeDecrease")   flagProcess =54;
  else if (processName=="alpha+_G4DNAChargeIncrease")   flagProcess =55;

  else if (processName=="helium_G4DNAElastic")          flagProcess =61;
  else if (processName=="helium_G4DNAExcitation")       flagProcess =62;
  else if (processName=="helium_G4DNAIonisation")       flagProcess =63;
  else if (processName=="helium_G4DNAChargeIncrease")   flagProcess =65;

  else if (processName=="GenericIon_G4DNAIonisation")   flagProcess =73;

  */

  return flagProcess;
}

