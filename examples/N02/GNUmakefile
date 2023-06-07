name = exampleN02
G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: lib 

include $(GAMOSINSTALL)/config/binmake.gmk
include $(GAMOSINSTALL)/config/general.gmk

EXTRALIBS += -lGamosCore_GamosBase_Base -lGamosCore_GamosUtils -lGamosCore_GamosUserActionMgr -lGamosCore_GamosGeometry -lGamosCore_GamosGenerator -lGamosCore_GamosPhysics_PhysicsList -lGamosCore_GamosSD
