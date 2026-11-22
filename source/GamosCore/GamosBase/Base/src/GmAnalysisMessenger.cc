#include "GmAnalysisMessenger.hh"
#include "GmAnalysisMgr.hh"

#include "G4UImanager.hh"
#include "GmUIcmdWithAString.hh"
#include <sstream>

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
GmAnalysisMessenger::GmAnalysisMessenger() 
{
  setFormatCmd = new GmUIcmdWithAString("/gamos/analysis/fileFormat",this);
  setFormatCmd->SetGuidance("Select the format of the analysis file: root, csv or txt");
  setFormatCmd->SetParameterName("choice",true);
  setFormatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  addFormatCmd = new GmUIcmdWithAString("/gamos/analysis/addFileFormat",this);
  addFormatCmd->SetGuidance("Adds another format of the analysis file: root, csv or txt");
  addFormatCmd->SetParameterName("choice",true);
  addFormatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  histo1NBinsCmd = new GmUIcmdWithAString("/gamos/analysis/histo1NBins",this);
  histo1NBinsCmd->SetGuidance("Changes the number of bins for histograms");
  histo1NBinsCmd->SetParameterName("choice",true);
  histo1NBinsCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  histo1MinCmd = new GmUIcmdWithAString("/gamos/analysis/histo1Min",this);
  histo1MinCmd->SetGuidance("Changes the minimum limit for histograms");
  histo1MinCmd->SetParameterName("choice",true);
  histo1MinCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  histo1MaxCmd = new GmUIcmdWithAString("/gamos/analysis/histo1Max",this);
  histo1MaxCmd->SetGuidance("Changes the maximum limit for histograms");
  histo1MaxCmd->SetParameterName("choice",true);
  histo1MaxCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  histo2NBinsXCmd = new GmUIcmdWithAString("/gamos/analysis/histo2NBinsX",this);
  histo2NBinsXCmd->SetGuidance("Changes the number of bins for histograms");
  histo2NBinsXCmd->SetParameterName("choice",true);
  histo2NBinsXCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  histo2MinXCmd = new GmUIcmdWithAString("/gamos/analysis/histo2MinX",this);
  histo2MinXCmd->SetGuidance("Changes the minimum limit for histograms");
  histo2MinXCmd->SetParameterName("choice",true);
  histo2MinXCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  histo2MaxXCmd = new GmUIcmdWithAString("/gamos/analysis/histo2MaxX",this);
  histo2MaxXCmd->SetGuidance("Changes the maximum limit for histograms");
  histo2MaxXCmd->SetParameterName("choice",true);
  histo2MaxXCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  histo2NBinsYCmd = new GmUIcmdWithAString("/gamos/analysis/histo2NBinsY",this);
  histo2NBinsYCmd->SetGuidance("Changes the number of bins for histograms");
  histo2NBinsYCmd->SetParameterName("choice",true);
  histo2NBinsYCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  histo2MinYCmd = new GmUIcmdWithAString("/gamos/analysis/histo2MinY",this);
  histo2MinYCmd->SetGuidance("Changes the minimum limit for histograms");
  histo2MinYCmd->SetParameterName("choice",true);
  histo2MinYCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  histo2MaxYCmd = new GmUIcmdWithAString("/gamos/analysis/histo2MaxY",this);
  histo2MaxYCmd->SetGuidance("Changes the maximum limit for histograms");
  histo2MaxYCmd->SetParameterName("choice",true);
  histo2MaxYCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  histoPrintCmd = new GmUIcmdWithAString("/gamos/analysis/histoPrint",this);
  histoPrintCmd->SetGuidance("Prints information about histograms on the screen");
  histoPrintCmd->SetParameterName("choice",true);
  histoPrintCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
GmAnalysisMessenger::~GmAnalysisMessenger() 
{
  if (setFormatCmd) delete setFormatCmd;
  if (addFormatCmd) delete addFormatCmd;
  if (histo1NBinsCmd) delete histo1NBinsCmd;
  if (histo1MinCmd) delete histo1MinCmd;
  if (histo1MaxCmd) delete histo1MaxCmd;
  if (histo2NBinsXCmd) delete histo2NBinsXCmd;
  if (histo2MinXCmd) delete histo2MinXCmd;
  if (histo2MaxXCmd) delete histo2MaxXCmd;
  if (histo2NBinsYCmd) delete histo2NBinsYCmd;
  if (histo2MinYCmd) delete histo2MinYCmd;
  if (histo2MaxYCmd) delete histo2MaxYCmd;
  if (histoPrintCmd) delete histoPrintCmd;
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
void GmAnalysisMessenger::SetNewValue(G4UIcommand * command,
						   G4String newValues)    
{ 
  if (command == setFormatCmd) {
    if( newValues != "csv" && newValues != "CSV"  
	&& newValues != "root" && newValues != "ROOT"
	&& newValues != "txt" && newValues != "TXT") {
      G4Exception("GmAnalysisMessenger::SetNewValue",
		  "/gamos/analysis/fileFormat",
		  FatalErrorInArgument,
		  (" The format of the analysis file has to be : root/ROOT, csv/CSV or txt/TXT, while it is " + newValues).c_str() ); 
    }
    GmAnalysisMgr::SetFileFormat( newValues );
  }
  else if (command == addFormatCmd) {
    if( newValues != "csv" && newValues != "CSV"  
	&& newValues != "root" && newValues != "ROOT"
	&& newValues != "txt" && newValues != "TXT") {
      G4Exception("GmAnalysisMessenger::SetNewValue",
		  "/gamos/analysis/fileFormat",
		  FatalErrorInArgument,
		  (" The format of the analysis file has to be : root/ROOT, csv/CSV or txt/TXT, while it is " + newValues).c_str() ); 
    }
    GmAnalysisMgr::AddFileFormat( newValues );
  }
  else if (command == histo1NBinsCmd) {
    GmAnalysisMgr::AddHisto1NBins( newValues );
  }
  else if (command == histo1MinCmd) {
    GmAnalysisMgr::AddHisto1Min( newValues );
  }
  else if (command == histo1MaxCmd) {
    GmAnalysisMgr::AddHisto1Max( newValues );
  }
  else if (command == histo2NBinsXCmd) {
    GmAnalysisMgr::AddHisto2NBinsX( newValues );
  }
  else if (command == histo2MinXCmd) {
    GmAnalysisMgr::AddHisto2MinX( newValues );
  }
  else if (command == histo2MaxXCmd) {
    GmAnalysisMgr::AddHisto2MaxX( newValues );
  }
  else if (command == histo2NBinsYCmd) {
    GmAnalysisMgr::AddHisto2NBinsY( newValues );
  }
  else if (command == histo2MinYCmd) {
    GmAnalysisMgr::AddHisto2MinY( newValues );
  }
  else if (command == histo2MaxYCmd) {
    GmAnalysisMgr::AddHisto2MaxY( newValues );
  }
  else if (command == histoPrintCmd) {
    GmAnalysisMgr::AddHistoPrint( newValues );
  }


}
