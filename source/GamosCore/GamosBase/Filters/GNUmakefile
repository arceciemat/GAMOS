name := GamosCore_GamosBase_Filters
G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: lib

include $(GAMOSINSTALL)/config/binmake.gmk
include $(GAMOSINSTALL)/config/general.gmk

EXTRALIBS +=  -lGamosCore_GamosUtils -lGamosCore_GamosBase_Base -lGamosCore_GamosBase_Classifiers -lGamosCore_GamosUserActionMgr
