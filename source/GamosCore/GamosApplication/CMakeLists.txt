#------------------------------------------------------------------------------
# 
# Module : GamosCore_GamosApplication
# Package: GamosCore
#
#------------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

add_definitions(-DGmApplication_bin_ALLOC_EXPORT)

set(CMAKE_VERBOSE_MAKEFILE ON)
#include(Geant4MacroLibraryTargets)
#GEANT4_GLOBAL_LIBRARY_TARGET(COMPONENTS sources.cmake)
project(gamos)

#option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
#if(WITH_GEANT4_UIVIS)
#  find_package(Geant4 REQUIRED ui_all vis_all)
#else()
#  find_package(Geant4 REQUIRED)
#endif()
#find_package(Gamos REQUIRED)
#find_package(ROOT REQUIRED)

#include(${Geant4_USE_FILE})

include(${CMAKE_SOURCE_DIR}/cmake/Modules/UseGamosAtGeant4.cmake) 
#include(${Geant4_DIR}/UseGamosAtGeant4_Bin.cmake)  ##!! HAS TO BE COPI#ED BY HAND

include_directories(${CMAKE_SOURCE_DIR}/../../)
include_directories(${CMAKE_SOURCE_DIR}/../../..) # for boost
#include_directories(../..);#source/GAMOS610)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#set(G4LIB ${Geant4_DIR}/../../BuildProducts/Debug/lib)

add_executable(gamos gamos.cc ${sources} ${headers})
target_link_libraries(gamos 
${Geant4_libs} 
${Gamos_LIBRARIES}
#${ROOT_LIBRARIES} 
${dlfcn_LIBRARIES}
${pcre_LIBRARIES}
)

install(TARGETS gamos DESTINATION bin)
