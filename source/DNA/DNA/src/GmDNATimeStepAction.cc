//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file GmDNATimeStepAction.hh
/// \brief Implementation of the GmDNATimeStepAction class

#include "GmDNATimeStepAction.hh"
#include "GmDNAITTrackingInteractivity.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include <G4Scheduler.hh>
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Molecule.hh"
//#include "G4AnalysisManager.hh"
#include "G4DNAMolecule.hh"
#include "G4MoleculeTable.hh"
#include "G4OH.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4DNAChemistryManager.hh"
#include "G4Scheduler.hh"
#include "G4RunManager.hh"
#include "GmDNAGeometry.hh"
#include "GmDNAVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmDNATimeStepAction::GmDNATimeStepAction() 
  : G4UserTimeStepAction()
{
  GmParameterMgr* parmgr = GmParameterMgr::GetInstance();
  G4double endTime = parmgr->GetNumericValue("GmDNATimeStepAction:EndTime",1.*microsecond);
  G4int schedulerVerbose = G4int(parmgr->GetNumericValue("GmDNATimeStepAction:G4SchedulerVerbose",1));

  if(G4DNAChemistryManager::IsActivated())
    {
      G4Scheduler::Instance()->
	SetUserAction(this);
//stop at this time
      G4Scheduler::Instance()->SetEndTime(endTime);
      G4Scheduler::Instance()->SetVerbose(schedulerVerbose);
      GmDNAITTrackingInteractivity* itInteractivity = 
        new GmDNAITTrackingInteractivity();
      G4Scheduler::Instance()->SetInteractivity(itInteractivity);
      GmDNAGeometry* fpDetector = dynamic_cast<GmDNAGeometry*>(
        const_cast<G4VUserDetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction()));
      G4DNAChemistryManager::Instance()->SetGun(fpDetector->GetGun());
    }
  
  AddTimeStep(1*picosecond, 0.35*picosecond);
  AddTimeStep(10*picosecond, 1*picosecond);
  AddTimeStep(100*picosecond, 3*picosecond);
  AddTimeStep(1000*picosecond, 10*picosecond);
  AddTimeStep(10000*picosecond, 100*picosecond);

  G4String fileOutName = parmgr->GetStringValue("GmDNATimeStepAction:FileOutName","DamagingChemicalReactions.out");
  G4String suffix = parmgr->GetStringValue("GmAnalysisMgr:FileNameSuffix","");
  fileOutName += suffix;
  G4String prefix = parmgr->GetStringValue("GmAnalysisMgr:FileNamePrefix","");
  fileOutName = prefix + fileOutName;
  if(fileOutName != "" ) {
    theFileOut = new std::ofstream(fileOutName);
  } else {
    theFileOut = 0;
  }
#ifndef GAMOS_NO_VERBOSE
  if( DNAVerb(infoVerb) ) G4cout << " GmDNATimeStepAction fileOutName " << fileOutName << " " << theFileOut << G4endl; 
