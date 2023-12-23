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
#include "GmDNASteppingAction.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "G4SystemOfUnits.hh"
#include "G4ITTrackHolder.hh"
#include "G4Track.hh"
#include <map>
#include "globals.hh"
#include "G4Molecule.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4DNAElastic.hh"
#include "G4DNAElectronSolvation.hh"
#include "GmDNAGeometry.hh"
#include "G4RunManager.hh"

using MapOfDelayedLists = 
std::map<double, std::map<int, G4TrackList*> >;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GmDNASteppingAction::GmDNASteppingAction()
    : GmUserSteppingAction()
{
  G4VUserDetectorConstruction* detNC = const_cast<G4VUserDetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  fpDetector = static_cast<GmDNAGeometry*>(detNC);
  
  GmParameterMgr* parmgr = GmParameterMgr::GetInstance();
  G4String fileOutName = parmgr->GetStringValue("GmDNASteppingAction:FileOutName","PhysicalInteractions.out");
  G4String suffix = parmgr->GetStringValue("GmAnalysisMgr:FileNameSuffix","");
  fileOutName += suffix;
  G4String prefix = parmgr->GetStringValue("GmAnalysisMgr:FileNamePrefix","");
  fileOutName = prefix + fileOutName;
  fout = std::ofstream(fileOutName);
 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GmDNASteppingAction::~GmDNASteppingAction()
{
  fout.close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void GmDNASteppingAction::UserSteppingAction(const G4Step* step)
{
  SetupFlags(step);
  
  //  G4cout << "GmDNASteppingAction fVolumeType " << fVolumeType << G4endl; //GDEB
  if(fVolumeType == GmDNAFabricVolumeType::physWorld)
    {
      step->GetTrack()->SetTrackStatus(fStopAndKill);
    }
  
  if(fVolumeType == GmDNAFabricVolumeType::VoxelStraight)
    {
      return;
    }
  
  G4double dE = step->GetTotalEnergyDeposit()/eV;
  const G4VProcess* pProcess = step->GetPostStepPoint()->
    GetProcessDefinedStep();
  
  //get all processes except G4DNAElectronSolvation
  if((0 > dE) || 
     (nullptr != dynamic_cast<const G4DNAElectronSolvation*>(pProcess)))
    {
      return;
    }
  
  //--- Write text file
  //     InitialPosX/1e-6 InitialPosY/1e-6 InitialPosZ/1e-6 AccumulatedEnergyDeposited/eV KineticEnergyChange/eV DNAInitialVolumeType InitialPVCopyNumber EventID TrackID ParentTrackID StepNumber Particle FinalProcess
  G4Track* aTrack = step->GetTrack();
  G4double xx=step->GetPreStepPoint()->
    GetPosition().x()/nanometer;
  G4double yy=step->GetPreStepPoint()->
    GetPosition().y()/nanometer;
  G4double zz=step->GetPreStepPoint()->
    GetPosition().z()/nanometer;
  G4double diffEKin = (step->GetPreStepPoint()->GetKineticEnergy() - step->GetPostStepPoint()->GetKineticEnergy())/eV;
  G4int copyNo = step->GetPostStepPoint()->GetTouchable()->GetReplicaNumber();
  G4int eventId = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
  
  G4int trackID = aTrack->GetTrackID();
  G4int parentID = aTrack->GetParentID();
  G4int stepNo = aTrack->GetCurrentStepNumber();
  G4String particle = aTrack->GetDefinition()->GetParticleName();
  G4String process = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
  
  fout << xx << "," << yy << "," << zz << ","
       << dE << "," << diffEKin << "," << fVolumeType << "," << copyNo << "," << eventId << "," 
       << trackID << "," << parentID << "," << stepNo << "," << particle << "," << process << G4endl;
  //  G4cout << "GmDNASteppingAction VolumeType WRITTEN " << fVolumeType << G4endl; //GDEB

  //--- Managed delayed tracks
    MapOfDelayedLists delayList = G4ITTrackHolder::Instance()->GetDelayedLists();
    for(auto& delayedmap_it : delayList)
    {       
        for (auto& trackList : delayedmap_it.second)
        {
            if (nullptr == trackList.second)
            {
                return;
            }
            G4TrackList::iterator itt = trackList.second->begin();
            G4TrackList::iterator endd = trackList.second->end();	    
            for(; itt != endd; ++itt)
            {
                G4Track* track = *itt;
		//		G4cout << "  MapOfDelayedLists " << track->GetParentID() << "!="<< step->GetTrack()->GetTrackID() 
		//     << " pos " <<  track->GetPosition() <<"!=" << step->GetPostStepPoint()->GetPosition() << G4endl; //GDEB
                if(track->GetParentID() != 
		   step->GetTrack()->GetTrackID() || 
		   track->GetPosition() !=
		   step->GetPostStepPoint()->GetPosition())
                {
		  // G4cout << " RETURN Delay track " << G4endl; //GDEB
                    return;
                }
                G4cout << " KILL Delay track " << G4endl; //GDEB
                track->SetTrackStatus(fStopAndKill);
            } 
        }
    }

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void GmDNASteppingAction::SetupFlags(const G4Step* step)
{
    fVolumeType = SetupVolumeType(step->GetPreStepPoint()->
    GetPhysicalVolume());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

GmDNAFabricVolumeType GmDNASteppingAction::SetupVolumeType(const G4VPhysicalVolume* 
                                                    pPhyVolume)
{   
    auto it = (fpDetector->GetGeoDataMap()).find(pPhyVolume); 
    
    if(it  == (fpDetector->GetGeoDataMap()).end())
    {
        G4ExceptionDescription exceptionDescription;
        exceptionDescription <<pPhyVolume->GetName()
                             <<" is wrong physical volume";
        G4Exception("GmDNASteppingAction::SetupVolumeFlag()",
                    "GmDNASteppingAction01", FatalException,
                    exceptionDescription);
    }
    
    return it->second;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
