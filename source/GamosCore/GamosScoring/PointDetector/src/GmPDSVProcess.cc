#define _USE_MATH_DEFINES
#include <cmath>

#include "GmPDSVProcess.hh"
#include "GmPDSInteractionAngleManager.hh"
#include "GmPDSScore.hh"
#include "GmPDSProcessHelper.hh"
#include "GmPDSDetector.hh"
#include "GmPDSUtils.hh"
#ifndef WIN32
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#endif
#include "GamosCore/GamosBase/Base/include/GmTrackInfo.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"

#include "G4Neutron.hh"
#include "G4Gamma.hh"
#include "G4OpticalPhoton.hh"
#include "G4Geantino.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SteppingManager.hh"
#include "G4ParticleTable.hh"
#include "G4VMultipleScattering.hh"

//------------------------------------------------------------------
GmPDSVProcess::GmPDSVProcess(const G4String& name, G4SteppingManager* fpSM ): G4VDiscreteProcess( name ), fpSteppingManager(fpSM)
{
   enableAtRestDoIt = true;
 
  /*  G4ParticleTable* theParticleTable = G4ParticleTable::GetParticleTable();
  G4ParticleTable::G4PTblDicIterator* theParticleIterator = theParticleTable->GetIterator();
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4ProcessVector* procVector = pmanager->GetProcessList();
    for( G4int ii = procVector->size()-1; ii >= 0; ii-- ) {
      //      G4cout << ii << " check RemoveProcess " << particle->GetParticleName() << " " << (*procVector)[ii]->GetProcessName() << " " << (*procVector)[ii]->GetProcessType() << " =? " << (*itep) << G4endl;
      //?      if( find ( itemmp.first, itemmp.second, (*procVector)[ii]->GetProcessName() ) != itemmp.second ) {
      //      if( "msc" == (*procVector)[ii]->GetProcessName() ) {
      if( static_cast<G4VMultipleScattering*>((*procVector)[ii]) ) {
	G4cout << " Found Process " << particle->GetParticleName() << " " << (*procVector)[ii]->GetProcessName() << G4endl;
	if( "msc" == (*procVector)[ii]->GetProcessName() ) {
	  G4VMultipleScattering* mscProc = (G4VMultipleScattering*)((*procVector)[ii]);
	  G4VMscModel* mscModel = mscProc->EmModel();
	  theMscModels[particle] = mscModel;
	  G4cout << " Found MSC Process " << particle->GetParticleName() << " " << (*procVector)[ii]->GetProcessName() << G4endl;
	}
      }
    }
  }
  */    

}

//------------------------------------------------------------------
GmPDSVProcess::~GmPDSVProcess()
{
}


//------------------------------------------------------------------
void GmPDSVProcess::ApplyRussianRoulette( G4Track* aTracknc, double weight )
{
  G4double threshold = 0.1;
  if( CLHEP::RandFlat::shoot() < threshold ) {
    aTracknc->SetWeight( weight/threshold );
    SetWeight( aTracknc, weight/threshold );
  } else {
    aTracknc->SetTrackStatus( fStopAndKill );
  }
}
    

//------------------------------------------------------------------
void GmPDSVProcess::AddHelper( GmPDSProcessHelper* helper  )
{
  theHelpers[helper->Get1aryType()] = helper;
  theCurrentHelper = helper;
  theClassifier = theCurrentHelper->theClassifier;
  switch( helper->Get1aryType() ) {
  case PDSNeutron:
    bClassifierOnTrack= G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmPDS:neutron:ClassifierOnTrack",1)); 
    bFiltersOnTrack= G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmPDS:neutron:FiltersOnTrack",1));
    break;
  case PDSGamma:
    bClassifierOnTrack= G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmPDS:gamma:ClassifierOnTrack",1)); 
    bFiltersOnTrack= G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmPDS:gamma:FiltersOnTrack",1)); 
    break;
#ifdef PDS_OP
  case PDSOpticalPhoton:
    bClassifierOnTrack= G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmPDS:opticalphoton:ClassifierOnTrack",1)); 
    bFiltersOnTrack= G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmPDS:opticalphoton:FiltersOnTrack",1));
    break;
#endif
  } 
} 


