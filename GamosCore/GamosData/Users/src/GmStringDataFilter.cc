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
  if( theStrings.find( theData[0]->GetStringValueFromStep( aStep ) ) != theStrings.end() ){
#ifndef GAMOS_NO_VERBOSE
    if( DataVerb(debugVerb) ) G4cout << " GmStringDataFilter::AcceptStep 1 " <<  theData[0]->GetStringValueFromStep( aStep ) << G4endl;
#endif
    return TRUE;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( DataVerb(debugVerb) ) G4cout << " GmStringDataFilter::AcceptStep 0 " <<  theData[0]->GetStringValueFromStep( aStep ) << G4endl;
#endif
    return FALSE;
  }
}

//----------------------------------------------------------------------------
G4bool GmStringDataFilter::AcceptTrack(const G4Track* aTrack)
{
  if( theStrings.find( theData[0]->GetStringValueFromTrack( aTrack ) ) != theStrings.end() ){
#ifndef GAMOS_NO_VERBOSE
    if( DataVerb(debugVerb) ) G4cout << " GmStringDataFilter::AcceptTrack 1 " <<  theData[0]->GetStringValueFromTrack( aTrack ) << G4endl;
#endif
    return TRUE;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( DataVerb(debugVerb) ) G4cout << " GmStringDataFilter::AcceptTrack 0 " <<  theData[0]->GetStringValueFromTrack( aTrack ) << G4endl;
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
  theData.push_back( data );

 if( theData[0]->GetCType() != "char" ) {
   G4Exception("GmStringDataFilter::SetParameters",
	       "Data is not of string type",
	       JustWarning,
	       G4String("Data is " + params[0] + " Maybe you want to use GmNumericDataFilter?").c_str());
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
   
 for( unsigned int ii = 0; ii < params.size(); ii++ ){
   theStrings.insert( params[ii] );
 }

}
