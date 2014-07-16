#include "Reflex/PluginService.h"

#include "GmStringDistribution.hh"
#include "GmGeometricalBiasingDistribution.hh"
#include "GmNumericDistributionLinLin.hh"
#include "GmNumericDistributionLinLog.hh"
#include "GmNumericDistributionLogLin.hh"
#include "GmNumericDistributionLogLog.hh"
#include "GmNumericDistributionUpper.hh"
#include "GmNumericDistributionLower.hh"
#include "GmDistributionRatio.hh"
#include "GmGaussianDistribution.hh"

PLUGINSVC_FACTORY(GmStringDistribution,GmVDistribution*(G4String))
PLUGINSVC_FACTORY(GmGeometricalBiasingDistribution,GmVDistribution*(G4String))
PLUGINSVC_FACTORY(GmNumericDistributionLinLin,GmVDistribution*(G4String))
PLUGINSVC_FACTORY(GmNumericDistributionLinLog,GmVDistribution*(G4String))
PLUGINSVC_FACTORY(GmNumericDistributionLogLin,GmVDistribution*(G4String))
PLUGINSVC_FACTORY(GmNumericDistributionLogLog,GmVDistribution*(G4String))
PLUGINSVC_FACTORY(GmNumericDistributionUpper,GmVDistribution*(G4String))
PLUGINSVC_FACTORY(GmNumericDistributionLower,GmVDistribution*(G4String))
PLUGINSVC_FACTORY(GmDistributionRatio,GmVDistribution*(G4String))
PLUGINSVC_FACTORY(GmGaussianDistribution,GmVDistribution*(G4String))

#include "GmDistributionVerbosity.hh"
PLUGINSVC_FACTORY(GmDistributionVerbosity,GmVVerbosity*())