//------------------------------------------------------------------
G4double GmPDSVProcess::PostStepGetPhysicalInteractionLength(
                             const G4Track& ,
			     G4double ,
			     G4ForceCondition* condition)
{
  *condition = StronglyForced;

  return DBL_MAX; //previousStepSize;
}



//------------------------------------------------------------------
G4double GmPDSVProcess::GetMeanFreePath(const G4Track& ,
				   G4double  ,
				   G4ForceCondition* )
{
  //  *fc = InActivated;
  //  return 0.1;
  return DBL_MAX;
}

//------------------------------------------------------------------
G4VParticleChange* GmPDSVProcess::AtRestDoIt(const G4Track& aTrack, const G4Step& aStep)
{
  if( !aStep.GetPostStepPoint()->GetProcessDefinedStep() ) { // should not be the first process called
    aStep.GetPostStepPoint()->SetProcessDefinedStep(this);
  }
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(testVerb) ) G4cout << " GmPDSVProcess::AtRestDoIt weight " << GetWeight(&aTrack, FALSE) << " = " << aTrack.GetWeight() << " " << aTrack.GetUserInformation() << " pos " << aTrack.GetVertexPosition() << G4endl;
#endif
#endif
  return CheckSecondaries( aTrack, aStep );
  
}


//------------------------------------------------------------------
G4VParticleChange* GmPDSVProcess::PostStepDoIt( const G4Track& aTrack, const G4Step& aStep)
{
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(testVerb) ) G4cout << " GmPDSVProcess::PostStepDoIt weight " << GetWeight(&aTrack, FALSE) << " = " << aTrack.GetWeight() << " " << aTrack.GetUserInformation() << " pos " << aStep.GetPreStepPoint()->GetPosition() << " TO " << aStep.GetPostStepPoint()->GetPosition() << G4endl;
#endif
#endif
  return CheckSecondaries( aTrack, aStep );
}

