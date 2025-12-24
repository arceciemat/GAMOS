#include "GmHistosAtCavityUA.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4VProcess.hh"
#include "G4Gamma.hh"
#include "G4PhysicalVolumeStore.hh"

/* README:
Counts interactions of primary gamma's
CH_EXN: interaction in chamber without previously traversing cavity
CH_EXN: interaction in chamber with previously traversing cavity
CAV_CO: Compton interaction in cavity
CAV_RA: Rayleigh interaction in cavity

The name of the type is built adding the interactions CH_EXN:CH_EXY 

It also counts how many of these interactions have left some energy in the cavity: CH_EXN.E1
E1: particle from first gamma interaction in the chamber
E2: particle from second or later interaction in the chamber

  $$HAC:                   CH_EXN=   4742     1
    $$HAC:                   CH_EXN.E1=     101
  $$HAC:                   CH_EXY=   1649     2
    $$HAC:                   CH_EXY.E1=      14
  $$HAC:                   CAV_CO=      4     3
  $$HAC:            CH_EXN:CH_EXN=    388     11
    $$HAC:            CH_EXN:CH_EXN.E1=      12
    $$HAC:            CH_EXN:CH_EXN.E2=       1
  $$HAC:            CH_EXN:CH_EXY=    132     21
    $$HAC:            CH_EXN:CH_EXY.E1=       1
  $$HAC:            CH_EXY:CH_EXY=    136     22
    $$HAC:            CH_EXY:CH_EXY.E1=       1
  $$HAC:     CH_EXN:CH_EXN:CH_EXY=     11     211
*/

//----------------------------------------------------------------------
GmHistosAtCavityUA::GmHistosAtCavityUA()
{
  //-  theAnaMgr = GmAnalysisMgr::GetInstance("gammaSD") ;
  G4String sepa = GmParameterMgr::GetInstance()->GetStringValue("Histos:Separator",":");

  //initialize counters
  theCounters.clear();
  SetParameters();
  bCheckPrimary = true;
}


