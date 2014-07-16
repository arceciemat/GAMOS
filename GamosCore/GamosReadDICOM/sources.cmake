#------------------------------------------------------------------------------
# 
# Module : GamosCore
# Package: GamosReadDICOM
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
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosReadDICOM

   HEADERS
       Gm3ddose.hh
       Gm3ddoseHeader.hh
       GmGenerDistPositionInPhantomVoxels.hh
       GmReadPhantomG4withPSGeometry.hh
       GmSqdoseHeader.hh
       GmPhantomStructures.hh
       GmSimplePhantomGeometry.hh
       GmReadPhantomG4BinGeometry.hh
       GmPhantomDoseUA.hh
       GmReadPhantomG4Geometry.hh
       GmDICOMIntersectVolume.hh
       GmReadPhantomSVMgr.hh
       GmReadPhantomGeometry.hh
       GmSqdose.hh
       GmRegularParamUtils.hh
       GmBuildPhantomStructuresUA.hh
       GmReadDICOMVerbosity.hh
       GmReadPhantomPartialG4Geometry.hh
       GmTouchablePhantomStructure.hh
       GmReadPhantomEGSGeometry.hh
       GmReadPhantomEGSwithPSGeometry.hh
       GmPhantomDensityHistosUA.hh

   SOURCES
       GmReadPhantomGeometry.cc
       plugin.cc
       GmSimplePhantomGeometry.cc
       GmReadPhantomG4Geometry.cc
       GmReadPhantomG4withPSGeometry.cc
       GmPhantomDensityHistosUA.cc
       GmReadPhantomEGSwithPSGeometry.cc
       Gm3ddose.cc
       GmReadPhantomEGSGeometry.cc
       GmBuildPhantomStructuresUA.cc
       GmGenerDistPositionInPhantomVoxels.cc
       GmReadPhantomPartialG4Geometry.cc
       Gm3ddoseHeader.cc
       GmReadDICOMVerbosity.cc
       GmTouchablePhantomStructure.cc
       GmRegularParamUtils.cc
       GmSqdose.cc
       GmReadPhantomSVMgr.cc
       GmPhantomDoseUA.cc
       GmSqdoseHeader.cc
       GmDICOMIntersectVolume.cc
       GmReadPhantomG4BinGeometry.cc
 
   GRANULAR_DEPENDENCIES

   GLOBAL_DEPENDENCIES
    ${Geant4_LIBRARIES}
    ${Root_LIBRARIES}
  GamosCore_GamosBase_Filters
  GamosCore_GamosGenerator
  GamosCore_GamosUtils
  GamosCore_GamosUserActionMgr
  GamosCore_GamosBase_Base
  GamosCore_GamosGeometry

   LINK_LIBRARIES
)

# List any source specific properties here