//------------------------------------------------------------------
G4VParticleChange* GmPDSVProcess::CheckSecondaries( const G4Track& aTrack, const G4Step& aStep)
{
  aParticleChange.Initialize(aTrack);
  const G4ParticleDefinition* particle = aTrack.GetDefinition();

#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(testVerb) ) G4cout << " GmPDSVProcess::CheckSecondaries weight " << GetWeight(&aTrack, FALSE) << " = " << aTrack.GetWeight() << " " << aTrack.GetUserInformation() << " pos " << aTrack.GetVertexPosition() << " OK particle " << (particle == theOriginalParticle) << G4endl;
#endif
#endif
  
  //---- Check if detector has been reached
  G4int DR = -1;
  if( particle == theOriginalParticle ) {
    DR = theCurrentHelper->DetectorReached( aStep );
    if( DR != -1 ) {
      if( !aTrack.GetUserInformation() ) {
	G4Track* aTrackNC = const_cast<G4Track*>(&aTrack);
	G4Point3D point = theCurrentHelper->GetDetector(DR)->GetCentrePoint();
	GmTrackInfo* trkInfo = new GmTrackInfo("GmPDSTrackInfo");
	trkInfo->SetStringValue("ProcessName","Primary");
	trkInfo->SetThreeVectorValue("Point",point);
	trkInfo->SetBoolValue("IsOriginalParticleSplit",0);
	aTrackNC->SetUserInformation(trkInfo);
	SetWeight(aTrackNC, aTrack.GetWeight());
      }
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::PostStepDoIt Detector REACHED  Fill scores" << G4endl;
#endif
#endif
      theCurrentHelper->FillScores(aTrack, FALSE,DR);

    }

    if( theCurrentHelper->bControlHistos ) {
      FillHistoEachStep( aStep );
    }
  }
    
  if( DR == -1 ) {
    //----- Get neutron/gamma/opticalphoton secondary track 
    std::map<G4int,G4int> detIDs = theCurrentHelper->GetDetectorIDs();
    std::map<G4int,G4int>::const_iterator iteid;
    std::vector< G4Track*> secoTracks = GetSecondaryAndPrimaryTracks(&aTrack);
    std::vector< G4Track*>::const_iterator itetrk;
    
    //---- Get process defining step
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::PostStepDoIt N SECOTRACK " << secoTracks.size() << G4endl;
#endif
#endif
    G4String procDefStepName = GetProcessDefiningStepName(aStep);
    for( iteid = detIDs.begin(); iteid != detIDs.end(); iteid++ ){
      for( itetrk = secoTracks.begin(); itetrk != secoTracks.end(); itetrk++ ){
	//	  G4cout << " CHECK " << (*itetrk)->GetDefinition()->GetParticleName() << G4endl; //GDEB
	//----- check filters first
	std::vector<GmVFilter*>::const_iterator itef;
	G4bool bAccepted = true;
	for( itef = theFilters.begin(); itef != theFilters.end(); itef++ ) {
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
	  if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::PostStepDoIt checking filter " << (*itef)->GetName()  << " : " << theOriginalParticle->GetParticleName() << G4endl;
#endif
#endif
	  if( bFiltersOnTrack ) {
	    if( !(*itef)->AcceptTrack( &aTrack ) ) {
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
	      if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::PostStepDoIt track rejected by filter " << (*itef)->GetName() << G4endl;
#endif
#endif
		  bAccepted = false;
	      break;
	    }
	  } else {
	    if( !(*itef)->AcceptStep( &aStep ) ) {
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
	      if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::PostStepDoIt step rejected by filter " << (*itef)->GetName() << G4endl;
#endif
#endif
		  bAccepted = false;
	      break;
	    }
	  }
	}
	
	if( bAccepted) {
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
	  if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::PostStepDoIt accepted by all filters : " << theOriginalParticle->GetParticleName() << G4endl;
#endif
#endif
	  //--- Get the classification index
	  G4int index = -1;
	  if( theClassifier) {
	    if(bClassifierOnTrack ) {
	      index = theClassifier->GetIndexFromTrack(&aTrack); // beware which classifier can be applied, as some of these are secondary tracks
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
	      if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::PostStepDoIt track classified by " << theClassifier->GetName() << " = " << index << G4endl;
#endif
#endif
		} else {
	      index = theClassifier->GetIndexFromStep(&aStep); // beware which classifier can be applied, as some of these are secondary tracks
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
	      if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::PostStepDoIt step classified by " << theClassifier->GetName() << " = " << index << G4endl;
#endif
#endif
		}
	  }
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
	  if( ScoringVerb(debugVerb) )   G4cout <<"  CreateGeantino STEP " << secoTracks.size() << " " << aTrack.GetCurrentStepNumber () << " CreateGeantino  SECOTRK " << aStep.GetPreStepPoint()->GetKineticEnergy() << " " << aStep.GetPostStepPoint() << " " << (*itetrk)->GetKineticEnergy() << " " <<(*iteid).second << " " << procDefStepName << "  " << index << G4endl; 
#endif
#endif
	  CreateGeantino( aTrack, aStep.GetPreStepPoint()->GetKineticEnergy(), aStep.GetPostStepPoint(), (*itetrk)->GetKineticEnergy() ,(*iteid).second, procDefStepName, index );
	}
      }
      
      if( particle == theOriginalParticle ) {
	//----- Create a geantino for primary neutrons/gammas/opticalphotons at first track step 
	if( aTrack.GetParentID() == 0 
	    && aTrack.GetCurrentStepNumber() == 1 ) {
	  
	  //----- check filters first
	  std::vector<GmVFilter*>::const_iterator itef;
	  G4bool bAccepted = true;
	  for( itef = theFilters.begin(); itef != theFilters.end(); itef++ ) {
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
	    if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::PostStepDoIt checking filter " << (*itef)->GetName() << " : " << theOriginalParticle->GetParticleName() << G4endl;
#endif
#endif
	    if( bFiltersOnTrack ) {
	      if( !(*itef)->AcceptTrack( &aTrack ) ) {
		bAccepted = false; 
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
		if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::PostStepDoIt track rejected by filter " << (*itef)->GetName() << G4endl;
#endif
#endif
		break;
	      }
	    } else {
	      if( !(*itef)->AcceptStep( &aStep ) ) {
		bAccepted = false; 
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
		if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::PostStepDoIt step rejected by filter " << (*itef)->GetName() << G4endl;
#endif
#endif
		break;
	      }
	    }
	  }
	  
	  if( bAccepted) {
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
	    if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::PostStepDoIt accepted by all filters : " << theOriginalParticle->GetParticleName() << G4endl;
#endif
#endif
		//--- Get the classification index
	    G4int index = -1;
	    if( theClassifier) {
	      if(bClassifierOnTrack ) {
		index = theClassifier->GetIndexFromTrack(&aTrack); // beware which classifier can be applied, as some of these are secondary tracks
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
		if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::PostStepDoIt track classified by " << theClassifier->GetName() << " = " << index << G4endl;
#endif
#endif
		  } else {
		index = theClassifier->GetIndexFromStep(&aStep); // beware which classifier can be applied, as some of these are secondary tracks
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
		if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::PostStepDoIt step classified by " << theClassifier->GetName() << " = " << index << G4endl;
#endif
#endif
		  }
	    }
	    
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
	    if( ScoringVerb(debugVerb) ) G4cout << " CreateGeantino PRIM " << aStep.GetPreStepPoint()->GetKineticEnergy() << " " << aStep.GetPreStepPoint() << " " <<  aStep.GetPreStepPoint()->GetKineticEnergy() << " " << (*iteid).second<<  " Primary " << index << G4endl;
#endif
#endif
		CreateGeantino( aTrack, aStep.GetPreStepPoint()->GetKineticEnergy(), aStep.GetPreStepPoint(), aStep.GetPreStepPoint()->GetKineticEnergy(), (*iteid).second, "Primary", index );
	  }
	  
	}
      }
    }
  }

  
  ClearNumberOfInteractionLengthLeft();
  
  return pParticleChange;

  //  return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
}


