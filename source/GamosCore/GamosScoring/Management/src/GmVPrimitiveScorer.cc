#include "GamosCore/GamosUserActionMgr/include/GmStepMgr.hh"
#include "G4VPrimitiveScorer.hh"

#include "GmCompoundScorer.hh"
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
#include "G4UnitsTable.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"
#else
#include "GmPSPrinterFactory.hh"
#endif

//--------------------------------------------------------------------
GmVPrimitiveScorer::GmVPrimitiveScorer(G4String name) 
  : G4VPrimitiveScorer( name, 0 )
{ 
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << this << " GmVPrimitiveScorer::GmVPrimitiveScorer " << name << G4endl;
#endif

  sumALL = 0.;

  HCID = -1;
  fWeighted = TRUE;
  bScoreErrors = TRUE;
  theNEventsType = SNET_ByEvent;
  theUnit = 1.;
  theUnitName = G4String("");

  AddDefaultClassifier();
  theFilter = 0;
  bNewEvent = TRUE;

  GmParameterMgr* paramMgr = GmParameterMgr::GetInstance();
  G4String dataName = paramMgr->GetStringValue(primitiveName+":MultiplyByData","");
  if( dataName != "" ) {
    theMultiplyingData = GmDataMgr::GetInstance()->BuildData(dataName);
  } else {
    theMultiplyingData = 0;
  }

  G4String distName = paramMgr->GetStringValue(primitiveName+":MultiplyByDistribution","");
  if( distName != "" ) {
    std::vector<G4String> params;
    params.push_back(distName);
    params.push_back(distName);
    theMultiplyingDistribution =  GmDistributionMgr::GetInstance()->FindOrBuildDistribution(params);
  } else {
    theMultiplyingDistribution = 0;
  }

  G4String convergence = paramMgr->GetStringValue(primitiveName+":ConvergenceTester","");
  if( convergence != "") {
    theConvergenceTester = new GmConvergenceTester(convergence); 
  } else {
    theConvergenceTester = 0;
  }

  theEnergySplitter = new GmEnergySplitter;

  bSkipEqualMaterials = G4bool(paramMgr->GetNumericValue("GmReadPhantomGeometry:Phantom:SkipEqualMaterials",0));
  bUseClassifierIndex = G4bool(paramMgr->GetNumericValue(primitiveName+":UseClassifierIndex",0));

  theGeomUtils = GmGeometryUtils::GetInstance();
  bUseVolume = false;
  theMFD = 0;
  bDefaultPrinter = true;
  bSubScorer = false;

  theMinDensity = paramMgr->GetNumericValue( primitiveName+":MinimumDensity", -DBL_MAX );

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

  if( thePrinters.size() == 0 && bDefaultPrinter ) {
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
  G4String param = "GmPSPrinterCout";
#ifdef ROOT5
  GmVPSPrinter* psp = Reflex::PluginService::Create<GmVPSPrinter*>(param,param);
#else
  GmVPSPrinter* psp = GmPSPrinterFactory::get()->create(param,param);
#endif
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
  // if( ScoringVerb(debugVerb) ) G4cout << EvtMap << " EvtMap Number of entries " << EvtMap->entries() << " tmp " << theSumV_tmp.size() << " val " << val << " weight " << wei << G4endl;
#endif
  
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
    //    if( ScoringVerb(debugVerb) ) G4cout << this <<  GetName() << " chk theSumV_tmp " << index << " = " << theSumV_tmp[index] << G4endl;
#endif
  }
  
  sumALL += val*wei;
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << GetName() << " EvtMap Nindices " << EvtMap->entries() << " SUMALL " << sumALL << G4endl;
#endif
  if( theNEventsType == SNET_ByNFilled ) {
    theNFilled_tmp.insert(index);
  }
  
  return true;
} 
 
