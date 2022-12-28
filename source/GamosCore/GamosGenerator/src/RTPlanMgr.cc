#include "RTPlanMgr.hh"
#include "RTVPlanSource.hh"
#include "RTPlanBeam.hh"
#include "GamosCore/GamosGenerator/include/GmIsotope.hh"
#include "GamosCore/GamosGenerator/include/GmIsotopeSource.hh"
#include "GamosCore/GamosGenerator/include/GmIsotopeDecay.hh"
#include "GamosCore/GamosGenerator/include/GmGeneratorMessenger.hh"
#include "GamosCore/GamosGenerator/include/GmGenerVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosData/Distributions/include/GmNumericDistributionLinLin.hh"
#include "GamosCore/GamosGeometry/include/GmVModule.hh"
#include "GamosCore/GamosGeometry/include/GmModuleJaws.hh"
#include "GamosCore/GamosGeometry/include/GmModuleMLC.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringRun.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"

typedef std::pair<G4double,G4double> pdd;
typedef std::map<G4double,pdd> mdpdd;

//----------------------------------------------------------------------
RTPlanMgr* RTPlanMgr::theInstance = 0;

//----------------------------------------------------------------------
RTPlanMgr* RTPlanMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new RTPlanMgr;
  }

  //-   GmUserAction* userAction = GmUserActionFactory::get()->create("GmCheckOriginalGamma");
  return theInstance;

}

//-------------------------------------------------------------
RTPlanMgr::RTPlanMgr()
{
}

//-------------------------------------------------------------
void RTPlanMgr::SetRTSource( RTVPlanSource* rts ){
  theRTSource = rts;
  Initialize();
}
    
//-------------------------------------------------------------
void RTPlanMgr::Initialize()
{
  theParamMgr = GmParameterMgr::GetInstance();
  bInitProtonsPerMU = 0;
  theTimeEnergyDataDistribution = 0;
  theCurrentBeam = 0;
  theCurrentControlPoint = 0;
  theCurrentMetersetEvtID = 0;

  G4String name = theRTSource->GetName();
  theMSUnitsToRun = theParamMgr->GetNumericValue(name+":MSUnitsToRun",0);
  theMSUnitsToSkip = theParamMgr->GetNumericValue(name+":MSUnitsToSkip",0.);
  theCurrentMetersetEvtID = 0;
  theNEventsPerMSUnit = theParamMgr->GetNumericValue(name+":NEventsPerMSUnit",-1);

  theG4EventManager = G4EventManager::GetEventManager();
  theG4RunManager = G4RunManager::GetRunManager();

  bRTHistoControlPoint = G4bool(theParamMgr->GetNumericValue(name+":RTHistoControlPoint",0.));
  bRTIonHistoControlPoint = G4bool(theParamMgr->GetNumericValue(name+":RTIonHistoControlPoint",0.));
  if( bRTHistoControlPoint ) { 
    theAnaMgr = GmAnalysisMgr::GetInstance(name+"_RTPos");
  }
  if( bRTIonHistoControlPoint ) { 
    theAnaMgr = GmAnalysisMgr::GetInstance(name+"_RTIonPos");
  }
  
  bDumpAtBeam = theParamMgr->GetNumericValue(name+":DumpAtBeam",0);
  
  /*  if( GmParameterMgr::GetInstance()->GetNumericValue(name+":PlotBeamStates",0)) {
    theRTSource->PlotBeamStates();
    } */
  theEventID = -1;
		
  //    G4cout << "G4tgrUtils::bAllowRepetitions0  " << G4tgrUtils::bAllowRepetitions  << G4endl; //GDEB

}

