#include "GmVData2DHisto.hh"
#include "GmDataVerbosity.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4Run.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------
GmVData2DHisto::GmVData2DHisto()
{

}

//----------------------------------------------------------------
GmVData2DHisto::~GmVData2DHisto()
{

}

//----------------------------------------------------------------
void GmVData2DHisto::BookHisto( G4String hNamePrefix, G4int hNumber, G4int index, GmAnalysisMgr* anaMgr )
{
  G4String sepa = GmParameterMgr::GetInstance()->GetStringValue("Histos:Separator",":");
  G4String hName = hNamePrefix+sepa+theName;

  anaMgr->CreateHisto2D(hName, theSubData[0]->GetHNBins(), theSubData[0]->GetHMin(), theSubData[0]->GetHMax(),
			theSubData[1]->GetHNBins(), theSubData[1]->GetHMin(), theSubData[1]->GetHMax(),
			hNumber);
  theHistos2[index] = anaMgr->GetHisto2D(hNumber);

}


//----------------------------------------------------------------
void GmVData2DHisto::FillHisto( const G4Step* aStep, G4int index )
{
  G4double data1 = theSubData[0]->GetValueFromStep( aStep, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVData2DHisto::FillHisto Step data1 " <<  theSubData[0]->GetName() << " = " << data1 << G4endl; 
#endif
  G4double data2 = theSubData[1]->GetValueFromStep( aStep, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVData2DHisto::FillHisto Step data2 " <<  theSubData[1]->GetName() << " = " << data2 << G4endl; 
#endif
  theHistos2[index]->Fill( data1, data2, aStep->GetTrack()->GetWeight());
}


//----------------------------------------------------------------
void GmVData2DHisto::FillHisto( const G4Track* aTrack, G4int index )
{
  G4double weight = 1.;
  if( aTrack ) weight = aTrack->GetWeight(); // Event or Run DataUser do not pass G4Track pointer

  G4double data1 = theSubData[0]->GetValueFromTrack( aTrack, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVData2DHisto::FillHisto Track data1 " << theSubData[0]->GetName() << " = " << data1 << G4endl; 
#endif
  G4double data2 = theSubData[1]->GetValueFromTrack( aTrack, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVData2DHisto::FillHisto Track data2 " <<  theSubData[1]->GetName() << " = " << data2 << G4endl; 
#endif
  theHistos2[index]->Fill( data1, data2, weight);
  
}

//----------------------------------------------------------------
void GmVData2DHisto::FillHisto( const G4Track* aTrack1, const G4Track* aTrack2, G4int index )
{ 
  //    G4cout << " fill histo " << data->GetName() << " data1 " << theSubData[0]->GetName() << " data2 "<< theSubData[1]->GetName() << G4endl;
  G4double data1 = theSubData[0]->GetValueFromSecoTrack( aTrack1, aTrack2, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVData2DHisto::FillHisto SecoTrack data1 " <<  theSubData[0]->GetName() << " = " << data1 << G4endl; 
#endif
  G4double data2 = theSubData[1]->GetValueFromSecoTrack( aTrack1, aTrack2, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVData2DHisto::FillHisto SecoTrack data2 " <<  theSubData[1]->GetName() << " = " << data2 << G4endl; 
#endif
  theHistos2[index]->Fill( data1, data2, aTrack2->GetWeight());

}


//----------------------------------------------------------------
void GmVData2DHisto::FillHisto( const G4Event* aEvent, G4int index )
{
  G4double weight = 1.;
  if( aEvent ) weight = aEvent->GetPrimaryVertex(0)->GetWeight(); // Event or Run DataUser do not pass G4Event pointer

  G4double data1 = theSubData[0]->GetValueFromEvent( aEvent, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVData2DHisto::FillHisto Event data1 " << theSubData[0]->GetName() << " = " << data1 << G4endl; 
#endif
  G4double data2 = theSubData[1]->GetValueFromEvent( aEvent, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVData2DHisto::FillHisto Event data2 " <<  theSubData[1]->GetName() << " = " << data2 << G4endl; 
#endif
  theHistos2[index]->Fill( data1, data2, weight);
  
}

//----------------------------------------------------------------
void GmVData2DHisto::FillHisto( const G4Run* aRun, G4int index )
{
  G4double weight = 1.;

  G4double data1 = theSubData[0]->GetValueFromRun( aRun, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVData2DHisto::FillHisto Run data1 " << theSubData[0]->GetName() << " = " << data1 << G4endl; 
#endif
  G4double data2 = theSubData[1]->GetValueFromRun( aRun, index );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmVData2DHisto::FillHisto Run data2 " <<  theSubData[1]->GetName() << " = " << data2 << G4endl; 
#endif
  theHistos2[index]->Fill( data1, data2, weight);
  
}
