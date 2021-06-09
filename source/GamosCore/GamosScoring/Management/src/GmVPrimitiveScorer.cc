#include "GamosCore/GamosUserActionMgr/include/GmStepMgr.hh"
#define protected public
#include "G4VPrimitiveScorer.hh"
//#define protected protected
#define private public
#include "G4EnergyLossForExtrapolator.hh"
//#define private private 

#include "GmVPrimitiveScorer.hh"
#include "GmVPSPrinter.hh"
#include "GmScoringVerbosity.hh"

#include "GamosCore/GamosBase/Classifiers/include/GmClassifierBy1Ancestor.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosData/Management/include/GmDataMgr.hh"
#include "GamosCore/GamosData/Management/include/GmVData.hh"
#include "GamosCore/GamosBase/Base/include/GmDistributionMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVDistribution.hh"

#include "GamosCore/GamosBase/Base/include/GmConvergenceTester.hh"
#include "GamosCore/GamosUserActionMgr/include/GmFutureFilter.hh"

#include "G4PhantomParameterisation.hh"
#include "G4EmCalculator.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4VSolid.hh"

#include "Reflex/PluginService.h"

//--------------------------------------------------------------------
GmVPrimitiveScorer::GmVPrimitiveScorer(G4String name) 
  : G4VPrimitiveScorer( name, 0 )
{ 
  sumALL = 0.;

  HCID = -1;
  fWeighted = TRUE;
  bScoreErrors = TRUE;
  bScoreByEvent = TRUE;
  theUnit = 1.;
  theUnitName = G4String("");

  AddDefaultClassifier();
  theFilter = 0;
  bNewEvent = TRUE;

  G4String dataName = GmParameterMgr::GetInstance()->GetStringValue(primitiveName+":MultiplyByData","");
  if( dataName != "" ) {
    theMultiplyingData = GmDataMgr::GetInstance()->BuildData(dataName);
  } else {
    theMultiplyingData = 0;
  }

  G4String distName = GmParameterMgr::GetInstance()->GetStringValue(primitiveName+":MultiplyByDistribution","");
  if( distName != "" ) {
    std::vector<G4String> params;
    params.push_back(distName);
    params.push_back(distName);
    theMultiplyingDistribution =  GmDistributionMgr::GetInstance()->FindOrBuildDistribution(params);
  } else {
    theMultiplyingDistribution = 0;
  }

  G4String convergence = GmParameterMgr::GetInstance()->GetStringValue(primitiveName+":ConvergenceTester","");
  if( convergence != "") {
    theConvergenceTester = new GmConvergenceTester(convergence); 
  } else {
    theConvergenceTester = 0;
  }

  theEnergySplitter = new GmEnergySplitter;

  bSkipEqualMaterials = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmReadPhantomGeometry:Phantom:SkipEqualMaterials",0));
  bUseClassifierIndex = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(primitiveName+":UseClassifierIndex",0));

  theRegularParamUtils = GmRegularParamUtils::GetInstance();

  theGeometryUtils = GmGeometryUtils::GetInstance();
  bUseVolume = false;
  theMFD = 0;
}

//--------------------------------------------------------------------
GmVPrimitiveScorer::~GmVPrimitiveScorer()
{ 
}

//--------------------------------------------------------------------
G4int GmVPrimitiveScorer::GetIndex(G4Step* aStep)
{
  return theClassifier->GetIndexFromStep( aStep );
}


//--------------------------------------------------------------------
void GmVPrimitiveScorer::DumpAll(G4THitsMap<G4double>* RunMap)
{
  //  G4cout <<" GmVPrimitiveScorer::DumpAll " << G4endl;

  if( thePrinters.size() == 0 ) {
    AddDefaultPrinter();
  }

  std::vector<GmVPSPrinter*>::const_iterator ite;
  for(ite = thePrinters.begin(); ite !=  thePrinters.end(); ite++ ){
    (*ite)->DumpAll( RunMap, this );
  }

  if( theConvergenceTester ) {
    theConvergenceTester->ShowResult();
    theConvergenceTester->ShowHistory();
  }

}

//--------------------------------------------------------------------
void GmVPrimitiveScorer::AddPrinter( GmVPSPrinter* prt ) 
{
  thePrinters.push_back(prt); 
}
 
