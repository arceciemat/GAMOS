#include "GmPSEnergyDeposit_PrimaryGammaAnyInter.hh"
#include "G4UnitsTable.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "G4Gamma.hh"
#include "G4Event.hh"

//----------------------------------------------------------------------
GmPSEnergyDeposit_PrimaryGammaAnyInter::GmPSEnergyDeposit_PrimaryGammaAnyInter(G4String name)
     :GmPSEnergyDeposit_PrimaryGamma1stInter(name)
{
  bCountAllInteractions = true;
}

//----------------------------------------------------------------------
GmPSEnergyDeposit_PrimaryGammaAnyInter::~GmPSEnergyDeposit_PrimaryGammaAnyInter()
{}

