#------------------------------------------------------------------------------
# 
# Module : GamosCore
# Package: GamosData_Distributions
#
#------------------------------------------------------------------------------
# List external includes needed.

# List external includes needed.
include(GamosSetup)
include(UseRoot)

#
# Define the GAMOS Module.
#
include(Geant4MacroDefineModule)
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosData_Distributions

   HEADERS
       GmNumericDistributionLinLog.hh
       GmPolynomialDistribution.hh
       GmNumericDistributionLower.hh
       GmDistributionVerbosity.hh
       GmNumericDistributionLogLog.hh
       GmNumericDistributionLogLin.hh
       GmNumericDistributionLinLin.hh
       GmStringDistribution.hh
       GmNumericDistributionUpper.hh
       GmVStringDistribution.hh
       GmGeometricalBiasingDistribution.hh
       GmVNumericDistribution.hh
       GmDistributionRatio.hh
       GmGaussianDistribution.hh

   SOURCES
       plugin.cc
       GmNumericDistributionLower.cc
       GmNumericDistributionUpper.cc
       GmGeometricalBiasingDistribution.cc
       GmNumericDistributionLinLin.cc
       GmGaussianDistribution.cc
       GmDistributionRatio.cc
       GmVStringDistribution.cc
       GmVNumericDistribution.cc
       GmDistributionVerbosity.cc
       GmStringDistribution.cc
       GmPolynomialDistribution.cc
       GmNumericDistributionLinLog.cc
       GmNumericDistributionLogLin.cc
       GmNumericDistributionLogLog.cc
 
   GRANULAR_DEPENDENCIES

   GLOBAL_DEPENDENCIES
    ${Geant4_LIBRARIES}
    ${Root_LIBRARIES}
  GamosCore_GamosBase_Base
  GamosCore_GamosUtils
  GamosCore_GamosData_Management

   LINK_LIBRARIES
)

# List any source specific properties here
