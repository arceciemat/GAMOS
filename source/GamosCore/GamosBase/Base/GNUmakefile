name := GamosCore_GamosBase_Base
G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: lib plugin

include $(GAMOSINSTALL)/config/binmake.gmk
include $(GAMOSINSTALL)/config/general.gmk
include $(GAMOSINSTALL)/config/seal.gmk

EXTRALIBS +=  -lGamosCore_GamosUtils 
