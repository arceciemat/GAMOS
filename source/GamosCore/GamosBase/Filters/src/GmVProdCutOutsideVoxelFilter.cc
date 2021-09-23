#include "GmVProdCutOutsideVoxelFilter.hh"
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

//-----------------------------------------------------------
GmVProdCutOutsideVoxelFilter::GmVProdCutOutsideVoxelFilter(G4String name)
  :GmVFilter(name)
{;}

//-----------------------------------------------------------
GmVProdCutOutsideVoxelFilter::~GmVProdCutOutsideVoxelFilter()
{;}

//-----------------------------------------------------------
G4bool GmVProdCutOutsideVoxelFilter::AcceptTrack(const G4Track* aTrack)
{
  theNewEventID = G4EventManager::GetEventManager()->GetNonconstCurrentEvent()->GetEventID();
  if( theNewEventID != theOldEventID ) theParticleIDs.clear();
  theOldEventID = theNewEventID;

  G4bool bAcceptTrack = FALSE;
  //---- Check if ancestor is cut
  if( aTrack->GetParentID() == 0 ) return FALSE;

  G4int trkID = -1;

  if( theParticleIDs.find(aTrack->GetParentID()) != theParticleIDs.end() ){
    trkID = aTrack->GetParentID();
    theCreationVoxels[aTrack->GetTrackID()] = theCreationVoxels[trkID];
  }
  if( theParticleIDs.find(aTrack->GetTrackID()) != theParticleIDs.end() ){
    trkID = aTrack->GetTrackID();
  }
  
  if( trkID != -1 ) {
    //    G4cout << " GmVProdCutOutsideVoxelFilter particle found , nvoxel " << aTrack->GetVolume()->GetCopyNo() << " creationVoxel " << theCreationVoxels[aTrack->GetTrackID()] << G4endl;
    theParticleIDs.insert(aTrack->GetTrackID()); // if parent is cut, this one is cut also
    if( theCreationVoxels[trkID] != aTrack->GetVolume()->GetCopyNo() ) {
      //      G4cout << " GmVProdCutOutsideVoxelFilter accepted nvoxel " << aTrack->GetVolume()->GetCopyNo() << " creationVoxel " << theCreationVoxels[trkID] << G4endl;
      bAcceptTrack = TRUE;
    }
  } else {

    theParticleIDs.clear();

    if( CheckStepNumber( aTrack->GetCurrentStepNumber() ) ) { // else it has been checked and should be in theParticleIDs list
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
      //      G4cout << "  GmVProdCutOutsideVoxelFilter check cut " <<  aTrack->GetVertexKineticEnergy() << " <? " << cut << G4endl;
      G4double ener = GetTrackEnergy( aTrack );
      if( ener != 0. && ener < cut && CheckProcess(aTrack->GetCreatorProcess()) ) {
	//	G4cout << " GmVProdCutOutsideVoxelFilter new particle below cut " << aTrack->GetVertexKineticEnergy() << " < " << cut  << G4endl;
	theParticleIDs.insert(aTrack->GetTrackID());
	theCreationVoxels[aTrack->GetTrackID()] = aTrack->GetVolume()->GetCopyNo();
      }
    }
  }

  return bAcceptTrack;
}


//-----------------------------------------------------------
void GmVProdCutOutsideVoxelFilter::SetParameters( std::vector<G4String>& params)
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
    G4cout << "GmVProdCutsOutsideVolume mate " << (*ite)->GetName() << " CUT gamma: " << theGammaCuts[*ite] << " e-: " << theElectronCuts[*ite] << " e+: " << thePositronCuts[*ite] << G4endl;
    G4cout << " gamma cut " << gammaCut << " e- cut " << electronCut << G4endl;
  }

}


//-----------------------------------------------------------
G4bool GmVProdCutOutsideVoxelFilter::CheckStepNumber( G4int nstep )
{
  if( nstep == 1 ) { // else it has been checked and should be in theParticleIDs list
    return TRUE;
  } else {
    return FALSE;
  }
}


//-----------------------------------------------------------
G4double GmVProdCutOutsideVoxelFilter::GetTrackEnergy( const G4Track* aTrack )
{
  return aTrack->GetVertexKineticEnergy();
}

//-------------------------------------------------------------------------
G4bool GmVProdCutOutsideVoxelFilter::AcceptStackedTrack(const G4Track* )
{
  G4Exception(" GmVProdCutOutsideVoxelFilter::AcceptStackedTrack",
	      "",
	      FatalException,
	      "Cannot be called for a stacking action");

  return FALSE;
}
