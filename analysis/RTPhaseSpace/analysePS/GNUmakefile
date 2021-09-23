name := analysePS
G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: lib bin

include $(GAMOSINSTALL)/config/binmake.gmk
include $(GAMOSINSTALL)/config/general.gmk

include $(GAMOSINSTALL)/config/gamos_libraries.gmk

EXTRALIBS += -lRadioTherapy -lgamos
