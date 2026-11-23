#include "GmPSLETByESpect.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "GamosCore/GamosData/Distributions/include/GmNumericDistributionLogLog.hh"

#include "G4UImanager.hh"
#include "G4UnitsTable.hh"
#include "G4MaterialTable.hh"
#include "G4Proton.hh"

//-----------------------------------------------------------------
GmPSLETByESpect::GmPSLETByESpect(G4String name)
     :GmVPrimitiveScorer(name)
{
  theUnit = 1.;
  theUnitName = G4String("MeV/mm");
  G4String convFN = GmParameterMgr::GetInstance()->GetStringValue(name+":MaterialConversionFileName","StoppingPowers/materialConversion.lis");
  /*  if( convFN == "" ) {
    G4Exception("GmPSLETByESpect::GmPSLETByESpect",
		"",
		FatalException,
		("No file name provided for material name conversion, please use the command: /gamos/setParam " + name + ":MaterialConversionFileName FILE_NAME").c_str());
		} */
  BuildMaterialConversionNames( convFN );
    
  InitialiseE2LET();
}

//-----------------------------------------------------------------
GmPSLETByESpect::~GmPSLETByESpect()
{;}

//-----------------------------------------------------------------
void GmPSLETByESpect::BuildMaterialConversionNames( G4String convFN )
{
  convFN = GmGenUtils::FileInPath( convFN );
  GmFileIn fin = GmFileIn::GetInstance(convFN);
  std::vector<G4String> wl;
  for( ;; ) {
    if( !fin.GetWordsInLine( wl ) ) break;
    if( wl.size() != 2 ) {
      G4String line;
      for( size_t ii = 0; ii < wl.size(); ii++ ) {
	line += wl[ii] + " ";
      }
      G4cerr << "@@@ READING LINE " << line << G4endl;	
      G4Exception("GmPSLETByESpect::BuildMaterialConversionNames",
		  "",
		  FatalErrorInArgument,
		  ("Lines in file " + convFN + " must contain only two words: GEANT4_MATERIAL_NAME STOPPINGPOWER_MATERIAL_NAME , while it contains " + GmGenUtils::itoa(wl.size())).c_str());
    }
    theNameConversions[wl[1]] = wl[0];    
  }

  fin.Close();
}

//-----------------------------------------------------------------
void GmPSLETByESpect::InitialiseE2LET()
{
  G4UImanager * UI = G4UImanager::GetUIpointer();
  G4String command;
  
  //--- Read Energy to StoppingPower -> LET for all materials
  const G4MaterialTable* matTable = G4Material::GetMaterialTable();
  G4MaterialTable::const_iterator ite;
  for( ite = matTable->begin(); ite != matTable->end(); ite++ ){
    G4String mateName = (*ite)->GetName();
    std::map<G4String,G4String>::const_iterator item = theNameConversions.find(mateName);
    G4String convMateName;
    if( item == theNameConversions.end() ) {
      G4cerr << " N CONVERSION NAMES " << theNameConversions.size() << G4endl; 
      for( item = theNameConversions.begin(); item != theNameConversions.end(); item++ ){
	G4cerr << " CONV " << (*item).first << " -> " << (*item).second << G4endl;
      }
      G4Exception("GmPSLETByESpect::InitializeE2LET",
		  "",
		  JustWarning,
		  ("Stopping power data not found for material " + mateName + "; LET will be set to 0 in this material").c_str());
    } else { // if not found, use G4 material name
      convMateName = (*item).second;

      //      G4String fileName = path + "/StoppingPowers/StoppingPower."+convMateName+".txt";
      G4String fileName = "StoppingPowers/StoppingPower."+convMateName+".txt";
      //--- Build GmNumericDistributionLogLog
      G4String distName = "NumericDistributionLogLog_"+ convMateName;
      command = "/gamos/setParam \"" + distName + ":FileName\" \"" + fileName + "\"";
      UI->ApplyCommand(command);
      command = "/gamos/setParam \"" + distName + ":Data\" InitialKineticEnergy";
      UI->ApplyCommand(command);
      command = "/gamos/setParam \"" + distName + ":ValueColumn\" 4";
      UI->ApplyCommand(command);
      theDistributions[mateName] = new GmNumericDistributionLogLog(distName);
    }
  }

}

//-----------------------------------------------------------------
G4bool GmPSLETByESpect::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( aStep == 0 ) return FALSE;  // it is 0 when called by GmScoringMgr after last event

  if( !AcceptByFilter( aStep ) ) return false;

  if( aStep->GetTrack()->GetDefinition() != G4Proton::Proton() ) {
    G4Exception("GmPSLETByESpect::processHits",
		"",
		JustWarning,
		("LET can only be calculated for protons, while particle is " +  aStep->GetTrack()->GetDefinition()->GetParticleName() + " , please set the appropiate filter for your scorer " + GetName()).c_str());
  }
  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;

  G4double let = GetLETFromEnergy( aStep );
  
  G4double weight = aStep->GetPreStepPoint()->GetWeight(); 
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) {
    G4cout << "  GmPSLETByESpect::ProcessHits let " << let
	   << G4endl;
    G4cout << aStep->GetTrack()->GetKineticEnergy() << "  GmPSLETByESpect::ProcessHits let TEST " << (1.-let/(aStep->GetTotalEnergyDeposit()/aStep->GetStepLength())) << " " << let << " = " << aStep->GetTotalEnergyDeposit()/aStep->GetStepLength() << G4endl;
  }
#endif

  G4int index = GetIndex(aStep);
  FillScorer( aStep, index, let, weight );
  
  return TRUE;
} 

//-----------------------------------------------------------------
G4double GmPSLETByESpect::GetLETFromEnergy(G4Step* aStep)
{
  G4StepPoint* stepPoint = aStep->GetPostStepPoint();

  G4Material*  mate = stepPoint->GetMaterial();
  G4String mateName = mate->GetName();
  G4double density = mate->GetDensity()/(CLHEP::g/CLHEP::cm3);
  GmNumericDistributionLogLog* dist = theDistributions[mateName];

  G4double let = 0.;
  if( dist ) let = dist->GetValueFromStep(aStep) * density/CLHEP::cm;
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) 
    G4cout << "  GmPSLETByESpect::GetLETFromEnergy let " << let
	   << " material " << mateName
	   << " density " << density
	   << G4endl;
#endif

  return let;
}
