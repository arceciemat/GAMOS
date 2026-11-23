#include "GmGamosVerboseByEventUA.hh"
#include "GmUtilsUAVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmVVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmVerbosity.hh"

#include "G4Track.hh"
#include "G4Event.hh"

//---------------------------------------------------------------------------
GmGamosVerboseByEventUA::GmGamosVerboseByEventUA()
{
  verbCmd = new GmUIcmdWithAString("/gamos/verbosity/byEvent",this);
}

//----------------------------------------------------------------
GmGamosVerboseByEventUA::~GmGamosVerboseByEventUA() 
{
  if (verbCmd) delete verbCmd;
}

//----------------------------------------------------------------
void GmGamosVerboseByEventUA::BeginOfEventAction( const G4Event* anEvent )
{
  //----------- Set /tracking/verbose for this event 
  mmis::const_iterator ite;
  for( ite = theVerbs.begin(); ite != theVerbs.end(); ite++ ){
    if( (*ite).first == anEvent->GetEventID() ) {
#ifndef GAMOS_NO_VERBOSE
      if( UtilsUAVerb(debugVerb) ) G4cout << " GmGamosVerboseByEventUA::BeginOfEventAction " << (*ite).second.first << " = " << (*ite).second.second << G4endl;
#endif
      GmVVerbosity::SetVerbosityLevel( (*ite).second.first, (*ite).second.second );
    }
    
  }
}

//----------------------------------------------------------------
void GmGamosVerboseByEventUA::SetNewValue(G4UIcommand * command,G4String newValues)
{
  if (command == verbCmd) {
    GmGenUtils::CheckNWords(newValues,4,"Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs 4 arguments: verbosity_class verbosity_value event_min event_max"); 
    std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );

    G4String verbName = wl[0];
    G4String verbValue = wl[1];
    G4int verbVal;
    if( GmGenUtils::IsNumber( verbValue ) ) {
      verbVal = atoi( verbValue );
    } else {
      if( verbValue == "silent" ) {
	verbVal = silentVerb;
      } else if( verbValue == "error" ) {
	verbVal = errorVerb;
      } else if( verbValue == "warning" ) {
	verbVal = warningVerb;
      } else if( verbValue == "info" ) {
	verbVal = infoVerb;
      } else if( verbValue == "debug" ) {
	verbVal = debugVerb;
      } else if( verbValue == "test" ) {
	verbVal = testVerb;
      } else {
	G4Exception("GmGamosVerboseByEventUA::SetNewValue",
		    "Error",
		    FatalException,
		    G4String("Invalid value = " + verbValue).c_str());
      }
    }

    G4int eventMin = G4int(GmGenUtils::GetValue(wl[2]));
    if( eventMin < 0 ) {
      G4Exception("GmGamosVerboseByEventUA::SetNewValue",
		  "Minimum event number must be >= 0",
		  FatalErrorInArgument,
		  (G4String("Value is = ") + wl[2]).c_str());
    }
    G4int eventMax = G4int(GmGenUtils::GetValue(wl[3]));
    if( eventMax < eventMin ) {
      G4Exception("GmGamosVerboseByEventUA::SetNewValue",
		  "Maximum event number must be > minimum event number",
		  FatalErrorInArgument,
		  (G4String("Value is = ") + wl[3]).c_str());
    }

    theVerbs.insert( mmis::value_type( eventMin, std::pair<G4String,G4int>(verbName,verbVal) ));
    theVerbs.insert( mmis::value_type( eventMax+1 , std::pair<G4String,G4int>(verbName,silentVerb)));

  }
}