//-------------------------------------------------------------
void RTPlanMgr::CalculateMetersetEvts()
{
  // number of protons for each energy is different: for each energy (ControlPoint) rescale number of events corresponding to it
  if( !bInitProtonsPerMU ) {
    G4String teddFileName = theParamMgr->GetStringValue("PTBeamProtonsPerMU:FileName","");
    
    if( teddFileName != "" ) {
      theParamMgr->AddParam("PTBeamProtonsPerMU:Data InitialKineticEnergy",PTstring);
      theTimeEnergyDataDistribution = new GmNumericDistributionLinLin("PTBeamProtonsPerMU"); 
      //--- Parameter found: use it
      bInitProtonsPerMU = 1;
    }
  }

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(testVerb) ) G4cout << "@@@@  RTPlanMgr::CalculateMetersetEvts() N BEAMS " << theBeams.size() << G4endl;
#endif
  //----- Calculate accumulated number of metersets in all beams, in all control points and in all sub control points
  G4double beamAccumMS = 0.;
  G4double CPAccumMS = 0.;
  G4double CPAccumMSPrev = 0.; // previous meterset (used for theMSUnitsToSkip)
  G4double MSUsed = 0.;
  G4double MSUsedProt = 0.;
  G4bool bMSEnough = false;
  G4double MSUnitsTOTAL = theMSUnitsToSkip+theMSUnitsToRun;
  G4double meterSetFraction = 0.;
  std::vector<G4double> Meterset;
  std::vector<G4double> MetersetProt; // weighted by MS from file PTBeamProtonsPerMU
  /*  G4double lastMeterset; // Meterset for MSfraction 
  G4double lastCPAccumMS;// MS of last CP used 
  G4double lastCPAccumMSProt; // MSprot of last CP used
  */
  
  for( size_t iib = 0; iib < theBeams.size(); iib++ ) {
    std::vector<RTVPlanControlPoint*> CPs = theBeams[iib]->GetControlPoints();
    G4double beamMS = 0.;
    //    G4double beamCMW = theBeams[iib]->GetParam("FinalCumulativeMetersetWeight",1);
    if( theBeams[iib]->ParamExists("BeamMeterset") ) {
      beamMS = theBeams[iib]->GetParam("BeamMeterset",0);
    } else {
      beamMS = theBeams[iib]->GetParam("MetersetWeight",0);
      //      beamMS = beamCMW;
    }
    
    if( dynamic_cast<RTPlanBeam*>(theBeams[iib]) ) {
      if( theBeams[iib]->ParamExists("FinalCumulativeMetersetWeight") ) {
	beamMS /= theBeams[iib]->GetParam("FinalCumulativeMetersetWeight",0);
      }
    }
    beamAccumMS += beamMS;
    
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << "ACCUMULATING BEAM METERSETS " << iib << " = " << beamAccumMS << " <- " << theBeams[iib]->GetParam("FinalCumulativeMetersetWeight",1) << " NControlPoints " << CPs.size() << " Enough? " << bMSEnough << G4endl;
#endif 
    if( !bMSEnough ) { // ONLY Accumate beamAccumMS
      for( size_t iicp = 0; iicp < CPs.size(); iicp++ ) {
	std::vector<RTVPlanSubControlPoint*> subCPs = CPs[iicp]->GetSubCPs();      
	G4double numberOfPaintings = CPs[iicp]->GetNumberOfPaintings();
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(debugVerb) ) G4cout << "LOOP beam: "<< iib << " CP: " << iicp << " nPaintings " << numberOfPaintings << G4endl; //GDEB	
#endif
	for( size_t iiscp = 0; iiscp < subCPs.size(); iiscp++ ) {
#ifndef GAMOS_NO_VERBOSE
	  if( GenerVerb(debugVerb) ) G4cout << "LOOP beam: "<< iib << " CP: " << iicp << " subCP(ScanSpot): " << iiscp << " nPaintings " << numberOfPaintings << G4endl; //GDEB	
#endif
	  G4double meterSet = subCPs[iiscp]->GetMeterset();
	  CPAccumMS += meterSet;
	  if( GenerVerb(debugVerb) ) G4cout << "CPAccumMSPrev=" << CPAccumMSPrev << " CPAccumMS=" << CPAccumMS << " theMSUnitsToSkip=" << theMSUnitsToSkip << " Skip+Run=" << MSUnitsTOTAL << G4endl; 
	  if( CPAccumMSPrev < theMSUnitsToSkip ) {  
	    if( CPAccumMS < theMSUnitsToSkip ) { // MS interval before Skip
	      meterSetFraction = 0.;
#ifndef GAMOS_NO_VERBOSE
	      if( GenerVerb(debugVerb) ) G4cout << " meterSetFraction= " << meterSetFraction << " MS before Skip " << G4endl; 
#endif
	      //	      break; 
	    } else {
	      if( CPAccumMS < MSUnitsTOTAL ) {
		meterSetFraction = CPAccumMS-theMSUnitsToSkip; // MS interval starts before Skip & ends before Run
#ifndef GAMOS_NO_VERBOSE
		if( GenerVerb(debugVerb) ) G4cout << " meterSetFraction= " << meterSetFraction << " ="<<CPAccumMS<<"-"<<theMSUnitsToSkip<<"  MS starts before Skip & ends before Run " << G4endl;
#endif
	      } else {
		meterSetFraction = theMSUnitsToRun; // MS interval starts before Skip & ends after Run
		if( GenerVerb(debugVerb) ) G4cout << " meterSetFraction= " << meterSetFraction << " =" <<MSUnitsTOTAL<<"-"<<theMSUnitsToSkip<<"  MS starts before Skip & ends after or = Run" <<  G4endl; 
	      }
	    }
	  } else {
		//  if( (CPAccumMS - (theMSUnitsToRun+theMSUnitsToSkip)) > -1.e-6 ) {
	      if( CPAccumMSPrev > MSUnitsTOTAL ) {  // MS interval starts after Run
		bMSEnough = true;
		meterSetFraction = 0.;
		if( GenerVerb(debugVerb) ) G4cout << "@@@ meterSetFraction= " << meterSetFraction << "MS starts after Run " << G4endl;
		//		break;
	      } else { //      if( CPAccumMSPrev < MSUnitsTOTAL  ) {
		if( CPAccumMS < MSUnitsTOTAL  ) { 
		  meterSetFraction = meterSet;	 // MS interval starts after Skip & ends before Run	
		  if( GenerVerb(debugVerb) ) G4cout << "@@@ meterSetFraction= " << meterSetFraction << " ="<<meterSet<< " MS starts after Skip & ends before Run" << G4endl;   
		} else {
		  meterSetFraction = MSUnitsTOTAL-CPAccumMSPrev; // MS interval starts after Skip & ends after Run 
		  if( GenerVerb(debugVerb) ) G4cout << "@@@ meterSetFraction= " << meterSetFraction << " ="<<MSUnitsTOTAL<<"-"<<CPAccumMSPrev<<"  MS  starts after Skip & ends after Run" << G4endl;
		}
	      }
		//	    }
	  }
	  MSUsed += meterSetFraction*numberOfPaintings;
	  Meterset.push_back(MSUsed);
	  G4double protPerMU = 1.;
	  G4double energy = 0.;
	  if( theTimeEnergyDataDistribution ) {
	    energy = CPs[iicp]->GetNominalBeamEnergy();
	    protPerMU = theTimeEnergyDataDistribution->GetNumericValueFromIndex(energy)/1.e9;
	    if( GenerVerb(testVerb) ) G4cout << "protPerMU " << protPerMU << " energy=" << energy << G4endl; 
	  }
	  MSUsedProt += meterSetFraction*numberOfPaintings*protPerMU;
	  MetersetProt.push_back(MSUsedProt);
	  if( meterSetFraction != 0. ) {
	    theSubCPEvt.push_back(subCPs[iiscp]);
	  } else {
	    theSubCPEvt.push_back((RTVPlanSubControlPoint*)(0));
	  }
#ifndef GAMOS_NO_VERBOSE
	  if( GenerVerb(testVerb) ) G4cout << " METERSET=" << meterSetFraction << " <- " << meterSet << G4endl;
	  if( GenerVerb(testVerb) ) G4cout << "ACCUMULATING CONTROL POINT METERSETS " << iib<<":"<<iicp<<":"<<iiscp << " = " << "Nmetersets= " << Meterset.size() << " MSUsedProt= " << std::setprecision(11) << MSUsedProt << " MSUsed " << MSUsed << " CPAccumMS " << CPAccumMS << " " << G4endl; 
#endif
      	  CPAccumMSPrev += meterSet;
	}
      }
    }
  }
  
  //--- Check user is not asking for a meterset bigger than total Plan meterset
  if( MSUnitsTOTAL-beamAccumMS > 1.E-6*beamAccumMS ) {
    G4Exception("RTPlanMgr::AccumulateMetersets",
		"",
		FatalException,
		("MSUnitsToRun+MSUnitsToSkip="+GmGenUtils::ftoa(theMSUnitsToRun+theMSUnitsToSkip)+ " > TotalBeamMS="+GmGenUtils::ftoa(beamAccumMS)).c_str());
  }
  
  //----- Calculate event corresponding to each subCPs, accumulating meterset
  //----- Associate to each change of meterset the corresponding sub control point
  G4int nEventsTotal = theG4RunManager->GetNumberOfEventsToBeProcessed();
  G4double EvtPerMSUnitProt = nEventsTotal / MSUsedProt;
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(testVerb) ) G4cout << "EvtPerMSUnitProt " << EvtPerMSUnitProt << " = " << nEventsTotal << " / " << MSUsedProt << G4endl; 
#endif
  
  for( size_t ii = 0; ii < MetersetProt.size(); ii++ ) {
    theMetersetEvt.push_back( G4int(MetersetProt[ii]*EvtPerMSUnitProt+0.50001));
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << " SubControlPoint " << ii << " Event= " << theMetersetEvt[theMetersetEvt.size()-1] << " = MS= " << MetersetProt[ii]*EvtPerMSUnitProt << " =" << MetersetProt[ii] << "*" << EvtPerMSUnitProt << G4endl;
    //    if( GenerVerb(debugVerb) ) G4cout << theMetersetEvt.size() << " METERSET EVTS " << iib << " : " << iicp << " : " << iiscp << " = " << theMetersetEvt[theMetersetEvt.size()-1] << " = MS= " << theMetersetEvt[ii] << " = " << MetersetProt[ii] << "*" << EvtPerMSUnitProt << G4endl;
#endif
  }
}

