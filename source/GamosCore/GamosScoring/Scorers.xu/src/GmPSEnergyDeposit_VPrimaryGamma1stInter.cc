#include "GmPSEnergyDeposit_VPrimaryGamma1stInter.hh"
#include "G4UnitsTable.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "G4Gamma.hh"
#include "G4Event.hh"
#include "G4EmCalculator.hh"
#include "G4SteppingManager.hh"
#include "G4PVParameterised.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4RunManager.hh"
#include "G4VEmModel.hh"
#include "G4VEmProcess.hh"
#include "G4BiasingProcessInterface.hh"

///////////////////////////////////////////////////////////////////////////////
GmPSEnergyDeposit_VPrimaryGamma1stInter::GmPSEnergyDeposit_VPrimaryGamma1stInter(G4String name)
     :GmVPrimitiveScorer(name)
{
  theUnit = 1.;
  theUnitName = G4String("");
  theEmCalculator = new G4EmCalculator;
  fpSteppingManager = G4EventManager::GetEventManager()->GetTrackingManager()->GetSteppingManager();
  bCheckPrimary = true;
  bCountAllInteractions = false;
  bComputeProbAtt = false;
  bBadIfHasExited = false; // Do not count it if it enters cavity and back to chamber
  bBadIfComptCavity = false; // Do not count it if it Compton in cavity
  bBadIfRaylCavity = false; //  Do not count it if it Rayleigh in cavity
  bBadIfPhotCavity = false; //  Do not count it if it PhotoElectric in cavity
}

//----------------------------------------------------------------------
GmPSEnergyDeposit_VPrimaryGamma1stInter::~GmPSEnergyDeposit_VPrimaryGamma1stInter()
{}

