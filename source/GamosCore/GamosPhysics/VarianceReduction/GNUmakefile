name := GamosCore_GamosPhysics_VarianceReduction
G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: lib

include $(GAMOSINSTALL)/config/binmake.gmk
include $(GAMOSINSTALL)/config/general.gmk

EXTRALIBS += -lGamosCore_GamosUtils -lGamosCore_GamosGeometry -lGamosCore_GamosPhysics_PhysicsList -lGamosCore_GamosUserActionMgr -lGamosCore_GamosUtils 
