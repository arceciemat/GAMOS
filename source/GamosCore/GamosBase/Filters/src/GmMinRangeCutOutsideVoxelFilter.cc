#include "GmMinRangeCutOutsideVoxelFilter.hh"
#include "G4Track.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4RToEConvForGamma.hh"
#include "G4RToEConvForElectron.hh"
#include "G4RToEConvForPositron.hh"
#include "G4MaterialTable.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------------
GmMinRangeCutOutsideVoxelFilter::GmMinRangeCutOutsideVoxelFilter(G4String name)
  :GmVFilter(name)
{;}

//----------------------------------------------------------------------
GmMinRangeCutOutsideVoxelFilter::~GmMinRangeCutOutsideVoxelFilter()
{;}

//----------------------------------------------------------------------
G4bool GmMinRangeCutOutsideVoxelFilter::AcceptTrack(const G4Track* aTrack)
{

  theNewEventID = G4EventManager::GetEventManager()->GetNonconstCurrentEvent()->GetEventID();
  if( theNewEventID != theOldEventID ) {
    theParticleIDs.clear();
    theSecoNotCut.clear();
  }
  theOldEventID = theNewEventID;

  G4bool bAcceptTrack = FALSE;
 
  if( aTrack->GetParentID() == 0 ) return FALSE;

  G4int trkID = -1;

  //---- Check if ancestor is cut
  if( theParticleIDs.find(aTrack->GetParentID()) != theParticleIDs.end() ){
    //----- check if it is one of the secondary tracks produced before the cut
    std::map<G4int, std::set<const G4Track*> >::iterator iteist = theSecoNotCut.find(aTrack->GetParentID());
    if( iteist == theSecoNotCut.end()  // if there is no list, it is a secondary of track that is cut
	|| ((*iteist).second).find(aTrack) == ((*iteist).second).end() ) { // the track is not in the list of tracks produced before cut
      trkID = aTrack->GetParentID();
      //      G4cout << " GmMinRangeCutOutsideVoxelFilter particle parent accepted " << aTrack->GetTrackID() << G4endl;
    } else {
      //      G4cout << " GmMinRangeCutOutsideVoxelFilter particle parent not accepted " << aTrack->GetTrackID() << G4endl;
    }
  }

  if( theParticleIDs.find(aTrack->GetTrackID()) != theParticleIDs.end() ){ // another step of the same track
    trkID = aTrack->GetTrackID();
  }
  
  if( trkID != -1 ) {
    //    G4cout << " GmMinRangeCutOutsideVoxelFilter particle found , nvoxel " << aTrack->GetVolume()->GetCopyNo() << " creationVoxel " << theCreationVoxel << G4endl;
    theParticleIDs.insert(aTrack->GetTrackID()); // if it is cut or if parent is cut ( as then this one is also cut)
    if( theCreationVoxel != aTrack->GetVolume()->GetCopyNo() ) {
      //      G4cout << " GmMinRangeCutOutsideVoxelFilter accepted nvoxel " << aTrack->GetVolume()->GetCopyNo() << " creationVoxel " << theCreationVoxel << G4endl;
      bAcceptTrack = TRUE;
    }
  } else {
    //----- Finish to track all children of track cut
    theParticleIDs.clear();
    theSecoNotCut.clear();

    if( CheckTrackNumber( aTrack->GetCurrentStepNumber() ) ) { // else it has been checked and should be in theParticleIDs list
      /*      //--- Get cut 
      G4ProductionCuts* cuts = aTrack->GetVolume()->GetLogicalVolume()->GBetMaterialCufgsCouple->GetProductionCuts();
      G4double cut;
      if( aTrack->GetDefinition() == G4Gamma::Gamma() ) { 
	cut = GetProductionCut(0);
      } else if( aTrack->GetDefinition() == G4Electron::Electron() ) { 
	cut = GetProductionCut(1);
      } else if( aTrack->GetDefinition() == G4Positron::Positron() ) { 
	cut = GetProductionCut(2);
	}*/
      //----- Check if particle is below fake cut
      G4double cut = 0.;
      if( aTrack->GetDefinition() == G4Gamma::Gamma() ) { 
	cut = theGammaCuts[aTrack->GetMaterial()];
      } else if( aTrack->GetDefinition() == G4Electron::Electron() ) { 
	cut = theElectronCuts[aTrack->GetMaterial()];
      } else if( aTrack->GetDefinition() == G4Positron::Positron() ) { 
	cut = thePositronCuts[aTrack->GetMaterial()];
      }
 //      //      G4cout << "  GmMinRangeCutOutsideVoxelFilter check cut " <<  aTrack->GetVertexKineticEnergy() << " <? " << cut << G4endl;
      G4double ener = GetTrackEnergy( aTrack );
      //      if( ener!= 0. && ener < cut && CheckProcess(aTrack->GetCreatorProcess()) ) {
      if( ener!= 0. && ener < cut && CheckProcess(aTrack->GetCreatorProcess()) ) {
	//	  G4cout << " GmMinRangeCutOutsideVoxelFilter new particle below cut " << ener << " < " << cut  << G4endl;
	  theParticleIDs.insert(aTrack->GetTrackID());
	theCreationVoxel = aTrack->GetVolume()->GetCopyNo(); // this is the voxel where all the energy would be counted as deposited if the track is cut
	//----- Store the secondary tracks that are created in previous steps (those are not cut)

	//	G4TrackVector* trackSeco = aTrack->GetSecondary();
	G4SteppingManager* theSteppingManager = G4EventManager::GetEventManager()->GetTrackingManager()->GetSteppingManager();
	G4int nTrackSeco = theSteppingManager->GetfN2ndariesAtRestDoIt() + theSteppingManager->GetfN2ndariesAlongStepDoIt() + theSteppingManager->GetfN2ndariesPostStepDoIt();
	const G4TrackVector* trackSeco = theSteppingManager->GetSecondary();
	//       	G4cout << " stepseco " << nTrackSeco << " trackseco " << trackSeco->size() << G4endl;
	std::set<const G4Track*> secoNotCut;
       	for(unsigned int ii = 0; ii < trackSeco->size()-nTrackSeco; ii++ ) {
	  secoNotCut.insert( (*trackSeco)[ii] );
	}
	//       	G4cout << " stepseco " << nTrackSeco << " trackseco " << trackSeco->size() << " nSecoNotCut " << secoNotCut.size() << G4endl;
	theSecoNotCut[aTrack->GetTrackID()] = secoNotCut;

      }
    }
  }

  return bAcceptTrack;
}


