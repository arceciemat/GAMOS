# - NuclMed_Base module build definition

include(UseGamosAtGeant4)
include(UseROOT)

geant4_add_module(NuclMed_Base
  PUBLIC_HEADERS
    DetCClassifNRecHit.hh
    DetCClassifZPosMin.hh
    DetCClassifYZPosMin.hh
    DetCountScatteringUA.hh
    DetCAlgoZPos.hh
    DetComptClassificationVariableFactory.hh
    DetV1stHitAlgorithm.hh
    Det1stHitByComptonCone.hh
    DetVComptAlgorithmVariable.hh
    DetCClassifXYZPosMin.hh
    DetCAlgoXZPos.hh
    DetCAlgoYZPos.hh
    DetCClassifYPosMin.hh
    Det1stHitByEnergy.hh
    DetCClassifXPosMin.hh
    DetHistosGammaDist.hh
    DetVComptClassificationVariable.hh
    DetComptAlgorithmVariableFactory.hh
    DetComptonStudyHistosUA.hh
    Det1stHitByXYPos.hh
    DetCutsStudyFilter.hh
    DetRecHitCone.hh
    DetCClassifXYPosMin.hh
    DetCAlgoXPos.hh
    Det1stHitByXPos.hh
    DetCClassifXZPosMin.hh
    DetCClassifEnergyMin.hh
    DetCClassifEnergyAverage.hh
    DetRecHitSet.hh
    DetCAlgoYPos.hh
    Det1stHitAlgorithmFactory.hh
    Det1stHitByDistanceToOther.hh
    DetVerbosity.hh
    DetCClassifEnergyMax.hh
    DetCAlgoXYZPos.hh
    Det1stHitByXYZPos.hh
    DetCAlgoEnergy.hh
    DetCClassifALL.hh
    DetCAlgoXYPos.hh
  SOURCES
    DetCClassifNRecHit.cc
    DetCClassifEnergyAverage.cc
    DetCAlgoXYPos.cc
    Det1stHitByXPos.cc
    DetCClassifALL.cc
    DetCountScatteringUA.cc
    DetVComptAlgorithmVariable.cc
    DetHistosGammaDist.cc
    DetCAlgoXPos.cc
    DetComptonStudyHistosUA.cc
    DetCClassifEnergyMin.cc
    Det1stHitByXYPos.cc
    Det1stHitByDistanceToOther.cc
    DetCClassifEnergyMax.cc
    DetCClassifYPosMin.cc
    plugin.cc
    DetRecHitCone.cc
    DetCutsStudyFilter.cc
    DetCClassifXPosMin.cc
    Det1stHitByXYZPos.cc
    Det1stHitAlgorithmFactory.cc
    Det1stHitByEnergy.cc
    DetCAlgoYZPos.cc
    DetCClassifYZPosMin.cc
    DetVerbosity.cc
    DetCAlgoXZPos.cc
    DetCAlgoXYZPos.cc
    DetCClassifXYPosMin.cc
    DetComptClassificationVariableFactory.cc
    DetComptAlgorithmVariableFactory.cc
    DetCClassifXZPosMin.cc
    DetVComptClassificationVariable.cc
    DetCClassifXYZPosMin.cc
    DetCClassifZPosMin.cc
    Det1stHitByComptonCone.cc
    DetCAlgoZPos.cc
    DetCAlgoYPos.cc
    DetCAlgoEnergy.cc
    DetV1stHitAlgorithm.cc
    DetRecHitSet.cc
)
# - Add path to generated header
geant4_module_include_directories(NuclMed_Base
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealBase/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealPlatform/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/PluginManager/include>
 PUBLIC  $<BUILD_INTERFACE:${ROOT_BASE_DIR}/include>)

geant4_module_link_libraries(NuclMed_Base
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    ${SEAL_LIBRARIES} 
)
