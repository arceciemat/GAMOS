#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(gamos)

#----------------------------------------------------------------------------
# Find Gamos package, activating all available UI and Vis drivers by default
#-
find_package(Geant4 REQUIRED)

#----------------------------------------------------------------------------
# Setup Gamos include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)
# for GAMOS includes
#include_directories(${Geant4_INCLUDE_DIR}/../source/)
include_directories(c:/gamos/GAMOS.5.0.0/source)


#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Gamos, Geant4 and ROOT libraries
#
include(GamosSetup)

add_executable(gamos gamos.cc ${sources} ${headers})
target_link_libraries(gamos ${Gamos_LIBRARIES} ${Geant4_LIBRARIES} ${Root_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#

#----------------------------------------------------------------------------
# For internal Geant4 use - but has no effect if you build this
# example standalone
#
#t add_custom_target(B1 DEPENDS exampleB1)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS gamos DESTINATION bin)


