#include "GmStepLimiter.hh"
#include "GmUserLimitsMgr.hh"

#include "G4Step.hh"
#include "G4UserLimits.hh"
#include "G4VParticleChange.hh"


////////////////////////////////////
GmStepLimiter::GmStepLimiter(const G4String& aName)
  : G4VProcess(aName)
{
   if (verboseLevel>1) {
     G4cout << GetProcessName() << " is created "<< G4endl;
   }
}


////////////////////////
GmStepLimiter::~GmStepLimiter()
{
}


////////////////////////
GmStepLimiter::GmStepLimiter(GmStepLimiter& right)
  : G4VProcess(right)
{
}

 
////////////////
G4double 
  GmStepLimiter::PostStepGetPhysicalInteractionLength( 
				       const G4Track& aTrack,
				       G4double, // previousStepSize
				       G4ForceCondition* condition  )
{
  // condition is set to "Not Forced"
  *condition = NotForced;

   G4double proposedStep = DBL_MAX;
   G4UserLimits* pUserLimits = GmUserLimitsMgr::GetInstance()->GetUserLimits( aTrack.GetVolume()->GetLogicalVolume(), aTrack.GetDefinition() );

   //                 aTrack.GetVolume()->GetLogicalVolume()->GetUserLimits();
   if (pUserLimits) {
     // max allowed step length
     //
     proposedStep = pUserLimits->GetMaxAllowedStep(aTrack);
     if (proposedStep < 0.) proposedStep =0.; 
   }
   return proposedStep;
}

///////////////
G4VParticleChange*
  GmStepLimiter::PostStepDoIt( const G4Track& aTrack,
                                 const G4Step&  )
// Do Nothing
//
{
   aParticleChange.Initialize(aTrack);
   return &aParticleChange;
}
