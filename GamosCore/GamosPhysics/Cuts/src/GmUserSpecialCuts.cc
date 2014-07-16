#include "GmUserSpecialCuts.hh"
#include "GmUserLimitsMgr.hh"

#include "G4Step.hh"
#include "G4UserLimits.hh"
#include "G4VParticleChange.hh"
#include "G4LossTableManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmUserSpecialCuts::GmUserSpecialCuts(const G4String& aName)
  : G4VProcess(aName)
{
   if (verboseLevel>0)
   {
     G4cout << GetProcessName() << " is created "<< G4endl;
   }
   theLossTableManager = G4LossTableManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmUserSpecialCuts::~GmUserSpecialCuts()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmUserSpecialCuts::GmUserSpecialCuts(GmUserSpecialCuts& right)
  : G4VProcess(right)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
G4double GmUserSpecialCuts::
PostStepGetPhysicalInteractionLength( const G4Track& aTrack,
                                            G4double, // previousStepSize
                                            G4ForceCondition* condition  )
{
  // condition is set to "Not Forced"
  *condition = NotForced;

   G4double ProposedStep = DBL_MAX;
   G4UserLimits* pUserLimits = GmUserLimitsMgr::GetInstance()->GetUserLimits( aTrack.GetVolume()->GetLogicalVolume(), aTrack.GetDefinition() );
   //                 aTrack.GetVolume()->GetLogicalVolume()->GetUserLimits();
   if (pUserLimits)
   {
     // max track length
     //
     ProposedStep = (pUserLimits->GetUserMaxTrackLength(aTrack)
                   - aTrack.GetTrackLength());
     if (ProposedStep < 0.) return 0.;

     // max time limit
     //
     G4double beta  = (aTrack.GetDynamicParticle()->GetTotalMomentum())
                     /(aTrack.GetTotalEnergy());
     G4double dTime = (pUserLimits->GetUserMaxTime(aTrack)
                     - aTrack.GetGlobalTime());
     G4double temp  = beta*CLHEP::c_light*dTime;
     if (temp < 0.) return 0.;
     if (ProposedStep > temp) ProposedStep = temp;
                 
     // min remaining range and min kinetic energy
     // (for charged particle except for chargedGeantino)
     //
     G4ParticleDefinition* Particle = aTrack.GetDefinition();
     if ( (Particle->GetPDGCharge() != 0.) 
	  && (Particle->GetParticleType() != "charged_geantino"))
     {
       G4double Emin = pUserLimits->GetUserMinEkine(aTrack);
       G4double Rmin = pUserLimits->GetUserMinRange(aTrack);      
       if (Emin <= DBL_MIN && Rmin <= DBL_MIN) {
	 return ProposedStep;
       }

       //--- Check min Ekin cut
       if (Emin > DBL_MIN) {
	 G4double Ekine = aTrack.GetKineticEnergy();
	 Ekine *= 1.001; //keeping the energy gives a crazy range!!!
	 if (Ekine < Emin ) {
	   return 0.;

	   /*
	   //       const G4MaterialCutsCouple* couple = aTrack.GetMaterialCutsCouple();
	   const G4MaterialCutsCouple* couple = aTrack.GetVolume()->GetLogicalVolume()->GetMaterialCutsCouple();
	 //       G4double Range = theLossTableManager->GetRange(Particle,Ekine,couple);
	   G4double Range = theLossTableManager->GetRange(Particle,Ekine,couple);
	   G4cout << " Range " << Range  << " Rmin " << Rmin << " Ekine " << Ekine << " part " << Particle->GetParticleName() << " dens " << couple->GetMaterial()->GetDensity()/(g/cm3) << G4endl;
	   
	   // min kinetic energy (for charged particle)
	 //
	   G4double REmin = theLossTableManager->GetRange(Particle,Emin,couple);
	   temp = Range - REmin;
	   if (temp < 0.) return 0.;
	   if (ProposedStep > temp) ProposedStep = temp;
	   G4cout << " Proposed Step " << temp << "  range " << Range << " rmin " << REmin << G4endl; */
	 }	
	 
       }
       
       //--- Check min range cut
       if (Rmin > DBL_MIN) {
	 G4double Ekine = aTrack.GetKineticEnergy();
	 Ekine *= 1.001; //keeping the energy gives a crazy range!!!
	 const G4MaterialCutsCouple* couple = aTrack.GetVolume()->GetLogicalVolume()->GetMaterialCutsCouple();

	 G4double Range = theLossTableManager->GetRange(Particle,Ekine,couple);
	 if( Range < Rmin ) {
	   return 0.;

	   /*	   temp = Range - Rmin;
	   if (temp < 0.) {
	     //	   G4cout << Range << " neg range special cut proposed " << temp <<  G4endl;	 
	     return 0.;
	     //	   return Range; // this would make an small step, but e.g msc may make an smaller step, and make then many steps 
	     
	   }
	   if (ProposedStep > temp) ProposedStep = temp;
	   //	 G4cout << Range << " pos range special cut proposed " << ProposedStep <<  G4endl;
	   */
	 }	 
       }
             

     // min kinetic energy for neutral particles
     } else { 
       G4double Emin = pUserLimits->GetUserMinEkine(aTrack);
       G4double Ekine = aTrack.GetKineticEnergy();
       if (Ekine < Emin ) {
	 return 0.;
       }	
     }	

   }
   //   G4cout << " Proposed Step " << ProposedStep<< G4endl;
   return ProposedStep;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VParticleChange*
GmUserSpecialCuts::PostStepDoIt( const G4Track& aTrack,
                                 const G4Step&  )
//
// Kill the current particle, if requested by G4UserLimits
//
{
   aParticleChange.Initialize(aTrack);
   aParticleChange.ProposeEnergy(0.) ;
   aParticleChange.ProposeLocalEnergyDeposit(aTrack.GetKineticEnergy()) ;
   aParticleChange.ProposeTrackStatus(fStopAndKill);
   return &aParticleChange;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