//-----------------------------------------------------------------------
RTBeamStateData RTPlanMgr::GetRTBeamStateData( G4double& time, G4Event* evt, G4bool& bOK ) // time is only used for printing 
{
  //----- Get Position, direction angle and energy from BeamState's
  //  G4cout <<" theEvtMgr->GetConstCurrentEvent() " <<  theEvtMgr->GetConstCurrentEvent() << G4endl;//GDEB
  if( evt == 0 ) evt = theG4EventManager->GetNonconstCurrentEvent();
  if( evt ) { // first event G4EventManager is not initialized
    theEventID = evt->GetEventID();
  } else {
    theEventID++;
  }

  //t  G4cout << " !!!!!RTPlanMgr::GetRTBeamStateData TIME " << time << " EVT " << evt << " EVENT_ID " << theEventID << G4endl; //GDEB
  RTBeamStateData bsdata;

  bChangedBeam = false;
  bChangedControlPoint = false;
  bChangedBeamState = false;
  
  //-  theEventID += theNEventsToSkip;

  //--- Change theCurrentMetersetEvtID if theEventID is higher than current one metersetEvt
  // update theCurrentMetersetEvtID if there is a jump in event (ID like for RTGeneratorPhaseSpaceRTPlan)
  G4int ie = theCurrentMetersetEvtID;
  //t  G4cout << " BEFORE jump  theCurrentMetersetEvtID " << theCurrentMetersetEvtID << ": " << theEventID << " > " << theMetersetEvt[theCurrentMetersetEvtID] << G4endl; //GDEB  
  while (theEventID > theMetersetEvt[theCurrentMetersetEvtID] ) {
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << "@@ RTPlanMgr::GetRTBeamStateData moving theCurrentMetersetEvtID to include eventID jump : eventID= " << theEventID << " > " << theMetersetEvt[theCurrentMetersetEvtID] << " theCurrentMetersetEvtID " << theCurrentMetersetEvtID << G4endl;
#endif
    theCurrentMetersetEvtID++;
    if( theCurrentMetersetEvtID >= G4int(theMetersetEvt.size()) ) {
      break;
    }
  }
  //t  G4cout << ie << "??Diminish 1 theCurrentMetersetEvtID " << theCurrentMetersetEvtID << G4endl; //GDEB
  if( ie - theCurrentMetersetEvtID != 0 ) {
    //t    G4cout << ie << " Diminish 1 theCurrentMetersetEvtID " << theCurrentMetersetEvtID << G4endl; //GDEB
    theCurrentMetersetEvtID--;
  }
  if( theCurrentMetersetEvtID >= G4int(theMetersetEvt.size()) ) {
    G4Exception("RTPlanMgr::GenerateVertex",
		"",
		JustWarning,
		"All BeamState's from all beams have been run, and still asking for more events! Run will be aborted");
    theG4RunManager->AbortRun();
    bOK = false;
    return bsdata;
  }
  
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "@@ RTPlanMgr::GenerateVertex moved theCurrentMetersetEvtID to include eventID jumped : " << ie << " -> " << theCurrentMetersetEvtID << " : " << theMetersetEvt[ie] << " -> " << theMetersetEvt[theCurrentMetersetEvtID] <<G4endl;
#endif
  
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "@@ RTPlanMgr::GenerateVertex change event data theEventID .eq. theNEventsToSkip || theEventID .eq. theMetersetEvt[theCurrentMetersetEvtID] " << G4endl
				    << " " << theEventID << " || " << theEventID << " >=? " << theMetersetEvt[ie] << " ; theCurrentMetersetEvtID(old) " << ie << " time " << time << G4endl;
#endif

  //----- theCurrentMetersetEvtID is pointing to the next EventID when Beam State Data should change
  if( theEventID == 0 
      || theEventID >= theMetersetEvt[ie] ) { // change event bsdata (may be bigger if eventID are not continous, like for RTGeneratorPhaseSpaceRTPlan)
#ifndef GAMOS_NO_VERBOSE   
    if( GenerVerb(testVerb) && theSubCPEvt[0] ) G4cout << "@@ RTPlanMgr::GenerateVertex  CHANGING EVENT DATA Evt " << theEventID << " Beam: " << theSubCPEvt[ie]->GetBeam()->GetIndex() <<G4endl;
#endif
    /*    if( theCurrentMetersetEvtID != 0 ){
      G4Exception("RTPlanMgr::GenerateVertex",
		  "",
		  JustWarning,
		  ("All BeamState's from all beams have been run, and still asking for more events! Run will be aborted at event " + GmGenUtils::itoa(theEventID) + ", while " + GmGenUtils::itoa(theG4RunManager->GetNumberOfEventsToBeProcessed())+" have been requested").c_str() );
      theG4RunManager->AbortRun();
      return (G4PrimaryVertex*)0;
      } */

    G4int ieMax = G4int(theMetersetEvt.size());
    if( theMetersetEvt.size() == 1 && theEventID > 0 ) ieMax = 2; // loop once to get at least one NextBeamState

    int ieOrig = ie;
    for( ; ie < ieMax; ie++ ){
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(testVerb) ) G4cout << "@@@ RTPlanMgr::GenerateVertex Looping to metersets which correspond to same EventID than the one ie= " << ie << ": theMetersetEvt[ie]= " << theMetersetEvt[ie] << " !=?Current " << theCurrentMetersetEvtID << ": "<< theMetersetEvt[theCurrentMetersetEvtID] << G4endl; 
#endif
      //--- If several theMeterSetEvts are equal, run until the last one
      /*      if( theEventID == 0 && theMetersetEvt[theCurrentMetersetEvtID] == 0 ) { //--- Skip first events if meterset = 0
	//      if( theEventID == theNEventsToSkip && ie == 0 ) { //--- Skip first events if meterset = 0
	theCurrentMetersetEvtID++;
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(testVerb) ) G4cout << "@ RTPlanMgr::GenerateVertex Add Data because Skip first events if meterset = 0, evt " << theEventID << " theCurrentMetersetEvtID " << theCurrentMetersetEvtID << G4endl;	
	#endif*/
      if( theEventID == 0 && theMetersetEvt[theCurrentMetersetEvtID] != 0 ) { //--- Skip first events if meterset = 0
	//      if( theEventID == theNEventsToSkip && ie == 0 ) { //--- Skip first events if meterset = 0
      //	theCurrentMetersetEvtID++;
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(testVerb) ) G4cout << "@ RTPlanMgr::GenerateVertex select meterset because theEventID=0 and meterset != 0, evt " << theEventID << " theCurrentMetersetEvtID " << theCurrentMetersetEvtID << G4endl;	
#endif
	break;
      } else {
	//--- meterset not repeated: break
	if( theMetersetEvt[ie] > theMetersetEvt[theCurrentMetersetEvtID] ) {
	  //	if( theMetersetEvt[ie+(theEventID!=theNEventsToSkip)] != theMetersetEvt[theCurrentMetersetEvtID+(theEventID!=theNEventsToSkip)] ) {
#ifndef GAMOS_NO_VERBOSE
	  if( GenerVerb(testVerb) ) G4cout << "@ RTPlanMgr::GenerateVertexStop adding Data DIFFERENT EvtsID [" << ie << "] = " << theMetersetEvt[ie] << " != [" << theCurrentMetersetEvtID << "] = " << theMetersetEvt[theCurrentMetersetEvtID] << G4endl;
	  //	  if( GenerVerb(testVerb) ) G4cout << "@@ DIFFERENT EvtsID [" << ie+(theEventID!=theNEventsToSkip) << "] = " << theMetersetEvt[ie+(theEventID!=theNEventsToSkip)] << " != [" << theCurrentMetersetEvtID+(theEventID!=theNEventsToSkip) << "] = " << theMetersetEvt[theCurrentMetersetEvtID+(theEventID!=theNEventsToSkip)] << G4endl;
#endif
	  break;
	} else {
#ifndef GAMOS_NO_VERBOSE
	  if( GenerVerb(testVerb) ) G4cout << "@ RTPlanMgr::GenerateVertex Add data EQUAL EvtsID [" << ie << "] = " << theMetersetEvt[ie] << " == [" << theCurrentMetersetEvtID << "] = " << theMetersetEvt[theCurrentMetersetEvtID] << G4endl;
	  //	  if( GenerVerb(testVerb) ) G4cout << "@@ EQUAL EvtsID [" << ie+(theEventID!=theNEventsToSkip) << "] = " << theMetersetEvt[ie+(theEventID!=theNEventsToSkip)] << " == [" << theCurrentMetersetEvtID+(theEventID!=theNEventsToSkip) << "] = " << theMetersetEvt[theCurrentMetersetEvtID+(theEventID!=theNEventsToSkip)] << G4endl;
#endif
	}
      }

    } // loop to CPs with same theMetersetEvt

    if( theEventID == 0 || ieOrig != ie ) {
      //--- get next scan spot
      bsdata = GetNextBeamStateData(ie); // avance N beam states (check if control point and beam have changed
      //?????? theBeams[iCurrentBeam]->NextBeamState(iNextBeam);

      bChangedBeamState = true;
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(testVerb) ) G4cout << "@@!!  RTPlanMgr::GenerateVertex CHANGED TO NEXT SCAN SPOT in event " << theEventID << " CurrentMetersetEvtID " << ie << G4endl; 
      //t      if( GenerVerb(testVerb) ) G4cout << "@@!!  RTPlanMgr::GenerateVertex NextBeam " << iNextBeam << " !=?Current " << iCurrentBeam << G4endl; 
