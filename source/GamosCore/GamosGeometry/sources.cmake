#------------------------------------------------------------------------------
# 
# Module : GamosCore
# Package: GamosGeometry
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
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosGeometry

   HEADERS
       GmGeometryFromText.hh
       GmGeomVerbosity.hh
       GmParallelWorld.hh
       GmModuleMLC.hh
       GmGeomTextDumperUA.hh
       GmDetector.hh
       GmVModule.hh
       GmModuleRangeModulator.hh
       GmUniformElectricField.hh
       GmGeomTextDetectorBuilder.hh
       GmUniformEMField.hh
       GmGeomTextLineProcessor.hh
       GmGeomUseMateDetectorBuilder.hh
       GmSolid.hh
       GmTouchable.hh
       GmRegionData.hh
       GmGeometryUtils.hh
       GmGeometryUseMaterials.hh
       GmParallelWorldCreator.hh
       GmModuleJaws.hh
       GmUniformMagField.hh
       GmRegionCutsMgr.hh
       GmGeometryMessenger.hh
       GmDetectorPart.hh
       GmOpticalPropertiesMgr.hh

   SOURCES
       GmGeomTextLineProcessor.cc
       plugin.cc
       GmGeometryUtils.cc
       GmGeometryUseMaterials.cc
       GmRegionCutsMgr.cc
       GmModuleRangeModulator.cc
       GmTouchable.cc
       GmVModule.cc
       GmGeometryMessenger.cc
       GmOpticalPropertiesMgr.cc
       GmUniformElectricField.cc
       GmSolid.cc
       GmGeometryFromText.cc
       GmGeomTextDumperUA.cc
       GmGeomVerbosity.cc
       GmParallelWorld.cc
       GmUniformMagField.cc
       GmModuleJaws.cc
       GmUniformEMField.cc
       GmRegionData.cc
       GmDetector.cc
       GmParallelWorldCreator.cc
       GmGeomUseMateDetectorBuilder.cc
       GmModuleMLC.cc
       GmGeomTextDetectorBuilder.cc
 
   GRANULAR_DEPENDENCIES

   GLOBAL_DEPENDENCIES
    ${Geant4_LIBRARIES}
    ${Root_LIBRARIES}
  GamosCore_GamosBase_Base
  GamosCore_GamosUtils
  GamosCore_GamosUserActionMgr

   LINK_LIBRARIES
)

# List any source specific properties here
