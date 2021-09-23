#include "GmScoringMessenger.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "GmPSPrinterMgr.hh"

#include "GamosCore/GamosBase/Base/include/GmUIcmdWithAString.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringMessenger.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringMgr.hh"

#include <sstream>

//---------------------------------------------------------------------
GmScoringMessenger::GmScoringMessenger() 
{
  theAction = GmScoringMgr::GetInstance();

  PrinterCmd = new GmUIcmdWithAString("/gamos/scoring/printer",this);
  PrinterCmd->SetGuidance("Creates a printer of one of the predefined classs (GmScorerPrinterFactory plug-in's) assigning parameters: PRINTER_NAME PRINTER_CLASS PARAMETERS");
  PrinterCmd->SetParameterName("choice",true);
  PrinterCmd->AvailableForStates(G4State_Idle);

  MFDetectorCmd = new GmUIcmdWithAString("/gamos/scoring/createMFDetector",this);
  MFDetectorCmd->SetGuidance("Creates a multi functional detector and associates it to one or several logical volumes. It must have as arguments the MFD name and then the list of LV names");
  MFDetectorCmd->SetParameterName("choice",true);
  MFDetectorCmd->AvailableForStates(G4State_Idle);

  Scorer2MFDCmd = new GmUIcmdWithAString("/gamos/scoring/addScorer2MFD",this);
  Scorer2MFDCmd->SetGuidance("Creates a scorer of one of the predefined classes or one created by the user and adds it to a multi functional detector. It must have as arguments the scorer name, the scorer class, the MFD name and the scorer parameters ");
  Scorer2MFDCmd->SetParameterName("choice",true);
  Scorer2MFDCmd->AvailableForStates(G4State_Idle);

  ScorerCmd = new GmUIcmdWithAString("/gamos/scoring/createScorer",this);
  ScorerCmd->SetGuidance("Creates a scorer of one of the predefined classes. It must have as arguments the scorer name, the scorer class and the scorer parameters (integer's)");
  ScorerCmd->SetParameterName("choice",true);
  ScorerCmd->AvailableForStates(G4State_Idle);

  FilterCmd = new GmUIcmdWithAString("/gamos/scoring/addFilter2Scorer",this);
  FilterCmd->SetGuidance("Creates a filter of one of the predefined classs (filter GmFilterFactory plug-in's) and adds it to an scorer. It must have as arguments the filter class/name and the scorer name");
  FilterCmd->SetParameterName("choice",true);
  FilterCmd->AvailableForStates(G4State_Idle);

  Printer2ScorerCmd = new GmUIcmdWithAString("/gamos/scoring/addPrinter2Scorer",this);
  Printer2ScorerCmd->SetGuidance("Creates a printer of one of the predefined classs (Gmprinter GmScorerPrinter plug-in's) and adds it to an scorer. It must have as arguments the printer class/name and the scorer name");
  Printer2ScorerCmd->SetParameterName("choice",true);
  Printer2ScorerCmd->AvailableForStates(G4State_Idle);

  ClassifierCmd = new GmUIcmdWithAString("/gamos/scoring/assignClassifier2Scorer",this);
  ClassifierCmd->SetGuidance("Creates a classifier of one of the predefined classs (GmClassifierFactory plug-in's) and adds it to an scorer. It must have as arguments the classifier class/name and the scorer name");
  ClassifierCmd->SetParameterName("choice",true);
  ClassifierCmd->AvailableForStates(G4State_Idle);
  
  TrkWeightCmd = new GmUIcmdWithAString("/gamos/scoring/useTrackWeight",this);
  TrkWeightCmd->SetGuidance("Selects if track weight is going to be used or not for an scorer. It must have as arguments the scorer name TRUE/FLASE");
  TrkWeightCmd->SetParameterName("choice",true);
  TrkWeightCmd->AvailableForStates(G4State_Idle);

  ScoreErrorsCmd = new GmUIcmdWithAString("/gamos/scoring/scoreErrors",this);
  ScoreErrorsCmd->SetGuidance("Selects if errors are scored for an scorer. It must have as arguments the scorer name TRUE/FLASE");
  ScoreErrorsCmd->SetParameterName("choice",true);
  ScoreErrorsCmd->AvailableForStates(G4State_Idle);

  PrintByEventCmd = new GmUIcmdWithAString("/gamos/scoring/printByEvent",this);
  PrintByEventCmd->SetGuidance("Selects if the scores (and errors) are given divided by the total number of events. It must have as arguments the printer name TRUE/FLASE");
  PrintByEventCmd->SetParameterName("choice",true);
  PrintByEventCmd->AvailableForStates(G4State_Idle);

  PrintNEventsTypeCmd = new GmUIcmdWithAString("/gamos/scoring/printNEventsType",this);
  PrintNEventsTypeCmd->SetGuidance("Selects if the scores (and errors) are given divided by the total number of runs (ByRun), the total number of events (ByEvent) or for each index the number of events it is filled");
  PrintNEventsTypeCmd->SetParameterName("choice",true);
  PrintNEventsTypeCmd->AvailableForStates(G4State_Idle);

  UnitCmd = new GmUIcmdWithAString("/gamos/scoring/scorerUnit",this);
  UnitCmd->SetGuidance("Changes the unit of an scorer.  It must have as arguments SCORER_NAME UNIT_NAME UNIT_FACTOR");
  //. For exceptional cases it can be only SCORER_NAME UNIT_NAME and the factor is calculated by the scorer itself");
  UnitCmd->SetParameterName("choice",true);
  UnitCmd->AvailableForStates(G4State_Idle);

}

