#include "GmPSKineticEnergyVoxelSpectrum.hh"
#include "G4UnitsTable.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for scoring energy deposit.
///////////////////////////////////////////////////////////////////////////////
#include "G4EnergyLossForExtrapolator.hh"
#include "G4StepPoint.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4AffineTransform.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"
#include "G4Material.hh"
#include "G4ParticleDefinition.hh"

GmPSKineticEnergyVoxelSpectrum::GmPSKineticEnergyVoxelSpectrum(G4String name)
     :GmVPrimitiveScorer(name)
{
  theUnit = 1.;
  theUnitName = G4String("MeV");
  SetSpectrum(true);

  theElossExt = new G4EnergyLossForExtrapolator(0);
  theEnergyStep = GmParameterMgr::GetInstance()->GetNumericValue(GetName()+":EnergyStep",-1.);
  SetSpectrumBinWidth( theEnergyStep );
  
  // Assume energy steps start at 0.
  theEnergyStart = GmParameterMgr::GetInstance()->GetNumericValue(GetName()+":EnergyStart",0.);

}

GmPSKineticEnergyVoxelSpectrum::~GmPSKineticEnergyVoxelSpectrum()
{;}

G4bool GmPSKineticEnergyVoxelSpectrum::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  // When particle enters in voxel, calculate the energy at the entry and at the exit (taking into account multiple scattering and energy loss). Divide the interval in energy intervals and score 1 per each energy interval. If last energy interval is only a fraction of the given interval, assign a weight equal to that fraction
  // It is meant to be used with a microdosimetry lineal energy spectrum (one spectrum per each proton energy interval)

  //----- Check particle enters volume
  const G4StepPoint* preSP = aStep->GetPreStepPoint();
  if( preSP->GetStepStatus() != fGeomBoundary ) {
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << " GmPSKineticEnergyVoxelSpectrum::ProcessHits Step not entering volume " << G4endl;
#endif
    return false;
  }
  
  if( aStep == 0 ) return FALSE;  // it is 0 when called by GmScoringMgr after last event

  if( !AcceptByFilter( aStep ) ) return false;
  //  G4cout << " 3GmPSKineticEnergyVoxelSpectrum::ProcessHits( " << G4endl; //GDEB

  G4double weight = aStep->GetPreStepPoint()->GetWeight(); 

  //--- Get distance to out
  G4LogicalVolume* LV = preSP->GetPhysicalVolume()->GetLogicalVolume();
  G4VSolid* solid = LV->GetSolid();
  G4Box* box = dynamic_cast<G4Box*>(solid);
  if( box == 0 ) {
    G4Exception("GmPSKineticEnergyVoxelSpectrum::ProcessHits",
		"",
		FatalException,
		("Scoring volume = "+LV->GetName()+" is not a solid, but a ",solid->GetEntityType()).c_str());
  }
  G4AffineTransform transform = G4TransportationManager::GetTransportationManager()->
    GetNavigatorForTracking()->GetGlobalToLocalTransform();
  G4ThreeVector globalPos = preSP->GetPosition();
  G4ThreeVector localPos = transform.TransformPoint(globalPos);
  G4ThreeVector globalDir = preSP->GetMomentumDirection();
  G4ThreeVector localDir = transform.TransformAxis(globalDir);
  G4double distToOut = box->DistanceToOut(localPos, localDir);
  G4Material* mate = LV->GetMaterial();
  G4ParticleDefinition* part = aStep->GetTrack()->GetDefinition();
  G4double kinEnergyPre = preSP->GetKineticEnergy();
  
  G4double distIn = theElossExt->TrueStepLength( kinEnergyPre, distToOut , mate, part );
  G4double kinEnergyPost = theElossExt->EnergyAfterStep(kinEnergyPre, distToOut, mate, part);
  //  G4cout << "GmPSKineticEnergyVoxelSpectrum kinEnergyPre " << kinEnergyPre << " kinEnergyPost " << kinEnergyPost << " distToOut " <<distToOut << " ->distIn " << distIn << " DENSITY " << mate->GetDensity()/(CLHEP::g/CLHEP::cm3) << G4endl; //GDEB
  
  if( theEnergyStep == -1 ){
    G4Exception("GmPSKineticEnergyVoxelSpectrum::ProcessHits",
		"",
		FatalException,
		("Energy Step not defined, define parameter: /P "+GetName()+":EnergyStep <VALUE>").c_str());
  }

  G4int iEnerPre = G4int((kinEnergyPre-theEnergyStart)/theEnergyStep);
  G4int iEnerPost = G4int((kinEnergyPost-theEnergyStart-0.01*theEnergyStep)/theEnergyStep);
  G4double enerPre = (iEnerPre+1)*theEnergyStep;
  G4double enerPost = (iEnerPost+1)*theEnergyStep;
  //  G4cout << "GmPSKineticEnergyVoxelSpectrum iEner " << iEnerPre << " " << iEnerPost << " ener " << enerPre << " " << enerPost << G4endl; //GDEB
  
 // use index from classifier, unless you are skipping borders of equal material voxels in G4RegularNavigation
  G4int index = GetIndex(aStep);
  for( G4double ener = enerPost; ener <= enerPre; ener+=theEnergyStep ) {
    G4double weightFS = weight;
    //    G4cout << " LOOPING " << ener << " WEIGHT " << weightFS << G4endl; //GDEB
    G4double enerFS = ener - theEnergyStep; // spectrum max index are named using the lower bound
    if( ener == kinEnergyPre ) {
      continue;
    } else if( ener == enerPost ) {
      weightFS *= (enerPost-kinEnergyPost)/theEnergyStep; 
      //      G4cout << "V:" << index << " " << enerPre-kinEnergyPre << " 1GmPSKineticEnergyVoxelSpectrum LOOP ener " << ener << " weightFS " <<weightFS << G4endl; //GDEB
    } else if( ener == enerPre ) {
      weightFS *= 1. - (enerPre-kinEnergyPre)/theEnergyStep;
      //      G4cout <<"V:" << index << " " << enerPost-kinEnergyPost << " 2GmPSKineticEnergyVoxelSpectrum LOOP ener " << ener << " weightFS " <<weightFS << G4endl; //GDEB
    } else {
      //      G4cout << "V:" << index << " " << " 0GmPSKineticEnergyVoxelSpectrum LOOP ener " << ener << " enerPre " << enerPre << " enerPost " << enerPost << " weightFS " <<weightFS << G4endl; //GDEB
    }

    FillScorer( aStep, index, enerFS, weightFS );
  }
  
  return TRUE;
} 