//--------------------------------------------------------------------
void GmVPrimitiveScorer::SumEndOfEvent() 
{
  GmCompoundScorer* scorerCp = dynamic_cast<GmCompoundScorer*>(this);
  if( scorerCp != 0 && EvtMap ) {
    scorerCp->BuildCompoundScores();
  }
  
  //--- For CompoundScorer, redo EvtMap and theSumV_tmp
  //--- set unitGmVPrimitiveScorer

  //--- Store X and X2 of last event
  G4double eventSum = 0.;

  std::map<G4int,G4double>::iterator ite;
  for( ite = theSumV_tmp.begin(); ite != theSumV_tmp.end(); ite++ ){
    G4int idx = (*ite).first;
    //      theSumV[idx] += (*ite).second; // this magnitude is stored in the usual G4 mechanism
    theSumV2[idx] += (*ite).second*(*ite).second;
    //    G4cout << " " << GetName() << " GmVPrimitiveScorer::SumEndOfEvent " << idx << " SUMV2 " << theSumV2[idx] << " " << (*ite).second <<G4endl;    //GDEB
    //	theNEntries[idx]+=1;
    
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << GetName() << " " << idx << " EVENT SumV2= " << (*ite).second*(*ite).second << " total= " << theSumV2[idx] << " from_score " << (*ite).second << G4endl;
#endif
    if( theConvergenceTester ) {
      eventSum += (*ite).second;
    }
  }

  std::set<size_t>::iterator itenf;
  for( itenf = theNFilled_tmp.begin(); itenf !=theNFilled_tmp.end(); itenf++ ){
    theNFilled[*itenf]++;
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << GetName() << " GmVPrimitiveScorer " << *itenf << " NFilled++ " << theNFilled[*itenf] << G4endl;
#endif
  }
  
  if( theConvergenceTester ) {
    theConvergenceTester->AddScore( eventSum );
  }

  if( !bSubScorer ) {
    theSumV_tmp.clear();
    theNFilled_tmp.clear();
    //    G4cout << this << GetName() << " NO bSubScorer CLEAR SumV " << theSumV_tmp.size() <<G4endl; //GDEB
  }
  
}

//--------------------------------------------------------------------
void GmVPrimitiveScorer::ClearSumV_tmp()
{
  theSumV_tmp.clear();
}

//--------------------------------------------------------------------
void GmVPrimitiveScorer::ClearNFilled_tmp()
{
  theNFilled_tmp.clear();
}

//--------------------------------------------------------------------
G4double GmVPrimitiveScorer::GetNEvents( G4int index ){
  //  G4cout << index << " GmVPrimitiveScorer::GetNEvents( " << theNEventsType << G4endl; //GDEB
  switch (theNEventsType) {
    case SNET_ByRun:
      return 1;
    case SNET_ByEvent:
      return GmNumberOfEvent::GetNumberOfEvent();
    case SNET_ByNFilled:
      return theNFilled[index];
  }
  
  return 1;
}

//--------------------------------------------------------------------
void GmVPrimitiveScorer::Normalize(G4THitsMap<G4double>* RunMap)
{
  std::map<G4int,G4double*>::iterator ite;
  for(ite = RunMap->GetMap()->begin(); ite != RunMap->GetMap()->end(); ite++){
    G4int index = ite->first;
    G4double nev = GetNEvents( index );
    G4double normFactor = nev*theUnit;
    G4double sumX = (*(ite->second))/normFactor;
    RunMap->set(index,sumX);
    if( ScoringVerb(debugVerb) ) G4cout << GetName() << " " << index << " Normalize SUMV2_bef " << theSumV2[index] << G4endl;
    theSumV2[index] = theSumV2[index] / (normFactor*normFactor);
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << GetName() << " " << index << " Normalize " << (*(ite->second)) << " " << sumX*normFactor << " / " << normFactor << " nev " << nev << " unit " << theUnit << G4endl;
    if( ScoringVerb(debugVerb) ) G4cout << GetName() << " " << index << " Normalize SUMV2_aft " << theSumV2[index] << G4endl;
#endif
  }
  
}