//------------------------------------------------------------------
void GmPDSVProcess::CreateGeantino( const G4Track& aTrack, const G4double primPreEner, const G4StepPoint* stepPoint2, const G4double secoEner, const G4int detID, const G4String procDefStepName, G4int classifierIndex )
{

#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::CreateGeantino  ENERGY= " << secoEner << " AT POSITION " <<  stepPoint2->GetPosition() << G4endl;
#endif
#endif
  G4double dist = theCurrentHelper->GetDistanceToDetectorIncm(stepPoint2->GetPosition(), detID);
  G4double wei = GetWeight(&aTrack,FALSE);

  if( bUseMaximumDistance ) {
     //---- Play Russian Roulette if distance is big
    if( dist > theMaximumDistance ) {
      if( CLHEP::RandFlat::shoot() < theInvMaximumDistanceRR ) {
	wei /= theInvMaximumDistanceRR;
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::CreateGeantino big distance " << dist << " weight increased by Russian Roulette to " << wei << G4endl;
#endif
#endif
	  } else {
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::CreateGeantino big distance " << dist << " killed by Russian Roulette " << G4endl;
#endif
#endif
	return;
      }
    }
  }
  
  //----- Get angle between current track direction and line to detector point
  //--- Get point in detector
  G4Point3D point = theCurrentHelper->GetDetector(detID)->GetCentrePoint();
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::CreateGeantino  det point = " << point << " detID " << detID << G4endl;
#endif
#endif
  
  //--- Get direction to point
  G4ThreeVector trkpos  = stepPoint2->GetPosition();
  G4ThreeVector stepPtDir = point - trkpos;
  stepPtDir *= 1./stepPtDir.mag();
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::CreateGeantino  dir to point = " << stepPtDir << " trkpos " << trkpos << G4endl;
#endif
#endif

  //--- Get angle between two directions
  //t  G4ThreeVector secoTrkDir2 = aTrack.GetStep()->GetPreStepPoint()->GetMomentumDirection();
  G4ThreeVector secoTrkDir2 = aTrack.GetStep()->GetPostStepPoint()->GetMomentumDirection();
  G4ParticleDefinition* particle = aTrack.GetDefinition();
  /* G4ThreeVector secoTrkDir2 = aTrack.GetStep()->GetPreStepPoint()->GetMomentumDirection();
  // Compute multiple scattering
  std::map<G4ParticleDefinition*,G4VMscModel*>::const_iterator itemm = theMscModels.find(particle);
  if( itemm != theMscModels.end() ) {
    G4cout << " MSC OLD DIR " << secoTrkDir2;
    G4double minLen = 1.;
    (*itemm).second->ComputeTruePathLengthLimit(aTrack, minLen); // just to set couple
    secoTrkDir2 = ((*itemm).second)->SampleScattering(secoTrkDir2,DBL_MAX);
    G4cout << " NEW DIR " << secoTrkDir2 << G4endl;
    }*/

  G4double emissionCosAngle = secoTrkDir2 * stepPtDir;
  
  //--- Get probability of producing particle at this angle
  G4double probEmiss;
  //  const G4ParticleDefinition* particle = aTrack.GetDefinition();
  //t tables should be made for other particles
  //  if( particle != theOriginalParticle ) {
  //    probEmiss = 0.5;
  //  } else {
  probEmiss = theInteractionAngleManager->GetHistoValue( particle->GetParticleName() + " - " + theOriginalParticle->GetParticleName() + " : " + procDefStepName, stepPoint2->GetMaterial()->GetName(), primPreEner, emissionCosAngle );
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::CreateGeantino ener(pre) " << primPreEner  << " emissionCosAngle = " << emissionCosAngle << " prob " << probEmiss << " secoTrkDir2 "<< secoTrkDir2 << " stepPtDir " << stepPtDir << " primTrkDir " << aTrack.GetMomentumDirection() << G4endl;
#endif
#endif
  //  }
  if( probEmiss == 0 ) {
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(infoVerb) ) G4cout << "GmPDSVProcess::CreateGeantino ener(pre) " << primPreEner << " emissionCosAngle = " << emissionCosAngle << " prob " << probEmiss << G4endl;
#endif
#endif
	return;
  }

  //------ Create a geantino to be tracked until detector
  G4Track* geantino = new G4Track( new G4DynamicParticle( G4Geantino::Geantino(), stepPtDir, secoEner ), stepPoint2->GetGlobalTime(), trkpos );
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << "GmPDSVProcess::PostStepDoIt  new G4Track: dir " << stepPtDir << " pos " << trkpos << " ener " << secoEner << " time " << stepPoint2->GetGlobalTime() << G4endl;
#endif
#endif
  //  G4Track* geantino = new G4Track( new G4DynamicParticle( G4Geantino::Geantino(), stepPoint2->GetMomentumDirection(), stepPoint2->GetKineticEnergy() ), 
  
  geantino->SetGoodForTrackingFlag();
  geantino->SetTouchableHandle(stepPoint2->GetTouchableHandle());
  G4VUserTrackInformation* trkiOrig = aTrack.GetUserInformation();
  G4bool bOrigSplit = false;
  if( trkiOrig != 0 ) { 
    GmTrackInfo* gmtrkiOrig = dynamic_cast<GmTrackInfo*>( trkiOrig );
    //    if( gmtrkiOrig->BoolValueExists("IsOriginalParticleSplit") )  bOrigSplit = gmtrkiOrig->GetBoolValue("IsOriginalParticleSplit");
    if( gmtrkiOrig->GetType() == "GmPDSTrackInfo" )  bOrigSplit = gmtrkiOrig->GetBoolValue("IsOriginalParticleSplit");
  }
  GmTrackInfo* trkInfo = new GmTrackInfo("GmPDSTrackInfo");
  trkInfo->SetStringValue("ProcessName",procDefStepName);
  trkInfo->SetThreeVectorValue("Point",point);
  trkInfo->SetBoolValue("IsOriginalParticleSplit",bOrigSplit);
  trkInfo->SetThreeVectorValue("OriginalParticleDir",stepPoint2->GetMomentumDirection() );
  trkInfo->SetParticle(theOriginalParticle);
  trkInfo->SetIntValue("DetectorID", detID );
  //--- Store classification index in track info
  trkInfo->SetIntValue("ClassifierIndex",classifierIndex );
  trkInfo->SetDoubleValue("ParentWeight", GetWeight(&aTrack,FALSE) );
  
  geantino->SetUserInformation( trkInfo );
  
