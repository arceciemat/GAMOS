name := resizeImage
G4TARGET := $(name)
G4EXLIB := true

.PHONY: all
all: lib bin

include $(GAMOSINSTALL)/config/binmake.gmk
include $(GAMOSINSTALL)/config/general.gmk

include $(GAMOSINSTALL)/config/gamos_libraries.gmk

INCFLAGS += -I$(GAMOSINSTALL)/analysis

include $(GAMOSINSTALL)/config/dcmtk.gmk

EXTRALIBS += -lDICOMBase -lDICOMReaders -lDICOM2G4
