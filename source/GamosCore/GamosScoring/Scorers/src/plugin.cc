#include "Reflex/PluginService.h"

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
#include "GmG4PSKerma.hh"

#include "GmG4PSData.hh"

#include "GmPSSurfaceFlux.hh"
#include "GmPSVolumeFlux.hh"


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
