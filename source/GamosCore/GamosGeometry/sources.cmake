# - GamosCore_GamosGeometry module build definition

include(UseGamosAtGeant4)
include(UseROOT)

geant4_add_module(GamosCore_GamosGeometry
  PUBLIC_HEADERS
    GmGeomVerbosity.hh
    GmModuleRangeModulator.hh
    GmOpticalPropertiesMgr.hh
    GmUniformMagField.hh
    GmSolid.hh
    GmDICOMIntersectVolume.hh
    GmGeometryUtils.hh
    GmParallelWorldCreator.hh
    GmTouchable.hh
    GmRegionData.hh
    GmModuleMLC.hh
    GmDetector.hh
    GmGeometryUseMaterials.hh
    GmDetectorPart.hh
    GmGeometryFactory.hh
    GmGeometryFromText.hh
    GmUniformElectricField.hh
    GmGeomTextLineProcessor.hh
    GmGeomTextDumperUA.hh
    GmGeomUseMateDetectorBuilder.hh
    GmModuleMgr.hh
    GmRegionCutsMgr.hh
    GmUniformEMField.hh
    GmReadPhantomGeometry.hh
    GmParallelWorld.hh
    GmGeomTextDetectorBuilder.hh
    GmModuleJaws.hh
    GmGeometryMessenger.hh
    GmVModule.hh
  SOURCES
    GmSolid.cc
    GmGeomTextLineProcessor.cc
    GmGeometryUtils.cc
    GmOpticalPropertiesMgr.cc.old
    GmDICOMIntersectVolume.cc
    GmModuleMLC.cc.old
    GmModuleMLC.cc
    GmVModule.cc
    GmTouchable.cc
    GmReadPhantomGeometry.cc
    GmUniformMagField.cc
    GmParallelWorldCreator.cc
    GmOpticalPropertiesMgr.cc
    plugin.cc
    GmParallelWorld.cc
    GmDetector.cc
    GmGeometryFromText.cc
    GmGeomUseMateDetectorBuilder.cc
    GmRegionCutsMgr.cc
    GmGeomTextDumperUA.cc
    GmRegionData.cc
    GmModuleJaws.cc
    GmGeometryMessenger.cc
    GmGeometryFactory.cc
    GmModuleJaws.cc.new
    GmUniformEMField.cc
    GmMaterialList.icc
    GmModuleMgr.cc
    GmModuleRangeModulator.cc
    GmGeomTextDetectorBuilder.cc
    GmUniformElectricField.cc
    GmGeomVerbosity.cc
    GmGeometryUseMaterials.cc
)
# - Add path to generated header
geant4_module_include_directories(GamosCore_GamosGeometry
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealBase/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealPlatform/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/PluginManager/include>
 PUBLIC  $<BUILD_INTERFACE:${ROOT_BASE_DIR}/include>)

geant4_module_link_libraries(GamosCore_GamosGeometry
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    ${SEAL_LIBRARIES} 
    GamosCore_GamosBase_Base
    GamosCore_GamosUtils
    GamosCore_GamosUserActionMgr
    GamosCore_GamosData_Distributions
)