//---------------------------------------------------------------------
GmScoringMessenger::~GmScoringMessenger() 
{
  if (MFDetectorCmd) delete MFDetectorCmd;
  if (PrinterCmd) delete PrinterCmd;
  if (Scorer2MFDCmd) delete Scorer2MFDCmd;
  if (ScorerCmd) delete ScorerCmd;
  if (FilterCmd) delete FilterCmd;
  if (Printer2ScorerCmd) delete Printer2ScorerCmd;
  if (ClassifierCmd) delete ClassifierCmd;
  if (TrkWeightCmd) delete TrkWeightCmd;
  if (ScoreErrorsCmd) delete ScoreErrorsCmd;
  if (PrintByEventCmd) delete PrintByEventCmd;
  if (PrintNEventsTypeCmd) delete PrintNEventsTypeCmd;
  if (UnitCmd) delete UnitCmd;
}

//---------------------------------------------------------------------
void GmScoringMessenger::SetNewValue(G4UIcommand * command,
						   G4String newValues)    
{ 
  if (command == PrinterCmd) {
    std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );
    if( wl.size() < 2 ) G4Exception("GmScoringMessenger::SetNewVAlue",
				    "Wrong argument",
				    FatalErrorInArgument,
				    G4String("Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs at least 2 arguments: PrinterName PrinterClass").c_str()); 
    
    GmPSPrinterMgr::GetInstance()->FindOrBuildPSPrinter( GmGenUtils::GetWordsInString(newValues) );

  } else if (command == MFDetectorCmd) {
    
    std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );
    if( wl.size() < 2 ) G4Exception("GmScoringMessenger::SetNewVAlue",
				    "Wrong argument",
				    FatalErrorInArgument,
				    G4String("Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs at least 2 arguments: MFDname LVname_1 (LVname_2 ...)").c_str()); 

    theAction->CreateMFD( wl );

  } else if (command == Scorer2MFDCmd) {
    std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << " CALL AddScorer2MFD " << wl[0] << " " << wl[1] << G4endl;
#endif
    theAction->AddScorer2MFD( wl );

  } else if (command == ScorerCmd) {
    std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );
    if( wl.size() < 2 ) G4Exception("GmScoringMessenger::SetNewVAlue",
				    "Wrong argument",
				    FatalErrorInArgument,
				    G4String("Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs at least 2 arguments: ScorerName ScorerClass (scorer parameters)").c_str()); 

    std::vector<G4String> params;
    for( unsigned int ii = 2; ii < wl.size(); ii++ ) {
      params.push_back( wl[ii] );
    }

