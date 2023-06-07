# - GamosCore_GamosReadDICOM module build definition

include(UseGamosAtGeant4)
include(UseROOT)

geant4_add_module(GamosCore_GamosReadDICOM
  PUBLIC_HEADERS
    GmDumpMateG4dcmUA.hh
    DicomNestedPhantomParameterisation.hh
    GmInterfileHeader.hh
    GmCrossPhantomGeometry.hh
    GmSimplePhantomGeometry.hh
    GmSqdose.hh
    GmReadNMDicom.hh
    GmGenerDistPositionInMaterialPhantomVoxels.hh
    GmReadPhantomPartialG4Geometry.hh
    Gm3ddoseHeader.hh
    GmClassifierByPhantomStructure.hh
    GmReadPETDicom.hh
    GmReadPhantomEGSwithStGeometry.hh
    GmReadPhantomG4withStGeometry.hh
    GmGenerDistPositionPETImage.hh
    GmReadPhantomStMgr.hh
    GmPhantomDoseUA.hh
    GmGenerDistPositionInStPhantomVoxels.hh
    GmBuildPhantomStructuresUA.hh
    GmRegularParamUtils.hh
    GmReadPhantomG4Geometry.hh
    GmClassifierCrossPhantom.hh
    GmGenerDistPositionInPhantomVoxels.hh
    GmReadDICOMVerbosity.hh
    GmReadPhantomEGSGeometry.hh
    GmGenerDistPositionNMImage.hh
    GmInterfile.hh
    GmReadPhantomPSMgr.hh
    GmTouchablePhantomStructure.hh
    GmReadPhantomG4BinGeometry.hh
    GmSqdoseHeader.hh
    GmPhantomDensityHistosUA.hh
    Gm3ddose.hh
    GmPhantomStructures.hh
  SOURCES
    GmBuildPhantomStructuresUA.cc
    GmInterfile.cc
    GmSqdoseHeader.cc.old
    GmClassifierCrossPhantom.cc
    GmSqdoseHeader.cc.new
    GmPhantomDensityHistosUA.cc
    GmSimplePhantomGeometry.cc
    GmReadPETDicom.cc
    GmReadPhantomPSMgr.cc
    GmReadDICOMVerbosity.cc
    GmSqdose.cc.old
    GmReadNMDicom.cc
    GmGenerDistPositionInMaterialPhantomVoxels.cc
    GmReadPhantomEGSwithStGeometry.cc
    GmGenerDistPositionInStPhantomVoxels.cc
    GmDumpMateG4dcmUA.cc
    GmGenerDistPositionNMImage.cc
    GmReadPhantomStMgr.cc
    plugin.cc
    GmReadPhantomEGSGeometry.cc
    GmTouchablePhantomStructure.cc
    GmGenerDistPositionInPhantomVoxels.cc
    GmPhantomDoseUA.cc
    GmCrossPhantomGeometry.cc
    Gm3ddose.cc
    GmReadPhantomG4BinGeometry.cc
    GmRegularParamUtils.cc
    GmSqdoseHeader.cc
    GmReadPhantomG4Geometry.cc
    GmClassifierByPhantomStructure.cc
    GmReadPhantomG4withStGeometry.cc
    GmInterfileHeader.cc
    Gm3ddoseHeader.cc
    GmReadPhantomPartialG4Geometry.cc
    DicomNestedPhantomParameterisation.cc
    GmSqdose.cc.new
    GmSqdose.cc
    GmGenerDistPositionPETImage.cc
)
# - Add path to generated header
geant4_module_include_directories(GamosCore_GamosReadDICOM
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealBase/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealPlatform/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/PluginManager/include>
 PUBLIC  $<BUILD_INTERFACE:${ROOT_BASE_DIR}/include>)

geant4_module_link_libraries(GamosCore_GamosReadDICOM
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    ${SEAL_LIBRARIES} 
    GamosCore_GamosBase_Base
    GamosCore_GamosUserActionMgr
    GamosCore_GamosGeometry
    GamosCore_GamosUtils
    GamosCore_GamosBase_Filters
    GamosCore_GamosGenerator
)
