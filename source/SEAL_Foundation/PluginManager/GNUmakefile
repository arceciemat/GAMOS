name := lcg_PluginManager
G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: lib 
INCFLAGS += -I$(GAMOSINSTALL)/source/SEAL_Foundation/PluginManager/ \
 	 -I$(GAMOSINSTALL)/source/SEAL_Foundation/SealPlatform/ \
 	 -I$(GAMOSINSTALL)/source/SEAL_Foundation/SealBase/

include $(GAMOSINSTALL)/config/binmake.gmk
include $(GAMOSINSTALL)/config/gamos.gmk

EXTRALIBS += -llcg_SealBase


#
