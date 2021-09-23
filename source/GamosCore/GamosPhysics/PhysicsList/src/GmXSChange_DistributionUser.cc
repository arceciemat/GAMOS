#include "GmXSChange_DistributionUser.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmDistributionMgr.hh"
#include "GamosCore/GamosData/Distributions/include/GmVNumericDistribution.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmXSChange_DistributionUser::GmXSChange_DistributionUser(const G4String& name)
{
  BuildDistribution(name);
}

void GmXSChange_DistributionUser::BuildDistribution(G4String name)
{
  //  G4cout << this << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ GmXSChange_DistributionUser::BuildDistribution " << G4endl; //GDEB
  
  G4String distName = GmParameterMgr::GetInstance()->GetStringValue(name+":Distribution","");
  if( distName != "" ) {
    std::vector<G4String> params;
    params.push_back(distName);
    params.push_back(distName);
    GmVDistribution* dist = GmDistributionMgr::GetInstance()->FindOrBuildDistribution(params);
    theDistribution = dynamic_cast<GmVNumericDistribution*>(dist);
    if( !dist ) {
      G4Exception("GmPEEffectFluoModel_XSChange::GmPEEffectFluoModel_XSChange",
		  "",
		  FatalException,
		  ("Distribution is not of type GmNumericDistribution: "+distName).c_str());
    }
    
  } else {
    G4Exception("GmPEEffectFluoModel_XSChange::GmPEEffectFluoModel_XSChange",
		"",
		FatalException,
		"No distribution given, please use /gamos/setParam phot:Distribution MY_DISTRIBUTION_NAME");
  }

}

G4double GmXSChange_DistributionUser::GetDistributionValue(G4double energy)
{
  return theDistribution->GetNumericValueFromIndex(energy);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

