#include "GmStringDataFilter.hh"
#include "GamosCore/GamosData/Management/include/GmDataVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Track.hh"
#include "G4UnitsTable.hh"

//----------------------------------------------------------------------------
GmStringDataFilter::GmStringDataFilter(G4String name)
  :GmVFilter(name)
{
}

//----------------------------------------------------------------------------
GmStringDataFilter::~GmStringDataFilter()
{
}

//----------------------------------------------------------------------------
G4bool GmStringDataFilter::AcceptStep(const G4Step* aStep)
{
  G4bool bFound = FALSE;
  std::set<G4String>::const_iterator ite;
  G4String stringData = theDataS[0]->GetStringValueFromStep( aStep );
  for( ite = theStrings.begin(); ite != theStrings.end(); ite++ ) {
    if( GmGenUtils::AreWordsEquivalent( *ite, stringData ) ) {
      bFound = TRUE;
      break;
    }
  }
    
  if( bFound ) {
#ifndef GAMOS_NO_VERBOSE
    if( DataVerb(debugVerb) ) G4cout << " GmStringDataFilter::AcceptStep 1 " <<  theDataS[0]->GetStringValueFromStep( aStep ) << G4endl;
#endif
    return TRUE;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( DataVerb(debugVerb) ) G4cout << " GmStringDataFilter::AcceptStep 0 " <<  theDataS[0]->GetStringValueFromStep( aStep ) << G4endl;
#endif
    return FALSE;
  }
}

//----------------------------------------------------------------------------
G4bool GmStringDataFilter::AcceptTrack(const G4Track* aTrack)
{
  G4bool bFound = FALSE;
  std::set<G4String>::const_iterator ite;
  G4String stringData = theDataS[0]->GetStringValueFromTrack( aTrack );
  for( ite = theStrings.begin(); ite != theStrings.end(); ite++ ) {
    if( GmGenUtils::AreWordsEquivalent( *ite, stringData ) ) {
      bFound = TRUE;
      break;
    }
  }
  
  if( bFound ) {
#ifndef GAMOS_NO_VERBOSE
    if( DataVerb(debugVerb) ) G4cout << " GmStringDataFilter::AcceptTrack 1 " <<  theDataS[0]->GetStringValueFromTrack( aTrack ) << G4endl;
#endif
    return TRUE;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( DataVerb(debugVerb) ) G4cout << " GmStringDataFilter::AcceptTrack 0 " <<  theDataS[0]->GetStringValueFromTrack( aTrack ) << G4endl;
#endif
    return FALSE;
  }
   
}

//----------------------------------------------------------------------------
G4bool GmStringDataFilter::AcceptStackedTrack(const G4Track* aTrack)
{
  G4bool bFound = FALSE;
  std::set<G4String>::const_iterator ite;
  G4String stringData = theDataS[0]->GetStringValueFromStackedTrack( aTrack );
  for( ite = theStrings.begin(); ite != theStrings.end(); ite++ ) {
    if( GmGenUtils::AreWordsEquivalent( *ite, stringData ) ) {
      bFound = TRUE;
      break;
    }
  }
  
  if( bFound ) {
#ifndef GAMOS_NO_VERBOSE
    if( DataVerb(debugVerb) ) G4cout << " GmStringDataFilter::AcceptStackedTrack 1 " <<  theDataS[0]->GetStringValueFromStackedTrack( aTrack ) << G4endl;
#endif
    return TRUE;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( DataVerb(debugVerb) ) G4cout << " GmStringDataFilter::AcceptStackedTrack 0 " <<  theDataS[0]->GetStringValueFromStackedTrack( aTrack ) << G4endl;
#endif
    return FALSE;
  }
   
}

//----------------------------------------------------------------------------
void GmStringDataFilter::show() 
{
}

//----------------------------------------------------------------------------
void GmStringDataFilter::SetParameters( std::vector<G4String>& params)
{
  if( params.size() == 0 ){
    G4Exception("GmStringDataFilter::SetParameters",
		"There should be at least one parameter: DATA_NAME ",
		FatalErrorInArgument,
		"There are none");
  }
  
  GmVData* data = Build1Data( params[0] );
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(infoVerb) ) G4cout << " GmStringDataFilter::SetParameters " << params[0] << G4endl;
#endif
  data->SetName( params[0] );
  G4cout << "GmStringDataFilter::SetParameters set DATA " << data->GetName() << G4endl; //GDEB
  if( static_cast<GmVDataString*>(data) == 0 ) {
      //  if( theData[0]->GetCType() != "char" ) {
    G4Exception("GmStringDataFilter::SetParameters",
		"Data is not of string type",
		JustWarning,
		G4String("Data is " + params[0] + " Maybe you want to use GmStringDataFilter?").c_str());
  } else {
    theDataS.push_back( static_cast<GmVDataString*>(data) );
    G4cout << "GmStringDataFilter::SetParameters set DATA N " << theDataS.size() << G4endl; //GDEB
  }
  
  if( params.size() == 1 ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("GmStringDataFilter::SetParameters",
		"There should at least two parameters: DATA_NAME VALUE_1 VALUE_2 ... ",
		FatalErrorInArgument,
		G4String("They are: "+parastr).c_str());
  }
  
  for( unsigned int ii = 1; ii < params.size(); ii++ ){
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(infoVerb) ) G4cout << " GmStringDataFilter::SetParameters DataValue " << params[ii] << G4endl;
#endif
    theStrings.insert( params[ii] );
  }
  
}
