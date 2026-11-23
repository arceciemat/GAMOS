name := sumProjdata
G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: lib bin

include $(GAMOSINSTALL)/config/architecture.gmk
#include $(GAMOSINSTALL)/config/globlib.gmk

include $(GAMOSINSTALL)/config/binmake.gmk
include $(GAMOSINSTALL)/config/general.gmk
#include $(GAMOSINSTALL)/config/gamos.gmk
#include $(GAMOSINSTALL)/config/clhep.gmk
#include $(GAMOSINSTALL)/config/seal.gmk
#include $(GAMOSINSTALL)/config/root.gmk

include $(GAMOSINSTALL)/config/gamos_libraries.gmk

#include $(GAMOSINSTALL)/config/geant4.gmk

EXTRALIBS +=