//----------------------------------------------------------------------
void GmHistosAtCavityUA::UserSteppingAction( const G4Step* aStep )  
{
 // number of secondaries in current step
  G4Track* aTrack = aStep->GetTrack();
  if( aTrack->GetParentID() == 0 ) {
    const G4VProcess* process = aStep->GetPostStepPoint()->GetProcessDefinedStep();
    if( process->GetProcessType() == fElectromagnetic ) { // compt, phot or Rayl
    //sum step length
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(testVerb) ) G4cout << " GmHistosAtCavityUA::UserSteppingAction interaction " << theExitingState << G4endl;
#endif
      if( theChamberPVs.find(aStep->GetPreStepPoint()->GetPhysicalVolume()) != theChamberPVs.end() ) {
	if( theExitingState == ESNotExited ) {
	  theClass += pow(10,theNInteractions)*1;
	} else if( theExitingState == ESExited ) {
	  theClass += pow(10,theNInteractions)*2;
	}	
	//	G4cout << " GmHistosAtCavityUA::UserSteppingAction theClass1 " << theClass << G4endl; 
	if( theNInteractions == 0 ) {
	  theGoodSteps1.push_back(aStep->GetPostStepPoint()->GetPosition());
	} else {
	  theGoodSteps2.push_back(aStep->GetPostStepPoint()->GetPosition());
	}
      } else if( theCavityPVs.find(aStep->GetPreStepPoint()->GetPhysicalVolume()) != theCavityPVs.end() ) {
	G4String processName = process->GetProcessName();
	if( processName == "compt" ) {
	  theClass += pow(10,theNInteractions)*3;
	} else if( processName == "Rayl" ) {
	  theClass += pow(10,theNInteractions)*4;
	}
	//	G4cout << " GmHistosAtCavityUA::UserSteppingAction theClass2 " << theClass << G4endl; 
      }
      theNInteractions++;
    }
    if( theExitingState == ESNotExited ) {
      if( aStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary ){
	if( theChamberPVs.find(aStep->GetPreStepPoint()->GetPhysicalVolume()) != theChamberPVs.end() &&
	    theCavityPVs.find(aStep->GetPostStepPoint()->GetPhysicalVolume()) != theCavityPVs.end() ) {
	  theExitingState = ESExited;
	}
      }
    }
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(testVerb) ) G4cout << " GmHistosAtCavityUA::UserSteppingAction theClassF " << theClass << G4endl;
#endif
  } else {
    // store steps of tracks from 1ary Good Step (= tracks that are GoodSecondaries)
    G4int nSecoStep = fpSteppingManager->GetfN2ndariesAtRestDoIt() + fpSteppingManager->GetfN2ndariesAlongStepDoIt() + fpSteppingManager->GetfN2ndariesPostStepDoIt();
    if( nSecoStep > 0 ) {
      if( theGoodSecondaries1.find(aTrack->GetTrackID()) != theGoodSecondaries1.end() ) {
	theGoodSteps1.push_back(aStep->GetPostStepPoint()->GetPosition());
	if( ScoringVerb(testVerb) ) G4cout << " GmHistosAtCavityUA::UserSteppingAction  ADD theGoodSteps 1ary " << aStep->GetPostStepPoint()->GetPosition() << G4endl;
      }
      if( theGoodSecondaries2.find(aTrack->GetTrackID()) != theGoodSecondaries2.end() ) {
	theGoodSteps2.push_back(aStep->GetPostStepPoint()->GetPosition());
	if( ScoringVerb(testVerb) ) G4cout << " GmHistosAtCavityUA::UserSteppingAction  ADD theGoodSteps 2ary " << aStep->GetPostStepPoint()->GetPosition() << G4endl;
      }
    }
  }

  //-- store energy
  G4double edep = aStep->GetTotalEnergyDeposit();
  if( edep != 0 ) {
    if( theCavityPVs.find(aStep->GetPreStepPoint()->GetPhysicalVolume()) != theCavityPVs.end() ) {
      if( theGoodSecondaries1.find(aTrack->GetTrackID()) != theGoodSecondaries1.end() ) {
	theEdepo1 += edep;	
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(testVerb) ) G4cout << " GmHistosAtCavityUA::UserSteppingAction E1 " << theEdepo1 << G4endl;
#endif
	if( theGoodSecondaries2.find(aTrack->GetTrackID()) != theGoodSecondaries2.end() ) {
	  theEdepo12 += edep;
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(testVerb) ) G4cout << " GmHistosAtCavityUA::UserSteppingAction E12 " << theEdepo12 << G4endl; 
#endif
	}
      }
      if( theGoodSecondaries2.find(aTrack->GetTrackID()) != theGoodSecondaries2.end() ) {
	theEdepo2 += edep;
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(testVerb) ) G4cout << " GmHistosAtCavityUA::UserSteppingAction E2 " << theEdepo2 << G4endl; 
#endif
      }
    }
  }

}
 
//----------------------------------------------------------------------
void GmHistosAtCavityUA::PreUserTrackingAction(const G4Track* aTrack)
{
  if( bCheckPrimary ) {
    if( aTrack->GetParentID() == 0 ) {
      if( aTrack->GetDefinition() != G4Gamma::Gamma() ) {
	G4Exception("GmHistosAtCavityUA::PreUserTrackingAction",
		    "",
		    FatalException,
		    "Primary track has to be a Gamma");
      }
      if( aTrack->GetTrackID() != 1 ) {
	G4Exception("GmHistosAtCavityUA::PreUserTrackingAction",
		    "",
		    FatalException,
		    "Only ONE primary track is allowed");
      }
    }
    bCheckPrimary = false;
  }
	
  //--- check if it is good (comes from first primary interaction)
  if( aTrack->GetParentID() != 0 ) {
    G4ThreeVector vtxPos = aTrack->GetVertexPosition();
    for( size_t ii = 0; ii < theGoodSteps1.size(); ii++ ) {
      G4double dist = (vtxPos-theGoodSteps1[ii]).mag();
      if( dist == 0 ) {
	theGoodSecondaries1.insert(aTrack->GetTrackID());
	if( ScoringVerb(testVerb) ) G4cout << " GmHistosAtCavityUA::PreUserTrackingAction  Dist to goodStep = 0 GOODSecondary1 " << ii << " : " << dist << " = " << vtxPos << " - " << theGoodSteps1[ii] << G4endl;
	break;
      }
      if( ScoringVerb(testVerb) ) G4cout << " GmHistosAtCavityUA::PreUserTrackingAction  Dist to goodStep " << ii << " : " << dist << " = " << vtxPos << " - " << theGoodSteps1[ii] << G4endl;
    }

    for( size_t ii = 0; ii < theGoodSteps2.size(); ii++ ) {
      G4double dist = (vtxPos-theGoodSteps2[ii]).mag();
      if( dist == 0 ) {
	theGoodSecondaries2.insert(aTrack->GetTrackID());
	if( ScoringVerb(testVerb) ) G4cout << " GmHistosAtCavityUA::PreUserTrackingAction  Dist to goodStep = 0 GOODSecondary2 " << ii << " : " << dist << " = " << vtxPos << " - " << theGoodSteps2[ii] << G4endl;
	break;
      }
      if( ScoringVerb(testVerb) ) G4cout << " GmHistosAtCavityUA::PreUserTrackingAction  Dist to goodStep " << ii << " : " << dist << " = " << vtxPos << " - " << theGoodSteps2[ii] << G4endl;
    }
  }
}

