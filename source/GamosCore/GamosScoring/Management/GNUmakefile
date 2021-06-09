name := GamosCore_GamosScoring_Management
G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: lib

include $(GAMOSINSTALL)/config/binmake.gmk
include $(GAMOSINSTALL)/config/general.gmk

EXTRALIBS +=  -lGamosCore_GamosUtils -lGamosCore_GamosBase_Base -lGamosCore_GamosBase_Filters -lGamosCore_GamosGeometry  -lGamosCore_GamosReadDICOM -lGamosCore_GamosGenerator -lGamosCore_GamosData_Management -lGamosCore_GamosData_Users