#endif
    }
    if( ie == ieMax ) {
      if( GenerVerb(debugVerb) ) G4cout << " @ RTPlanMgr::GenerateVertex CHANGED TO NEXT SUB CONTROL POINT " << ie << " CP= " << theSubCPEvt[ie]->GetControlPoint()->GetIndex() << " Beam= " << theSubCPEvt[ie]->GetControlPoint()->GetBeam()->GetIndex() << G4endl;
      G4Exception("RTPlanMgr::GenerateVertex",
		  "",
		  JustWarning,
		  "All BeamState's from all beams have been run, and still asking for more events! Run will be aborted");
      theG4RunManager->AbortRun();
      bOK = false;
      return bsdata;
    }
    
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(testVerb) )
      G4cout << " GOT NEW BEAM STATE DATA "
	     << " EVENT_ID= " << theEventID 
	     << " X= " << bsdata.PositionX 
	     << " Y= " << bsdata.PositionY 
	     << " Z= " << bsdata.PositionZ
	     << " E= " << bsdata.Energy 
	     << " AngleX= " << bsdata.RotAngleX/CLHEP::deg 
	     << " AngleY= " << bsdata.RotAngleY/CLHEP::deg 
	     << " AngleZ= " << bsdata.RotAngleZ/CLHEP::deg 
	     << " SpotSize= " << bsdata.SpotSize 
	     << " SourceAxisDistanceX= " << bsdata.SourceAxisDistanceX
	     << " SourceAxisDistanceY= " << bsdata.SourceAxisDistanceY
	     << " ControlPoint " << bsdata.ControlPoint->GetIndex()
	     << " Beam " << bsdata.Beam->GetIndex() << G4endl; 
