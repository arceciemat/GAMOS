name := lcg_SealBase
G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: lib 

include $(GAMOSINSTALL)/config/binmake.gmk
include $(GAMOSINSTALL)/config/gamos.gmk

INCFLAGS += -I$(GAMOSINSTALL)/source/SEAL_Foundation/PluginManager/ \
 	 -I$(GAMOSINSTALL)/source/SEAL_Foundation/SealPlatform/ \
 	 -I$(GAMOSINSTALL)/source/SEAL_Foundation/SealBase/

ifndef EXTRALIBS
EXTRALIBS := -L$(GAMOSINSTALL)/lib/$(G4SYSTEM)
else 
EXTRALIBS += -L$(GAMOSINSTALL)/lib/$(G4SYSTEM)
endif