#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << " CALL AddScorer2MFD " << wl[0] << " " << wl[1] << G4endl;
#endif
    theAction->CreateScorer( wl[0], wl[1], params );

  } else if (command == FilterCmd) {
    std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );
    if( wl.size() < 2 ) G4Exception("GmScoringMessenger::SetNewVAlue",
				    "Wrong argument",
				    FatalErrorInArgument,
				    G4String("Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs at least 2 arguments: FilterClass/Name ScorerName").c_str()); 
    if( wl.size() == 3 ) {
      G4Exception("GmScoringMessenger::SetNewValue",
		  "Deprecated command",
		  JustWarning,
		  G4String("Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + " has 3 arguments, first one will be ignored").c_str()); 
    wl.erase(wl.begin());
    }
    if( wl.size() > 3 ) {
      G4Exception("GmScoringMessenger::SetNewValue",
		  "Wrong command",
		  FatalErrorInArgument,
		  G4String("Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + " has more than 3 arguments, it must have only two").c_str());       
    }

    theAction->AddFilter2Scorer( wl );

  } else if (command == Printer2ScorerCmd) {
    std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );
    if( wl.size() < 2 ) G4Exception("GmScoringMessenger::SetNewVAlue",
				    "Wrong argument",
				    FatalErrorInArgument,
				    G4String("Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs at least 2 arguments: PrinterClass/Name ScorerName").c_str()); 

    if( wl.size() == 3 ) {
      G4Exception("GmScoringMessenger::SetNewValue",
		  "Deprecated command",
		  JustWarning,
		  G4String("Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + " has 3 arguments, first one will be ignored").c_str()); 
      wl.erase(wl.begin());
    }
    if( wl.size() > 3 ) {
      G4Exception("GmScoringMessenger::SetNewValue",
		  "Wrong command",
		  FatalErrorInArgument,
		  G4String("Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + " has more than 3 arguments, it must have only two").c_str());       
    }

    theAction->AddPrinter2Scorer( wl );

  } else if (command == ClassifierCmd) {
    std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );
    if( wl.size() < 2 ) G4Exception("GmScoringMessenger::SetNewVAlue",
				    "Wrong argument",
				    FatalErrorInArgument,
				    G4String("Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs at least 2 arguments:  ClassifierClass/Name ScorerName").c_str()); 
    if( wl.size() == 3 ) {
      G4Exception("GmScoringMessenger::SetNewValue",
		  "Wrong argument",
		  FatalErrorInArgument,
		  G4String(G4String("Deprecated command") + " Command: "+ command->GetCommandPath() 
			   + "/" + command->GetCommandName() + " " + newValues + " has 3 arguments, first one will be ignored").c_str()); 
      wl.erase(wl.begin());
    }
    if( wl.size() > 3 ) {
      G4Exception("GmScoringMessenger::SetNewValue",
		  "Wrong command",
		  FatalErrorInArgument,
		  G4String("Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + " has more than 3 arguments, it must have only two").c_str());       
    }

    theAction->AssignClassifier2Scorer( wl );

  } else if (command == TrkWeightCmd) {
    std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );
    if( wl.size() != 2 ) G4Exception("GmScoringMessenger::SetNewVAlue",
				    "Wrong argument",
				    FatalErrorInArgument,
				    G4String("Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs 2 arguments: ScorerName TRUE/FALSE").c_str()); 

    theAction->AddTrkWeight2Scorer( wl[0], wl[1] );

  } else if (command == ScoreErrorsCmd) {
    std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );
    if( wl.size() != 2 ) G4Exception("GmScoringMessenger::SetNewVAlue",
				    "Wrong argument",
				    FatalErrorInArgument,
				    G4String("Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs 2 arguments: ScorerName TRUE/FALSE").c_str()); 

    theAction->AddScoreErrors2Scorer( wl[0], wl[1] );
    
  } else if (command == PrintByEventCmd) {
    std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );
    if( wl.size() != 2 ) G4Exception("GmScoringMessenger::SetNewValue",
				    "Wrong argument",
				    FatalErrorInArgument,
				    G4String("Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs 2 arguments: ScorerName TRUE/FALSE").c_str()); 

    theAction->AddPrintByEvent2Scorer( wl[0], wl[1] );

  } else if (command == PrintNEventsTypeCmd) {
    std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );
    if( wl.size() != 2 ) G4Exception("GmScoringMessenger::SetNewValue",
				    "Wrong argument",
				    FatalErrorInArgument,
				    G4String("Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs 2 arguments: ScorerName TRUE/FALSE").c_str()); 

    theAction->AddPrintNEventsType2Scorer( wl[0], wl[1] );

  } else if (command == UnitCmd) {
    std::vector<G4String> wl = GmGenUtils::GetWordsInString( newValues );
    if( wl.size() != 3 ) G4Exception("GmScoringMessenger::SetNewValue",
				    "Wrong argument",
				    FatalErrorInArgument,
				    G4String("Command: "+ command->GetCommandPath() + "/" + command->GetCommandName() + " " + newValues + "  needs 3 arguments: ScorerName UnitName UnitValue").c_str()); 
    theAction->ChangeScorerUnit( wl[0], wl[1], wl[2] );

  }


}