//--------------------------------------------------------------------
void GmVPrimitiveScorer::SetClassifier(GmVClassifier* idx )
{ 
  theClassifier = idx; 
}

//--------------------------------------------------------------------
void GmVPrimitiveScorer::AddDefaultPrinter()
{
  G4String param = "GmPSPrinterG4cout";
  GmVPSPrinter* psp = Reflex::PluginService::Create<GmVPSPrinter*>(param,param);
  thePrinters.push_back( psp ); 
}


//--------------------------------------------------------------------
void GmVPrimitiveScorer::AddDefaultClassifier()
{
  std::vector<G4String> params;
  params.push_back("0");
  theClassifier = new GmClassifierBy1Ancestor("GmClassifierBy1Ancestor");
  theClassifier->SetParameters( params );
}


//--------------------------------------------------------------------
void GmVPrimitiveScorer::Initialize(G4HCofThisEvent* HCE)
{
  EvtMap = new G4THitsMap<G4double>(detector->GetName(),
				    GetName());
  if(HCID < 0) {HCID = GetCollectionID(0);}
  HCE->AddHitsCollection(HCID, EvtMap);
}

//--------------------------------------------------------------------
G4bool GmVPrimitiveScorer::FillScorer(G4Step* aStep, G4double val, G4double wei)
{
  G4int index = -1;
  if( aStep != 0 ) { // it is 0 when called by GmScoringMgr after last event
    index = GetIndex(aStep);
  }

  if( !fWeighted ) wei = 1.;
  FillScorer( aStep, index, val, wei );

  return true;
}

//--------------------------------------------------------------------
G4bool GmVPrimitiveScorer::FillScorer(G4Step* aStep, G4int index, G4double val, G4double wei)
{
#ifndef GAMOS_NO_VERBOSE
  //  if( ScoringVerb(debugVerb) ) G4cout << EvtMap << " EvtMap Number of entries " << EvtMap->entries() << " tmp " << theSumV_tmp.size() << " val " << val << " weight " << wei << G4endl;
#endif
  if( bScoreErrors ) {
    if( bNewEvent ) {
      ScoreNewEvent();
    }
    bNewEvent = false;
  }

  if( index == -1 ) return true;


  if( theMultiplyingData ) {
    G4double vald = theMultiplyingData->GetValueFromStep(aStep, index);
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout  << " GmVPrimitiveScorer::FillScorer MultiplyingData "  << primitiveName << " " << val << " * " << vald << " = " << val*vald << G4endl;
#endif
    val *= vald;
  }
  if( theMultiplyingDistribution ) {
    G4double vald = theMultiplyingDistribution->GetValueFromStep(aStep);
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << " GmVPrimitiveScorer::FillScorer MultiplyingDistribution "  << primitiveName << " " << val << " * " << vald << " = " << val*vald << G4endl;
#endif
    val *= vald;
  }

  G4double valwei = val*wei;
  EvtMap->add(index,valwei);
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << GetName() 
	 << " index " << index 
	 << " val " << val 
	 << " wei " << wei << G4endl;
#endif
  
  if( bScoreErrors ) {
    theSumV_tmp[index] += val*wei; 
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << GetName() << " chk theSumV_tmp " << index << " = " << theSumV_tmp[index] << G4endl;
#endif
  }
  
  sumALL += val*wei;
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << " EvtMap Number of entries " << EvtMap->entries() << " tmp " << theSumV2.size() << " SUMALL " << sumALL << G4endl;
#endif

  return true;
} 

//--------------------------------------------------------------------
void GmVPrimitiveScorer::FillScorerAtPostCheckingRegular(G4Step* aStep, G4double val, G4double wei)
{
  if( !IsRegularScoring(aStep) ){
    FillScorer( aStep, val, wei );
  } else { // use the last voxel traversed
    FillScorer( aStep, G4RegularNavigationHelper::Instance()->theStepLengths[ G4RegularNavigationHelper::Instance()->theStepLengths.size()-1 ].first, val, wei );
  }

}
 
