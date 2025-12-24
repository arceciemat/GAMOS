#include "G4VPrimitiveScorer.hh"
#include "G4EnergyLossForExtrapolator.hh"
#include "GmG4PSDoseDeposit_SkipEqualMateSimple.hh"

#include "G4VSolid.hh"
#include "G4UnitsTable.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "G4RegularNavigationHelper.hh"
#include "G4EmCalculator.hh"
#include "GamosCore/GamosReadDICOM/include/GmRegularParamUtils.hh"
#include "G4PhantomParameterisation.hh"

#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

//G4bool GmG4PSDoseDeposit_SkipEqualMateSimple::bNewEvent = TRUE;

//--------------------------------------------------------------------
GmG4PSDoseDeposit_SkipEqualMateSimple::GmG4PSDoseDeposit_SkipEqualMateSimple(G4String name) 
   :GmVPrimitiveScorer(name)
{ 
  theElossExt = new G4EnergyLossForExtrapolator(0);

  theUnit = CLHEP::gray;
  theUnitName = G4String("Gy");

  bUseVolume = true;
}


//--------------------------------------------------------------------
G4bool GmG4PSDoseDeposit_SkipEqualMateSimple::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( !AcceptByFilter( aStep ) ) return false;

  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;
  G4double volume = GetVolume(aStep);
  G4double density = aStep->GetTrack()->GetMaterial()->GetDensity();
  G4double dose    = edep / ( density * volume );
  G4double weight = 1.;
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << "  GmG4PSDoseDeposit_NoSkipEqualMate::ProcessHits dose " << dose << " edep " << edep << " density " << density << " volume " << volume << G4endl;
#endif
  if( fWeighted ) {
    weight = aStep->GetPreStepPoint()->GetWeight(); 
    //    dose *= weight; 
  }

  FillScorer( aStep, dose, weight );
  //  FillScorer( aStep, dose, aStep->GetPreStepPoint()->GetWeight());
  return TRUE;
}