#endif

    //    theLastEventID = theEventID;
    theCurrentMetersetEvtID = ie;
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(testVerb) ) G4cout << "@ RTPlanMgr::GenerateVertex ENDED ie loop " << ie << " theCurrentMetersetEvtID " << theCurrentMetersetEvtID << G4endl; 
#endif
    //    if( theEventID == theNEventsToSkip && theMetersetEvt[0] != 0 ) theCurrentMetersetEvtID--; // first event: it was forced to take the first metersetEvt
    //    if( theEventID == 0 ) theCurrentMetersetEvtID--; // first event: it was forced to take the first metersetEvt

    //---- New BeamState data FILL HISTOGRAMS
    //    G4cout << " FILLING RTHistoControlPoint " << G4endl; //GDEB
    /*    if( bRTHistoControlPoint ) {
      //	if( theHCurrentControlPoint != 0 || theHPreviousBeam == theHCurrentBeam ) {
      theHCurrentBeam = iCurrentBeam;
      theHCurrentControlPoint = theBeams[iCurrentBeam]->GetCurrentControlPointIndex();
      theHCurrentBeam += 1;
      //	} 
      BookHistoControlPoint(theHCurrentBeam,theHCurrentControlPoint);
      G4cout << " theHCurrentPos " << theHCurrentPosX << " " << theHCurrentPosY << G4endl; //GDEB
      //--- Increment the histogram binContent corresponding to beam and control_point (without increment NEntries)
      FillRTHistoControlPoint(theHCurrentNumber,bsdata);
      FillRTHistoControlPoint(870000,bsdata);
      G4int hCurrentNumberBeam = theHCurrentNumber- (theHCurrentNumber-870000)%theMaxNCP;
      FillRTHistoControlPoint(hCurrentNumberBeam,bsdata);
      //	theHPreviousBeam = theHCurrentBeam;
    }
    if( bRTIonHistoControlPoint ) {
      //	if( theHCurrentControlPoint != 0 || theHPreviousBeam == theHCurrentBeam ) {
      theHCurrentBeam = iCurrentBeam;
      theHCurrentControlPoint = theBeams[iCurrentBeam]->GetCurrentControlPointIndex();
      theHCurrentBeam += 1;
      //	} 
      BookHistoControlPoint(theHCurrentBeam,theHCurrentControlPoint);
      theHCurrentPosX = bsdata.PositionX;
      theHCurrentPosY = bsdata.PositionY;
      G4cout << " theHCurrentPos " << theHCurrentPosX << " " << theHCurrentPosY << G4endl; //GDEB
      FillRTIonHistoControlPoint(theHCurrentNumber,bsdata);
      FillRTIonHistoControlPoint(870000,bsdata);
      FillRTIonHistoControlPoint(870000,bsdata);
      //	theHPreviousBeam = theHCurrentBeam;
      }*/

  } // END NEW bsdata

  return bsdata;
  
}

