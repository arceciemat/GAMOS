name := Ex5BinReader
G4TARGET := $(name)
G4EXLIB := true


ifndef G4INSTALL
  G4INSTALL = ../../..
endif

.PHONY: all
all: lib bin 

include $(GAMOSINSTALL)/config/binmake.gmk
include $(GAMOSINSTALL)/config/general.gmk

EXTRALIBS += -lGamosUtilsUA -lGamosSD -lGamosGenerator \
         -lGamosMovement -lGamosReadDICOM -lGamosPhysics_Cuts \
         -lGamosGeometry -lGamosPhysics_VarianceReduction \
         -lGamosPhysics_PhysicsList -lGamosPhysics_OtherPhysicsLists \
         -lGamosScoring -lGamosAnalysis -lGamosUserActionMgr \
         -lGamosFactories -lGamosBase -lGamosUtils

include $(GAMOSINSTALL)/config/general.gmk
