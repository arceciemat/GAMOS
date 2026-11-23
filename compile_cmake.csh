source $HOME/gamos/GAMOS.7.0/config/confgamos.csh
# Compile GAMOS with cmake:  source compile_cmake.csh [NCPUS]
if ( "$1" != "" ) then
  set NCPUS = $1
else
  set NCPUS = `nproc`
endif

set CMAKE_OPTS = "-DCMAKE_INSTALL_PREFIX=$GAMOSINSTALL -DGeant4_DIR=$G4INSTALL/lib/cmake/Geant4"
if ( -d "$DCMTK_BASE_DIR/lib/cmake/dcmtk" ) then
  set CMAKE_OPTS = "$CMAKE_OPTS -DDCMTK_DIR=$DCMTK_BASE_DIR/lib/cmake/dcmtk"
else
  set CMAKE_OPTS = "$CMAKE_OPTS -DGAMOS_USE_DCMTK=OFF"
endif
if ( $?GAMOS_ROOT ) then
  set CMAKE_OPTS = "$CMAKE_OPTS -DGAMOS_ROOT=ON -DROOT_DIR=$ROOT_BASE_DIR/cmake"
endif

mkdir -p $GAMOSINSTALL/build
cd $GAMOSINSTALL/build
cmake $CMAKE_OPTS $GAMOSINSTALL && make -j$NCPUS
cd $GAMOSINSTALL