//-----------------------------------------------------------------------
void RTPlanMgr::BookHistoControlPoint(G4int iib, G4int iicp)
{
  G4String name = theRTSource->GetName();
  G4int nBinsXHistoCP = G4int(theParamMgr->GetNumericValue(name+":RTHistoNBinsX",200));
  G4double minXHistoCP = theParamMgr->GetNumericValue(name+":RTHistoMinX",-100.);
  G4double maxXHistoCP = theParamMgr->GetNumericValue(name+":RTHistoMaxX",100.);
  G4int nBinsYHistoCP = G4int(theParamMgr->GetNumericValue(name+":RTHistoNBinsY",200));
  G4double minYHistoCP = theParamMgr->GetNumericValue(name+":RTHistoMinY",-100.);
  G4double maxYHistoCP = theParamMgr->GetNumericValue(name+":RTHistoMaxY",100.);
  
  theMaxNCP = G4int(theParamMgr->GetNumericValue(name+":RTHistoMaxNCP",1000));

  G4String hnam = "RTPlan_"+GmGenUtils::itoa(iib)+"_"+GmGenUtils::itoa(iicp);
  theHCurrentNumber = 870000+theMaxNCP*iib+iicp;
  theAnaMgr->CreateHisto2D(hnam.c_str(),nBinsXHistoCP,minXHistoCP,maxXHistoCP,nBinsYHistoCP,minYHistoCP,maxYHistoCP,theHCurrentNumber);
  if( theAnaMgr->GetHisto2D(870000, false) == 0 ) {
    theAnaMgr->CreateHisto2D(hnam.c_str(),nBinsXHistoCP,minXHistoCP,maxXHistoCP,nBinsYHistoCP,minYHistoCP,maxYHistoCP,870000);
  }
  if( theAnaMgr->GetHisto2D(870000+theMaxNCP*iib, false) == 0 ) {
    theAnaMgr->CreateHisto2D(hnam.c_str(),nBinsXHistoCP,minXHistoCP,maxXHistoCP,nBinsYHistoCP,minYHistoCP,maxYHistoCP,870000+theMaxNCP*iib);
  }
}

//------------------------------------------------------------------------
RTBeamStateData RTPlanMgr::GetNextBeamStateData(G4int ie)
{
  bChangedBeamState = true;
  // avance N beam states (check if control point and beam have changed
  if( theCurrentControlPoint == theSubCPEvt[ie]->GetControlPoint()->GetIndex() ) {
    bChangedControlPoint = true;
  } else {
    bChangedControlPoint = false;
  }
  //t  G4cout << " RTPlanMgr::GetNextBeamStateData " << ie << " " << theSubCPEvt[ie] << G4endl << theSubCPEvt[ie]->GetControlPoint() << G4endl << theSubCPEvt[ie]->GetControlPoint()->GetBeam() << G4endl << theSubCPEvt[ie]->GetControlPoint()->GetBeam()->GetIndex() << G4endl; //GDEB
  if( theCurrentBeam == theSubCPEvt[ie]->GetControlPoint()->GetBeam()->GetIndex() ) {
    bChangedBeam = true;
    if( bDumpAtBeam ) {
      DumpHistosAndScorersBeam(); 
    }
  } else {
    bChangedBeam = false;
  }

  RTBeamStateData bsData = theSubCPEvt[ie]->GetBeamStateData();
    
  theCurrentControlPoint = theSubCPEvt[ie]->GetControlPoint()->GetIndex();
  theCurrentBeam = theSubCPEvt[ie]->GetControlPoint()->GetBeam()->GetIndex();

  return bsData;
}

