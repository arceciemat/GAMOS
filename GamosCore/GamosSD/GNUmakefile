name := GamosCore_GamosSD
G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: lib

include $(GAMOSINSTALL)/config/binmake.gmk
include $(GAMOSINSTALL)/config/general.gmk

EXTRALIBS +=  -lGamosCore_GamosUtils -lGamosCore_GamosBase_Base -lGamosCore_GamosGeometry -lGamosCore_GamosUserActionMgr -lGamosCore_GamosAnalysis

ss: $(G4TMPDIR)/obj.last
	@echo Moving shared library $@  ...
	@mv $(G4LIBDIR)/lib$(G4TARGET).so .
