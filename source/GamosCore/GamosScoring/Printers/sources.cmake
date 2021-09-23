#------------------------------------------------------------------------------
# Module : Printers
# Package: GamosCore_GamosScoring_Printers
#
#------------------------------------------------------------------------------
#
set(CMAKE_VERBOSE_MAKEFILE ON)
include_directories(${CMAKE_SOURCE_DIR}/source)
include_directories(${CMAKE_SOURCE_DIR}/include)
#
# Define the GEANT4 Module.
include(UseGamosAtGeant4)
#
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosScoring_Printers
	 HEADERS
		GmPSPrinterCoutContIndex.hh
		GmPSPrinterSqdose.hh
		GmPSPrinterBinFile.hh
		GmPSPrinterCout.hh
		GmPSPrinter3ddoseSplitZ.hh
		GmPSPrinterHistos.hh
		GmPSPrinterCSVFile.hh
		GmPSPrinter3ddose.hh
		GmPSPrinterTextFile.hh
		GmPSPrinterXYZ.hh
	 SOURCES
		plugin.cc
		GmPSPrinterCout.cc
		GmPSPrinter3ddoseSplitZ.cc
		GmPSPrinterTextFile.cc
		GmPSPrinter3ddose.cc
		GmPSPrinterSqdose.cc
		GmPSPrinterXYZ.cc
		GmPSPrinterBinFile.cc
		GmPSPrinterCSVFile.cc
		GmPSPrinterHistos.cc
		GmPSPrinterCoutContIndex.cc
	 
	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
		${Geant4_libs} 
		${ROOT_LIBRARIES} 
	GamosCore_GamosUtils
	GamosCore_GamosBase_Base
	GamosCore_GamosBase_Classifiers
	GamosCore_GamosGeometry
	GamosCore_GamosReadDICOM
	GamosCore_GamosScoring_Management
		${SEAL_LIBRARIES} 

	LINK_LIBRARIES
)
