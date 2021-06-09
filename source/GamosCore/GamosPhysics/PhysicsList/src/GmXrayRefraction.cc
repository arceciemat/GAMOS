#include "G4ios.hh"
#include "G4Gamma.hh"
#include "GmXrayRefraction.hh"
#include "G4GeometryTolerance.hh"
#include "GmPhysicsVerbosity.hh"

/////////////////////////////////////////////////////
// Author:      zhentian Wang
// 		2009.6.12 
//      Many thanks to implement of Optical Photon, kind of a simple version of OP
/////////////////////////////////////////////////////

GmXrayRefraction::GmXrayRefraction(const G4String& processName,
		G4ProcessType type) :
	G4VDiscreteProcess(processName, type) {

  kCarTolerance = G4GeometryTolerance::GetInstance()->GetSurfaceTolerance();
}

GmXrayRefraction::~GmXrayRefraction() {
}

G4VParticleChange*GmXrayRefraction::PostStepDoIt(const G4Track& aTrack,
		const G4Step& aStep) {

	aParticleChange.Initialize(aTrack);

	G4StepPoint* pPreStepPoint = aStep.GetPreStepPoint();
	G4StepPoint* pPostStepPoint = aStep.GetPostStepPoint();

#ifndef GAMOS_NO_VERBOSE
	if( PhysicsVerb(debugVerb) ) G4cout << "GmXrayRefraction::PostStepDoIt StepStatus " << pPostStepPoint->GetStepStatus() << G4endl;
#endif
	if (pPostStepPoint->GetStepStatus() != fGeomBoundary) {
		return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
	}

	if (aTrack.GetStepLength()<=kCarTolerance/2) {
		return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
	}

	Material1 = pPreStepPoint -> GetMaterial();
	Material2 = pPostStepPoint -> GetMaterial();

	const G4DynamicParticle* aParticle = aTrack.GetDynamicParticle();

	thePhotonMomentum = aParticle->GetTotalMomentum();
	OldMomentum = aParticle->GetMomentumDirection();

	G4MaterialPropertiesTable* aMaterialPropertiesTable = 0;
	G4MaterialPropertyVector* Rindex = 0;

#ifndef GAMOS_NO_VERBOSE
	if( PhysicsVerb(debugVerb)) G4cout << "GmXrayRefraction::PostStepDoIt MaterialPropertiesTable 1 " << aMaterialPropertiesTable << G4endl;
#endif
	aMaterialPropertiesTable = Material1->GetMaterialPropertiesTable();
	if (aMaterialPropertiesTable) {
		Rindex = aMaterialPropertiesTable->GetProperty("RINDEX");
	} else {
		aParticleChange.ProposeTrackStatus(fStopAndKill);
		return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
	}

#ifndef GAMOS_NO_VERBOSE
	if( PhysicsVerb(debugVerb)) G4cout << "GmXrayRefraction::PostStepDoIt Rindex 1 " << Rindex << G4endl;
#endif
	if (Rindex) {
	  G4bool bIsOutOfRange;
	  Rindex1 = Rindex->GetValue(thePhotonMomentum, bIsOutOfRange);
	} else {
		aParticleChange.ProposeTrackStatus(fStopAndKill);
		return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
	}
#ifndef GAMOS_NO_VERBOSE
	    if( PhysicsVerb(debugVerb)) G4cout << "GmXrayRefraction::PostStepDoIt Rindex 1 value " << Rindex1 << " for energy " << thePhotonMomentum << G4endl;
#endif

	Rindex = NULL;
#ifndef GAMOS_NO_VERBOSE
        if( PhysicsVerb(debugVerb)) G4cout << "GmXrayRefraction::PostStepDoIt Material1= " << Material1->GetName() << " Material2 " << Material2->GetName() << G4endl;
#endif
	if (Material1 == Material2) {
		return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
	}
#ifndef GAMOS_NO_VERBOSE
	if( PhysicsVerb(debugVerb)) G4cout << "GmXrayRefraction::PostStepDoIt MaterialPropertiesTable 2 " << aMaterialPropertiesTable << G4endl;
#endif
	aMaterialPropertiesTable =Material2->GetMaterialPropertiesTable();
	if (aMaterialPropertiesTable)
		Rindex = aMaterialPropertiesTable->GetProperty("RINDEX");

#ifndef GAMOS_NO_VERBOSE
	if( PhysicsVerb(debugVerb)) G4cout << "GmXrayRefraction::PostStepDoIt Rindex 2 " << Rindex << G4endl;
#endif
	if (Rindex) {
	  G4bool bIsOutOfRange;
	  Rindex2 = Rindex->GetValue(thePhotonMomentum, bIsOutOfRange);
	} else {
		aParticleChange.ProposeTrackStatus(fStopAndKill);
		return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
	}
#ifndef GAMOS_NO_VERBOSE
	    if( PhysicsVerb(debugVerb)) G4cout << "GmXrayRefraction::PostStepDoIt Rindex 2 value " << Rindex2 << " for energy " << thePhotonMomentum << G4endl;
#endif

	G4ThreeVector theGlobalPoint = pPostStepPoint->GetPosition();

	G4Navigator
			* theNavigator =G4TransportationManager::GetTransportationManager()->
			GetNavigatorForTracking();

	G4ThreeVector theLocalPoint = theNavigator->
	GetGlobalToLocalTransform().
	TransformPoint(theGlobalPoint);

	G4ThreeVector theLocalNormal;

	G4bool valid;
	theLocalNormal = theNavigator->GetLocalExitNormal(&valid);

	if (valid) {
		theLocalNormal = -theLocalNormal;
	} else {
		G4cerr << " Err in GmXrayRefraction/PostStepDoIt(). "<< G4endl;
	}
	
	theGlobalNormal = theNavigator->GetLocalToGlobalTransform().
	TransformAxis(theLocalNormal);
	if (OldMomentum * theGlobalNormal > 0.0) {

		theGlobalNormal = -theGlobalNormal;
	}
	
    Snell();//Snell's law
	NewMomentum = NewMomentum.unit();
	
#ifndef GAMOS_NO_VERBOSE
	if( PhysicsVerb(debugVerb)) G4cout << "GmXrayRefraction::PostStepDoIt momentum new " << NewMomentum << " old " << aTrack.GetMomentumDirection() << " ANGLE " << acos(NewMomentum*aTrack.GetMomentumDirection()) << G4endl;
#endif
	aParticleChange.ProposeMomentumDirection(NewMomentum);
	
	return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
}