//----------------------------------------------------------------------
void GmMinRangeCutOutsideVoxelFilter::SetParameters( std::vector<G4String>& params)
{
  if( params.size() != 2 && params.size() != 3 ){
    G4Exception("GmProdCutsOutsideVoxel::SetParameters","There should be two or three parameters: gamma / electron / (positron) cut ",FatalErrorInArgument,G4String("Number of parameters = "+GmGenUtils::itoa(params.size())).c_str());
  }
  
  G4double gammaCut = GmGenUtils::GetValue( params[0] );
  G4double electronCut = GmGenUtils::GetValue( params[1] );
  G4double positronCut;
  if( params.size() == 3 ) {
    positronCut = GmGenUtils::GetValue( params[2] );
  } else {
    positronCut = electronCut;
  }
  
  G4VRangeToEnergyConverter* R2EGamma = new G4RToEConvForGamma();
  R2EGamma->SetVerboseLevel(0);
  G4VRangeToEnergyConverter* R2EElectron = new G4RToEConvForElectron();
  R2EElectron->SetVerboseLevel(0);
  G4VRangeToEnergyConverter* R2EPositron = new G4RToEConvForPositron();
  R2EPositron->SetVerboseLevel(0);

  const G4MaterialTable* matTable = G4Material::GetMaterialTable();
  G4MaterialTable::const_iterator ite;
  for( ite = matTable->begin(); ite != matTable->end(); ite++ ){    
    theGammaCuts[*ite] = R2EGamma->Convert(gammaCut,*ite);
    theElectronCuts[*ite] = R2EElectron->Convert( electronCut,*ite );
    thePositronCuts[*ite] = R2EPositron->Convert( positronCut,*ite );
    G4cout << "GmMinRangeCutOutsideVolume mate " << (*ite)->GetName() << " CUT gamma: " << theGammaCuts[*ite] << " e-: " << theElectronCuts[*ite] << " e+: " << thePositronCuts[*ite] << G4endl;
    //    G4cout << " gamma cut " << gammaCut << " e- cut " << electronCut << G4endl;
  }

}


//----------------------------------------------------------------------
G4bool GmMinRangeCutOutsideVoxelFilter::CheckTrackNumber( G4int )
{
  return TRUE;
}


//----------------------------------------------------------------------
G4double GmMinRangeCutOutsideVoxelFilter::GetTrackEnergy( const G4Track* aTrack )
{
  return aTrack->GetKineticEnergy();
}


//----------------------------------------------------------------------
G4bool GmMinRangeCutOutsideVoxelFilter::CheckProcess(const G4VProcess*)
{
  return TRUE;
}
