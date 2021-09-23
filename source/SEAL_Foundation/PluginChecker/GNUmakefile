name := SealModuleSymCheck
G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: lib bin

include $(GAMOSINSTALL)/config/binmake.gmk
include $(GAMOSINSTALL)/config/gamos.gmk
include $(GAMOSINSTALL)/config/seal.gmk
include $(GAMOSINSTALL)/config/geant4.gmk

#
EXTRALIBS += -llcg_SealBase -llcg_PluginManager

