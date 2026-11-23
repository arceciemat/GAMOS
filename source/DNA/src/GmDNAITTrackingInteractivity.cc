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
/// \file GmDNAITTrackingInteractivity.hh
/// \brief Implementation of the GmDNAITTrackingInteractivity class

#include "GmDNAITTrackingInteractivity.hh"
#include "G4TrackingInformation.hh"
#include "G4VTrajectory.hh"
#include "G4Trajectory.hh"
#include "G4SmoothTrajectory.hh"
#include "G4RichTrajectory.hh"
#include "G4UserTrackingAction.hh"
#include "G4UserSteppingAction.hh"
#include "G4IT.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4VSteppingVerbose.hh"
#include "G4VisManager.hh"

#include "GmDNAVerbosity.hh"

class G4Trajectory_Lock
{
    friend class GmDNAITTrackingInteractivity;
    G4Trajectory_Lock()
        : fpTrajectory(0)
    {;}

    ~G4Trajectory_Lock()
    {;}

    G4VTrajectory* fpTrajectory;
};

GmDNAITTrackingInteractivity::GmDNAITTrackingInteractivity()
    : G4ITTrackingInteractivity()
    , fStoreTrajectory(0)
{
    fpUserTrackingAction.reset(
    G4EventManager::GetEventManager()->
    GetTrackingManager()->GetUserTrackingAction());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmDNAITTrackingInteractivity::~GmDNAITTrackingInteractivity()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmDNAITTrackingInteractivity::Initialize()
{
    G4TrackingManager* trackingManager = 
    G4EventManager::GetEventManager()->GetTrackingManager();
    fStoreTrajectory = trackingManager->GetStoreTrajectory();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmDNAITTrackingInteractivity::StartTracking(G4Track* track)
{
#ifndef GAMOS_NO_VERBOSE
  if( DNAVerb(debugVerb) ) {
    TrackBanner(track, "G4ITTrackingManager::StartTracking : ");
  }
#endif

#ifndef GAMOS_NO_VERBOSE
  if( DNAVerb(debugVerb) ) {
    TrackBanner(track);
  }
#endif
    if( fpUserTrackingAction != nullptr ) 
    {
        fpUserTrackingAction->PreUserTrackingAction(track);
    }

    G4TrackingInformation* trackingInfo = GetIT(track)->GetTrackingInfo();
    G4Trajectory_Lock* trajectory_lock = trackingInfo->GetTrajectory_Lock();

    if(fStoreTrajectory && (!trajectory_lock))
    {
        trajectory_lock = new G4Trajectory_Lock();
        trackingInfo->SetTrajectory_Lock(trajectory_lock);
        G4VTrajectory* trajectory = 0;
        switch (fStoreTrajectory) 
        {
            default:
            case 1: trajectory = new G4Trajectory(track); break;
            case 2: trajectory = new G4SmoothTrajectory(track); break;
            case 3: trajectory = new G4RichTrajectory(track); break;
        }
        trajectory_lock->fpTrajectory = trajectory;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void
GmDNAITTrackingInteractivity::AppendStep(G4Track* track, G4Step* step)
{
    if(fpUserSteppingAction != nullptr)
    {
        fpUserSteppingAction->UserSteppingAction(step);
    }

    if(fStoreTrajectory)
    {
        G4TrackingInformation* trackingInfo = GetIT(track)->GetTrackingInfo();
        G4Trajectory_Lock* trajectory_lock = trackingInfo->GetTrajectory_Lock();
        trajectory_lock->fpTrajectory->AppendStep(step);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmDNAITTrackingInteractivity::EndTracking(G4Track* track)
{
#ifndef GAMOS_NO_VERBOSE
  if( DNAVerb(debugVerb) ) {
    TrackBanner(track, "G4ITTrackingManager::EndTracking : ");
  }
#endif
    if( fpUserTrackingAction != nullptr ) 
    {
        fpUserTrackingAction->PostUserTrackingAction(track);
    }
    G4TrackingInformation* trackingInfo = GetIT(track)->GetTrackingInfo();
  G4Trajectory_Lock* trajectory_lock = trackingInfo->GetTrajectory_Lock();

    if(trajectory_lock)
    {
        G4VTrajectory*& trajectory = trajectory_lock->fpTrajectory;

        if(fStoreTrajectory && trajectory)
        {
#ifndef GAMOS_NO_VERBOSE
	  if( DNAVerb(debugVerb) ) {
	    trajectory->ShowTrajectory();
	  }
#endif
            G4TrackStatus istop = track->GetTrackStatus();

            if(trajectory && (istop != fStopButAlive) && 
            (istop != fSuspend))
            {
                G4Event* currentEvent = G4EventManager::GetEventManager()
                                    ->GetNonconstCurrentEvent();
                if(currentEvent != nullptr)
                {
                    G4TrajectoryContainer* trajectoryContainer = 
                    currentEvent->GetTrajectoryContainer();
                    if (!trajectoryContainer)
                    {
                        trajectoryContainer = new G4TrajectoryContainer;
                        currentEvent->
                        SetTrajectoryContainer(trajectoryContainer);
                    }
                    trajectoryContainer->insert(trajectory);
                }
                else
                {
                    fTrajectories.push_back(trajectory);
                }
            }
        }
        else if( (!fStoreTrajectory)&&trajectory ) 
        {
            delete trajectory;
            trajectory = nullptr;
        }
        delete trajectory_lock;
        trackingInfo->SetTrajectory_Lock(0);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GmDNAITTrackingInteractivity::Finalize()
{
    for (std::vector<G4VTrajectory*>::iterator it = 
    fTrajectories.begin();
    it != fTrajectories.end(); it++)
    {
        G4VisManager::GetConcreteInstance()->Draw(**it);
    }
}
