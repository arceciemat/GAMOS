# - gamos module build definition

gamos_add_module(gamos
  SOURCES
    dummy.cc
)

# Program gamos
# As in the GNUmake build (config/gamos_libraries.gmk), it is linked with all the
# GamosCore libraries and MagFieldManager, that are loaded when it starts; the
# other libraries are loaded as SEAL plugins when they are needed
gamos_add_executable(gamos
  SOURCES
    gamos.cc
  LINK_LIBRARIES
    gamos
    MagFieldManager
    GamosCore_GamosRunManager
    GamosCore_GamosUtilsUA
    GamosCore_GamosScoring_PointDetector
    GamosCore_GamosScoring_Printers
    GamosCore_GamosScoring_Scorers
    GamosCore_GamosPhysics_Cuts
    GamosCore_GamosPhysics_VarianceReduction
    GamosCore_GamosPhysics_Biasing
    GamosCore_GamosPhysics_PhysicsList
    GamosCore_GamosPhysics_OtherPhysicsLists
    GamosCore_GamosMovement
    GamosCore_GamosData_Users
    GamosCore_GamosData_Data
    GamosCore_GamosSD
    GamosCore_GamosGenerator
    GamosCore_GamosScoring_Management
    GamosCore_GamosReadDICOM
    GamosCore_GamosBase_Filters
    GamosCore_GamosBase_Classifiers
    GamosCore_GamosGeometry
    GamosCore_GamosData_Distributions
    GamosCore_GamosData_Management
    GamosCore_GamosAnalysis
    GamosCore_GamosUserActionMgr
    GamosCore_GamosBase_Base
    GamosCore_GamosUtils
    lcg_PluginManager
    lcg_SealBase
  NO_AS_NEEDED
)