//  G4double dist = (point-trkpos).mag()/cm;
  SetWeight( geantino, wei * probEmiss / (2.*M_PI*sqr(dist) ) );
  //SetWeight( geantino, 1. / (2.*M_PI*pow( dist,2 ) ) );
  // SetWeight( geantino, 1. );
  
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(infoVerb) ) G4cout << "GmPDSVProcess::PostStepDoIt  probEmiss = " << probEmiss << " dist (cm) " << dist << " weight old " << wei << " weight new " << wei * probEmiss / (2.*M_PI*sqr(dist) ) << G4endl;
#endif
#endif
  aParticleChange.AddSecondary(geantino); 
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(infoVerb) ) G4cout << " n secondaries " << aParticleChange.GetNumberOfSecondaries() << G4endl;
#endif
#endif
}
 

//------------------------------------------------------------------
void GmPDSVProcess::FillHistoEachStep(const G4Step& aStep )
{
  //----- Count each step 
  G4int nh = theCurrentHelper->theHistoNumber;

  G4int index = -1;
  if( theClassifier) {
    if(bClassifierOnTrack ) {
      index = theClassifier->GetIndexFromTrack(aStep.GetTrack()); // beware which classifier can be applied, as some of these are secondary tracks
    } else {
      index = theClassifier->GetIndexFromStep(&aStep); // beware which classifier can be applied, as some of these are secondary tracks
    }
  }

  theCurrentHelper->CheckNewIndex(index);

  //----- Count also starting neutron/gamma/opticalphoton step as source (take preStep info)
  if( aStep.GetTrack()->GetCurrentStepNumber() == 1 ){
    theCurrentHelper->theAnaMgr->GetHisto1(nh+100000*index+90)->Fill(log10(aStep.GetPreStepPoint()->GetKineticEnergy()));
  }

  //----- Count each interaction if not transportation
  if( aStep.GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() != "Transportation" ){
    //    G4cout << "GmPDSVProcess::FillHistoEachStep filling histo  " << theCurrentHelper->theAnaMgr->GetHisto1(nh+0*100+90)->GetName() << " " << nh+0*100+90 << " " << log10(aStep.GetPostStepPoint()->GetKineticEnergy()) << G4endl;
    theCurrentHelper->theAnaMgr->GetHisto1(nh+100000*index+90)->Fill(log10(aStep.GetPostStepPoint()->GetKineticEnergy()));
  }
  
}