//----------------------------------------------------------------------
void GmHistosAtCavityUA::BeginOfEventAction( const G4Event* )
{
  theClass = 0;
  theNInteractions = 0;
  theExitingState = ESNotExited;
  theEdepo1 = 0.;
  theEdepo2 = 0.;
  theGoodSteps1.clear();
  theGoodSteps2.clear();
  theGoodSecondaries1.clear();
  theGoodSecondaries2.clear();
}

//----------------------------------------------------------------------
void GmHistosAtCavityUA::EndOfEventAction( const G4Event* )
{
  if( theClass == 0 ) return;
  //----- Classify the events
  //----- Delete repeated interactions
  size_t iord = size_t(log10(theClass));
  size_t icl = UINT_MAX;
  size_t iclold = UINT_MAX;
  size_t newClass = 0;
  size_t icn = 0;
  size_t icham = 0;
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(testVerb) ) G4cout << " GmHistosAtCavityUA::EndOfEventAction theClass " <<theClass << " : " << iord << G4endl;
#endif
  for( size_t ii = 0; ii <= iord; ii++ ) {
    icl = size_t(theClass/pow(10,ii))%10;
    if( icl <= 2 ) icham++;
    if( (icl <=2 && icham <=2) // accept first two cham class
	|| icl != iclold ) { 
      newClass += pow(10,icn)*icl;
      icn++;
    }
    //    G4cout << ii << " GmHistosAtCavityUA::EndOfEventAction( new " << newClass << " icl " << icl << " icn " << icn << " icham " << icham << " iclold " << iclold << G4endl; //GDEB
    iclold = icl;
  }
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(testVerb) ) G4cout << "  GmHistosAtCavityUA::EndOfEventAction( " << theClass << " new " << newClass << G4endl; 
#endif
  theClass = newClass;
  
  //----- Delete last interaction if it is Rayleigh
  if( size_t(newClass%10) == 4 ) {
    theClass = size_t(theClass/10);
  }
  
  theCounters[theClass]++;
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(testVerb) ) G4cout << "  GmHistosAtCavityUA::EndOfEventAction( " << theClass << " COUNTER " << theCounters[theClass] << G4endl; 
#endif
  iord = size_t(log10(theClass));
  if( theEdepo1 != 0. ) {
    theCountersE1[pow(10,iord+1)*8+theClass]++; 
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(testVerb) ) G4cout << "  GmHistosAtCavityUA::EndOfEventAction( " << pow(10,iord+1)*8+theClass << " E1=  " << theEdepo1 << G4endl; //GDEB 
#endif
  }
  if( theEdepo2 != 0. ) {
    theCountersE2[pow(10,iord+1)*9+theClass]++;
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(testVerb) ) G4cout << "  GmHistosAtCavityUA::EndOfEventAction( " << pow(10,iord+1)*9+theClass << " E2=  " << theEdepo1 << G4endl; //GDEB 
#endif
  }
  if( theEdepo1 != 0. && theEdepo2 != 0. ) {
    theCountersE12[pow(10,iord+1)*89+theClass]++;
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(testVerb) ) G4cout << "  GmHistosAtCavityUA::EndOfEventAction( " << pow(10,iord+1)*89+theClass << " E12= " << theEdepo12 << G4endl; //GDEB 
#endif
  }
  
}