//--------------------------------------------------------------------
void GmVPrimitiveScorer::ScoreNewEvent() 
{
  //--- Store X and X2 of last event
  G4double eventSum = 0.;

  std::map<G4int,G4double>::iterator ite;
  for( ite = theSumV_tmp.begin(); ite !=theSumV_tmp.end(); ite++ ){
    G4int idx = (*ite).first;
    //      theSumV[idx] += (*ite).second; // this magnitude is stored in the usual G4 mechanism
    theSumV2[idx] += (*ite).second*(*ite).second;
    //	(*ite).second = 0.;
    //	theNEntries[idx]+=1;
    
    /*	std::map<G4int,G4double*>::iterator itee;
	for( itee =  EvtMap->GetMap()->begin(); itee !=  EvtMap->GetMap()->end(); itee++ ){
	//	G4cout << " EvtMap " << (*itee).first << " = " << (*itee).second << G4endl;
	} */
    
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout<< GetName() << " " << idx << " EVENT SumV= " << (*ite).second << " SumV2= " <<  theSumV2[idx] << G4endl;
#endif
    if( theConvergenceTester ) {
      eventSum += (*ite).second;
    }
  }
 
  if( theConvergenceTester ) {
    theConvergenceTester->AddScore( eventSum );
  }

  theSumV_tmp.clear();

}


//--------------------------------------------------------------------
void GmVPrimitiveScorer::CalculateErrors(G4THitsMap<G4double>* RunMap)
{
  G4double nev;
  if( bScoreByEvent ) {
    nev = GmNumberOfEvent::GetNumberOfEvent();
  } else {
    nev = 1;
  }
  
  std::map<G4int,G4double*>::iterator ite;
  for(ite = RunMap->GetMap()->begin(); ite != RunMap->GetMap()->end(); ite++){
    G4double sumWX = (*(ite->second));
    theError[ite->first]  = GetError( ite->first, sumWX, nev );
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << ite->first << " CERROR " << theError[ ite->first ] << G4endl;
#endif
  }
  
}


//--------------------------------------------------------------------
G4double GmVPrimitiveScorer::GetError( G4int index )
{
  return theError[index];
}

//--------------------------------------------------------------------
G4double GmVPrimitiveScorer::GetError( G4int index, G4double sumWX, G4double nEvents )
{
  if( theSumV2.find( index ) == theSumV2.end() ) return 0.;
  //  G4double error = (theSumV2[index]*nEvents - sumWX*sumWX) / (nEvents*nEvents*(nEvents-1));
  G4double error = (theSumV2[index]*nEvents - sumWX*sumWX) / (nEvents-1);
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << " GetError " << index << " e= " << error/(nEvents*nEvents) << " S1 " << theSumV2[index]*nEvents << " S2 " << sumWX*sumWX << " S3 " << (nEvents*nEvents*(nEvents-1)) << G4endl;
#endif
  
  if( error <= 0. ) {
    if( error < -1.E-30 ) G4cerr << " !!WARNING  Error squared in scorer " << GetName() << " is negative: " << error << G4endl;
    error = 0.;
  } else {
    error = std::sqrt(error)/nEvents;
  }

#ifndef GAMOS_NO_VERBOSE
  G4cout<< ScoringVerb(debugVerb) << " GetError " << index << " e= " << error << " e2= " << error*error << " nev " << nEvents << " sumWX " << sumWX << " sumW2 " << theSumV2[index] << G4endl;
#endif
 
  //  G4cout << " GmVPrimitiveScorer::GetError " << index << " = " << error << " sumW " << sumWX << " sumW2 " << theSumV2[index] << " nEvents " << nEvents << G4endl;
  return error;
}

//--------------------------------------------------------------------
G4double GmVPrimitiveScorer::GetErrorRelative( G4int index, G4double sumWX, G4double nEvents )
{
  G4double errorrel = GetError( index );
#ifndef GAMOS_NO_VERBOSE
   if( ScoringVerb(debugVerb) )  G4cout << " GetErrorRelative temp er " << errorrel << G4endl;
#endif
  // divide by averageX
  if( bScoreByEvent ) {
    if( sumWX != 0. ) errorrel /= (sumWX/nEvents); 
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << " GetErrorRelative " << index << " er= " << errorrel << " nev " << nEvents << " sumWX " << sumWX << G4endl;
#endif
  } else {
    if( sumWX != 0. ) errorrel /= sumWX;    
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << " GetErrorRelative " << index << " er= " << errorrel << " sumWX " << sumWX << G4endl;
#endif
  }

  return errorrel;
}

