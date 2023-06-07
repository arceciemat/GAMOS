#!/bin/bash
export G4SYSTEM=Linux-g++

#--- GAMOS variables
export GAMOSDIR=/home/arce/gamos
export GAMOS_VER=GAMOS.6.3
export GAMOSINSTALL=$GAMOSDIR/$GAMOS_VER
export GAMOS_BASE=$GAMOSINSTALL
export GAMOSEXTERNAL=$GAMOSDIR/external
export GAMOS_SEARCH_PATH=.:$GAMOSINSTALL/data

export PATH="${GAMOSINSTALL}/bin/${G4SYSTEM}:${PATH}"

#--- SEAL variables
export SEAL_BASE_DIR=${GAMOSINSTALL}
export SEAL_PLUGIN_DIR=${GAMOSINSTALL}/module/$G4SYSTEM
export SEAL_KEEP_MODULES=true
export SEAL_PLUGINS=${GAMOSINSTALL}/module/${G4SYSTEM}:${SEAL_BASE_DIR}/${G4SYSTEM}/lib/modules

#--- GEANT4 variables
export G4INSTALL=${GAMOSEXTERNAL}/geant4/geant4.11.00.p03.gamos
export G4TMP=$GAMOSINSTALL/tmp
export G4BIN=$GAMOSINSTALL/bin
export G4NEUTRONHPDATA=$G4INSTALL/../data/G4NDL4.6
export G4LEDATA=$G4INSTALL/../data/G4EMLOW8.0
export G4LEVELGAMMADATA=$G4INSTALL/../data/PhotonEvaporation5.7
export G4RADIOACTIVEDATA=$G4INSTALL/../data/RadioactiveDecay5.6
export G4SAIDXSDATA=$G4INSTALL/../data/G4SAIDDATA2.0
export G4PARTICLEXSDATA=$G4INSTALL/../data/G4PARTICLEXS3.1.1
export G4ABLADATA=$G4INSTALL/../data/G4ABLA3.1
export G4INCLDATA=$G4INSTALL/../data/G4INCL1.0
export G4PIIDATA=$G4INSTALL/../data/G4PII1.3
export G4ENSDFSTATEDATA=$G4INSTALL/../data/G4ENSDFSTATE2.3
export G4REALSURFACEDATA=$G4INSTALL/../data/RealSurface2.2
export G4PROTONHPDATA=$G4INSTALL/../data/G4TENDL1.4/Proton
export G4DEUTERONHPDATA=$G4INSTALL/../data/G4TENDL1.4/Deuteron
export G4TRITONHPDATA=$G4INSTALL/../data/G4TENDL1.4/Triton
export G4HE3HPDATA=$G4INSTALL/../data/G4TENDL1.4/He3
export G4ALPHAHPDATA=$G4INSTALL/../data/G4TENDL1.4/Alpha
export G4PARTICLEHP_DO_NOT_ADJUST_FINAL_STATE=1
export G4NEUTRONHP_DO_NOT_ADJUST_FINAL_STATE=1
export G4NEUTRONHP_SKIP_MISSING_ISOTOPES=1

export G4LIB_BUILD_SHARED=1
 
#--- ROOT variables
export ROOT_BASE_DIR=${GAMOSEXTERNAL}/root/6.26.04/root
export PATH="${ROOT_BASE_DIR}/bin:${PATH}"
export ROOTSYS=${ROOT_BASE_DIR}

#--- FFTW variables
export FFTW_BASE_DIR=${GAMOSEXTERNAL}/fftw/fftw-3.2.2
# by default do not use it
export GAMOS_NO_FFTW=1

#---- DCMTK variables
export DCMTK_BASE_DIR=${GAMOSEXTERNAL}/dcmtk/dcmtk-3.6.4
export DCMDICTPATH=${GAMOSEXTERNAL}/dcmtk/dcmtk-3.6.4/dcmdata/data/dicom.dic
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${DCMTK_BASE_DIR}/lib

#---- OPENJPEG variables
export OPENJPEG_BASE_DIR=${GAMOSEXTERNAL}/openjpeg/openjpeg-2.1.1

#export G4DEBUG=1
#export G4NEUTRONHP_NEGLECT_DOPPLER=1

source ${GAMOSINSTALL}/config/gamosvis.sh

export AllowForHeavyElements=1


#export NeutronHPNames=1

if [ "$G4SYSTEM" == "Linux-g++"  ] ;  then
#--- Initialize LD_LIBRARY_PATH
  if [ "$LD_LIBRARY_PATH" == "" ] ; then
    export LD_LIBRARY_PATH=""
  fi
  export LD_LIBRARY_PATH="${GAMOSINSTALL}/lib/${G4SYSTEM}:${LD_LIBRARY_PATH}"

  export LD_LIBRARY_PATH="${G4INSTALL}/lib/${G4SYSTEM}:${LD_LIBRARY_PATH}"
  export LD_LIBRARY_PATH="${ROOT_BASE_DIR}/lib:${LD_LIBRARY_PATH}"
  export LD_LIBRARY_PATH="/usr/lib:/usr/local/lib:/usr/local/lib/X11:/usr/explorer/lib:/usr/lib/Motif1.2:/usr/X11R6/include/X11:${LD_LIBRARY_PATH}"
  export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${DCMTK_BASE_DIR}/lib"
  export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${OPENJPEG_BASE_DIR}/lib"
elif [ "$G4SYSTEM" == "Darwin-clang"  ] ;  then
#--- Initialize DYLD_LIBRARY_PATH
  export DYLD_LIBRARY_PATH="${GAMOSINSTALL}/lib/${G4SYSTEM}"

  export DYLD_LIBRARY_PATH="${G4INSTALL}/lib/${G4SYSTEM}:${DYLD_LIBRARY_PATH}"
  export DYLD_LIBRARY_PATH="${ROOT_BASE_DIR}/lib:${DYLD_LIBRARY_PATH}"
  export DYLD_LIBRARY_PATH="/usr/lib:/usr/local/lib/X11:/usr/explorer/lib:/usr/lib/Motif1.2:/usr/X11R6/include/X11:${DYLD_LIBRARY_PATH}"
  export DYLD_LIBRARY_PATH="${DYLD_LIBRARY_PATH}:${DCMTK_BASE_DIR}/lib"
  export DYLD_LIBRARY_PATH="${DYLD_LIBRARY_PATH}:${OPENJPEG_BASE_DIR}/lib"
fi

#export BEAMZPOS=1
export G4VRML_PV_PICKABLE=1

export G4USE_STD17=1

export PYTHONPATH="${GAMOSINSTALL}/analysis/pythonUtilities"