//------------------------------------------------------------------------
void RTPlanMgr::FillRTHistoControlPoint( G4int hNum, const RTBeamStateData& bsdata )
{
  mdpdd theRTHistosXFields;
  mdpdd theRTHistosYFields;

  //----- MOVE BLD'S (JAWS and MLC's)
  //--- Loop to all RTPlan BLD's found
  std::map<G4String,std::vector<G4VPhysicalVolume*> >::const_iterator itespv;  
  std::map<G4String,GmVModule*> RTPlanGmModuleAssoc = theRTSource->GetRTPlanGmModuleAssoc();
  std::map<G4String,std::vector<G4VPhysicalVolume*> > RTPlanG4PVsAssoc = theRTSource->GetRTPlanG4PVsAssoc();
  for( itespv = RTPlanG4PVsAssoc.begin(); itespv != RTPlanG4PVsAssoc.end(); itespv++ ) {
    G4String rtVol2G4Name = (*itespv).first;
    std::vector<G4VPhysicalVolume*> PVs = (*itespv).second;
    
    std::map<G4String,G4double> beamParams = bsdata.Beam->GetParams();
    std::map<G4String,G4double>::const_iterator itep;
    
    //--- NOW LOOK FOR posX/Y IN RTPlanControlPoint PARAMETERS
    std::map<G4String,GmVModule*>::const_iterator itemod = RTPlanGmModuleAssoc.find(rtVol2G4Name);
    GmVModule* gmModule = (*itemod).second;
    G4String g4PVName = gmModule->GetName();
    if( GenerVerb(debugVerb) ) G4cout << " g4PVName = gmModule->GetName() " << g4PVName << G4endl;
    
    if( dynamic_cast<GmModuleJaws*>(gmModule) ) {
      G4String orientation = gmModule->GetParam("ORIENTATION");
      //---- Transversal dimension (in XY plane)
      G4double dimTransv;
      if( orientation == "X" ) {
	dimTransv = GmGenUtils::GetValue(gmModule->GetParam("YHDIM"));
      } else {
	dimTransv = GmGenUtils::GetValue(gmModule->GetParam("XHDIM"));
      }
      G4double posZ = GmGenUtils::GetValue(gmModule->GetParam("Z_CENTRE"));
      G4double isocPosZ = GmGenUtils::GetValue(gmModule->GetParam("Z_ISOCENTRE"));
      //t      G4cout << "  RTPlanMgr::FillRTHistoControlPoint  JAWS " << orientation << " dimTransv " <<dimTransv*isocPosZ/posZ << " from " << dimTransv << G4endl; //GDEB
      dimTransv *= isocPosZ/posZ;    
      G4double posNeg = bsdata.ControlPoint->GetParam(rtVol2G4Name+"_1_POS",1);
      G4double posPos = bsdata.ControlPoint->GetParam(rtVol2G4Name+"_2_POS",1);
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(debugVerb) ) G4cout << "RTPlanMgr::FillRTHistoControlPoint " << rtVol2G4Name << " position leaf1 " << posNeg << " leaf2 " << posPos << G4endl;
#endif
      if( orientation == "X" ) {
	theRTHistosXFields[-dimTransv] = pdd(posNeg,posPos);
	theRTHistosXFields[dimTransv] = pdd(posNeg,posPos);
      } else {
	theRTHistosYFields[-dimTransv] = pdd(posNeg,posPos);
	theRTHistosYFields[dimTransv] = pdd(posNeg,posPos);
      }
      //t      G4cout << "  RTPlanMgr::FillRTHistoControlPoint  JAWS  FIELD " << dimTransv << " = " << posNeg << " " << posPos << G4endl; //GDEB
															     
    } else if( dynamic_cast<GmModuleMLC*>(gmModule) ) {

	G4String orientation = gmModule->GetParam("ORIENTATION");
      //---- Transversal dimension (in XY plane)
      G4double topZ = GmGenUtils::GetValue(gmModule->GetParam("Z_TOP")); // start of MLC
      G4double gapZ = GmGenUtils::GetValue(gmModule->GetParam("Z_GAP")); // where leaf dimensions are given
      gapZ += topZ;
      G4double gap = GmGenUtils::GetValue(gmModule->GetParam("GAP"));
      G4double crossStartPoint = GmGenUtils::GetValue(gmModule->GetParam("CROSS_START_POINT"));
      G4int nLeafCrossProfiles = GmGenUtils::GetInt(gmModule->GetParam("N_LEAF_CROSS_PROFILES"));
      std::map<G4double,G4double> dimTransvLeafs;
      for(int ii=1;ii < 1+nLeafCrossProfiles;ii++) {
	G4int nLeafCrossPoints = GmGenUtils::GetInt(gmModule->GetParam("NLeafCrossPoints["+GmGenUtils::itoa(ii)+"]"));
	G4double minXP = DBL_MAX;
	G4double maxXP = -DBL_MAX;
	for(int jj=1; jj<nLeafCrossPoints+1; jj++){	
	  minXP = std::min(minXP, GmGenUtils::GetValue(gmModule->GetParam("c["+GmGenUtils::itoa(ii)+","+GmGenUtils::itoa(jj)+"]")));
	  maxXP = std::max(maxXP, GmGenUtils::GetValue(gmModule->GetParam("c["+GmGenUtils::itoa(ii)+","+GmGenUtils::itoa(jj)+"]")));
	  //t	  G4cout << ii << "  RTPlanMgr::FillRTHistoControlPoint  MLC dimTransvLeafs " << maxXP << " : " << gmModule->GetParam("c["+GmGenUtils::itoa(ii)+","+GmGenUtils::itoa(jj)+"]") << " = " <<GmGenUtils::GetValue(gmModule->GetParam("c["+GmGenUtils::itoa(ii)+","+GmGenUtils::itoa(jj)+"]")) << G4endl; //GDEB
	  //t G4cout << ii << "  RTPlanMgr::FillRTHistoControlPoint  MLC dimTransvLeafs " << maxXP << " : " << gmModule->GetParam("z["+GmGenUtils::itoa(ii)+","+GmGenUtils::itoa(jj)+"]") << " = " <<GmGenUtils::GetValue(gmModule->GetParam("z["+GmGenUtils::itoa(ii)+","+GmGenUtils::itoa(jj)+"]")) << G4endl; //GDEB
	}
	dimTransvLeafs[ii] = maxXP-minXP;
	//t	G4cout << ii << "  RTPlanMgr::FillRTHistoControlPoint  MLC dimTransvLeafs " << maxXP-minXP << " max " << maxXP << " min " << minXP << G4endl; //GDEB
	
      }

      G4double isocPosZ = GmGenUtils::GetValue(gmModule->GetParam("Z_ISOCENTRE"));
      //t      G4cout << " MLC isocPosZ " << isocPosZ << " gapZ " << gapZ << G4endl; //GDEB

      G4int nLeafPairs = GmGenUtils::GetValue(gmModule->GetParam("N_LEAF_PAIRS"));
      G4double startTransv = crossStartPoint;
      G4double endTransv = crossStartPoint;
      for(int ii=1;ii<1+nLeafPairs;ii++){
	G4int ilt = GmGenUtils::GetInt(gmModule->GetParam("LEAF_TYPE["+GmGenUtils::itoa(ii)+"]"));  
 	endTransv = startTransv+dimTransvLeafs[ilt];
	//	G4cout << " endTransv " << endTransv << " = " << startTransv << "+" << dimTransvLeafs[ilt] << " " << ilt << G4endl; //GDEB
	G4double posNeg =  GmGenUtils::GetValue(gmModule->GetParam("OPEN_POSITION_NEG["+GmGenUtils::itoa(ii)+"]")); 
	G4double posPos =  GmGenUtils::GetValue(gmModule->GetParam("OPEN_POSITION_POS["+GmGenUtils::itoa(ii)+"]")); 
	if( orientation == "X" ) {
	  theRTHistosXFields[startTransv*isocPosZ/gapZ] = pdd(posNeg,posPos);
	  //t	  G4cout << ii << "  RTPlanMgr::FillRTHistoControlPoint  MLC " << orientation << " FIELD " << startTransv*isocPosZ/gapZ << " from " << startTransv << " = " << posNeg << " " << posPos << G4endl; //GDEB
	  theRTHistosXFields[endTransv*isocPosZ/gapZ] = pdd(posNeg,posPos);
	  //t	  G4cout << ii << "  RTPlanMgr::FillRTHistoControlPoint  MLC  FIELD " << endTransv*isocPosZ/gapZ << " from " << endTransv << " = " << posNeg << " " << posPos << G4endl; //GDEB
	} else {
	  theRTHistosYFields[startTransv*isocPosZ/gapZ] = pdd(posNeg,posPos);
	  //t	  G4cout << ii << "  RTPlanMgr::FillRTHistoControlPoint  MLC " << orientation << " FIELD " << startTransv*isocPosZ/gapZ << " from " << startTransv << " = " << posNeg << " " << posPos << G4endl; //GDEB
	  theRTHistosYFields[endTransv*isocPosZ/gapZ] = pdd(posNeg,posPos);
	  //t	  G4cout << ii << "  RTPlanMgr::FillRTHistoControlPoint  MLC  FIELD " << endTransv*isocPosZ/gapZ << " from " << endTransv << " = " << posNeg << " " << posPos << G4endl; //GDEB
	}
	startTransv = endTransv+gap;
      }
      
      }
  }
      
  //----- FILL HISTOGRAMS 
  TH2F* histo2 = theAnaMgr->GetHisto2D(hNum);
  G4int hisNBinsX = histo2->GetXaxis()->GetNbins();
  G4double hisMinX = histo2->GetXaxis()->GetXmin();
  G4double hisWidthX = histo2->GetXaxis()->GetBinWidth(1);
  G4int hisNBinsY = histo2->GetYaxis()->GetNbins();
  G4double hisMinY = histo2->GetYaxis()->GetXmin();
  G4double hisWidthY = histo2->GetYaxis()->GetBinWidth(1);
  for( int iix = 0; iix < hisNBinsX; iix++ ) {
    G4double histoX = hisMinX+(iix+0.5)*hisWidthX;
    //    G4cout << " FILLHISTO histoX= " << histoX << " " << (*(theRTHistosYFields.begin())).first << G4endl; //GDEB
    mdpdd::const_iterator mitex = theRTHistosYFields.upper_bound(histoX);  // look for the transvDim of Y  
    for( int iiy = 0; iiy < hisNBinsY; iiy++ ) {
      G4double histoY = hisMinY+(iiy+0.5)*hisWidthY;
      //      G4cout << " FILLHISTO histoY= " << histoY << G4endl; //GDEB
      mdpdd::const_iterator mitey = theRTHistosXFields.upper_bound(histoY);
      if( mitex != theRTHistosYFields.begin() || mitex != theRTHistosYFields.end() ||
	  mitey != theRTHistosXFields.begin() || mitey != theRTHistosXFields.begin() ) {
	pdd posXNP = mitey->second;
	if( histoX < posXNP.first || histoX > posXNP.second ) {
	  //t	  G4cout << " 2FILLHISTO for histoY " << histoY << " histoX OUT " << posXNP.second << " < " << histoX << " < " << posXNP.first << G4endl; //GDEB
	  continue;
	}
        pdd posYNP = mitex->second;
	if( histoY < posYNP.first || histoY > posYNP.second ) {
	  //t	  G4cout << " 2FILLHISTO histoX " << histoX << " histoY OUT " << posYNP.second << " < " << histoY << " < " << posYNP.first << G4endl; //GDEB
	  continue;
	}
        histo2->SetBinContent(iix,iiy,1.);
	//      histo2->SetBinContent(iix,iiy,histo2->GetBinContent(iix,iiy)+1);
	//t	G4cout << " FILLHISTO SetBinContent " << iix << " : " << iiy << " " << histoX << " : " << histoY << G4endl; //GDEB
      } else {
	//t	  G4cout << " FILLHISTO histoXY OUT " << histoX << " : " << histoY << G4endl; //GDEB
	continue;
      }
    }
  }

  histo2->SetEntries(0);
	  
}

