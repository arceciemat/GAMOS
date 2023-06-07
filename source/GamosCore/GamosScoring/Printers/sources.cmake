# - GamosCore_GamosScoring_Printers module build definition

include(UseGamosAtGeant4)
include(UseROOT)

geant4_add_module(GamosCore_GamosScoring_Printers
  PUBLIC_HEADERS
    GmPSPrinterInterfile.hh
    GmPSPrinterXYZ.hh
    GmPSPrinterHistos.hh
    GmPSPrinterTextFile.hh
    GmPSPrinterSqdose.hh
    GmPSPrinter3ddose.hh
    GmPSPrinterCout.hh
    GmPSPrinterBinFile.hh
    GmPSPrinter3ddoseSplitZ.hh
    GmPSPrinterCSVFile.hh
    GmPSPrinterCoutContIndex.hh
  SOURCES
    GmPSPrinterHistos.cc
    GmPSPrinterCout.cc
    GmPSPrinterSqdose.cc-
    GmPSPrinterInterfile.cc
    plugin.cc
    GmPSPrinterBinFile.cc
    GmPSPrinterCSVFile.cc
    GmPSPrinter3ddoseSplitZ.cc
    GmPSPrinterCoutContIndex.cc
    GmPSPrinterSqdose.cc
    GmPSPrinter3ddose.cc
    GmPSPrinterXYZ.cc
    GmPSPrinterTextFile.cc
)
# - Add path to generated header
geant4_module_include_directories(GamosCore_GamosScoring_Printers
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealBase/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealPlatform/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/PluginManager/include>
 PUBLIC  $<BUILD_INTERFACE:${ROOT_BASE_DIR}/include>)

geant4_module_link_libraries(GamosCore_GamosScoring_Printers
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    ${SEAL_LIBRARIES} 
    GamosCore_GamosUtils
    GamosCore_GamosBase_Base
    GamosCore_GamosBase_Classifiers
    GamosCore_GamosGeometry
    GamosCore_GamosReadDICOM
    GamosCore_GamosScoring_Management
)
