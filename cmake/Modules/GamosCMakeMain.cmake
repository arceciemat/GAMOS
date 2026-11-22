#.rst:
# GamosCMakeMain
# --------------
#
# Main configuration of the GAMOS build, included by the top CMakeLists.txt.
# It plays the role of G4CMakeMain.cmake in Geant4.
#
# Usage, as for Geant4:
#
#   mkdir build ; cd build
#   cmake -DCMAKE_INSTALL_PREFIX=<install dir> -DGeant4_DIR=<geant4>/lib/cmake/Geant4 <GAMOS source dir>
#   make -j<N>
#   make install
#   source <install dir>/bin/gamos.sh     (or gamos.csh)
#
# The build tree can also be used without installing it: source <build dir>/gamos.sh

include_guard(GLOBAL)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(CMakeDependentOption)

#-----------------------------------------------------------------------------
# Build type: Release by default, as in Geant4
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING
    "Build type: Release, Debug, RelWithDebInfo or MinSizeRel" FORCE)
endif()

#-----------------------------------------------------------------------------
# Options
# ROOT: OFF by default; ON with -DGAMOS_ROOT=ON or if the environment variable GAMOS_ROOT is set
# (as in the GNUmake build, config/confgamos.csh). With it, the macro GAMOS_ROOT is defined
if(DEFINED ENV{GAMOS_ROOT})
  set(_gamos_root_default ON)
else()
  set(_gamos_root_default OFF)
endif()
option(GAMOS_ROOT "Build GAMOS with ROOT (histograms in ROOT files); it defines GAMOS_ROOT" ${_gamos_root_default})
option(GAMOS_USE_DCMTK "Build the DICOM libraries and utilities (needs DCMTK)" ON)
option(GAMOS_BUILD_ANALYSIS "Build the analysis utilities (directory analysis)" ON)
option(GAMOS_BEAMZPOS "Define BEAMZPOS (radiotherapy beams along +Z)" OFF)
option(GAMOS_NO_VERBOSE "Define GAMOS_NO_VERBOSE (remove the verbosity code)" OFF)
mark_as_advanced(GAMOS_BEAMZPOS GAMOS_NO_VERBOSE)

#-----------------------------------------------------------------------------
# Windows: GAMOS libraries are DLLs that export all their symbols (GAMOS has no
# export macros), programs and DLLs are put together in bin/ so that Windows finds them
if(WIN32)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