//----------------------------------------------------------------------
void GmPSEnergyDeposit_VPrimaryGamma1stInter::UserSteppingAction( const G4Step* aStep )  
{
 // number of secondaries in current step
  G4Track* aTrack = aStep->GetTrack();
  if( aTrack->GetParentID() == 0 ) {
    //sum step length in chamber
    if( theChamberPVs.find(aStep->GetPreStepPoint()->GetPhysicalVolume()) != theChamberPVs.end() ) {
      if( aStep->GetTrack()->GetParentID() == 0 ) {
	if( bIsFirstInteraction ) {
	  theStepLength += aStep->GetStepLength();
	  theStepLengthTotal += aStep->GetStepLength();	
	}
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(testVerb) ) G4cout << GmVPrimitiveScorer::GetName() << " GmPSEnergyDeposit_VPrimaryGamma1stInter::UserSteppingAction  ADD theStepLength " << aStep->GetStepLength() << " -> " << theStepLength << G4endl;
#endif
      }
    }
    
    if( bBadIfHasExited ) { 
      if( theExitingState == ESExited ) {
	return;
      }
    }
    const G4VProcess* process = aStep->GetPostStepPoint()->GetProcessDefinedStep();
    if( process->GetProcessType() == fElectromagnetic ) { // compt, rayl, phot or conv Primary
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(testVerb) ) G4cout << GmVPrimitiveScorer::GetName() << " GmPSEnergyDeposit_VPrimaryGamma1stInter::UserSteppingAction Interaction in " << aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() << " bIsFirstInteraction " << bIsFirstInteraction << G4endl;
#endif
      //----- Interaction volume must be chamber volume
      if( theChamberPVs.find(aStep->GetPreStepPoint()->GetPhysicalVolume()) != theChamberPVs.end() ) {
	if( bIsFirstInteraction || bCountAllInteractions ) {
	  theGoodSteps.push_back(aStep->GetPostStepPoint()->GetPosition());
	  //  thePrimaryGammaVtxEnergy = aTrack->GetVertexKineticEnergy(); //ONLY FOR DEBUGGING
#ifndef GAMOS_NO_VERBOSE
	  if( ScoringVerb(testVerb) ) G4cout << GmVPrimitiveScorer::GetName() << " GmPSEnergyDeposit_VPrimaryGamma1stInter::UserSteppingAction  ADD theGoodSteps 1ary " << aStep->GetPostStepPoint()->GetPosition() << G4endl;
	  //	  G4cout <<" GmPSEnergyDeposit_VPrimaryGamma1stInter: bIsFirstInteraction " << bIsFirstInteraction << " bComputeProbAtt " << bComputeProbAtt << G4endl; //GDEB
#endif
	  if( bIsFirstInteraction && bComputeProbAtt ) {
	    ComputeProbAtt(aStep);
	    /*  G4VPhysicalVolume* pv = aStep->GetPreStepPoint()->GetPhysicalVolume();
	    G4VSolid* solid = pv->GetLogicalVolume()->GetSolid();
	    G4ThreeVector pos = aTrack->GetPosition();
	    const G4VTouchable* touch = aTrack->GetTouchable();
	    G4ThreeVector localPos = touch->GetHistory()->GetTopTransform().TransformPoint(aTrack->GetPosition());
	    G4ThreeVector localDir = touch->GetHistory()->GetTopTransform().TransformPoint(aStep->GetPreStepPoint()->GetMomentumDirection());
	    if( theExitingState == ESNotExited ) {
	      theDistanceToBorder = solid->DistanceToBorder( localPos, localDir );
	    } else if( theExitingState == ESExited ) {
	      theDistanceToBorder = solid->DistanceToBorder( localPos, -localDir );
	      G4cout << dto2 << " theDistanceToBorder " <<theDistanceToBorder << " ~= " << solid->DistanceToBorder( localPos ) << " lpos " << localPos << " lDir " << localDir << G4endl; //GDEB 
	      }*/
	    /* TO STUDY TYPE OF INTERACTION : theDistanceToBorder / theExitingState
	    double R2;
	    if( solid->GetEntityType() == "G4Tubs" ) {
	      R2 = 11*11;
	    } else if( solid->GetEntityType() == "G4Sphere" ) {
	      R2 = 11*11 - std::pow(11-fabs(pos.z())-5.5,2.);
	    }
	    G4double dirX = aStep->GetPreStepPoint()->GetMomentumDirection().x();
	    G4double dirY = aStep->GetPreStepPoint()->GetMomentumDirection().y();
	    G4double posX0 = pos.x();
	    G4double posY0 = pos.y();
	    G4double aa = dirX*dirX+dirY*dirY;
	    G4double bb = 2*posX0*dirX+2*posY0*dirY;
	    G4double cc = posX0*posX0+posY0*posY0-R2;
	    //   G4cout << cc << " pos " << pos << " R2= " << R2 <<  " " << posX0*posX0 << "+" << posY0*posY0<< "-" <<R2 << G4endl; //GDEB
	    G4double dto2;
	    //	    if( bb*bb-4*aa*cc < -DBL_MAX ) { //no intersection with cavity: nan
	    if( bb*bb-4*aa*cc < 0 ) { //no intersection with cavity 
	      theDistanceToBorder = DBL_MAX;
	      dto2 = DBL_MAX;
#ifndef GAMOS_NO_VERBOSE
	      if( ScoringVerb(testVerb) ) G4cout << dto2 << " theDistanceToBorder " <<theDistanceToBorder << G4endl;
#endif
	    } else {
	      if( theExitingState == ESNotExited ) {
		theDistanceToBorder = (-bb-sqrt(bb*bb-4*aa*cc))/(2*aa);
		dto2 = (-bb+sqrt(bb*bb-4*aa*cc))/(2*aa);
	      } else {
		theDistanceToBorder = -(-bb+sqrt(bb*bb-4*aa*cc))/(2*aa);
		dto2 = (-bb-sqrt(bb*bb-4*aa*cc))/(2*aa);
	      }
	      G4double dirZ = aStep->GetPreStepPoint()->GetMomentumDirection().z();
	      theDistanceToBorder *= sqrt(1+1./tan(acos(dirZ))*1./tan(acos(dirZ)));
	      
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(testVerb) ) G4cout << dto2 << " theDistanceToBorder " <<theDistanceToBorder << " aa= " << aa << " bb= " << bb << " cc= " << cc << " " << bb*bb-4*aa*cc << " corr= " << sqrt(1+1./tan(acos(dirZ))*1./tan(acos(dirZ))) <<G4endl;
#endif
	    }
	    */
	  }

	  bIsFirstInteraction = false;

	}
      }
    }
    //----- Check if primary track has previously exited chamber before first interaction
    // can be used independently by daughter classes    if( bBadIfHasExited )
    if( theExitingState == ESNotExited ) {
      if( aStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary ){
	G4VPhysicalVolume* exitPV = aStep->GetPreStepPoint()->GetPhysicalVolume();
	//t	  if( theChamberPVs.find(exitPV) != theChamberPVs.end() ) {
	if( theChamberPVs.find(exitPV) != theChamberPVs.end()
	    && aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName().find("cavity") != std::string::npos
	    && (bIsFirstInteraction || bCountAllInteractions) ) {
	  theExitingState = ESExited;
	  //	    G4cout << " CHANGE ExitingState " << G4endl; //GDEB
	}
      }
    }
  } else { // not Primary track
    // store steps of tracks from 1ary Good Step (= tracks that are GoodSecondaries)
    G4int nSecoStep = fpSteppingManager->GetfN2ndariesAtRestDoIt() + fpSteppingManager->GetfN2ndariesAlongStepDoIt() + fpSteppingManager->GetfN2ndariesPostStepDoIt();
    if( nSecoStep > 0 ) {
      //---- if thtrack is is from good gamma, set its steps as theGoodSteps
      if( theGoodSecondaries.find(aTrack->GetTrackID()) != theGoodSecondaries.end() ) {
	theGoodSteps.push_back(aStep->GetPostStepPoint()->GetPosition());
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(testVerb) ) G4cout << GmVPrimitiveScorer::GetName() << " GmPSEnergyDeposit_VPrimaryGamma1stInter::UserSteppingAction  ADD theGoodSteps 2ary " << aStep->GetPostStepPoint()->GetPosition() << G4endl;
#endif	
      }
    }
  }
}
 
