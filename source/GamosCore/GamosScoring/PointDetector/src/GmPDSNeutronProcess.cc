#include "GmPDSNeutronProcess.hh"
#include "GmPDSProcessHelper.hh"
#include "GmPDSInteractionAngleManager.hh"
#include "GmPDSScore.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GmPDSUtils.hh"

#include "G4Neutron.hh"
#include "G4Geantino.hh"
#include "G4SteppingManager.hh"
#include "G4LogicalVolumeStore.hh"

//#include <math.h>

//------------------------------------------------------------------
GmPDSNeutronProcess::GmPDSNeutronProcess(const G4String& name, G4SteppingManager* fpSM ): GmPDSVProcess( name, fpSM)
{
  theOriginalParticle = G4Neutron::Neutron();

  G4String angleFileName = GetPDSStringParameter("AngleDeviationFileName","neutron","");

  theInteractionAngleManager = new GmPDSInteractionAngleManager(angleFileName);

  bUseMaximumDistance = G4bool(GetPDSNumericParameter("UseVRMaximumDistance","neutron",0));
  theMaximumDistance = GetPDSNumericParameter("MaximumDistance","neutron",1000.)/CLHEP::cm;
  theInvMaximumDistanceRR = 1./(GetPDSNumericParameter("MaximumDistanceRRFactor","neutron",100.));


  theExclusionRadius = GetPDSNumericParameter("ExclusionRadius",theOriginalParticle->GetParticleName(),1.*CLHEP::mm);

}
  
//------------------------------------------------------------------
GmPDSNeutronProcess::~GmPDSNeutronProcess()
{
}


//------------------------------------------------------------------
void GmPDSNeutronProcess::AddHelper( GmPDSProcessHelper* helper )
{
  theHelpers[helper->IsForNeutron()] = helper;
  theCurrentHelper = helper; //there is only one helper

  theClassifier = theCurrentHelper->theClassifier;
  bClassifierOnTrack= G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmPDS:ClassifierOnTrack",1)); 
  bFiltersOnTrack= G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmPDS:FiltersOnTrack",1));
}
