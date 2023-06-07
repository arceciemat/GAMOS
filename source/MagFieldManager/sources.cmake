# - MagFieldManager module build definition

include(UseGamosAtGeant4)
include(UseROOT)

geant4_add_module(MagFieldManager
  PUBLIC_HEADERS
    IdealToroidWithCircularCoil.hh
    CoilType2.hh
    JCurrentCalculator.hh
    ParametrizedFieldCreator.hh
    LinearSegment.hh
    CurvedSegmentCreator.hh
    MagFieldModManMessenger.hh
    MagneticFieldModelManager.hh
    ConfinedFieldCreator.hh
    CoilType3Creator.hh
    LinearSegmentCreator.hh
    IdealToroidWithCircularCoilCreator.hh
    MultiplyField.hh
    ReplicateFieldCreator.hh
    CircularSolenoidCreator.hh
    MagneticDipole.hh
    RacetrackCoil.hh
    RacetrackCoilCreator.hh
    IdealToroidWithRectangularCoil.hh
    CurvedSegment.hh
    UniformFieldCreator.hh
    CircularCoilCreator.hh
    BiotSavartIntegrator.hh
    special_functions.hh
    ReplicateField.hh
    CircularCoil.hh
    InterpolatedField.hh
    CompoundFieldCreator.hh
    TransManMessenger.hh
    MagneticEquationOfMotion.hh
    MultiplyFieldCreator.hh
    ConfinedField.hh
    CoilType3.hh
    CoilType2Creator.hh
    CircularSolenoid.hh
    MagneticDipoleCreator.hh
    IdealToroidWithRectangularCoilCreator.hh
    TransformationManager.hh
    InterpolatedFieldCreator.hh
    CompoundField.hh
  SOURCES
    IdealToroidWithCircularCoilCreator.cc
    ReplicateFieldCreator.cc
    CircularSolenoidCreator.cc
    ParametrizedFieldCreator.cc
    CircularCoilCreator.cc
    InterpolatedFieldCreator.cc
    CompoundFieldCreator.cc
    CompoundField.cc
    MagneticDipole.cc
    JCurrentCalculator.cc
    CoilType2Creator.cc
    RacetrackCoil.cc
    CoilType2.cc
    special_functions.cc
    LinearSegment.cc
    CoilType3.cc
    TransManMessenger.cc
    InterpolatedField.cc
    ConfinedFieldCreator.cc
    TransformationManager.cc
    CurvedSegment.cc
    MagneticDipoleCreator.cc
    ConfinedField.cc
    plugin.cc
    CoilType3Creator.cc
    MagFieldModManMessenger.cc
    CurvedSegmentCreator.cc
    LinearSegmentCreator.cc
    IdealToroidWithCircularCoil.cc
    MultiplyFieldCreator.cc
    MultiplyField.cc
    UniformFieldCreator.cc
    ReplicateField.cc
    CircularSolenoid.cc
    CircularCoil.cc
    BiotSavartIntegrator.cc
    IdealToroidWithRectangularCoil.cc
    MagneticFieldModelManager.cc
    RacetrackCoilCreator.cc
    IdealToroidWithRectangularCoilCreator.cc
    MagneticEquationOfMotion.cc
)
# - Add path to generated header
geant4_module_include_directories(MagFieldManager
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealBase/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealPlatform/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/PluginManager/include>
 PUBLIC  $<BUILD_INTERFACE:${ROOT_BASE_DIR}/include>)

geant4_module_link_libraries(MagFieldManager
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    ${SEAL_LIBRARIES} 
)
