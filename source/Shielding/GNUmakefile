name := Shielding
G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: lib

include $(GAMOSINSTALL)/config/binmake.gmk
include $(GAMOSINSTALL)/config/general.gmk

EXTRALIBS += -lGamosCore_GamosBase_Base -lGamosCore_GamosSD -lGamosCore_GamosUserActionMgr -lGamosCore_GamosUtils -lGamosCore_GamosReadDICOM -lGamosCore_GamosUtilsUA -lGamosCore_GamosScoring_Management -lGamosCore_GamosPhysics_Cuts -lGamosCore_GamosAnalysis