//--------------------------------------------------------------------
void GmVPrimitiveScorer::CalculateErrors(G4THitsMap<G4double>* RunMap)
{
  
  std::map<G4int,G4double*>::iterator ite;
  for(ite = RunMap->GetMap()->begin(); ite != RunMap->GetMap()->end(); ite++){
    G4double sumWX = (*(ite->second));
    G4int index = ite->first;
    G4double nev = GetNEvents( index );
    theError[index] = GetError( index, sumWX, nev );
#ifndef GAMOS_NO_VERBOSE
    //    if( ScoringVerb(debugVerb) ) G4cout << GetName() << " " << ite->first << " CalculateError " << theError[ index ] << G4endl;
    if( ScoringVerb(debugVerb) ) G4cout << GetName() << " " << index << " CalculateError " << theError[ index ] << " sumWX " << sumWX << " nev " << nev <<G4endl;
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
  G4double error = 0.;
  if( nEvents > 1 ) error = (theSumV2[index]*nEvents - sumWX*sumWX) / (nEvents-1);
    //  G4cout << " ERROR " << (theSumV2[index]*nEvents - sumWX*sumWX) << " " << theSumV2[index]*nEvents << " - " << sumWX*sumWX << G4endl; //GDEB
#ifndef GAMOS_NO_VERBOSE
  G4double normF = nEvents*theUnit;
  if( ScoringVerb(debugVerb) ) G4cout << GetName() <<" GetError " << index << " e2= " << error*normF*normF << " S1 " << theSumV2[index]/nEvents << " S2 " << sumWX*sumWX*normF*normF << " S3 " << nEvents-1 << G4endl;
  //t  if( ScoringVerb(debugVerb) ) G4cout << GetName() <<" GetError " << index << " e2= " << error << " S1 " << theSumV2[index]/nEvents << " S2 " << sumWX*sumWX << " S3 " << nEvents-1 << G4endl;
#endif
  
  if( error <= 0. ) {
    if( error/sumWX < -1.E-3 ) G4cerr << " !!WARNING  Error squared in scorer " << GetName() << " index= " << index << " is negative: " << error << " value " << sumWX << G4endl;
    //    if( error < -1.E-15 ) G4cerr << " !!WARNING  Error squared in scorer " << GetName() << " index= " << index << " is negative: " << error << G4endl;
    error = 0.;
  } else {
    error = std::sqrt(error);
  }
  //  if( index%1000 == 1 ) G4cout << " DOSE GetError " << index << " " << sumWX << " +- " << error << " dose2 " << theSumV2[index] << " nev " << nEvents << G4endl; //GDEB

#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << " GetError " << index << " e= " << error << " e2= " << error*error << " nev " << nEvents << " sumWX " << sumWX << " sumW2 " << theSumV2[index] << G4endl;
#endif
  //  if( index%1000 == 0 ) G4cout   << " GetError " << index << " e= " << error << " e2= " << error*error << " nev " << nEvents << " sumWX " << sumWX << " sumW2 " << theSumV2[index] << G4endl; //GDEB 
 
  //  G4cout << " GmVPrimitiveScorer::GetError " << index << " = " << error << " sumW " << sumWX << " sumW2 " << theSumV2[indqex] << " nEvents " << nEvents << G4endl;
  return error;
}

//--------------------------------------------------------------------
G4double GmVPrimitiveScorer::GetErrorRelative( G4int index, G4double sumWX)
{
  G4double errorrel = GetError( index );
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) )  G4cout << " GetErrorRelative temp er " << errorrel << G4endl;
#endif
  // divide by averageX
  if( sumWX != 0. ) errorrel /= sumWX; 
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << " GetErrorRelative " << index << " er= " << errorrel << " nev " << " sumWX " << sumWX << G4endl;
#endif

  return errorrel;
}

//--------------------------------------------------------------------
G4bool GmVPrimitiveScorer::IsRegularScoring( G4Step*aStep )
{
 
  if( bUseClassifierIndex ||
      !bSkipEqualMaterials ||
      !theGeomUtils->IsPhantomVolume( aStep->GetPreStepPoint()->GetPhysicalVolume() ) 
      ) {
    return FALSE;
  } else {
    return TRUE;
  }

}

//--------------------------------------------------------------------
G4bool GmVPrimitiveScorer::AcceptByFilter( G4Step*aStep )
{
  //  G4cout << " GmVPrimitiveScorer::AcceptByFilter( " << G4endl; //GDEB
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
void GmVPrimitiveScorer::SetUnit( const G4String& unitName2, G4double val )
{
  theUnitName = unitName2;
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
  for( size_t ii = 0; ii < volumes.size(); ii++ ) {
    G4int nTouchables = theGeomUtils->GetTouchables( volumes[ii]->GetName() ).size();
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
    return theGeomUtils->GetCubicVolume( aStep->GetPreStepPoint()->GetPhysicalVolume() );
  }
}

//--------------------------------------------------------------------
std::map<G4int,size_t> GmVPrimitiveScorer::GetNFilled() const
{
  return theNFilled;
}

//--------------------------------------------------------------------
void GmVPrimitiveScorer::PrintAll()
{
  G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
  G4cout << " PrimitiveScorer " << GetName() << G4endl;
  G4cout << " Number of entries " << EvtMap->entries() << G4endl;
  std::map<G4int,G4double*>::iterator itr = EvtMap->GetMap()->begin();
  for(; itr != EvtMap->GetMap()->end(); itr++) {
    G4cout << "  copy no.: " << itr->first
	   << "  val: " << G4BestUnit(*(itr->second),"Energy") 
	   << G4endl;
  }
}

void GmVPrimitiveScorer::EndOfEvent(G4HCofThisEvent*)
{;}

void GmVPrimitiveScorer::DrawAll()
{;}

