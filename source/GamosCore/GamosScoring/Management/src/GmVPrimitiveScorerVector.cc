#include "G4VPrimitiveScorer.hh"
#include "G4EnergyLossForExtrapolator.hh"
#include "GmVPrimitiveScorerVector.hh"
#include "GmVPSPrinter.hh"
#include "GmScoringVerbosity.hh"

#include "GamosCore/GamosBase/Classifiers/include/GmClassifierBy1Ancestor.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"
//#include "GamosCore/GamosGeometry/include/GmParallelToMassUA.hh"

#include "G4PhantomParameterisation.hh"
#include "G4EmCalculator.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4VSolid.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"
#else
#include "GmPSPrinterFactory.hh"
#endif

//--------------------------------------------------------------------
GmVPrimitiveScorerVector::GmVPrimitiveScorerVector(G4String name) 
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

  theNIndices = G4int(GmParameterMgr::GetInstance()->GetNumericValue(primitiveName+":NIndices",1E6));

}

//--------------------------------------------------------------------
GmVPrimitiveScorerVector::~GmVPrimitiveScorerVector()
{ 
}


//--------------------------------------------------------------------
G4int GmVPrimitiveScorerVector::GetIndex(G4Step* aStep)
{
  return theClassifier->GetIndexFromStep( aStep );
}


//--------------------------------------------------------------------
void GmVPrimitiveScorerVector::DumpAll(std::vector<G4double>* RunMap)
{
  //  G4cout <<" GmVPrimitiveScorerVector::DumpAll " << G4endl;

  if( thePrinters.size() == 0 ) {
    AddDefaultPrinter();
  }

  std::vector<GmVPSPrinter*>::const_iterator ite;
  for(ite = thePrinters.begin(); ite !=  thePrinters.end(); ite++ ){
    (*ite)->DumpAllVector( RunMap, this );
  }
}

//--------------------------------------------------------------------
void GmVPrimitiveScorerVector::AddPrinter( GmVPSPrinter* prt ) 
{
  thePrinters.push_back(prt); 
}
 
//--------------------------------------------------------------------
void GmVPrimitiveScorerVector::SetClassifier(GmVClassifier* idx )
{ 
  theClassifier = idx; 
}

//--------------------------------------------------------------------
void GmVPrimitiveScorerVector::AddDefaultPrinter()
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
void GmVPrimitiveScorerVector::AddDefaultClassifier()
{
  std::vector<G4String> params;
  params.push_back("0");
  theClassifier = new GmClassifierBy1Ancestor("GmClassifierBy1Ancestor");
  theClassifier->SetParameters( params );
}


//--------------------------------------------------------------------
void GmVPrimitiveScorerVector::Initialise(G4HCofThisEvent* )
{
  /*  EvtMap = new G4THitsMap<G4double>(detector->GetName(),
				    GetName());
  if(HCID < 0) {HCID = GetCollectionID(0);}
  HCE->AddHitsCollection(HCID, EvtMap); */
  EvtMap.reserve(theNIndices);
  for( G4int ii = 0; ii < theNIndices; ii++ ) {
    theSumV_tmp.push_back(0.);
    theSumV2.push_back(0.);
    theError.push_back(0.);
  }
  //    theSumV_tmp.reserve(theNIndices);
  //  theSumV2.reserve(theNIndices);
  // theError.reserve(theNIndices);
}

//--------------------------------------------------------------------
G4bool GmVPrimitiveScorerVector::FillScorer(G4Step* aStep, G4double val, G4double wei)
{
  G4int index = -1;
  if( aStep != 0 ) { // it is 0 when called by GmScoringMgr after last event
    index = GetIndex(aStep);
  }
  FillScorer( index, val, wei );

  return true;
}

//--------------------------------------------------------------------
G4bool GmVPrimitiveScorerVector::FillScorer(G4int index, G4double val, G4double wei)
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

  G4double valwei = val*wei;
  EvtMap[index] = valwei;
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
  if( ScoringVerb(debugVerb) ) G4cout << " EvtMap Number of entries " << EvtMap.size() 
	 << " tmp " << theSumV2.size() 
	 << " SUMALL " << sumALL << G4endl;
#endif

  return true;
} 