//--------------------------------------------------------------------
G4bool GmG4PSDoseDeposit_SkipEqualMateSimple::FillScorer(G4Step* aStep, G4double val, G4double wei)
{

#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << EvtMap << " EvtMap Number of entries " << EvtMap->entries() << " tmp " << theSumV_tmp.size() << G4endl;
#endif
  if( bScoreErrors ) {
    if( bNewEvent ) {
      //--- Store X and X2 of last event
      std::map<G4int,G4double>::iterator ite;
      for( ite = theSumV_tmp.begin(); ite !=theSumV_tmp.end(); ite++ ){
	G4int idx = (*ite).first;
	//      theSumV[idx] += (*ite).second; // this magnitude is stored in the usual G4 mechanism
	theSumV2[idx] += (*ite).second*(*ite).second;
	//	(*ite).second = 0.;
	//	theNEntries[idx]+=1;
	
	/*	std::map<G4int,G4double*>::iterator itee;
	for( itee =  EvtMap->GetMap()->begin(); itee !=  EvtMap->GetMap()->end(); itee++ ){
	  //	G4cout << " EvtMap " << (*itee).first << " = " << (*itee).second << G4endl;
	  } */
	
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(debugVerb) ) G4cout<< GetName() << " " << idx << " theSumV= " << (*ite).second << " theSumV2= " <<  theSumV2[idx] << G4endl;
#endif
      }
      theSumV_tmp.clear();
    }
    bNewEvent = false;
  }

  G4bool verbose = 0;

  if( aStep != 0 ) { // it is 0 when called by GmScoringMgr after last event
    if( G4RegularNavigationHelper::Instance()->theStepLengths.size() <= 1 || aStep->GetTrack()->GetDefinition()->GetPDGCharge() == 0 )  { // we are only counting dose deposit
      G4int index = GetIndex(aStep);
      G4double valwei = val*wei;
      EvtMap->add(index,valwei);
      if( verbose) G4cout << " RN: 1 index energy lost " << valwei << " index " << index << G4endl;
      
      if( bScoreErrors ) {
	theSumV_tmp[index] += valwei; 
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(debugVerb) ) G4cout << GetName() << " theSumV_tmp " << index << " = " <<  theSumV_tmp[index] << G4endl;
#endif
      }
      
      sumALL += val*wei;
#ifndef GAMOS_NO_VERBOSE
      if( verbose )if( ScoringVerb(debugVerb) ) G4cout << " EvtMap Number of entries " << EvtMap->entries() << " tmp " << theSumV2.size() << " SUMALL " << sumALL << G4endl;
#endif

    } else {
      std::vector< std::pair<G4int,G4double> > rnsl = G4RegularNavigationHelper::Instance()->theStepLengths; 
      G4double stepLength = aStep->GetStepLength(); 
      G4double slSum = 0;
      for( unsigned int ii = 0; ii < rnsl.size(); ii++ ){
	G4double sl = G4RegularNavigationHelper::Instance()->theStepLengths[ii].second;
	slSum += sl;
	if( slSum > stepLength ) G4Exception("GmG4PSDoseDeposit_SkipEqualMateSimple::FillScorer",
					     "Wrong argument",
					     FatalErrorInArgument,
					     "This should not happen");
      }

      for( unsigned int ii = 0; ii < rnsl.size(); ii++ ){
	G4int index = G4RegularNavigationHelper::Instance()->theStepLengths[ii].first;
	G4double sl = G4RegularNavigationHelper::Instance()->theStepLengths[ii].second;
	G4double valStep = val * sl/slSum; //divide dose along steps, proportional to step lengthr
	G4double valwei = valStep*wei;
	EvtMap->add(index,valwei);
#ifndef GAMOS_NO_VERBOSE
	//	G4cout << ScoringVerb(debugVerb) << GetName() << " " << index << " val "<< val << " valStep " << valStep << " stepl " << sl << " TOTALstepLength " << stepLength << " slSum " << slSum << " old sl " << G4RegularNavigationHelper::theStepLengths[ii].second << " " << G4RegularNavigationHelper::theStepLengths[ii].second-stepLength << G4endl;
#endif
	
	if( bScoreErrors ) {
	  theSumV_tmp[index] += valStep*wei; 
#ifndef GAMOS_NO_VERBOSE
	  if( ScoringVerb(debugVerb) ) G4cout << GetName() << " theSumV_tmp " << index << " = " << theSumV_tmp[index] << " valStep " << valStep << G4endl;
#endif
	}
	
	sumALL += valStep*wei;
#ifndef GAMOS_NO_VERBOSE
	if( ScoringVerb(debugVerb) ) G4cout << " EvtMap Number of entries " << EvtMap->entries() << " tmp " << theSumV2.size() << " SUMALL " << sumALL << G4endl;
#endif
      }
	
      //      G4cout << " NSTEPS " << ii << G4endl;
    }
     
  }

  G4RegularNavigationHelper::Instance()->ClearStepLengths();

  return TRUE;
} 


//--------------------------------------------------------------------
G4double GmG4PSDoseDeposit_SkipEqualMateSimple::GetGeom2TrueStepLength( G4double kinEnergy ) 
{

  G4double g2tratio;
  if( kinEnergy < 0.04 ) {
    g2tratio = 1.776 - 22.4*kinEnergy + 988.6*pow(kinEnergy,2.) -2.231e4*pow(kinEnergy,3.) + 1.718e5*pow(kinEnergy,4.);
  } else if( kinEnergy < 1. ) {
    g2tratio = 1.405 - 0.9334*kinEnergy + 1.072*pow(kinEnergy,2.) -0.4395*pow(kinEnergy,3.);
  } else {// if( kinEnergy < 6. ) {
    g2tratio = 1.1 + 0.008554*kinEnergy - 0.009195*pow(kinEnergy,2.) + 0.001009*pow(kinEnergy,3.);
  }

  return g2tratio;
}
