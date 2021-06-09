name := GamosCore_GamosReadDICOM
G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: lib

include $(GAMOSINSTALL)/config/binmake.gmk
include $(GAMOSINSTALL)/config/general.gmk

CPPFLAGS += -I../../text_read/include 

EXTRALIBS += -lGamosCore_GamosBase_Base -lGamosCore_GamosUserActionMgr -lGamosCore_GamosGeometry -lGamosCore_GamosUtils -lGamosCore_GamosBase_Filters -lGamosCore_GamosGenerator

