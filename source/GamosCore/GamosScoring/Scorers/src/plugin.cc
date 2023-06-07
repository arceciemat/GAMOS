#include "GmG4PSCellCharge.hh"
#include "GmG4PSCellFlux.hh"
#include "GmG4PSDoseDeposit_old.hh"
#include "GmG4PSDoseDeposit.hh"
#include "GmG4PSDoseDeposit_NoSkipEqualMate.hh"
#include "GmG4PSDoseDeposit_SkipEqualMateSimple.hh"
#include "GmPSSphericalDoseDeposit.hh"
#include "GmPSCylindricalRZDoseDeposit.hh"
#include "GmPSCylindricalRPhiDoseDeposit.hh"
#include "GmPSCylindricalZPhiDoseDeposit.hh"
#include "GmG4PSEnergyDeposit.hh"
#include "GmG4PSEnergyLoss.hh"
#include "GmG4PSMinKinEAtGeneration.hh"
#include "GmG4PSNofCollision.hh"
#include "GmG4PSNofSecondary.hh"
#include "GmG4PSNofStep.hh"
#include "GmG4PSPassageCellFlux.hh"
#include "GmG4PSPassageCellCurrent.hh"
#include "GmG4PSPassageTrackLength.hh"
#include "GmG4PSPopulation.hh"
#include "GmG4PSTrackCounter.hh"
#include "GmG4PSTrackLength.hh"
#include "GmG4PSTermination.hh"
#include "GmG4PSKerma.hh"
#include "GmPSConstant.hh"
#include "GmPSRBEPhenom_Carabe.hh"

#include "GmG4PSData.hh"

#include "GmPSSurfaceFlux.hh"
#include "GmPSVolumeFlux.hh"
#include "GmPSEquivDose.hh"

#include "GmPSEnergyDeposit_PrimaryGamma1stInter.hh"
#include "GmPSEnergyDeposit_PrimaryGamma1stInterAttenuation.hh"
#include "GmPSEnergyDeposit_PrimaryGammaAnyInter.hh"

#include "GmPSLET_EDep.hh"
#include "GmPSDose_LET_EDep.hh"
#include "GmPSLET_EDepD.hh"
#include "GmPSLET_EDepT.hh"

#include "GmPSLET_dEdx.hh"
#include "GmPSdx_LET_dEdx.hh"
#include "GmPSDose_LET_dEdx.hh"
#include "GmPSLET_dEdxD.hh"
#include "GmPSLET_dEdxT.hh"
#include "GmPSDEDX.hh"

#include "GmPSLET_ESpect.hh"
#include "GmPSLET_LETESpect.hh"
#include "GmPSLET_ESpectD.hh"
#include "GmPSLET_ESpectT.hh"

#include "GmPSLET_dEdx_unrestr.hh"
#include "GmPSLET_dEdx_unrestrD.hh"
#include "GmPSLET_dEdx_unrestrT.hh"
#include "GmPSDose_LET_dEdx_unrestr.hh"
#include "GmPSdx_LET_dEdx_unrestr.hh"
#include "GmPSElectronicdEdx.hh"
#include "GmPSdEdxElectronicELoss.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"

PLUGINSVC_FACTORY(GmG4PSCellCharge,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSCellFlux,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSDoseDeposit_old,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSDoseDeposit,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSDoseDeposit_NoSkipEqualMate,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSDoseDeposit_SkipEqualMateSimple,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSEnergyDeposit,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSEnergyLoss,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSMinKinEAtGeneration,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSNofCollision,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSNofSecondary,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSNofStep,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSPassageCellFlux,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSPassageCellCurrent,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSPassageTrackLength,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSPopulation,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSTrackCounter,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSTrackLength,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSKerma,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSConstant,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSRBEPhenom_Carabe,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSData,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSSurfaceFlux,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSVolumeFlux,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSSphericalDoseDeposit,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSCylindricalRZDoseDeposit,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSCylindricalRPhiDoseDeposit,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSCylindricalZPhiDoseDeposit,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSEquivDose,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSTermination,GmVPrimitiveScorer*(G4String))

PLUGINSVC_FACTORY(GmPSLET_EDep,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSDose_LET_EDep,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSLET_EDepT,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSLET_EDepD,GmVPrimitiveScorer*(G4String))

