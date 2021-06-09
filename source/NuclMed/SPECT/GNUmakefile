name := NuclMed_SPECT
G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: lib plugin

include $(GAMOSINSTALL)/config/binmake.gmk
include $(GAMOSINSTALL)/config/general.gmk

EXTRALIBS += -lGamosCore_GamosBase_Base -lGamosCore_GamosSD -lGamosCore_GamosUserActionMgr -lGamosCore_GamosUtils -lGamosCore_GamosPhysics_Cuts -lGamosCore_GamosMovement 

EXTRALIBS += -lNuclMed_Base