//------------------------------------------------------------------------
void RTPlanMgr::FillRTIonHistoControlPoint( G4int hNum, const RTBeamStateData& bsData )
{

}


//------------------------------------------------------------------------
void RTPlanMgr::DumpHistosAndScorersBeam( ) 
{
  G4String suffix = theParamMgr->GetStringValue("GmAnalysisMgr:FileNameSuffix","");
  G4String suffixNew = suffix + "_b"+GmGenUtils::itoa(theCurrentBeam);

  DumpHistosAndScorers();

  theParamMgr->AddParam("GmAnalysisMgr:FileNameSuffix "+suffix,PTstring);
  
}


//------------------------------------------------------------------------
void RTPlanMgr::DumpHistosAndScorers()
{
  G4String suffix = theParamMgr->GetStringValue("GmAnalysisMgr:FileNameSuffix","");
  G4String suffixNew = suffix + "_b"+GmGenUtils::itoa(theCurrentBeam);

  const G4Run* aRun = G4RunManager::GetRunManager()->GetCurrentRun();
  std::cout << " RTPlanMgr::DumpHistosAndScorers " << GmNumberOfEvent::GetNumberOfEvent() << std::endl;//GDEB
  GmScoringRun* scrun = (GmScoringRun*)aRun;
  scrun->DumpAllScorers();
  
  std::map<G4String, GmAnalysisMgr*> analInstances = GmAnalysisMgr::GetAllInstances();
  std::set<G4String> fileFormats = GmAnalysisMgr::GetFileFormats();
  std::map<G4String, GmAnalysisMgr*>::const_iterator itea;
  std::set<G4String>::const_iterator itef;
  for( itea = analInstances.begin(); itea != analInstances.end(); itea++ ) {
    GmAnalysisMgr* analMgr = itea->second;
    for( itef = fileFormats.begin(); itef != fileFormats.end(); itef++ ){
      analMgr->Save( analMgr->GetFileName(), *itef ); 
    }
  }
    
}
