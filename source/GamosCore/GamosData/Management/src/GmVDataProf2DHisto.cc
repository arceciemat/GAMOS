#include "GmVDataProf2DHisto.hh"
#include "GmDataVerbosity.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4Run.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmVDataProf2DHisto::GmVDataProf2DHisto() 
{

}

//----------------------------------------------------------------
GmVDataProf2DHisto::~GmVDataProf2DHisto()
{

}

//----------------------------------------------------------------
void GmVDataProf2DHisto::BookHisto( G4String hNamePrefix, G4int hNumber, G4int index, GmAnalysisMgr* anaMgr )
{
  G4String sepa = GmParameterMgr::GetInstance()->GetStringValue("Histos:Separator",":");
  G4String hName = hNamePrefix+sepa+theName;

  anaMgr->CreateHistoProfile2D(hName
			    , theSubData[0]->GetHNBins(), theSubData[0]->GetHMin(), theSubData[0]->GetHMax(),
			    theSubData[1]->GetHNBins(), theSubData[1]->GetHMin(), theSubData[1]->GetHMax(),
			hNumber);
  theHistosProf2[index] = anaMgr->GetHistoProf2(hNumber);

}


//----------------------------------------------------------------
void GmVDataProf2DHisto::FillHisto( const G4Step* aStep, G4int index )
{
  G4double data1 = theSubData[0]->GetValueFromStep( aStep, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataProf2DHisto::FillHisto Step data1 " <<  theSubData[0]->GetName() << " = " << data1 << G4endl; 
#endif
  G4double data2 = theSubData[1]->GetValueFromStep( aStep, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataProf2DHisto::FillHisto Step data2 " <<  theSubData[1]->GetName() << " = " << data2 << G4endl; 
#endif
  G4double data3 = theSubData[2]->GetValueFromStep( aStep, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataProf2DHisto::FillHisto Step data3 " <<  theSubData[1]->GetName() << " = " << data3 << G4endl; 
#endif
  theHistosProf2[index]->Fill( data1, data2, data3, aStep->GetTrack()->GetWeight());
}


//----------------------------------------------------------------
void GmVDataProf2DHisto::FillHisto( const G4Track* aTrack, G4int index )
{
  G4double weight = 1.;
  if( aTrack ) weight = aTrack->GetWeight(); // Event or Run DataUser do not pass G4Track pointer

  G4double data1 = theSubData[0]->GetValueFromTrack( aTrack, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataProf2DHisto::FillHisto Track data1 " << theSubData[0]->GetName() << " = " << data1 << G4endl; 
#endif
  G4double data2 = theSubData[1]->GetValueFromTrack( aTrack, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataProf2DHisto::FillHisto Track data2 " <<  theSubData[1]->GetName() << " = " << data2 << G4endl; 
#endif
  G4double data3 = theSubData[1]->GetValueFromTrack( aTrack, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataProf2DHisto::FillHisto Track data3 " <<  theSubData[1]->GetName() << " = " << data3 << G4endl; 
#endif
  theHistosProf2[index]->Fill( data1, data2, data3, weight);
  
}

//----------------------------------------------------------------
void GmVDataProf2DHisto::FillHisto( const G4Track* aTrack1, const G4Track* aTrack2, G4int index )
{ 
  //    G4cout << " fill histo " << data->GetName() << " data1 " << theSubData[0]->GetName() << " data2 "<< theSubData[1]->GetName() << G4endl;
  G4double data1 = theSubData[0]->GetValueFromSecoTrack( aTrack1, aTrack2, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataProf2DHisto::FillHisto SecoTrack data1 " <<  theSubData[0]->GetName() << " = " << data1 << G4endl; 
#endif
  G4double data2 = theSubData[1]->GetValueFromSecoTrack( aTrack1, aTrack2, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataProf2DHisto::FillHisto SecoTrack data2 " <<  theSubData[1]->GetName() << " = " << data2 << G4endl; 
#endif
  G4double data3 = theSubData[1]->GetValueFromSecoTrack( aTrack1, aTrack2, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataProf2DHisto::FillHisto SecoTrack data3 " <<  theSubData[1]->GetName() << " = " << data3 << G4endl; 
#endif
  theHistosProf2[index]->Fill( data1, data2, data3, aTrack2->GetWeight());

}


//----------------------------------------------------------------
void GmVDataProf2DHisto::FillHisto( const G4Event* aEvent, G4int index )
{
  G4double data1 = theSubData[0]->GetValueFromEvent( aEvent, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataProf2DHisto::FillHisto Event data1 " <<  theSubData[0]->GetName() << " = " << data1 << G4endl; 
#endif
  G4double data2 = theSubData[1]->GetValueFromEvent( aEvent, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataProf2DHisto::FillHisto Event data2 " <<  theSubData[1]->GetName() << " = " << data2 << G4endl; 
#endif
  G4double data3 = theSubData[2]->GetValueFromEvent( aEvent, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataProf2DHisto::FillHisto Event data3 " <<  theSubData[1]->GetName() << " = " << data3 << G4endl; 
#endif
  theHistosProf2[index]->Fill( data1, data2, data3, aEvent->GetPrimaryVertex(0)->GetWeight());
}


//----------------------------------------------------------------
void GmVDataProf2DHisto::FillHisto( const G4Run* aRun, G4int index )
{
  G4double data1 = theSubData[0]->GetValueFromRun( aRun, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataProf2DHisto::FillHisto Run data1 " <<  theSubData[0]->GetName() << " = " << data1 << G4endl; 
#endif
  G4double data2 = theSubData[1]->GetValueFromRun( aRun, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataProf2DHisto::FillHisto Run data2 " <<  theSubData[1]->GetName() << " = " << data2 << G4endl; 
#endif
  G4double data3 = theSubData[2]->GetValueFromRun( aRun, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVDataProf2DHisto::FillHisto Run data3 " <<  theSubData[1]->GetName() << " = " << data3 << G4endl; 
#endif
  theHistosProf2[index]->Fill( data1, data2, data3, 1.);
}