#endif
  
  // Define which products are considered as damage
  std::vector<G4String> damagedProductNames;
  damagedProductNames.push_back("Damaged_Deoxyribose");
  damagedProductNames = GmParameterMgr::GetInstance()->GetVStringValue("GmDNATimeStepAction:DamagedProducts",damagedProductNames);
  for( size_t ii = 0; ii < damagedProductNames.size(); ii++ ) {
    G4String damagedProdName = damagedProductNames[ii];
    if( damagedProdName.substr(0,8) != "Damaged_" )
      G4Exception("GmDNATimeStepAction::GmDNATimeStepAction",
		  "",
		  FatalException,
		  ("Damaged product has to be a damaged molecule: "+damagedProdName).c_str());
    G4ParticleDefinition* particle = GmG4Utils::GetG4Particle(damagedProdName);
    const G4MoleculeDefinition* molecule = dynamic_cast<const G4MoleculeDefinition*>(particle);
    theDamagedProducts.push_back(molecule);
#ifndef GAMOS_NO_VERBOSE
    if( DNAVerb(debugVerb) ) {
      G4cout << "@@@ GmDNATimeStepAction: adding damaged product " << damagedProdName << G4endl;
    }
#endif
  }

  // Define which radical molecules are considered as damaging
  std::vector<G4String> damagingRadicalNames;
  damagingRadicalNames.push_back("OH");
  damagingRadicalNames = GmParameterMgr::GetInstance()->GetVStringValue("GmDNATimeStepAction:DamagingRadicals",damagingRadicalNames);
  for( size_t ii = 0; ii < damagingRadicalNames.size(); ii++ ) {
    G4String damagingRadiName =  damagingRadicalNames[ii];
    G4ParticleDefinition* particle = GmG4Utils::GetG4Particle(damagingRadiName);
    const G4MoleculeDefinition* molecule = dynamic_cast<const G4MoleculeDefinition*>(particle);
    if( !molecule ) {
      G4Exception("GmDNATimeStepAction::GmDNATimeStepAction",
		  "",
		  FatalException,
		  ("Damaged product has to be a molecule: "+damagingRadiName).c_str());
    }
    theDamagingRadicals.push_back(molecule);
#ifndef GAMOS_NO_VERBOSE
    if( DNAVerb(debugVerb) ) {
      G4cout << "@@@ GmDNATimeStepAction: adding damaging radical " << damagingRadiName << G4endl;
    }
#endif
  }

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmDNATimeStepAction::~GmDNATimeStepAction()
{
  if( theFileOut ) {
    theFileOut->close();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmDNATimeStepAction::GmDNATimeStepAction(const GmDNATimeStepAction& other) 
    : G4UserTimeStepAction(other)
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmDNATimeStepAction&
GmDNATimeStepAction::operator=(const GmDNATimeStepAction& rhs)
{
    if (this == &rhs) return *this;
    return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmDNATimeStepAction::UserReactionAction(const G4Track& trackA,
                                        const G4Track& trackB,
                                        const std::vector<G4Track*>* 
                                        pProducts)
{
#ifndef GAMOS_NO_VERBOSE
  if( DNAVerb(debugVerb) ) {
    G4cout << " GmDNATimeStepAction::UserReactionAction trackA " << trackA.GetDefinition()->GetParticleName() << G4endl; 
    G4cout << " GmDNATimeStepAction::UserReactionAction trackB " << trackB.GetDefinition()->GetParticleName() << G4endl; 
    
    if( pProducts ) {
      G4cout << " GmDNATimeStepAction::UserReactionAction NPROD " << pProducts->size() << G4endl;
    } else {
      G4cout << " GmDNATimeStepAction::UserReactionAction NO PROD " << pProducts << G4endl;
    }
  }
#endif

  //check for the case "no product"
  if(!pProducts)
    {
      return;
    }
  

  const G4MoleculeDefinition* product0 = GetMolecule((*pProducts)[0])->GetDefinition();
  G4bool bDamagedProductOK = false;
  const G4MoleculeDefinition* damagedProduct = 0;
  for( size_t ii = 0; ii < theDamagedProducts.size(); ii++ ) {
    if(pProducts && (product0 == theDamagedProducts[ii]) ){
      bDamagedProductOK = true;
      damagedProduct = theDamagedProducts[ii];
      break;
    }
  }
  if( !bDamagedProductOK )
    {
      return;
    }    

  G4MoleculeDefinition* damagedProductOrigin = dynamic_cast<G4MoleculeDefinition*>(GmG4Utils::GetG4Particle(damagedProduct->GetName().substr(8,999))); // No 'Damaged_'

#ifndef GAMOS_NO_VERBOSE
  if( DNAVerb(debugVerb) ) {
    G4cout << " GmDNATimeStepAction::UserReactionAction damagedProductOrigin " << damagedProductOrigin->GetName() << G4endl;
  }
#endif

  const G4Track* DNAElement = nullptr;
  const G4Track* radical    = nullptr;
#ifndef GAMOS_NO_VERBOSE
if( DNAVerb(debugVerb) ) {
    G4cout << " GmDNATimeStepAction::UserReactionAction MOLECULE " << GetMolecule(&trackA)->GetDefinition()->GetName() << G4endl; 
  }
#endif

  if(GetMolecule(&trackA)->
     GetDefinition() == damagedProductOrigin ) //G4Deoxyribose::Definition())
    {
      DNAElement = &trackA;
      radical    = &trackB;
    }
  else 
    {
      DNAElement = &trackB;
      radical    = &trackA;
    }
  
#ifndef GAMOS_NO_VERBOSE
if( DNAVerb(debugVerb) ) {
  G4cout << " GmDNATimeStepAction::UserReactionAction radical " << radical->GetDefinition()->GetParticleName() << G4endl;
 }
#endif

  G4bool bDamagingRadicalOK = false;
  for( size_t ii = 0; ii < theDamagingRadicals.size(); ii++ ) {
    if(GetMolecule(radical)->GetDefinition() == theDamagingRadicals[ii] ){
      bDamagingRadicalOK = true;
    }
  }
  if( !bDamagingRadicalOK )
    {
      return;
    }    

  if( theFileOut ) {
    *theFileOut << DNAElement->GetPosition().getX()/nm << ",";
    *theFileOut << DNAElement->GetPosition().getY()/nm << ",";
    *theFileOut << DNAElement->GetPosition().getZ()/nm << ",";
    *theFileOut << GetMolecule(radical)->GetName() << ",";
    auto phyEventId = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
    *theFileOut << G4int(phyEventId) << G4endl;
#ifndef GAMOS_NO_VERBOSE
  if( DNAVerb(debugVerb) ) G4cout << " GmDNATimeStepAction write fileOut " <<  DNAElement->GetPosition()/nm << "," << GetMolecule(radical)->GetName() << "," << phyEventId << G4endl;
  #endif
  }
}

