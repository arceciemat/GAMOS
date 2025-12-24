#include "GmPSRBEBWFy.hh"
#include "G4UnitsTable.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

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

GmPSRBEBWFy::GmPSRBEBWFy(G4String name)
     :GmVPrimitiveScorer(name)
{
  theUnit = 1.;
  theUnitName = G4String("MeV");

  theElossExt = new G4EnergyLossForExtrapolator(0);
  theEnergyStep = GmParameterMgr::GetInstance()->GetNumericValue(GetName()+":EnergyStep",-1.);
    if( theEnergyStep == -1 ){
    G4Exception("GmPSRBEBWFy::ProcessHits",
		"",
		FatalException,
		("Energy Step not defined, define parameter: /P "+GetName()+":EnergyStep <VALUE>").c_str());
  }

  // Assume energy steps start at 0.
  theEnergyStart = GmParameterMgr::GetInstance()->GetNumericValue(GetName()+":EnergyStart",0.);

  ReadBWFLinealEnergy();
}

//---------------------------------------------------------
GmPSRBEBWFy::~GmPSRBEBWFy()
{;}

//---------------------------------------------------------
void GmPSRBEBWFy::ReadBWFLinealEnergy()
{
  G4String fileName = GmParameterMgr::GetInstance()->GetStringValue(GetName()+":BWFyFileName","");
  if( fileName == "" ) {
      G4Exception("GmPSRBEBWFy::ReadBWF",
		"",
		FatalException,
		("No Biogical Weighting Function linear energy file name, please use parameter: "+GetName()+":BWFyFileName").c_str());
  }
  GmFileIn fin = GmFileIn::GetInstance(fileName);
  std::vector<G4String> wl;
  for(;;) {
    if(! fin.GetWordsInLine( wl ) ) break;
    if( wl[0] == ":DATA" ) {
      continue;
    } else if( wl.size() != 2. ) {
      G4ExceptionDescription msg;
      msg << "Line read with number of words different than two: ";
      for( size_t ii = 0; ii < wl.size(); ii++ ) {
	msg << " " << wl[ii];
      }
      G4Exception("GmPSRBEBWFy::ReadBWF",
		  "",
		  FatalException,
		  msg);
    }
    theLinealEnergy[GmGenUtils::GetValue(wl[0])] = GmGenUtils::GetValue(wl[1]);
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) )
      G4cout << " GmPSRBEBWFy::ReadBWFLinealEnergy() " << wl[0] << " = " << wl[1] << G4endl;
#endif
    
  }

}

//---------------------------------------------------------
G4bool GmPSRBEBWFy::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  // When particle enters in voxel, calculate the energy at the entry and at the exit (taking into account multiple scattering and energy loss). Divide the interval in energy intervals and score 1 per each energy interval. If last energy interval is only a fraction of the given interval, assign a weight equal to that fraction
  // It is meant to be used with a microdosimetry lineal energy, read from lineal energy file constructed multiplying y spectrum by Biological Weighting Function for a list of energies

  //----- Check particle enters volume
  const G4StepPoint* preSP = aStep->GetPreStepPoint();
  if( preSP->GetStepStatus() != fGeomBoundary ) {
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << " GmPSRBEBWFy::ProcessHits Step not entering volume " << G4endl;
#endif
    return false;
  }
  
  if( aStep == 0 ) return FALSE;  // it is 0 when called by GmScoringMgr after last event

  if( !AcceptByFilter( aStep ) ) return false;
  //  G4cout << " 3GmPSRBEBWFy::ProcessHits( " << G4endl; //GDEB

  G4double weight = aStep->GetPreStepPoint()->GetWeight(); 

  //--- Get distance to out
  G4LogicalVolume* LV = preSP->GetPhysicalVolume()->GetLogicalVolume();
  G4VSolid* solid = LV->GetSolid();
  G4Box* box = dynamic_cast<G4Box*>(solid);
  if( box == 0 ) {
    G4Exception("GmPSRBEBWFy::ProcessHits",
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
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(testVerb) )
      G4cout << "GmPSRBEBWFy kinEnergyPre " << kinEnergyPre << " kinEnergyPost " << kinEnergyPost << " distToOut " <<distToOut << " ->distIn " << distIn << " DENSITY " << mate->GetDensity()/(CLHEP::g/CLHEP::cm3) << G4endl; 
#endif  
  
  G4int iEnerPre = G4int((kinEnergyPre-theEnergyStart)/theEnergyStep);
  G4int iEnerPost = G4int((kinEnergyPost-theEnergyStart-0.01*theEnergyStep)/theEnergyStep);
  G4double enerPre = (iEnerPre+1)*theEnergyStep;
  G4double enerPost = (iEnerPost+1)*theEnergyStep;
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(testVerb) )
      G4cout << "GmPSRBEBWFy iEner " << iEnerPre << " " << iEnerPost << " ener " << enerPre << " " << enerPost << G4endl; 