PLUGINSVC_FACTORY(GmPSLET_dEdx,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSdx_LET_dEdx,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSDose_LET_dEdx,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSLET_dEdxD,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSLET_dEdxT,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSDEDX,GmVPrimitiveScorer*(G4String))

PLUGINSVC_FACTORY(GmPSLET_ESpect,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSLET_LETESpect,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSLET_ESpectD,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSLET_ESpectT,GmVPrimitiveScorer*(G4String))

PLUGINSVC_FACTORY(GmPSEnergyDeposit_PrimaryGamma1stInter,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSEnergyDeposit_PrimaryGamma1stInterAttenuation,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSEnergyDeposit_PrimaryGammaAnyInter,GmVPrimitiveScorer*(G4String))

#else

#include "GamosCore/GamosScoring/Management/include/GmPrimitiveScorerFactory.hh"
DEFINE_SEAL_MODULE ();

DEFINE_GAMOS_SCORER(GmG4PSCellCharge);
DEFINE_GAMOS_SCORER(GmG4PSCellFlux);
DEFINE_GAMOS_SCORER(GmG4PSDoseDeposit_old);
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSDoseDeposit_old,"GmG4PSDoseDeposited_old");
DEFINE_GAMOS_SCORER(GmG4PSDoseDeposit);
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSDoseDeposit,"GmG4PSDoseDeposited");
DEFINE_GAMOS_SCORER(GmG4PSDoseDeposit_NoSkipEqualMate);
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSDoseDeposit_NoSkipEqualMate,"GmG4PSDoseDeposited_NoSkipEqualMate");
DEFINE_GAMOS_SCORER(GmG4PSDoseDeposit_SkipEqualMateSimple);
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSDoseDeposit_SkipEqualMateSimple,"GmG4PSDoseDeposited_SkipEqualMateSimple");
DEFINE_GAMOS_SCORER(GmG4PSEnergyDeposit);
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSEnergyDeposit,"GmG4PSEnergyDeposited");
DEFINE_GAMOS_SCORER(GmG4PSEnergyLoss);
DEFINE_GAMOS_SCORER(GmG4PSMinKinEAtGeneration);
DEFINE_GAMOS_SCORER(GmG4PSNofCollision);
DEFINE_GAMOS_SCORER(GmG4PSNofSecondary);
DEFINE_GAMOS_SCORER(GmG4PSNofStep);
DEFINE_GAMOS_SCORER(GmG4PSPassageCellFlux);
DEFINE_GAMOS_SCORER(GmG4PSPassageCellCurrent);
DEFINE_GAMOS_SCORER(GmG4PSPassageTrackLength);
DEFINE_GAMOS_SCORER(GmG4PSPopulation);
DEFINE_GAMOS_SCORER(GmG4PSTrackCounter);
DEFINE_GAMOS_SCORER(GmG4PSTrackLength);
DEFINE_GAMOS_SCORER(GmG4PSKerma);
DEFINE_GAMOS_SCORER(GmPSConstant);
DEFINE_GAMOS_SCORER(GmPSRBEPhenom_Carabe);
DEFINE_GAMOS_SCORER(GmG4PSData);
DEFINE_GAMOS_SCORER(GmPSSurfaceFlux);
DEFINE_GAMOS_SCORER(GmPSVolumeFlux);
DEFINE_GAMOS_SCORER(GmPSSphericalDoseDeposit);
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmPSSphericalDoseDeposit,"GmPSSphericalDoseDeposited");
DEFINE_GAMOS_SCORER(GmPSCylindricalRZDoseDeposit);
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmPSCylindricalRZDoseDeposit,"GmPSCylindricalRZDoseDeposited");
DEFINE_GAMOS_SCORER(GmPSCylindricalRPhiDoseDeposit);
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmPSCylindricalRPhiDoseDeposit,"GmPSCylindricalRPhiDoseDeposited");
DEFINE_GAMOS_SCORER(GmPSCylindricalZPhiDoseDeposit);
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmPSCylindricalZPhiDoseDeposit,"GmPSCylindricalZPhiDoseDeposited");
DEFINE_GAMOS_SCORER(GmPSEquivDose);
DEFINE_GAMOS_SCORER(GmG4PSTermination);

