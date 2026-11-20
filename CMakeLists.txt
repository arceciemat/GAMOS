#------------------------------------------------------------------------------
# Top level CMakeLists.txt for building GAMOS, as Geant4:
#
#   mkdir build ; cd build
#   cmake -DCMAKE_INSTALL_PREFIX=<install dir> \
#         -DGeant4_DIR=<geant4 install>/lib/cmake/Geant4 \
#         [-DDCMTK_DIR=<dcmtk install>/lib/cmake/dcmtk] \
#         [-DGAMOS_USE_ROOT=ON -DROOT_DIR=<root>/cmake] \
#         <GAMOS source dir>
#   make -j<N>
#   source <GAMOS source dir>/config/confgamos.csh   (or confgamos.sh)
#
# The build creates the links lib/<G4SYSTEM>, bin/<G4SYSTEM>, module/<G4SYSTEM> and
# data/plugin.lis in the source tree, pointing to <build>/BuildProducts, so "make install" is not needed
#
# Options: see cmake/Modules/GamosCMakeMain.cmake
#------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.16...3.31)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "GAMOS cannot be built in its source directory: "
    "create a build directory and run cmake from it")
endif()

project(GAMOS VERSION 7.0.1 LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/Modules")
include(GamosCMakeMain)
