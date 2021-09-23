#include "GmInteractionFilter.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"

//--------------------------------------------------------------------
GmInteractionFilter::GmInteractionFilter(G4String name)
  :GmVFilter(name)
{
}

//--------------------------------------------------------------------
GmInteractionFilter::~GmInteractionFilter()
{ 
}

//--------------------------------------------------------------------
G4bool GmInteractionFilter::AcceptStep(const G4Step* aStep)
{
  G4VProcess* proc = const_cast<G4VProcess*>(aStep->GetPostStepPoint()->GetProcessDefinedStep());
  G4ProcessType procType = proc->GetProcessType();
  if( procType == fElectromagnetic
      || procType == fOptical
      || procType == fHadronic
      || procType == fPhotolepton_hadron
      || procType == fDecay
      || procType == fGeneral
      || procType == fParameterisation ) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmInteractionFilter::AcceptStep 1 " << G4endl;
#endif
    return TRUE;
  }

#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmInteractionFilter::AcceptStep 0 " << G4endl;
#endif
  return FALSE;
}



//-------------------------------------------------------------------------
G4bool GmInteractionFilter::AcceptStackedTrack(const G4Track* )
{
  G4Exception(" GmInteractionFilter::AcceptStackedTrack",
	      "",
	      FatalException,
	      "Cannot be called for a stacking action");

  return FALSE;
}