//----------------------------------------------------------------------
void GmHistosAtCavityUA::EndOfRunAction( const G4Run* )
{
  std::map<size_t,G4String> theClassNames;
  theClassNames[1] = "CH_EXN";
  theClassNames[2] = "CH_EXY";
  theClassNames[3] = "CAV_CO";
  theClassNames[4] = "CAV_RA";
  
  for( std::map<size_t,size_t>::const_iterator ite = theCounters.begin(); ite != theCounters.end(); ite++ ) {
    //--- CONVERT VALUE TO STRING
    size_t cla = ite->first;
    if(cla == 0 ) continue;
    size_t iord = size_t(log10(cla));
    G4String clname = "";
    for( size_t ii = 0; ii <= iord; ii++ ) {
      size_t icl = size_t(cla/pow(10,ii))%10;
      clname += theClassNames[icl] + ":";
      //      G4cout << cla << " CLNAME " << clname << " icl " << icl << " " << theClassNames[icl]  << G4endl; //GDEB
    }
    clname = clname.substr(0,clname.size()-1);
    G4cout << " $$HAC:" << std::setw(25) << clname << "= " << std::setw(6) << theCounters[cla] << "     " << cla << G4endl;
    if( theCountersE1.find(pow(10,iord+1)*8+cla) != theCountersE1.end() ) {
      G4cout << "   $$HAC:" << std::setw(25) << clname << ".E1=  " << std::setw(6) << theCountersE1[pow(10,iord+1)*8+cla] << G4endl;
    }
    if( theCountersE2.find(pow(10,iord+1)*9+cla) != theCountersE2.end() ) {
      G4cout << "   $$HAC:" << std::setw(25) << clname << ".E2=  " << std::setw(6) << theCountersE2[pow(10,iord+1)*9+cla] << G4endl;
    }
    if( theCountersE12.find(pow(10,iord+1)*89+cla) != theCountersE12.end() ) {
      G4cout << "   $$HAC:" << std::setw(25) << clname << ".E12= " << std::setw(6) << theCountersE12[pow(10,iord+1)*89+cla] << G4endl;
    }
    
  }
}
 
//--------------------------------------------------------------------
 void GmHistosAtCavityUA::SetParameters()
{
  std::vector<G4String> paramsCh;
  paramsCh = GmParameterMgr::GetInstance()->GetVStringValue("GmHistosAtCavityUA:ChamberVolumes",paramsCh);
  if( paramsCh.size() == 0 ) {
    G4Exception("GmPSCavityKwallOld::SetParameters",
		"",
		FatalErrorInArgument,
		"There should be at least one parameter: chamber_volume_names, please use /gamos/setParam GmHistosAtCavityUA:ChamberVolumes");
  }

  G4PhysicalVolumeStore* pvs = G4PhysicalVolumeStore::GetInstance();
  std::vector<G4VPhysicalVolume*>::iterator cite;
  for( cite = pvs->begin(); cite != pvs->end(); cite++ ) {
    for( size_t ii = 0; ii < paramsCh.size(); ii++ ) {
      //      G4cout << " CHECK " << paramsCh[0]<< " =? " << (*cite)->GetName() << G4endl; //GDEB
      if( GmGenUtils::AreWordsEquivalent(paramsCh[ii],(*cite)->GetName()) ) {
	G4VPhysicalVolume* pv = *cite;
	theChamberPVs.insert(pv);
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(debugVerb) ) G4cout << " GmPSEnergyDeposit_PrimaryGamma1stInterAttenuation::SetParameters Added chamber PV " << pv->GetName() << G4endl;
#endif
      }
    }
  }

  std::vector<G4String> paramsCav;
  paramsCav = GmParameterMgr::GetInstance()->GetVStringValue("GmHistosAtCavityUA:CavityVolumes",paramsCav);
  if( paramsCav.size() == 0 ) {
    G4Exception("GmPSCavityKwallOld::SetParameters",
		"",
		FatalErrorInArgument,
		"There should be at least one parameter: chamber_volume_names, please use /gamos/setParam GmHistosAtCavityUA:CavityVolumes");
  }

  for( cite = pvs->begin(); cite != pvs->end(); cite++ ) {
    for( size_t ii = 0; ii < paramsCav.size(); ii++ ) {
      if( GmGenUtils::AreWordsEquivalent(paramsCav[ii],(*cite)->GetName()) ) {
	G4VPhysicalVolume* pv = *cite;
	theCavityPVs.insert(pv);
	
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(debugVerb) ) G4cout << " GmPSEnergyDeposit_PrimaryGamma1stInterAttenuation::SetParameters Added cavity PV " << pv->GetName() << G4endl;
#endif
      }
    }
  }

}