//--------------------------------------------------------------------
void GmVPrimitiveScorerVector::ScoreNewEvent() 
{
  //--- Store X and X2 of last event
  //  std::vector<G4double>::iterator ite;
  for( unsigned int idx = 0; idx < theSumV_tmp.size(); idx++ ){
    //      theSumV[idx] += (*ite).second; // this magnitude is stored in the usual G4 mechanism
    theSumV2[idx] += theSumV_tmp[idx]*theSumV_tmp[idx];
    //	(*ite).second = 0.;
    //	theNEntries[idx]+=1;
    
    /*	std::map<G4int,G4double*>::iterator itee;
	for( itee =  EvtMap->GetMap()->begin(); itee !=  EvtMap->GetMap()->end(); itee++ ){
	//	G4cout << " EvtMap " << (*itee).first << " = " << (*itee).second << G4endl;
	} */
    
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout<< GetName() << " " << idx << " theSumV= " << theSumV_tmp[idx] << " theSumV2= " <<  theSumV2[idx] << G4endl;
#endif
  }
  theSumV_tmp.clear();
}


//--------------------------------------------------------------------
void GmVPrimitiveScorerVector::CalculateErrors(std::vector<G4double>* RunMap)
{
  G4double nev;
  if( bScoreByEvent ) {
    nev = GmNumberOfEvent::GetNumberOfEvent();
  } else {
    nev = 1;
  }
  
  std::map<G4int,G4double*>::iterator ite;
  for( unsigned int idx = 0; idx < RunMap->size(); idx++ ){
    G4double sumWX = (*RunMap)[idx];
    theError[idx]  = GetError( idx, sumWX, nev );
#ifndef GAMOS_NO_VERBOSE
    if( idx%10000 == 0 && ScoringVerb(debugVerb) ) G4cout << idx << " CERROR " << theError[ idx ] << G4endl;
#endif
  }
  
}


//--------------------------------------------------------------------
G4double GmVPrimitiveScorerVector::GetError( G4int index )
{
  return theError[index];
}

//--------------------------------------------------------------------
G4double GmVPrimitiveScorerVector::GetError( G4int index, G4double sumWX, G4double nEvents )
{
  //m  if( theSumV2.find( index ) == theSumV2.end() ) return 0.;
  //  G4double error = (theSumV2[index]*nEvents - sumWX*sumWX) / (nEvents*nEvents*(nEvents-1));
  G4double error = (theSumV2[index]*nEvents - sumWX*sumWX) / (nEvents-1);
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << " GetError " << index << " e= " << error/(nEvents*nEvents) << " S1 " << theSumV2[index]*nEvents << " s2 " << sumWX*sumWX << " S3 " << (nEvents*nEvents*(nEvents-1)) << G4endl;
#endif
  
  if( error <= 0. ) {
    if( error < -1.E-30 ) G4cerr << " !!WARNING  Error squared in scorer " << GetName() << " is negative: " << error << G4endl;
    error = 0.;
  } else {
    error = std::sqrt(error)/nEvents;
  }

#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << " GetError " << index << " e= " << error << " e2= " << error*error << " nev " << nEvents << " sumWX " << sumWX << " sumW2 " << theSumV2[index] << G4endl;
#endif
 
  //  G4cout << " GmVPrimitiveScorerVector::GetError " << index << " = " << error << " sumW " << sumWX << " sumW2 " << theSumV2[index] << " nEvents " << nEvents << G4endl;
  return error;
}

//--------------------------------------------------------------------
G4double GmVPrimitiveScorerVector::GetErrorRelative( G4int index, G4double sumWX, G4double nEvents )
{
  G4double errorrel = GetError( index );
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << " GetErrorRelative temp er " << errorrel << G4endl;
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
G4bool GmVPrimitiveScorerVector::AcceptByFilter( G4Step*aStep )
{
  //  G4cout << GetName() << " gmfiltera " << theFilter << G4endl;
  if(theFilter) { 
    //    G4cout << " call gmfilter " << theFilter->Accept(aStep) << G4endl;
    if(!(theFilter->AcceptStep(aStep))) return false; 
  }
  return true;
}

//--------------------------------------------------------------------
void GmVPrimitiveScorerVector::SetUnit( const G4String& unitName2, G4double val )
{
  theUnitName = unitName2;
  theUnit = val;

}
