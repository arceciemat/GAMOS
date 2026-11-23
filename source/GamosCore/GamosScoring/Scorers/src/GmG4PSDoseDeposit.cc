//#define VERBOSE_DOSEDEP
#include "G4VPrimitiveScorer.hh"
#include "G4EnergyLossForExtrapolator.hh"
#include "GmG4PSDoseDeposit.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

#include "G4VSolid.hh"
#include "G4UnitsTable.hh"
#include "G4PhantomParameterisation.hh"
#include "G4RegularNavigationHelper.hh"
#include "G4EmCalculator.hh"

//--------------------------------------------------------------------
GmG4PSDoseDeposit::GmG4PSDoseDeposit(G4String name) 
   :GmVPrimitiveScorer(name)
{ 
  theUnit = CLHEP::gray;
  theUnitName = G4String("Gy");

  theElossExt = new G4EnergyLossForExtrapolator(0);
  thePhantomParam = 0;

  theGeomUtils = GmGeometryUtils::GetInstance();  

}


//--------------------------------------------------------------------
G4bool GmG4PSDoseDeposit::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( aStep == 0 ) return false;  // it is 0 when called by GmScoringMgr after last event

  if( !AcceptByFilter( aStep ) ) return false;
  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return false;
  G4double volume = theGeomUtils->GetCubicVolume( aStep->GetPreStepPoint()->GetPhysicalVolume() );

  G4double density = aStep->GetTrack()->GetMaterial()->GetDensity();
  
  G4double weight = aStep->GetPreStepPoint()->GetWeight(); 

  G4int index = GetIndex(aStep);
    
  if( density < theMinDensity ) {
    FillScorer( aStep, index, 0., weight );
    return true;
  }

#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) 
    G4cout << "GmG4PSDoseDeposit::ProcessHits edep " << edep 
	   << " density " << density / (CLHEP::g/CLHEP::cm3) << "=" << density 
	   << " volume " << volume << G4endl;
#endif

#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << "GmG4PSDoseDeposit::ProcessHits  EvtMap " << EvtMap << G4endl;
  if( ScoringVerb(debugVerb) ) G4cout << "GmG4PSDoseDeposit::ProcessHits  EvtMap Number of entries " << EvtMap->entries() << " tmp " << theSumV_tmp.size() << G4endl;
#endif

  // use index from classifier, unless you are skipping borders of equal material voxels in G4RegularNavigation
  //  if( bUseClassifierIndex || !bSkipEqualMaterials || !theRegularParamUtils->IsPhantomVolume( aStep->GetPreStepPoint()->GetPhysicalVolume() ) ) {
  G4double dose  = edep / ( density * volume );
  FillScorer( aStep, index, dose, weight );
  
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb)) G4cout  << "GmG4PSDoseDeposit::ProcessHits  dose " << dose*weight << " index " << index << G4endl;
#endif

  return true;
} 


//--------------------------------------------------------------------
G4double GmG4PSDoseDeposit::GetGeom2TrueStepLength( G4double kinEnergy ) 
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

