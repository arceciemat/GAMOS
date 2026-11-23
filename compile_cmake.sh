source $HOME/gamos/GAMOS.7.0/config/confgamos.sh
# Compile GAMOS with cmake:  source compile_cmake.sh [NCPUS]   (or: bash compile_cmake.sh [NCPUS])
NCPUS=${1:-$(nproc)}

CMAKE_OPTS="-DCMAKE_INSTALL_PREFIX=$GAMOSINSTALL -DGeant4_DIR=$G4INSTALL/lib/cmake/Geant4"
if [ -d "$DCMTK_BASE_DIR/lib/cmake/dcmtk" ] ; then
  CMAKE_OPTS="$CMAKE_OPTS -DDCMTK_DIR=$DCMTK_BASE_DIR/lib/cmake/dcmtk"
else
  CMAKE_OPTS="$CMAKE_OPTS -DGAMOS_USE_DCMTK=OFF"
fi
if [ -n "$GAMOS_ROOT" ] ; then
  CMAKE_OPTS="$CMAKE_OPTS -DGAMOS_ROOT=ON -DROOT_DIR=$ROOT_BASE_DIR/cmake"
fi

mkdir -p $GAMOSINSTALL/build
cd $GAMOSINSTALL/build
cmake $CMAKE_OPTS $GAMOSINSTALL && make -j$NCPUS
cd $GAMOSINSTALL
