setenv G4SYSTEM Linux-g++

#--- GAMOS variables
setenv GAMOSDIR /home/arce/gamos
setenv GAMOS_VER GAMOS.6.3
setenv GAMOSINSTALL $GAMOSDIR/$GAMOS_VER
setenv GAMOS_BASE $GAMOSINSTALL
setenv GAMOSEXTERNAL $GAMOSDIR/external
setenv GAMOS_SEARCH_PATH .:$GAMOSINSTALL/data
setenv PATH "${GAMOSINSTALL}/bin/${G4SYSTEM}:${PATH}"

#--- SEAL variables
setenv SEAL_BASE_DIR ${GAMOSINSTALL}
setenv SEAL_PLUGIN_DIR ${GAMOSINSTALL}/module/$G4SYSTEM
setenv SEAL_KEEP_MODULES true
setenv SEAL_PLUGINS "${GAMOSINSTALL}/module/${G4SYSTEM}:${SEAL_BASE_DIR}/${G4SYSTEM}/lib/modules"

#--- GEANT4 variables
setenv G4INSTALL ${GAMOSEXTERNAL}/geant4/geant4.11.00.p03.gamos
setenv G4TMP $GAMOSINSTALL/tmp
setenv G4BIN $GAMOSINSTALL/bin
setenv G4NEUTRONHPDATA $G4INSTALL/../data/G4NDL4.6
setenv G4LEDATA $G4INSTALL/../data/G4EMLOW8.0
setenv G4LEVELGAMMADATA $G4INSTALL/../data/PhotonEvaporation5.7
setenv G4RADIOACTIVEDATA $G4INSTALL/../data/RadioactiveDecay5.6
setenv G4SAIDXSDATA $G4INSTALL/../data/G4SAIDDATA2.0
setenv G4PARTICLEXSDATA $G4INSTALL/../data/G4PARTICLEXS3.1.1
setenv G4ABLADATA $G4INSTALL/../data/G4ABLA3.1
setenv G4INCLDATA $G4INSTALL/../data/G4INCL1.0
setenv G4PIIDATA $G4INSTALL/../data/G4PII1.3
setenv G4ENSDFSTATEDATA $G4INSTALL/../data/G4ENSDFSTATE2.3
setenv G4REALSURFACEDATA $G4INSTALL/../data/RealSurface2.2
setenv G4PROTONHPDATA $G4INSTALL/../data/G4TENDL1.4/Proton
setenv G4DEUTERONHPDATA $G4INSTALL/../data/G4TENDL1.4/Deuteron
setenv G4TRITONHPDATA $G4INSTALL/../data/G4TENDL1.4/Triton
setenv G4HE3HPDATA $G4INSTALL/../data/G4TENDL1.4/He3
setenv G4ALPHAHPDATA $G4INSTALL/../data/G4TENDL1.4/Alpha
setenv G4PHP_DO_NOT_ADJUST_FINAL_STATE 1
setenv G4NEUTRONHP_DO_NOT_ADJUST_FINAL_STATE 1
setenv G4NEUTRONHP_SKIP_MISSING_ISOTOPES 1

setenv G4LIB_BUILD_SHARED 1

#--- ROOT variables
setenv ROOT_BASE_DIR  ${GAMOSEXTERNAL}/root/6.26.04/root
setenv PATH "${ROOT_BASE_DIR}/bin:${PATH}"
setenv ROOTSYS ${ROOT_BASE_DIR}

#--- FFTW variables
setenv FFTW_BASE_DIR ${GAMOSEXTERNAL}/fftw/fftw-3.2.2
# by default do not use it
setenv GAMOS_NO_FFTW 1

#---- DCMTK variables
setenv DCMTK_BASE_DIR ${GAMOSEXTERNAL}/dcmtk/dcmtk-3.6.4/
setenv DCMDICTPATH ${GAMOSEXTERNAL}/dcmtk/dcmtk-3.6.4/dcmdata/data/dicom.dic

#---- OPENJPEG variables
setenv OPENJPEG_BASE_DIR ${GAMOSEXTERNAL}/openjpeg/openjpeg-2.1.1

unsetenv G4DEBUG 
#setenv G4NEUTRONHP_NEGLECT_DOPPLER 1
source ${GAMOSINSTALL}/config/gamosvis.csh

setenv AllowForHeavyElements 1

#setenv NeutronHPNames 1

if ( "$G4SYSTEM" == "Linux-g++"  )  then
#--- Initialize LD_LIBRARY_PATH
  if ("$?LD_LIBRARY_PATH" == 0) then
    setenv LD_LIBRARY_PATH ""
  endif
  setenv LD_LIBRARY_PATH "${GAMOSINSTALL}/lib/${G4SYSTEM}:${LD_LIBRARY_PATH}"

  setenv LD_LIBRARY_PATH "${G4INSTALL}/lib/${G4SYSTEM}:${LD_LIBRARY_PATH}"
  setenv LD_LIBRARY_PATH "${ROOT_BASE_DIR}/lib:${LD_LIBRARY_PATH}"
  setenv LD_LIBRARY_PATH "/usr/lib:/usr/local/lib:/usr/local/lib/X11:/usr/explorer/lib:/usr/lib/Motif1.2:/usr/X11R6/include/X11:${LD_LIBRARY_PATH}"
  setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:${DCMTK_BASE_DIR}/lib
  setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:${OPENJPEG_BASE_DIR}/lib
else if ( "$G4SYSTEM" == "Darwin-clang"  )  then
#--- Initialize DYLD_LIBRARY_PATH
  setenv DYLD_LIBRARY_PATH "${GAMOSINSTALL}/lib/${G4SYSTEM}"

  setenv DYLD_LIBRARY_PATH "${G4INSTALL}/lib/${G4SYSTEM}:${DYLD_LIBRARY_PATH}"
  setenv DYLD_LIBRARY_PATH "${ROOT_BASE_DIR}/lib:${DYLD_LIBRARY_PATH}"
  setenv DYLD_LIBRARY_PATH "/usr/lib:/usr/local/lib/X11:/usr/explorer/lib:/usr/lib/Motif1.2:/usr/X11R6/include/X11:${DYLD_LIBRARY_PATH}"
  setenv DYLD_LIBRARY_PATH ${DYLD_LIBRARY_PATH}:${DCMTK_BASE_DIR}/lib
  setenv DYLD_LIBRARY_PATH ${DYLD_LIBRARY_PATH}:${OPENJPEG_BASE_DIR}/lib
endif

setenv G4VRML_PV_PICKABLE 1

setenv G4USE_STD17 1

setenv PYTHONPATH "${GAMOSINSTALL}/analysis/pythonUtilities"
#setenv BEAMZPOS 1
