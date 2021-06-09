#------------------------------------------------------------------------------
# 
# Module : GamosCore
# Package: GamosGenerator
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
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosGenerator

   HEADERS
       GmGenerDistPositionInUserVolumes.hh
       GmGenerEnergyDistBetaDecay.hh
       GmGenerEnergyDistConstant.hh
       GmGenerDistDirectionRandom.hh
       GmGenerDistPositionInG4Surfaces.hh
       GmVPositionOrigin.hh
       GmGenerDistPositionDiscGaussian.hh
       GmVGenerDistDirection.hh
       GmVGenerDistPosition2D.hh
       GmGenerator.hh
       GmGenerDistPositionPoint.hh
       GmGenerDistPositionInVoxelsFromFile.hh
       GmVGenerDistTime.hh
       GmIsotopeDecay.hh
       GmVGenerDist.hh
       GmGeneratorFromFile.hh
       GmGenerDistPositionVoxelPhantomMaterials.hh
       GmGeneratorIsotope.hh
       GmPositionUserVolumes.hh
       GmGenerDistEnergyFromFile.hh
       GmGenerDistEnergyGaussian.hh
       GmVGenerDistEnergy.hh
       GmGeneratorFromBinFile.hh
       GmParticleSource.hh
       GmIsotopeMessenger.hh
       GmVGenerDistPosition.hh
       GmGeneratorMgr.hh
       GmGenerDistEnergyConstantIsotopeDecay.hh
       GmGeneratorMessenger.hh
       GmGenerVerbosity.hh
       GmVPositionObjectPos.hh
       GmIsotopeMgr.hh
       GmGeneratorChangeEnergyAndMaterial.hh
       GmGenerDistPositionDirectionTowardsBox.hh
       GmGenerDistEnergyRandomFlat.hh
       GmGenerDistPositionSquare.hh
       GmGeneratorFromTextFile.hh
       GmGenerDistTimeConstant.hh
       GmPositionG4Volumes.hh
       GmGenerDistPositionRectangle.hh
       GmGenerDistPositionDisc.hh
       GmGenerDistDirectionGaussian.hh
       GmGenerDistTimeDecay.hh
       GmIsotopeSource.hh
       GmSingleParticleSource.hh
       GmGenerDistPositionDirectionInVolumeSurface.hh
       GmGenerDistDirectionCone.hh
       GmPositionSurfacePos.hh
       GmGenerDistEnergyMCNPisolethargical.hh
       GmIsotope.hh
       GmGenerDistEnergyIsolethargical.hh
       GmDoubleBackToBackParticleSource.hh
       GmGenerDistPositionInUserSurfaces.hh
       GmVGenerDistPositionVolumesAndSurfaces.hh
       GmGenerDistTimeConstantChange.hh
       GmPositionVolumePos.hh
       GmGenerDistPositionInG4VolumesGeneral.hh
       GmGenerDistEnergyConstant.hh
       GmGenerDistPositionLineSteps.hh
       GmGenerDistEnergyBetaDecay.hh
       GmGenerDistDirectionConst.hh
       GmGenerDistPositionInG4Volumes.hh

   SOURCES
       plugin.cc
       GmGenerDistEnergyIsolethargical.cc
       GmIsotopeSource.cc
       GmGenerDistPositionInVoxelsFromFile.cc
       GmGeneratorMessenger.cc
       GmGenerDistTimeDecay.cc
       GmGenerDistPositionDirectionTowardsBox.cc
       GmGenerDistEnergyGaussian.cc
       GmGenerDistPositionDirectionInVolumeSurface.cc
       GmGenerDistPositionPoint.cc
       GmPositionVolumePos.cc
       GmGenerDistPositionSquare.cc
       GmGenerDistEnergyConstantIsotopeDecay.cc
       GmGeneratorFromBinFile.cc
       GmGenerDistPositionVoxelPhantomMaterials.cc
       GmGenerDistPositionDisc.cc
       GmIsotope.cc
       GmGenerDistTimeConstant.cc
       GmVGenerDist.cc
       GmGenerDistEnergyConstant.cc
       GmPositionG4Volumes.cc
       GmGeneratorChangeEnergyAndMaterial.cc
       GmGenerDistPositionRectangle.cc
       GmVGenerDistPosition2D.cc
       GmGenerDistPositionInG4VolumesGeneral.cc
       GmGenerDistPositionDiscGaussian.cc
       GmGenerDistEnergyMCNPisolethargical.cc
       GmVGenerDistPositionVolumesAndSurfaces.cc
       GmGenerDistPositionLineSteps.cc
       GmGeneratorFromFile.cc
       GmGeneratorMgr.cc
       GmPositionUserVolumes.cc
       GmSingleParticleSource.cc
       GmGenerDistDirectionRandom.cc
       GmGenerDistDirectionConst.cc
       GmGenerDistTimeConstantChange.cc
       GmGenerVerbosity.cc
       GmGenerDistEnergyRandomFlat.cc
       GmGenerDistEnergyBetaDecay.cc
       GmPositionSurfacePos.cc
       GmIsotopeDecay.cc
       GmGenerDistDirectionGaussian.cc
       GmGeneratorFromTextFile.cc
       GmGenerDistEnergyFromFile.cc
       GmParticleSource.cc
       GmDoubleBackToBackParticleSource.cc
       GmIsotopeMgr.cc
       GmGenerDistDirectionCone.cc
       GmGenerator.cc
 
   GRANULAR_DEPENDENCIES

   GLOBAL_DEPENDENCIES
    ${Geant4_LIBRARIES}
    ${Root_LIBRARIES}
  GamosCore_GamosUserActionMgr
  GamosCore_GamosBase_Base
  GamosCore_GamosUtils
  GamosCore_GamosData_Distributions
  GamosCore_GamosGeometry

   LINK_LIBRARIES
)

# List any source specific properties here
