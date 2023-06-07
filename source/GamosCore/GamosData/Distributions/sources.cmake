# - GamosCore_GamosData_Distributions module build definition

include(UseGamosAtGeant4)
include(UseROOT)

geant4_add_module(GamosCore_GamosData_Distributions
  PUBLIC_HEADERS
    GmPolynomialDistribution.hh
    GmNumericDistributionLogLin.hh
    GmGeometricalBiasingDistribution.hh
    GmGaussianDistribution.hh
    GmNumericNDimDistributionUpper.hh
    GmVNumericDistribution.hh
    GmNumericDistributionUpper.hh
    GmVStringDistribution.hh
    GmDistributionRatio.hh
    GmDistributionVerbosity.hh
    GmNumericDistributionLinLog.hh
    GmNumericDistributionLower.hh
    GmNumericDistributionLogLog.hh
    GmNumericDistributionLinLin.hh
    GmVNumericNDimDistribution.hh
    GmStringDistribution.hh
  SOURCES
    GmNumericNDimDistributionUpper.cc
    GmNumericDistributionLinLin.cc
    GmNumericDistributionUpper.cc
    GmDistributionVerbosity.cc
    GmNumericDistributionLower.cc
    GmNumericDistributionLogLin.cc
    GmVNumericNDimDistribution.cc
    GmDistributionRatio.cc
    GmStringDistribution.cc
    GmGaussianDistribution.cc
    GmGeometricalBiasingDistribution.cc
    plugin.cc
    GmPolynomialDistribution.cc
    GmNumericDistributionLogLog.cc
    GmVNumericDistribution.cc
    GmVStringDistribution.cc
    GmNumericDistributionLinLog.cc
)
# - Add path to generated header
geant4_module_include_directories(GamosCore_GamosData_Distributions
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealBase/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealPlatform/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/PluginManager/include>
 PUBLIC  $<BUILD_INTERFACE:${ROOT_BASE_DIR}/include>)

geant4_module_link_libraries(GamosCore_GamosData_Distributions
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    ${SEAL_LIBRARIES} 
    GamosCore_GamosUtils
    GamosCore_GamosBase_Base
    GamosCore_GamosData_Management
)