DEFINE_GAMOS_SCORER(GmPSLET_EDep);
DEFINE_GAMOS_SCORER(GmPSDose_LET_EDep);
DEFINE_GAMOS_SCORER(GmPSLET_EDepT);
DEFINE_GAMOS_SCORER(GmPSLET_EDepD);

DEFINE_GAMOS_SCORER(GmPSLET_dEdx);
DEFINE_GAMOS_SCORER(GmPSdx_LET_dEdx);
DEFINE_GAMOS_SCORER(GmPSDose_LET_dEdx);
DEFINE_GAMOS_SCORER(GmPSLET_dEdxD);
DEFINE_GAMOS_SCORER(GmPSLET_dEdxT);
DEFINE_GAMOS_SCORER(GmPSDEDX);

DEFINE_GAMOS_SCORER(GmPSLET_ESpect);
DEFINE_GAMOS_SCORER(GmPSLET_LETESpect);
DEFINE_GAMOS_SCORER(GmPSLET_ESpectD);
DEFINE_GAMOS_SCORER(GmPSLET_ESpectT);
DEFINE_GAMOS_SCORER(GmPSLET_dEdx_unrestr);
DEFINE_GAMOS_SCORER(GmPSLET_dEdx_unrestrD);
DEFINE_GAMOS_SCORER(GmPSLET_dEdx_unrestrT);
DEFINE_GAMOS_SCORER(GmPSDose_LET_dEdx_unrestr);
DEFINE_GAMOS_SCORER(GmPSdx_LET_dEdx_unrestr);
DEFINE_GAMOS_SCORER(GmPSElectronicdEdx);
DEFINE_GAMOS_SCORER(GmPSdEdxElectronicELoss);

DEFINE_GAMOS_SCORER(GmPSEnergyDeposit_PrimaryGamma1stInter);
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmPSEnergyDeposit_PrimaryGamma1stInter,"GmPSEnergyDeposited_PrimaryGamma1stInter");
DEFINE_GAMOS_SCORER(GmPSEnergyDeposit_PrimaryGamma1stInterAttenuation);
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmPSEnergyDeposit_PrimaryGamma1stInterAttenuation,"GmPSEnergyDeposited_PrimaryGamma1stInterAttenuation");
DEFINE_GAMOS_SCORER(GmPSEnergyDeposit_PrimaryGammaAnyInter);
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmPSEnergyDeposit_PrimaryGammaAnyInter,"GmPSEnergyDeposited_PrimaryGammaAnyInter");

DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSCellCharge,"GmPSCellCharge");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSCellFlux,"GmPSCellFlux");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSData,"GmPSData");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSDoseDeposit,"GmPSDoseDeposit");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSDoseDeposit,"GmPSDoseDeposited");
//DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSDoseDepositVector,"GmPSDoseDepositVector");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSDoseDeposit_NoSkipEqualMate,"GmPSDoseDeposit_NoSkipEqualMate");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSDoseDeposit_NoSkipEqualMate,"GmPSDoseDeposited_NoSkipEqualMate");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSDoseDeposit_SkipEqualMateSimple,"GmPSDoseDeposit_SkipEqualMateSimple");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSDoseDeposit_SkipEqualMateSimple,"GmPSDoseDeposited_SkipEqualMateSimple");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSDoseDeposit_old,"GmPSDoseDeposit_old");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSDoseDeposit_old,"GmPSDoseDeposited_old");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSEnergyDeposit,"GmPSEnergyDeposit");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSEnergyDeposit,"GmPSEnergyDeposited");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSEnergyLoss,"GmPSEnergyLost");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSKerma,"GmPSKerma");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSMinKinEAtGeneration,"GmPSMinKinEAtGeneration");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSNofCollision,"GmPSNofCollision");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSNofSecondary,"GmPSNofSecondary");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSNofStep,"GmPSNofStep");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSPassageCellCurrent,"GmPSPassageCellCurrent");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSPassageCellFlux,"GmPSPassageCellFlux");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSPassageTrackLength,"GmPSPassageTrackLength");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSPopulation,"GmPSPopulation");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSTermination,"GmPSTermination");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSTrackCounter,"GmPSTrackCounter");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSTrackLength,"GmPSTrackLength");

#endif

