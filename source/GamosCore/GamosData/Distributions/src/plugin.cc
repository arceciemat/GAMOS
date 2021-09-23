#include "GmStringDistribution.hh"
#include "GmGeometricalBiasingDistribution.hh"
#include "GmNumericDistributionLinLin.hh"
#include "GmNumericDistributionLinLog.hh"
#include "GmNumericDistributionLogLin.hh"
#include "GmNumericDistributionLogLog.hh"
#include "GmNumericDistributionUpper.hh"
#include "GmNumericDistributionLower.hh"
#include "GmNumericNDimDistributionUpper.hh"
#include "GmDistributionRatio.hh"
#include "GmGaussianDistribution.hh"

#include "GmDistributionVerbosity.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"

PLUGINSVC_FACTORY(GmStringDistribution,GmVDistribution*(G4String))
PLUGINSVC_FACTORY(GmGeometricalBiasingDistribution,GmVDistribution*(G4String))
PLUGINSVC_FACTORY(GmNumericDistributionLinLin,GmVDistribution*(G4String))
PLUGINSVC_FACTORY(GmNumericDistributionLinLog,GmVDistribution*(G4String))
PLUGINSVC_FACTORY(GmNumericDistributionLogLin,GmVDistribution*(G4String))
PLUGINSVC_FACTORY(GmNumericDistributionLogLog,GmVDistribution*(G4String))
PLUGINSVC_FACTORY(GmNumericDistributionUpper,GmVDistribution*(G4String))
PLUGINSVC_FACTORY(GmNumericDistributionLower,GmVDistribution*(G4String))

PLUGINSVC_FACTORY(GmNumericNDimDistributionUpper,GmVDistribution*(G4String))

PLUGINSVC_FACTORY(GmDistributionRatio,GmVDistribution*(G4String))
PLUGINSVC_FACTORY(GmGaussianDistribution,GmVDistribution*(G4String))

PLUGINSVC_FACTORY(GmDistributionVerbosity,GmVVerbosity*())

#else 
#include "SEAL_Foundation/PluginManager/PluginManager/ModuleDef.h"
DEFINE_SEAL_MODULE ();

#include "GamosCore/GamosBase/Base/include/GmDistributionFactory.hh"

DEFINE_GAMOS_DISTRIBUTION(GmStringDistribution);
DEFINE_GAMOS_DISTRIBUTION(GmGeometricalBiasingDistribution);
DEFINE_GAMOS_DISTRIBUTION(GmNumericDistributionLinLin);
DEFINE_GAMOS_DISTRIBUTION(GmNumericDistributionLinLog);
DEFINE_GAMOS_DISTRIBUTION(GmNumericDistributionLogLin);
DEFINE_GAMOS_DISTRIBUTION(GmNumericDistributionLogLog);
DEFINE_GAMOS_DISTRIBUTION(GmNumericDistributionUpper);
DEFINE_GAMOS_DISTRIBUTION(GmNumericDistributionLower);
DEFINE_GAMOS_DISTRIBUTION(GmNumericNDimDistributionUpper);
DEFINE_GAMOS_DISTRIBUTION(GmDistributionRatio);
DEFINE_GAMOS_DISTRIBUTION(GmGaussianDistribution);

#include "GamosCore/GamosBase/Base/include/GmVerbosityFactory.hh"
#include "GmDistributionVerbosity.hh"

DEFINE_SEAL_PLUGIN(GmVerbosityFactory, GmDistributionVerbosity, "GmDistributionVerbosity");

#endif
