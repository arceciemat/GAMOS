name := GamosCore_GamosBase_Classifiers
G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: lib plugin

include $(GAMOSINSTALL)/config/binmake.gmk
include $(GAMOSINSTALL)/config/general.gmk

EXTRALIBS +=  -lGamosCore_GamosUtils -lGamosCore_GamosBase_Base -lGamosCore_GamosGeometry