//----------------------------------------------------------------------
void GmPSEnergyDeposit_VPrimaryGamma1stInter::PreUserTrackingAction(const G4Track* aTrack)
{
  //t  if( bBadIfHasExited ) {
  if( aTrack->GetParentID() == 0 ) {
    theExitingState = ESNotExited;
  }
    //  }
  if( bCheckPrimary ) {
    if( aTrack->GetParentID() == 0 ) {
      if( aTrack->GetDefinition() != G4Gamma::Gamma() ) {
	G4Exception("GmPSEnergyDeposit_VPrimaryGamma1stInter::PreUserTrackingAction",
		    "",
		    FatalException,
		    "Primary track has to be a Gamma");
      }
      if( aTrack->GetTrackID() != 1 ) {
	G4Exception("GmPSEnergyDeposit_VPrimaryGamma1stInter::PreUserTrackingAction",
		    "",
		    FatalException,
		    "Only ONE primary track is allowed");
      }
    }
    bCheckPrimary = false;
  }

  //  if( bComputeProbAtt ) { // i is only used for attenuation
  //--- Initialize step lengths (only used at ComputeProbAtt, for Primary)
  theStepLength = 0;
  theStepLengthTotal = 0;
  if( aTrack->GetParentID() == 0 ) {
    theDistanceToBorder = 0;
  }
    //  }
  
  //--- check if it is good (comes from first primary interaction)
  if( aTrack->GetParentID() != 0 
      && aTrack->GetDefinition() != G4Gamma::Gamma() ) { // secondary gammas are not counted 
    G4ThreeVector vtxPos = aTrack->GetVertexPosition();
    for( size_t ii = 0; ii < theGoodSteps.size(); ii++ ) {
      G4double dist = (vtxPos-theGoodSteps[ii]).mag();
      if( dist == 0. ) {
	theGoodSecondaries.insert(aTrack->GetTrackID());
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(testVerb) ) G4cout <<  GmVPrimitiveScorer::GetName() << " GmPSEnergyDeposit_VPrimaryGamma1stInter::PreUserTrackingAction  Dist to goodStep = 0 GOOD " << ii << " : " << dist << " = " << vtxPos << " - " << theGoodSteps[ii] << G4endl;
#endif
	break;
      }
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(testVerb) ) G4cout << " GmPSEnergyDeposit_VPrimaryGamma1stInter::PreUserTrackingAction  Dist to goodStep " << ii << " : " << dist << " = " << vtxPos << " - " << theGoodSteps[ii] << G4endl;
#endif
    }
    bIsFirstInteraction = false;
  } else { // start primary track
    bIsFirstInteraction = true;
  }
	
}


//----------------------------------------------------------------------
void GmPSEnergyDeposit_VPrimaryGamma1stInter::EndOfEventAction(const G4Event* )
{
  //  thePrimaryGammaVtxEnergy = -DBL_MAX;
  theGoodSecondaries.clear();
  theGoodSteps.clear();
  
}

//----------------------------------------------------------------------
G4bool GmPSEnergyDeposit_VPrimaryGamma1stInter::IsFromGoodGamma(const G4Track* aTrack )
{
  G4int trkid = aTrack->GetTrackID();
  if( trkid == 1 ) { // first track interacts in cavity
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << GmVPrimitiveScorer::GetName() << " GmPSEnergyDeposit_VPrimaryGamma1stInter::IsGoodGamma  track is primary gamma Interaction in cavity  BAD " << G4endl;
#endif
    return false;
  } else {
    if( theGoodSecondaries.find(trkid) != theGoodSecondaries.end() ) {
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(debugVerb) ) G4cout << GmVPrimitiveScorer::GetName() << " GmPSEnergyDeposit_VPrimaryGamma1stInter::IsGoodGamma  track is secondary  from primary gamma First Interaction " << trkid << " GOOD " << G4endl;
#endif
      return true;
    }
  }