//------------------------------------------------------------------
std::vector<G4Track*> GmPDSVProcess::GetSecondaryAndPrimaryTracks( const G4Track* aTrack)
{
  std::vector<G4Track*> secos; 
  G4TrackVector secoList = GmG4Utils::GetSecondariesOfCurrentStep();
  G4TrackVector::const_iterator ite;
  for( ite = secoList.begin(); ite != secoList.end(); ite++ ){
    //    G4cout << theOriginalParticle << " NEWSECO " << (*ite)->GetDefinition()->GetParticleName() << " " << (*ite)->GetKineticEnergy() << G4endl; //GDEB
    if( (*ite)->GetDefinition() == theOriginalParticle ){
      secos.push_back(*ite);
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
     if( ScoringVerb(debugVerb) ) G4cout << " GmPDSVProcess::GetSecondaryAndPrimaryTracks  new seco " << theOriginalParticle->GetParticleName() << " ENER " <<  (*ite)->GetKineticEnergy() << G4endl;    
#endif
#endif
	}
  }

  //----- Get current track unless it is stopped (case of inelastic)
  if( aTrack->GetDefinition() == theOriginalParticle 
      && aTrack->GetTrackStatus() != fStopAndKill 
      && aTrack->GetTrackStatus() != fStopButAlive
      && aTrack->GetStep()->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() != "Transportation" ) {
    secos.push_back( const_cast<G4Track*>(aTrack) );
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << " GmPDSVProcess::GetSecondaryAndPrimaryTracks  process deviates particle " << aTrack->GetStep()->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() << G4endl;    
#endif
#endif
  }

  return secos;
}


//------------------------------------------------------------------
G4String GmPDSVProcess::GetProcessDefiningStepName(const G4Step& aStep)
{
  G4String procName;

  const G4VProcess* proc = aStep.GetPostStepPoint()->GetProcessDefinedStep();
  if( proc ) {
    procName = proc->GetProcessName();
  } else {
    G4Exception("GmPDSVNProcess::GetProcessDefiningStepName",
                "Wrong argument",
                FatalErrorInArgument,
                "Step has no process defining it");
   }
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(testVerb) ) G4cout << "GmPDSVProcess::GetProcessDefiningStepName  " << procName << G4endl;
#endif
#endif

  return procName;

}
