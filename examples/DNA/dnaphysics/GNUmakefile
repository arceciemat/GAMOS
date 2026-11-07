name := dnaphysics

G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: lib 

include $(GAMOSINSTALL)/config/binmake.gmk
include $(GAMOSINSTALL)/config/general.gmk

EXTRALIBS += -lGamosCore_GamosBase_Base -lGamosCore_GamosUtils -lGamosCore_GamosUserActionMgr -lGamosCore_GamosData_Distributions  -lG4root -lG4tools -lG4hntools -lG4analysismng