#endif
  
 // use index from classifier, unless you are skipping borders of equal material voxels in G4RegularNavigation
  G4int index = GetIndex(aStep);
  std::map<G4double,G4double> theEnergyWeights;
  for( G4double ener = enerPost; ener <= enerPre; ener+=theEnergyStep ) {
    G4double weightFS = weight;
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(testVerb) )
      G4cout << " LOOPING " << ener << " WEIGHT " << weightFS << G4endl; 
#endif
    G4double enerFS = ener - theEnergyStep; // spectrum max index are named using the lower bound
    if( ener == kinEnergyPre ) {
      continue;
    } else if( ener == enerPost ) {
      weightFS *= (enerPost-kinEnergyPost)/theEnergyStep; 
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(testVerb) )
	G4cout << "V:" << index << " " << enerPre-kinEnergyPre << " 1GmPSRBEBWFy LOOP ener " << ener << " weightFS " <<weightFS << G4endl; 
#endif
    } else if( ener == enerPre ) {
      weightFS *= 1. - (enerPre-kinEnergyPre)/theEnergyStep;
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(testVerb) )
	G4cout <<"V:" << index << " " << enerPost-kinEnergyPost << " 2GmPSRBEBWFy LOOP ener " << ener << " weightFS " <<weightFS << G4endl; 
#endif
    } else {
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(testVerb) )
	G4cout << "V:" << index << " " << " 0GmPSRBEBWFy LOOP ener " << ener << " enerPre " << enerPre << " enerPost " << enerPost << " weightFS " <<weightFS << G4endl;
#endif
     
    }

    theEnergyWeights[enerFS+theEnergyStep] = weightFS; // Energy in BWFy file represents the initial proton energy
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(testVerb) )
      G4cout << " GmPSRBEBWFy::ProcessHits theEnergyWeights " << enerFS << " " << theEnergyWeights[enerFS] << G4endl; 
#endif
  }

  G4double sumyws = 0.;
  G4double sumWeights = 0.;
  for( auto ite = theEnergyWeights.begin(); ite != theEnergyWeights.end(); ite++ ) {
    auto itey = theLinealEnergy.find(ite->first);
    G4double y = 0.;
    if( itey != theLinealEnergy.end() ) {
      y = itey->second;
      //    } else {
      // G4cerr << " theLinealEnergy for energy " << ite->first << " DOES NOT EXISTS, USING 0." << G4endl; //GDEB
    }
    G4double wei = ite->second;
    sumyws += y * wei;
    sumWeights += wei;
    #ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(testVerb) )
      G4cout << " GmPSRBEBWFy::ProcessHits y=y" << y << " wei " << wei << " sumyws " << sumyws << " sumWeights " << sumWeights << G4endl;
#endif
  }
  G4double averagey = 1.;
  if( sumWeights != 0. ) {
    averagey = sumyws/sumWeights;
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(testVerb) )
      G4cout << " IN GmPSRBEBWFy::ProcessHits averagey " << averagey << " sumyws " << sumyws << " sumWeights " << sumWeights << G4endl; 
#endif
  }
  FillScorer( aStep, index, averagey, weight );

  return TRUE;
} 