#-----------------------------------------------------------------------------
# Geant4 (with all the user interfaces and visualization drivers it has)
find_package(Geant4 11.4 REQUIRED ui_all vis_all)
set(GAMOS_CXX_STANDARD ${Geant4_CXX_STANDARD})
if(NOT GAMOS_CXX_STANDARD)
  set(GAMOS_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_STANDARD ${GAMOS_CXX_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

# Directory of the Geant4 libraries: put in front of LD_LIBRARY_PATH by gamos.sh/gamos.csh and when the
# SEAL plugins are listed, because the Geant4 libraries use RUNPATH, that is searched after LD_LIBRARY_PATH
# (an LD_LIBRARY_PATH with another Geant4 version would mix the libraries of both versions)
set(GAMOS_GEANT4_LIBDIR "")
foreach(_cfg RELEASE RELWITHDEBINFO DEBUG MINSIZEREL NOCONFIG)
  if(TARGET Geant4::G4global AND NOT GAMOS_GEANT4_LIBDIR)
    get_target_property(_g4loc Geant4::G4global IMPORTED_LOCATION_${_cfg})
    if(_g4loc)
      get_filename_component(GAMOS_GEANT4_LIBDIR "${_g4loc}" DIRECTORY)
    endif()
  endif()
endforeach()

#-----------------------------------------------------------------------------
# ROOT (optional)
set(GAMOS_ROOT_LIBRARIES)
if(GAMOS_ROOT)
  find_package(ROOT REQUIRED)
  set(GAMOS_ROOT_LIBRARIES ${ROOT_LIBRARIES})
  include_directories(${ROOT_INCLUDE_DIRS})
endif()

#-----------------------------------------------------------------------------
# DCMTK (optional, for the DICOM libraries and utilities)
if(GAMOS_USE_DCMTK)
  find_package(DCMTK CONFIG)
  if(NOT DCMTK_FOUND)
    message(FATAL_ERROR
      "DCMTK not found: set DCMTK_DIR to the directory with DCMTKConfig.cmake "
      "(e.g. <dcmtk install>/lib/cmake/dcmtk), or use -DGAMOS_USE_DCMTK=OFF "
      "to build GAMOS without the DICOM libraries and utilities")
  endif()
  set(GAMOS_DCMTK_LIBRARIES ${DCMTK_LIBRARIES})
  # the DCMTK targets do not carry their include directories (DCMTK 3.6.4)
  include_directories(${DCMTK_INCLUDE_DIRS})
endif()

#-----------------------------------------------------------------------------
# Compile definitions and flags (as in the GNUmake build)
set(GAMOS_COMPILE_DEFINITIONS G4VIS_USE)
# directory of the installed Geant4 headers, where GmGenUtils::ReadUnits finds CLHEP/Units/SystemOfUnits.h
foreach(_inc ${Geant4_INCLUDE_DIRS})
  if(EXISTS "${_inc}/CLHEP/Units/SystemOfUnits.h")
    list(APPEND GAMOS_COMPILE_DEFINITIONS "GAMOS_G4_INCLUDE_DIR=\"${_inc}\"")
    break()
  endif()
endforeach()
if(GAMOS_ROOT)
  list(APPEND GAMOS_COMPILE_DEFINITIONS GAMOS_ROOT)
endif()
if(GAMOS_BEAMZPOS)
  list(APPEND GAMOS_COMPILE_DEFINITIONS BEAMZPOS)
endif()
if(GAMOS_NO_VERBOSE)
  list(APPEND GAMOS_COMPILE_DEFINITIONS GAMOS_NO_VERBOSE)
endif()

set(GAMOS_CXX_WARNING_FLAGS)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(GAMOS_CXX_WARNING_FLAGS -W -Wall -pedantic -Wno-non-virtual-dtor -Wno-long-long
    -Wwrite-strings -Wpointer-arith -Woverloaded-virtual -Wshadow)
elseif(MSVC)
  # as Geant4: the code uses WIN32 (defined by CMake for MSVC), M_PI, std::min/max and POSIX names
  list(APPEND GAMOS_COMPILE_DEFINITIONS WIN32 _USE_MATH_DEFINES NOMINMAX _CRT_SECURE_NO_WARNINGS _CRT_NONSTDC_NO_WARNINGS)
  set(GAMOS_CXX_WARNING_FLAGS /W1 /bigobj /wd4251 /wd4275)
endif()

#-----------------------------------------------------------------------------
# Output directories of the build tree (as Geant4: BuildProducts)
set(GAMOS_BUILD_LIBDIR "${PROJECT_BINARY_DIR}/BuildProducts/${CMAKE_INSTALL_LIBDIR}")
set(GAMOS_BUILD_BINDIR "${PROJECT_BINARY_DIR}/BuildProducts/${CMAKE_INSTALL_BINDIR}")
set(GAMOS_BUILD_MODULE_DIR "${PROJECT_BINARY_DIR}/BuildProducts/module")

# Install directories
set(GAMOS_INSTALL_MODULEDIR "module")
set(GAMOS_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/GAMOS")
foreach(_dir MODULEDIR)
  if(IS_ABSOLUTE "${GAMOS_INSTALL_${_dir}}")
    set(GAMOS_INSTALL_FULL_${_dir} "${GAMOS_INSTALL_${_dir}}")
  else()
    set(GAMOS_INSTALL_FULL_${_dir} "${CMAKE_INSTALL_PREFIX}/${GAMOS_INSTALL_${_dir}}")
  endif()
endforeach()

# RPATH: the programs find the GAMOS, Geant4, DCMTK and ROOT libraries
# without LD_LIBRARY_PATH, both in the build and in the install tree
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_FULL_LIBDIR}")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)
# RPATH instead of RUNPATH, so that it is also used for the libraries that the
# GAMOS libraries load (e.g. the DCMTK libraries load other DCMTK libraries),
# also when the libraries are loaded as SEAL plugins
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  add_link_options("LINKER:--disable-new-dtags")
endif()

#-----------------------------------------------------------------------------
include(GamosMacros)

add_subdirectory(source)
if(GAMOS_BUILD_ANALYSIS)
  add_subdirectory(analysis)
endif()

#-----------------------------------------------------------------------------
# Geant4 DLLs (Windows): bin directory of the Geant4 installation
get_filename_component(GAMOS_GEANT4_DLLDIR "${Geant4_DIR}/../../../bin" ABSOLUTE)
# List of the SEAL plugins (plugin.lis), read by the GAMOS version of Geant4:
# generated after all the libraries are built, in BuildProducts/data
set(GAMOS_BUILD_DATADIR "${PROJECT_BINARY_DIR}/BuildProducts/data")
get_property(_gamos_libs GLOBAL PROPERTY GAMOS_LIBRARIES)
set(_gamos_lib_targets)
foreach(_lib ${_gamos_libs})
  __gamos_target(${_lib} _t)
  list(APPEND _gamos_lib_targets ${_t})
endforeach()
if(WIN32)
  set(_gamos_dump_env "PATH=${GAMOS_BUILD_BINDIR};${GAMOS_GEANT4_DLLDIR};$ENV{PATH}")
else()
  set(_gamos_dump_env "LD_LIBRARY_PATH=${GAMOS_BUILD_LIBDIR}:${GAMOS_GEANT4_LIBDIR}:$ENV{LD_LIBRARY_PATH}")
endif()
add_custom_command(
  OUTPUT "${GAMOS_BUILD_DATADIR}/plugin.lis"
  COMMAND ${CMAKE_COMMAND} -E make_directory "${GAMOS_BUILD_DATADIR}"
  COMMAND ${CMAKE_COMMAND} -E env "${_gamos_dump_env}" ${CMAKE_COMMAND}
    "-DDUMP=$<TARGET_FILE:SealPluginDump-exe>"
    "-DMODULEDIR=${GAMOS_BUILD_MODULE_DIR}"
    "-DOUTPUT=${GAMOS_BUILD_DATADIR}/plugin.lis"
    -P "${PROJECT_SOURCE_DIR}/cmake/Modules/GamosPluginList.cmake"
  DEPENDS SealPluginDump-exe ${_gamos_lib_targets} "${PROJECT_SOURCE_DIR}/cmake/Modules/GamosPluginList.cmake"
  COMMENT "Writing the list of SEAL plugins: ${GAMOS_BUILD_DATADIR}/plugin.lis"
  VERBATIM)
add_custom_target(GamosPluginList ALL DEPENDS "${GAMOS_BUILD_DATADIR}/plugin.lis")

#-----------------------------------------------------------------------------
# Nothing is installed in <prefix>/share: the data files (data/), plugin.lis and the python
# utilities are used from the source tree (config/confgamos.csh)

#-----------------------------------------------------------------------------
# Links in the source tree, as the directories of the GNUmake build, so that GAMOS runs from the
# build tree after every "make" (no "make install") with only "source config/confgamos.csh":
#   lib/<G4SYSTEM>     -> <build>/BuildProducts/lib
#   bin/<G4SYSTEM>     -> <build>/BuildProducts/bin
#   module/<G4SYSTEM>  -> <build>/BuildProducts/module
#   data/plugin.lis    -> <build>/BuildProducts/data/plugin.lis
# An existing directory or file (e.g. from the GNUmake build) is renamed to <name>.gmake
if(DEFINED ENV{G4SYSTEM})
  set(_g4system "$ENV{G4SYSTEM}")
elseif(APPLE)
  set(_g4system "Darwin-clang")
else()
  set(_g4system "Linux-g++")
endif()
set(GAMOS_G4SYSTEM "${_g4system}" CACHE STRING "Name of the links lib/<G4SYSTEM>, bin/<G4SYSTEM>, module/<G4SYSTEM> in the source tree")
option(GAMOS_SOURCE_TREE_LINKS "Create the links lib/<G4SYSTEM>, bin/<G4SYSTEM>, module/<G4SYSTEM> and data/plugin.lis in the source tree" ON)

if(GAMOS_SOURCE_TREE_LINKS AND NOT WIN32)
  # created at every "make" (also if the links or the directories lib, bin, module were deleted)
  add_custom_target(GamosSourceTreeLinks ALL
    COMMAND ${CMAKE_COMMAND}
      "-DSOURCE_DIR=${PROJECT_SOURCE_DIR}"
      "-DLINKS=lib/${GAMOS_G4SYSTEM}=${GAMOS_BUILD_LIBDIR},bin/${GAMOS_G4SYSTEM}=${GAMOS_BUILD_BINDIR},module/${GAMOS_G4SYSTEM}=${GAMOS_BUILD_MODULE_DIR},data/plugin.lis=${GAMOS_BUILD_DATADIR}/plugin.lis"
      -P "${PROJECT_SOURCE_DIR}/cmake/Modules/GamosSourceTreeLinks.cmake"
    COMMENT "Links lib/${GAMOS_G4SYSTEM}, bin/${GAMOS_G4SYSTEM}, module/${GAMOS_G4SYSTEM}, data/plugin.lis -> ${PROJECT_BINARY_DIR}/BuildProducts"
    VERBATIM)
endif()

#-----------------------------------------------------------------------------
# Package configuration files, as Geant4Config.cmake: an application uses GAMOS with
#   find_package(GAMOS REQUIRED)
#   target_link_libraries(myapp ${GAMOS_LIBRARIES})
get_property(GAMOS_LIBRARIES GLOBAL PROPERTY GAMOS_LIBRARIES)
install(EXPORT GAMOSLibraryDepends
  NAMESPACE GAMOS::
  DESTINATION ${GAMOS_INSTALL_CMAKEDIR}
  COMPONENT Development)
configure_package_config_file(cmake/Templates/GAMOSConfig.cmake.in
  "${PROJECT_BINARY_DIR}/InstallTreeFiles/GAMOSConfig.cmake"
  INSTALL_DESTINATION ${GAMOS_INSTALL_CMAKEDIR}
  PATH_VARS CMAKE_INSTALL_INCLUDEDIR GAMOS_INSTALL_MODULEDIR)
write_basic_package_version_file(
  "${PROJECT_BINARY_DIR}/InstallTreeFiles/GAMOSConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMinorVersion)
install(FILES
  "${PROJECT_BINARY_DIR}/InstallTreeFiles/GAMOSConfig.cmake"
  "${PROJECT_BINARY_DIR}/InstallTreeFiles/GAMOSConfigVersion.cmake"
  DESTINATION ${GAMOS_INSTALL_CMAKEDIR}
  COMPONENT Development)

#-----------------------------------------------------------------------------
# Uninstall target, as in Geant4
if(NOT TARGET uninstall)
  configure_file(cmake/Templates/cmake_uninstall.cmake.in
    "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake" @ONLY)
  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake")
endif()

#-----------------------------------------------------------------------------
# Summary
get_property(_gamos_exes GLOBAL PROPERTY GAMOS_EXECUTABLES)
list(LENGTH GAMOS_LIBRARIES _nlibs)
list(LENGTH _gamos_exes _nexes)
message(STATUS "")
message(STATUS "GAMOS ${PROJECT_VERSION} configured:")
message(STATUS "  Geant4            : ${Geant4_VERSION} (${Geant4_DIR})")
message(STATUS "  Geant4 multithreaded : ${Geant4_multithreaded_FOUND}")
message(STATUS "  Build type        : ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ standard      : ${GAMOS_CXX_STANDARD}")
message(STATUS "  ROOT              : ${GAMOS_ROOT}")
message(STATUS "  DCMTK (DICOM)     : ${GAMOS_USE_DCMTK}")
message(STATUS "  Analysis programs : ${GAMOS_BUILD_ANALYSIS}")
message(STATUS "  Libraries         : ${_nlibs}")
message(STATUS "  Programs          : ${_nexes}")
message(STATUS "  Install prefix    : ${CMAKE_INSTALL_PREFIX}")
message(STATUS "")
