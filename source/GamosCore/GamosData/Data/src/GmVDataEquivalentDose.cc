#include "GmVDataEquivalentDose.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosData/Management/include/GmDataVerbosity.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VSolid.hh"

//----------------------------------------------------------------
GmVDataEquivalentDose::GmVDataEquivalentDose()
{
  theHMax = 100.;
}

//----------------------------------------------------------------
GmVDataEquivalentDose::~GmVDataEquivalentDose()
{
}

//----------------------------------------------------------------
G4double GmVDataEquivalentDose::DoseFromEnergy( G4double ener, const G4Step* aStep )
{
  G4double eqdose = aStep->GetStepLength() / aStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetSolid()->GetCubicVolume();
  G4double doseFactor =  EnergyToDoseFactor( ener );
#ifndef GAMOS_NO_VERBOSE 
  if( DataVerb(debugVerb) ) G4cout << "GmVDataEquivalentDose::DoseFromEnergy " << eqdose * doseFactor
				   << " flux = " << eqdose 
				   << " DoseFactor = " << doseFactor << G4endl;
#endif
  return eqdose * doseFactor;
}


//----------------------------------------------------------------
G4double GmVDataEquivalentDose::EnergyToDoseFactor( G4double ener )
{
  Flux2Dose f2d  = (*(theFlux2Dose->upper_bound(ener))).second;
  G4double doseFactor = 1.;

  // multiply by flux to eqDose factor
  if( theEquivDose == edHstar ) {
    doseFactor = f2d.Hstar;
  } else if( theEquivDose == edHp0 ) {
    doseFactor = f2d.Hp0; 
  } else if( theEquivDose == edHp15) {
    doseFactor = f2d.Hp15; 
  } else if( theEquivDose == edHp30) {
    doseFactor = f2d.Hp30; 
  } else if( theEquivDose == edHp45) {
    doseFactor = f2d.Hp45; 
  } else if( theEquivDose == edHp60) {
    doseFactor = f2d.Hp60; 
  } else if( theEquivDose == edHp75) {
    doseFactor = f2d.Hp75; 
  }

#ifndef GAMOS_NO_VERBOSE 
  if( DataVerb(debugVerb) ) G4cout << "GmVDataEquivalentDose::EnergyToDoseFactor = " << doseFactor << G4endl;
#endif

  return doseFactor;
}

//------------------------------------------------------------------
std::map<G4double,Flux2Dose>* GmVDataEquivalentDose::ReadEnergyBinsForNeutrons()
{
  G4String flux2DoseFile = GmParameterMgr::GetInstance()->GetStringValue(theName+":Flux2DoseFileName","Flux2Dose.neutron.ICRU57.lis");

  //  std::set<float>* energyBins = new std::set<float>;
  std::map<G4double,Flux2Dose>* flux2DoseNeutron  = new std::map<G4double,Flux2Dose>;
  G4String path( getenv( "GAMOS_SEARCH_PATH" ) );
  flux2DoseFile = GmGenUtils::FileInPath( path, flux2DoseFile );
  GmFileIn fin = GmFileIn::GetInstance(flux2DoseFile);
  std::vector<G4String> wl;
  for( ;; ){
    if(! fin.GetWordsInLine( wl ) ) break;
    if( wl.size() != 8 ) { 
      G4Exception("GmPDSScorer::ReadEnergyBinsForNeutrons",
		   G4String("Wrong number of words in file "+flux2DoseFile + " line number = " + GmGenUtils::itoa(fin.Nline()).c_str()),
		  FatalErrorInArgument,
		  G4String("There must be 8 words, there are "+ GmGenUtils::itoa(wl.size())).c_str());
    }
    Flux2Dose f2d;
    f2d.Hstar = GmGenUtils::GetValue(wl[1])*100; //flux will be calculated in mm-2
    f2d.Hp0 = GmGenUtils::GetValue(wl[2])*100;
    f2d.Hp15 = GmGenUtils::GetValue(wl[3])*100;
    f2d.Hp30 = GmGenUtils::GetValue(wl[4])*100;
    f2d.Hp45 = GmGenUtils::GetValue(wl[5])*100;
    f2d.Hp60 = GmGenUtils::GetValue(wl[6])*100;
    f2d.Hp75 = GmGenUtils::GetValue(wl[7])*100;
    (*flux2DoseNeutron)[GmGenUtils::GetValue(wl[0])] = f2d;
  }

  fin.Close();

  return flux2DoseNeutron;
}


//------------------------------------------------------------------
std::map<G4double,Flux2Dose>* GmVDataEquivalentDose::ReadEnergyBinsForGammas()
{
  G4String energyBinsFile = GmParameterMgr::GetInstance()->GetStringValue(theName+":GammaFlux2DoseFileName","Flux2Dose.gamma.ICRU57.lis");

  //  std::set<float>* energyBins = new std::set<float>;
  std::map<G4double,Flux2Dose>* flux2DoseGamma 
    = new std::map<G4double,Flux2Dose>;
  G4String path( getenv( "GAMOS_SEARCH_PATH" ) );
  energyBinsFile = GmGenUtils::FileInPath( path, energyBinsFile );
  GmFileIn fin = GmFileIn::GetInstance(energyBinsFile);
  std::vector<G4String> wl;
  for( ;; ){
    if(! fin.GetWordsInLine( wl ) ) break;
    if( wl.size() != 6 ) {
      G4Exception("GmPDSScorer::ReadEnergyBinsForGammas",
		   G4String("Wrong number of words in file "+energyBinsFile + " line number = " + GmGenUtils::itoa(fin.Nline()).c_str()),
		  FatalErrorInArgument,
		  G4String("There must be 6 words, there are "+ GmGenUtils::itoa(wl.size())).c_str());
    }
    Flux2Dose f2d;
    f2d.Hstar = GmGenUtils::GetValue(wl[4])*100;
    f2d.Hp0 = 0.;
    f2d.Hp15 = 0.;
    f2d.Hp30 = 0.;
    f2d.Hp45 = 0.;
    f2d.Hp60 = 0.;
    f2d.Hp75 = 0.;
    (*flux2DoseGamma)[GmGenUtils::GetValue(wl[0])] = f2d;
  }

  fin.Close();

  return flux2DoseGamma;
}

//------------------------------------------------------------------
void GmVDataEquivalentDose::SetEquivDoseType( const G4String& doseName ) 
{
  theName = doseName;

  G4String eqDoseS = GmParameterMgr::GetInstance()->GetStringValue(doseName+":EquivDoseType","Hstar");

  if( eqDoseS == "Hstar" ) {
    theEquivDose = edHstar;
  } else if( eqDoseS == "Hp0" ) {
    theEquivDose = edHp0;
  } else if( eqDoseS == "Hp15" ) {
    theEquivDose = edHp15;
  } else if( eqDoseS == "Hp30" ) {
    theEquivDose = edHp30;
  } else if( eqDoseS == "Hp45" ) {
    theEquivDose = edHp45;
  } else if( eqDoseS == "Hp60" ) {
    theEquivDose = edHp60;
  } else if( eqDoseS == "Hp75" ) {
    theEquivDose = edHp75;
  }

}
