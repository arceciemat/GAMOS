name := GmDNA
G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: lib

include $(GAMOSINSTALL)/config/binmake.gmk
include $(GAMOSINSTALL)/config/general.gmk

EXTRALIBS += -lGamosCore_GamosBase_Base -lGamosCore_GamosUtils -lGamosCore_GamosGeometry -lGamosCore_GamosUserActionMgr -lGamosCore_GamosAnalysis -lGamosCore_GamosScoring_Management -lGamosCore_GamosPhysics_PhysicsList 