//--------------------------------------------------------------------
G4bool GmVPrimitiveScorer::IsRegularScoring( G4Step*aStep )
{
 
  if( bUseClassifierIndex ||
      !bSkipEqualMaterials ||
      !theRegularParamUtils->IsPhantomVolume( aStep->GetPreStepPoint()->GetPhysicalVolume() ) 
      ) {
    return FALSE;
  } else {
    return TRUE;
  }

}

//--------------------------------------------------------------------
G4bool GmVPrimitiveScorer::AcceptByFilter( G4Step*aStep )
{
  if(theFilter) { 
    if( !theFilter->IsFutureFilter() ) {
      if(!(theFilter->AcceptStep(aStep))) return false; 

    } else {
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(infoVerb) ) G4cout << " GmVPrimitiveScorer::AcceptByFilter " << GetName() << " futureFilter " << theFilter->GetName() << G4endl;
#endif
      GmStepMgr* stepMgr = GmStepMgr::GetInstance();
      GmFutureFilter * futureFilter = (GmFutureFilter*)(theFilter);
      
      if( futureFilter->AcceptFutureStep(aStep) ) {
	//      stepMgr->SaveStep( aStep ); // it is not saved by GmFutureFilterUA yet
	std::vector<G4Step*> steps = stepMgr->GetSteps( futureFilter, aStep );
	std::vector<G4Step*>::const_iterator ites;
	for( ites = steps.begin(); ites != steps.end(); ites++ ) {
#ifndef GAMOS_NO_VERBOSE
	  if( ScoringVerb(infoVerb) ) G4cout << " GmVPrimitiveScorer::AcceptByFilter " << GetName() << " futureFilter invoking scorer for step" << G4endl;
#endif
	  ProcessHits( *ites, (G4TouchableHistory*)0 );
	}
      }
    }

  }
  return true;
}

//--------------------------------------------------------------------
void GmVPrimitiveScorer::SetUnit( const G4String& unitName, G4double val )
{
  theUnitName = unitName;
  theUnit = val;

}

//--------------------------------------------------------------------
void GmVPrimitiveScorer::RegisterMFD( G4MultiFunctionalDetector * mfd ) 
{
  if( theMFD ) {
    G4Exception("GmVPrimitiveScorer::RegisterMFD",
		"MFD already assigned to this scorer",
		FatalException,
		(G4String("May be you are using scorer: ") + GetName() + " for two MFD's?").c_str());
  }
  theMFD = mfd;
  if( bUseVolume ) {
    bIntegrateVolumes = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(primitiveName+":IntegrateVolumes",0));
    if( bIntegrateVolumes ) CalculateTotalVolume();
  }
}



//--------------------------------------------------------------------
void GmVPrimitiveScorer::CalculateTotalVolume()
{
  theTotalVolume = 0.;
  std::vector<G4LogicalVolume*> volumes = GetMFDVolumes();
  GmGeometryUtils* geomUtils = GmGeometryUtils::GetInstance();
  for( size_t ii = 0; ii < volumes.size(); ii++ ) {
    G4int nTouchables = geomUtils->GetTouchables( volumes[ii]->GetName() ).size();
    theTotalVolume += volumes[ii]->GetSolid()->GetCubicVolume() * nTouchables;
  }

}


//--------------------------------------------------------------------
std::vector<G4LogicalVolume*> GmVPrimitiveScorer::GetMFDVolumes()
{
  std::vector<G4LogicalVolume*> volumes;
  const G4LogicalVolumeStore* lvs = G4LogicalVolumeStore::GetInstance();
  std::vector<G4LogicalVolume*>::const_iterator lvcite;
  for( lvcite = lvs->begin(); lvcite != lvs->end(); lvcite++ ) {
    if( (*lvcite)->GetSensitiveDetector() == theMFD ) {
      volumes.push_back( (*lvcite) );
    }
  }

  return volumes;
}

//--------------------------------------------------------------------
G4double GmVPrimitiveScorer::GetVolume( const G4Step* aStep )
{
  if( bIntegrateVolumes ) {
    return theTotalVolume;
  } else {
    return theGeometryUtils->GetCubicVolume( aStep->GetPreStepPoint()->GetPhysicalVolume() );
  }
}
 
