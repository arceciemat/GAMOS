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
#include "GmG4PSEnergyLost.hh"
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

#include "GmG4PSData.hh"

#include "GmPSSurfaceFlux.hh"
#include "GmPSVolumeFlux.hh"
#include "GmPSEquivDose.hh"

#include "GmPSEnergyDeposit_PrimaryGamma1stInter.hh"
#include "GmPSEnergyDeposit_PrimaryGamma1stInterAttenuation.hh"
#include "GmPSEnergyDeposit_PrimaryGammaAnyInter.hh"

#include "GmPSLET.hh"
#include "GmPSDose_LET.hh"
#include "GmPSLETEDepD.hh"
#include "GmPSLETEDepT.hh"

#include "GmPSLETBydEdx.hh"
#include "GmPSdx_LETBydEdx.hh"
#include "GmPSDose_LETBydEdx.hh"
#include "GmPSLETBydEdxD.hh"
#include "GmPSLETBydEdxT.hh"

#include "GmPSLETByESpect.hh"
#include "GmPSLET_LETByESpect.hh"
#include "GmPSLETByESpectD.hh"
#include "GmPSLETByESpectT.hh"

#include "GmPSDose_LETBydEdx_Unrestricted.hh"
#include "GmPSdx_LETBydEdx_Unrestricted.hh"
#include "GmPSLETBydEdxD_Unrestricted.hh"
#include "GmPSElectronicELost.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"

PLUGINSVC_FACTORY(GmG4PSCellCharge,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSCellFlux,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSDoseDeposit_old,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSDoseDeposit,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSDoseDeposit_NoSkipEqualMate,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSDoseDeposit_SkipEqualMateSimple,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSEnergyDeposit,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSEnergyLost,GmVPrimitiveScorer*(G4String))
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
PLUGINSVC_FACTORY(GmG4PSData,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSSurfaceFlux,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSVolumeFlux,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSSphericalDoseDeposit,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSCylindricalRZDoseDeposit,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSCylindricalRPhiDoseDeposit,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSCylindricalZPhiDoseDeposit,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSEquivDose,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmG4PSTermination,GmVPrimitiveScorer*(G4String))

PLUGINSVC_FACTORY(GmPSLET,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSDose_LET,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSLETEDepT,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSLETEDepD,GmVPrimitiveScorer*(G4String))

PLUGINSVC_FACTORY(GmPSLETBydEdx,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSdx_LETBydEdx,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSDose_LETBydEdx,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSLETBydEdxD,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSLETBydEdxT,GmVPrimitiveScorer*(G4String))

PLUGINSVC_FACTORY(GmPSLETByESpect,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSLET_LETByESpect,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSLETByESpectD,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSLETByESpectT,GmVPrimitiveScorer*(G4String))

PLUGINSVC_FACTORY(GmPSEnergyDeposit_PrimaryGamma1stInter,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSEnergyDeposit_PrimaryGamma1stInterAttenuation,GmVPrimitiveScorer*(G4String))
PLUGINSVC_FACTORY(GmPSEnergyDeposit_PrimaryGammaAnyInter,GmVPrimitiveScorer*(G4String))

#else

#include "GamosCore/GamosScoring/Management/include/GmPrimitiveScorerFactory.hh"
DEFINE_SEAL_MODULE ();

DEFINE_GAMOS_SCORER(GmG4PSCellCharge);
DEFINE_GAMOS_SCORER(GmG4PSCellFlux);
DEFINE_GAMOS_SCORER(GmG4PSDoseDeposit_old);
DEFINE_GAMOS_SCORER(GmG4PSDoseDeposit);
DEFINE_GAMOS_SCORER(GmG4PSDoseDeposit_NoSkipEqualMate);
DEFINE_GAMOS_SCORER(GmG4PSDoseDeposit_SkipEqualMateSimple);
DEFINE_GAMOS_SCORER(GmG4PSEnergyDeposit);
DEFINE_GAMOS_SCORER(GmG4PSEnergyLost);
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
DEFINE_GAMOS_SCORER(GmG4PSData);
DEFINE_GAMOS_SCORER(GmPSSurfaceFlux);
DEFINE_GAMOS_SCORER(GmPSVolumeFlux);
DEFINE_GAMOS_SCORER(GmPSSphericalDoseDeposit);
DEFINE_GAMOS_SCORER(GmPSCylindricalRZDoseDeposit);
DEFINE_GAMOS_SCORER(GmPSCylindricalRPhiDoseDeposit);
DEFINE_GAMOS_SCORER(GmPSCylindricalZPhiDoseDeposit);
DEFINE_GAMOS_SCORER(GmPSEquivDose);
DEFINE_GAMOS_SCORER(GmG4PSTermination);

DEFINE_GAMOS_SCORER(GmPSLET);
DEFINE_GAMOS_SCORER(GmPSDose_LET);
DEFINE_GAMOS_SCORER(GmPSLETEDepT);
DEFINE_GAMOS_SCORER(GmPSLETEDepD);

DEFINE_GAMOS_SCORER(GmPSLETBydEdx);
DEFINE_GAMOS_SCORER(GmPSdx_LETBydEdx);
DEFINE_GAMOS_SCORER(GmPSDose_LETBydEdx);
DEFINE_GAMOS_SCORER(GmPSLETBydEdxD);
DEFINE_GAMOS_SCORER(GmPSLETBydEdxT);

DEFINE_GAMOS_SCORER(GmPSLETByESpect);
DEFINE_GAMOS_SCORER(GmPSLET_LETByESpect);
DEFINE_GAMOS_SCORER(GmPSLETByESpectD);
DEFINE_GAMOS_SCORER(GmPSLETByESpectT);
DEFINE_GAMOS_SCORER(GmPSDose_LETBydEdx_Unrestricted);
DEFINE_GAMOS_SCORER(GmPSdx_LETBydEdx_Unrestricted);
DEFINE_GAMOS_SCORER(GmPSLETBydEdxD_Unrestricted);
DEFINE_GAMOS_SCORER(GmPSElectronicELost);

DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory, GmPSLETBydEdxD,"GmPSLETD");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory, GmPSLETBydEdxT,"GmPSLETT");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory, GmPSLETBydEdxD_Unrestricted,"GmPSLETD_Unrest");

DEFINE_GAMOS_SCORER(GmPSEnergyDeposit_PrimaryGamma1stInter);
DEFINE_GAMOS_SCORER(GmPSEnergyDeposit_PrimaryGamma1stInterAttenuation);
DEFINE_GAMOS_SCORER(GmPSEnergyDeposit_PrimaryGammaAnyInter);

DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSCellCharge,"GmPSCellCharge");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSCellFlux,"GmPSCellFlux");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSData,"GmPSData");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSDoseDeposit,"GmPSDoseDeposit");
//DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSDoseDepositVector,"GmPSDoseDepositVector");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSDoseDeposit_NoSkipEqualMate,"GmPSDoseDeposit_NoSkipEqualMate");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSDoseDeposit_SkipEqualMateSimple,"GmPSDoseDeposit_SkipEqualMateSimple");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSDoseDeposit_old,"GmPSDoseDeposit_old");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSEnergyDeposit,"GmPSEnergyDeposit");
DEFINE_SEAL_PLUGIN(GmPrimitiveScorerFactory,GmG4PSEnergyLost,"GmPSEnergyLost");
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

