#!/bin/bash
export G4VIS_USE_VRML=1
export G4VIS_USE_DAWN=1

##### X11 and OpenGL #####
export X11LIB_HOMEDIR=/usr/lib/lib
export X11INC_HOMEDIR=/usr/include/X11
export X11INC_HOME_EXT=/usr/include/X11/extensions
export OGLHOME=/usr/lib/x86_64-linux-gnu
export G4VIS_BUILD_OPENGLX_DRIVER=1
export G4VIS_USE_OPENGLX=1
export G4VIS_BUILD_OPENGL_DRIVER=1
export G4VIS_USE_OPENGL=1
export OGLLIBS="-L/usr/lib/x86_64-linux-gnu -lGLU -lGL"
