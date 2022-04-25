#!/bin/csh
setenv G4VIS_USE_VRML 1
setenv G4VIS_USE_DAWN 1

##### X11 and OpenGL #####
setenv X11LIB_HOMEDIR /usr/lib/lib
setenv X11INC_HOMEDIR /usr/include/X11
setenv X11INC_HOME_EXT /usr/include/X11/extensions
setenv OGLHOME /usr/lib/x86_64-linux-gnu
setenv G4VIS_BUILD_OPENGLX_DRIVER 1
setenv G4VIS_USE_OPENGLX 1
setenv G4VIS_BUILD_OPENGL_DRIVER 1
setenv G4VIS_USE_OPENGL 1
setenv OGLLIBS "-L/usr/lib/x86_64-linux-gnu -lGLU -lGL"