#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << GmVPrimitiveScorer::GetName() << " GmPSEnergyDeposit_VPrimaryGamma1stInter::IsGoodGamma  track is secondary  NON from primary gamma First Interaction " << trkid << " BAD " << G4endl;
#endif
  return false;
}
									       


//----------------------------------------------------------------------
void GmPSEnergyDeposit_VPrimaryGamma1stInter::SetParameters( const std::vector<G4String>& params )
{
  /*  for( size_t ii = 0; ii < params.size(); ii++ ) {
    G4cerr << " VPrimaryGamma1stInter::SetParameters(  PARAM: " << params[ii] << G4endl; //GDEB
    } */
  if( params.size() == 0 ) {
    for( size_t ii = 0; ii < params.size(); ii++ ) {
      G4cerr << " PARAM: " << params[ii] << G4endl;
    }
    G4Exception("GmPSEnergyDeposit_VPrimaryGamma1stInter::SetParameters",
		"",
		FatalErrorInArgument,
		("There should be at least one parameter: chamber_volume_name1 chamber_volume_name2 ..., while there are "+GmGenUtils::itoa(params.size())).c_str());
  }
  G4PhysicalVolumeStore* pvs = G4PhysicalVolumeStore::GetInstance();
  std::vector<G4VPhysicalVolume*>::iterator cite;
  G4Material* mate = 0;
  for( cite = pvs->begin(); cite != pvs->end(); cite++ ) {
    for( size_t ii = 0; ii < params.size(); ii++ ) {
      if( GmGenUtils::AreWordsEquivalent(params[ii],(*cite)->GetName()) ) {
	G4VPhysicalVolume* pv = *cite;
	theChamberPVs.insert(pv);
	G4Material* mateNew = 0;
	if( !pv->IsParameterised() ) {
	  mateNew = pv->GetLogicalVolume()->GetMaterial();
	} else {
	  mateNew = ((G4PVParameterised*)pv)->GetParameterisation()->ComputeMaterial(pv->GetCopyNo(),pv);
	}
	if( mate != 0 && mate != mateNew )  {
	  G4Exception("GmPSEnergyDeposit_VPrimaryGamma1stInter::SetParameters",
		      "",
		      FatalException,
		      "All chamber volumes must have the same material");
	}
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(debugVerb) ) G4cout << " GmPSEnergyDeposit_PrimaryGamma1stInterAttenuation::SetParameters Added chamber PV " << pv->GetName() << G4endl;
#endif
      }
    }
  }

}

