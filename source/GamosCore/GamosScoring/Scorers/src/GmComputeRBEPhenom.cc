#include "GmComputeRBEPhenom.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "G4EmCalculator.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Gamma.hh"
#include "G4VEnergyLossProcess.hh"
#include "G4ProcessManager.hh"
#include <cmath>

GmComputeRBEPhenom::GmComputeRBEPhenom(G4String name)
{
  theAlphaBetaX = GmParameterMgr::GetInstance()->GetNumericValue(name+":AlphaBetaX",-DBL_MAX);
  if ( theAlphaBetaX == -DBL_MAX ) {
    G4Exception("GmComputeRBEPhenom::GmComputeRBEPhenom",
		"Scor025",
		FatalException,
		("It is mandatory to set the parameter "+name+":AlphaBetaX").c_str());    
  }
  if( ScoringVerb(debugVerb) ) G4cout << " GmComputeRBEPhenom::GmComputeRBEPhenom AlphaBetaX " << theAlphaBetaX << G4endl;
}

GmComputeRBEPhenom::~GmComputeRBEPhenom()
{;}


G4double GmComputeRBEPhenom::GetRBE(G4Step* aStep )
{
  G4double dedx = GetElectronicDEDX(aStep, false);
  //  G4double dedx = aStep->GetTotalEnergyDeposit()/aStep->GetStepLength();
  G4double dose = aStep->GetTotalEnergyDeposit();
  G4double rbeMin = GetRBEMin(dedx,theAlphaBetaX);
  G4double rbeMax = GetRBEMax(dedx,theAlphaBetaX);

  G4double rbe = 1./dose*(sqrt(sqr(theAlphaBetaX)+4*dose*theAlphaBetaX*rbeMax+4*sqr(dose*rbeMin))-theAlphaBetaX);

  if( ScoringVerb(debugVerb) ) G4cout << " GmComputeRBEPhenom::GetRBE posZ " << aStep->GetPostStepPoint()->GetPosition().z() << " energy " << 
				   (aStep->GetPreStepPoint()->GetKineticEnergy()+aStep->GetPostStepPoint()->GetKineticEnergy())/2.
				       << " dedx " << dedx << " dose " << dose
				       << " rbeMin " << rbeMin << " rbeMax " << rbeMax << " rbe= " << rbe << G4endl;

  return rbe;
  
}
