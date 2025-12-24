#include "GmPSVolumeFlux.hh"
#include "GmScoringSurfaceBox.hh"
#include "GmScoringSurfaceTubs.hh"
#include "GmScoringSurfaceSphere.hh"
#include "GmScoringSurfaceOrb.hh"

#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

//-----------------------------------------------------------------------
GmPSVolumeFlux::GmPSVolumeFlux(G4String name)
 :GmVPrimitiveScorer(name)
{
  theUnit = CLHEP::mm*CLHEP::mm/(CLHEP::cm*CLHEP::cm);
  theUnitName = G4String("cm-2");

  theGeometryUtils = GmGeometryUtils::GetInstance();  

  theTotalVolume = 0.;

  bUseVolume = true;
}


//-----------------------------------------------------------------------
G4bool GmPSVolumeFlux::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{

  if( !AcceptByFilter( aStep ) ) return false;

  G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
  G4double volume = GetVolume(aStep);

  G4double flux = aStep->GetStepLength()/volume;

#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << " GmPSVolumeFlux::ProcessHits flux= " << flux << " stepLength " << aStep->GetStepLength() << " volume= " << volume << G4endl;
#endif     
  FillScorer( aStep, flux, preStepPoint->GetWeight() );

  return TRUE;
}