void GmXrayRefraction::Snell() {

	G4bool Swap = false;
	G4bool Through = false;
	G4bool Done = false;

	do {

		if (Through) {
			Swap = !Swap;
			Through = false;
			theGlobalNormal = -theGlobalNormal;
			G4Swap(Material1, Material2);
			G4Swap(&Rindex1, &Rindex2);
		}
		theFacetNormal = theGlobalNormal;
		G4double PdotN = OldMomentum * theFacetNormal;
		cost1 = -PdotN;
		if (std::abs(cost1) < 1.0-kCarTolerance) {
			sint1 = std::sqrt(1.-cost1*cost1);
			sint2 = sint1*Rindex1/Rindex2; 
		} else {
			sint1 = 0.0;
			sint2 = 0.0;
		}
		if (sint2 >= 1.0) {
			if (Swap)
				Swap = !Swap;
				PdotN = OldMomentum * theFacetNormal;
				NewMomentum = OldMomentum - (2.*PdotN)*theFacetNormal;


		} else if (sint2 < 1.0) {
			if (cost1 > 0.0) {
				cost2 = std::sqrt(1.-sint2*sint2);
			} else {
				cost2 = -std::sqrt(1.-sint2*sint2);
			}
			Through = true;
			if (sint1 > 0.0) {
				G4double alpha = cost1 - cost2*(Rindex2/Rindex1);
				NewMomentum = OldMomentum + alpha*theFacetNormal;
				NewMomentum = NewMomentum.unit();
				PdotN = -cost2;
			} else { 
				NewMomentum = OldMomentum;

			}	
		}
		OldMomentum = NewMomentum.unit();
		Done = (NewMomentum * theGlobalNormal >= 0.0);


	} while (!Done);

}

G4double GmXrayRefraction::GetMeanFreePath(const G4Track&, G4double,
		G4ForceCondition* condition) {
	*condition = Forced;

	return DBL_MAX;
}
