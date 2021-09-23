#------------------------------------------------------------------------------
# 
# Module : SEAL_Foundation_PluginDumper
# Package: SEAL_Foundation
#
#------------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

set(CMAKE_VERBOSE_MAKEFILE ON)
#include(Geant4MacroLibraryTargets)
#GEANT4_GLOBAL_LIBRARY_TARGET(COMPONENTS sources.cmake)
project(gamos)

find_package(Gamos REQUIRED)
#find_package(Geant4 REQUIRED)
#find_package(ROOT REQUIRED)

#include(${CMAKE_BINARY_DIR}/UseGamos.cmake)
#include(/home/arce/gamos/GAMOS.6.0.0.cmake/cmake/Templates/UseGamos.cmake)
include_directories(${CMAKE_SOURCE_DIR}/../../../source)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(SealPluginDump SealPluginDump.cc ${sources} ${headers})
target_link_libraries(SealPluginDump ${Gamos_LIBRARIES} ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

#tadd_custom_target(gamos DEPEND gamos)

install(TARGETS SealPluginDump DESTINATION bin)