//----------------------------------------------------------------------
void GmPSEnergyDeposit_VPrimaryGamma1stInter::ComputeProbAtt(const G4Step* aStep)
{
  //-- Calculate theProbAtt
  G4double energyGood = aStep->GetTrack()->GetVertexKineticEnergy();  
  G4VPhysicalVolume* pv = *(theChamberPVs.begin());
  G4Material* mate;  
  if( !pv->IsParameterised() ) {
    mate = pv->GetLogicalVolume()->GetMaterial();
  } else {
    mate = ((G4PVParameterised*)pv)->GetParameterisation()->ComputeMaterial(pv->GetCopyNo(),pv);
  }
  
  G4double attLength = theEmCalculator->ComputeGammaAttenuationLength( energyGood, mate );
  G4double linAttCoeff = 1./attLength;
  //  theProbAtt = exp(linAttCoeff*theStepLength);
  theProbAtt = exp(linAttCoeff*theStepLengthTotal);// also count if bIsFirstInteraction is after entered chamber, exited and entered again
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(testVerb) ) G4cout << " GmPSEnergyDeposit_PrimaryGamma1stInterAttenuation::ComputeProbAtt  theProbAtt= " << theProbAtt << " = exp(" << linAttCoeff << "*" << theStepLength << ") EGood= " << energyGood <<  " ProbAttcm2/g " << linAttCoeff*CLHEP::cm/(mate->GetDensity()/(CLHEP::g/CLHEP::cm3)) << " density " << mate->GetDensity()/(CLHEP::g/CLHEP::cm3) << G4endl;
#endif

  /*
  ///////////////////////////
  G4Material* currentMaterial = aStep->GetPreStepPoint()->GetMaterial();
  G4double kinE =  aStep->GetPreStepPoint()->GetKineticEnergy();
  G4double aCut = 1*CLHEP::keV;
  //t  G4double aCut = std::max(0., theParameters->LowestElectronEnergy()); 
  G4double res = 0.0;
  G4ProcessVector* pvect = G4Gamma::Gamma()->GetProcessManager()->GetProcessList();
  G4cout << " GAMMA pvect " << pvect->size() << G4endl; //GDEB
  for( G4int ii = 0; ii < pvect->size(); ii++ ) {
    G4VProcess* proc = (*pvect)[ii];
    G4VEmProcess* procEm = dynamic_cast<G4VEmProcess*>(proc);
    if( !procEm ) {
      G4BiasingProcessInterface* procBias = dynamic_cast<G4BiasingProcessInterface*>(proc);
      if( procBias ) {
	procEm = dynamic_cast<G4VEmProcess*>(procBias->GetWrappedProcess());
      }
    }
    //    G4cout << " GAMMA pvect " << proc->GetProcessName() << " " << procEm << G4endl; //GDEB
    if( procEm ) {
      G4VEmModel* currentModel = procEm->SelectModelForMaterial(kinE, 0);
      G4cout << " GmPSEnergyDeposit_VPrimaryGamma1stInter: SELECTED MODEL " << currentModel << " " <<currentModel->GetName() << G4endl; //GDEB
      G4double res1 = currentModel->CrossSectionPerVolume( currentMaterial, G4Gamma::Gamma(), kinE, aCut, kinE );
      G4cout << "GmPSEnergyDeposit_VPrimaryGamma1stInter::ComputeXSPerVolume: "
	//	     << " cross(cm-1)= " << res*CLHEP::cm
	     << " cut(keV)= " << aCut/CLHEP::keV
	     << " in " << currentMaterial->GetName()
	     << " process " << proc->GetProcessName() 
	     << " E= " << kinE 
	     << " = " << res1 << G4endl;
      res += res1;
    }
  }
  */
  /*  for( double ener = 0.001; ener < 0.01; ener+=0.0005 ) {
    G4double linAttCoeff2 = 1./theEmCalculator->ComputeGammaAttenuationLength( ener, mate );
    G4cout << " Total att " << ener << " = " << linAttCoeff2*CLHEP::cm/(mate->GetDensity()/(CLHEP::g/CLHEP::cm3)) << G4endl;
    } 
  for( double ener = 0.01; ener < 0.1; ener+=0.001 ) {
    G4double linAttCoeff2 = 1./theEmCalculator->ComputeGammaAttenuationLength( ener, mate );
    G4cout << " Total att " << ener << " = " << linAttCoeff2*CLHEP::cm/(mate->GetDensity()/(CLHEP::g/CLHEP::cm3)) << G4endl; 
    } */
  /*for( double ener = 0.1; ener < 1.; ener+=0.1 ) {
    G4double linAttCoeff2 = 1./theEmCalculator->ComputeGammaAttenuationLength( ener, mate );
    G4cout << " Total att " << ener << " = " << linAttCoeff2*CLHEP::cm/(mate->GetDensity()/(CLHEP::g/CLHEP::cm3)) << G4endl;
  }
  G4double ener =  0.6617;
  G4double linAttCoeff2 = 1./theEmCalculator->ComputeGammaAttenuationLength( ener, mate );
  G4cout << " Total att " << ener << " = " << linAttCoeff2*CLHEP::cm/(mate->GetDensity()/(CLHEP::g/CLHEP::cm3)) << G4endl;
  */      
}

//----------------------------------------------------------------------
void GmPSEnergyDeposit_VPrimaryGamma1stInter::CheckProcessInCavity(const G4Step* aStep)
{
  if( aStep->GetTrack()->GetParentID() != 0 ) return;
						
  G4String procName = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
  if( bBadIfComptCavity && procName == "compt " ) {
    G4RunManager::GetRunManager()->AbortEvent();
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(testVerb) ) G4cout << " GmPSEnergyDeposit_VPrimaryGamma1stInter::CheckProcessInCavity  Abort event because Compton scattering  in Cavity " << G4endl;
#endif
  }
  if( bBadIfRaylCavity && procName == "rayl " ) {
    G4RunManager::GetRunManager()->AbortEvent();
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(testVerb) ) G4cout << " GmPSEnergyDeposit_VPrimaryGamma1stInter::CheckProcessInCavity  Abort event because Rayleigh scattering in Cavity " << G4endl;
#endif
  }
  if( bBadIfPhotCavity && procName == "phot " ) {
    G4RunManager::GetRunManager()->AbortEvent();
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(testVerb) ) G4cout << " GmPSEnergyDeposit_VPrimaryGamma1stInter::CheckProcessInCavity  Abort event because Photelectric effect in Cavity " << G4endl;
#endif
  }
}
