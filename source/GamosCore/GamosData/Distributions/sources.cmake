#------------------------------------------------------------------------------
# Module : Distributions
# Package: GamosCore_GamosData_Distributions
#
#------------------------------------------------------------------------------
#
set(CMAKE_VERBOSE_MAKEFILE ON)
include_directories(${CMAKE_SOURCE_DIR}/source)
include_directories(${CMAKE_SOURCE_DIR}/include)
#
# Define the GEANT4 Module.
include(UseGamosAtGeant4)
#
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosData_Distributions
	 HEADERS
		GmVNumericNDimDistribution.hh
		GmDistributionVerbosity.hh
		GmNumericDistributionLogLin.hh
		GmNumericDistributionLower.hh
		GmPolynomialDistribution.hh
		GmStringDistribution.hh
		GmNumericDistributionLinLin.hh
		GmVStringDistribution.hh
		GmNumericDistributionLinLog.hh
		GmVNumericDistribution.hh
		GmNumericDistributionUpper.hh
		GmNumericNDimDistributionUpper.hh
		GmGeometricalBiasingDistribution.hh
		GmNumericDistributionLogLog.hh
		GmDistributionRatio.hh
		GmGaussianDistribution.hh
	 SOURCES
		GmGeometricalBiasingDistribution.cc
		GmVStringDistribution.cc
		GmVNumericDistribution.cc
		plugin.cc
		GmDistributionVerbosity.cc
		GmNumericDistributionUpper.cc
		GmNumericDistributionLogLog.cc
		GmNumericNDimDistributionUpper.cc
		GmNumericDistributionLinLog.cc
		GmStringDistribution.cc
		GmVNumericNDimDistribution.cc
		GmNumericDistributionLinLin.cc
		GmPolynomialDistribution.cc
		GmNumericDistributionLogLin.cc
		GmDistributionRatio.cc
		GmNumericDistributionLower.cc
		GmGaussianDistribution.cc
	 
	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
		${Geant4_libs} 
		${ROOT_LIBRARIES} 
	GamosCore_GamosUtils
	GamosCore_GamosBase_Base
	GamosCore_GamosData_Management
		${SEAL_LIBRARIES} 

	LINK_LIBRARIES
)
