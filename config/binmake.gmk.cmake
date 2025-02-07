# $Id: binmake.gmk,v 1.4 2018/06/05 11:17:53 arce Exp $
# ----------------------------------------------------------
# Script defining rules and paths for making binaries.
#    First implementation: Gabriele Cosmo, 25/06/1998.
# ----------------------------------------------------------

ifndef LDFLAGS
#  include $(G4INSTALL)/config/architecture.gmk
  include $(GAMOSINSTALL)/config/architecture.gmk
endif

G4LIBDIR := $(GAMOSINSTALL)/lib/$(G4SYSTEM)
#?? G4LIBDIR := $(G4LIB)/$(G4SYSTEM)

# Define variable checking for existence of centrally installed headers.
# If check will be true, avoid explicit definition of paths to INCFLAGS.
#
G4INCLUDE_EXEC := $(shell [ -r $(G4INCLUDE)/globals.hh ] && echo 1)

include $(G4INSTALL)/config/G4UI_USE.gmk
include $(G4INSTALL)/config/G4VIS_USE.gmk
include $(G4INSTALL)/config/interactivity.gmk
include $(G4INSTALL)/config/analysis.gmk

# Verify the existance of the global static libraries first.
# if not verify the existance of global shared libraries (Unix only).
#
ifndef G4LIB_USE_GRANULAR
    GLOBALLIBS := $(shell test -f $(G4LIB)/$(G4SYSTEM)/libG4global.a && echo yes)
  ifndef GLOBALLIBS
    GLOBALLIBS := $(shell test -f $(G4LIB)/$(G4SYSTEM)/libG4global.$(SHEXT) && echo yes)
    SHAREDLIBS := $(shell test -f $(G4LIB)/$(G4SYSTEM)/libG4global.$(SHEXT) && echo yes)
  endif
endif

# Verify the existance of granular shared libraries (Unix) or global DLLs (Windows)
#
ifndef SHAREDLIBS
  SHAREDLIBS := $(shell test -f $(G4LIB)/$(G4SYSTEM)/libG4globman.$(SHEXT) && echo yes)
endif
ifneq (,$(findstring WIN32-VC,$(G4SYSTEM)))
  SHAREDLIBS := $(shell test -f $(G4LIB)/$(G4SYSTEM)/G4global.$(SHEXT) && echo yes)
endif  

ifndef INCFLAGS
 ifeq ($(G4INCLUDE_EXEC),1)
  INCFLAGS := -I./ \
  	      -I$(G4INCLUDE)
 else
  INCFLAGS := \
              -I.\
              -I$(G4BASE)/analysis/g4tools/include \
              -I$(G4BASE)/analysis/management/include \
              -I$(G4BASE)/analysis/hntools/include \
              -I$(G4BASE)/analysis/csv/include \
              -I$(G4BASE)/analysis/root/include \
              -I$(G4BASE)/analysis/xml/include \
              -I$(G4BASE)/analysis/factory/include \
              -I$(G4BASE)/analysis/accumulables/include \
	      -I$(G4BASE)/global/management/include \
              -I$(G4BASE)/global/HEPRandom/include \
              -I$(G4BASE)/global/HEPGeometry/include \
              -I$(G4BASE)/global/HEPNumerics/include \
              -I$(G4BASE)/run/include \
              -I$(G4BASE)/materials/include \
              -I$(G4BASE)/parameterisations/gflash/include \
              -I$(G4BASE)/particles/management/include \
              -I$(G4BASE)/particles/adjoint/include \
              -I$(G4BASE)/particles/bosons/include \
              -I$(G4BASE)/particles/leptons/include \
              -I$(G4BASE)/particles/hadrons/barions/include \
              -I$(G4BASE)/particles/hadrons/ions/include \
              -I$(G4BASE)/particles/hadrons/mesons/include \
              -I$(G4BASE)/particles/shortlived/include \
              -I$(G4BASE)/physics_lists/lists/include \
	      -I$(G4BASE)/physics_lists/builders/include \
	      -I$(G4BASE)/physics_lists/util/include \
	      -I$(G4BASE)/physics_lists/constructors/limiters/include \
	      -I$(G4BASE)/physics_lists/constructors/electromagnetic/include \
      	      -I$(G4BASE)/physics_lists/constructors/gamma_lepto_nuclear/include \
     	      -I$(G4BASE)/physics_lists/constructors/factory/include \
	      -I$(G4BASE)/physics_lists/constructors/hadron_inelastic/include \
	      -I$(G4BASE)/physics_lists/constructors/hadron_elastic/include \
	      -I$(G4BASE)/physics_lists/constructors/stopping/include \
	      -I$(G4BASE)/physics_lists/constructors/decay/include \
	      -I$(G4BASE)/physics_lists/constructors/ions/include \
              -I$(G4BASE)/processes/management/include \
              -I$(G4BASE)/processes/cuts/include \
              -I$(G4BASE)/processes/biasing/importance/include \
              -I$(G4BASE)/processes/biasing/management/include \
              -I$(G4BASE)/processes/biasing/generic/include \
              -I$(G4BASE)/processes/scoring/include \
              -I$(G4BASE)/processes/decay/include \
              -I$(G4BASE)/processes/optical/include \
              -I$(G4BASE)/processes/transportation/include \
              -I$(G4BASE)/processes/parameterisation/include \
              -I$(G4BASE)/processes/electromagnetic/adjoint/include \
              -I$(G4BASE)/processes/electromagnetic/standard/include \
              -I$(G4BASE)/processes/electromagnetic/muons/include \
              -I$(G4BASE)/processes/electromagnetic/utils/include \
              -I$(G4BASE)/processes/electromagnetic/xrays/include \
              -I$(G4BASE)/processes/electromagnetic/lowenergy/include \
              -I$(G4BASE)/processes/electromagnetic/highenergy/include \
              -I$(G4BASE)/processes/electromagnetic/polarisation/include \
              -I$(G4BASE)/processes/electromagnetic/dna/processes/include \
              -I$(G4BASE)/processes/electromagnetic/dna/management/include \
              -I$(G4BASE)/processes/electromagnetic/dna/utils/include \
              -I$(G4BASE)/processes/electromagnetic/dna/models/include \
              -I$(G4BASE)/processes/electromagnetic/dna/molecules/types/include \
              -I$(G4BASE)/processes/electromagnetic/dna/molecules/management/include \
              -I$(G4BASE)/processes/hadronic/cross_sections/include \
              -I$(G4BASE)/processes/hadronic/management/include \
              -I$(G4BASE)/processes/hadronic/models/abrasion/include \
              -I$(G4BASE)/processes/hadronic/models/binary_cascade/include \
              -I$(G4BASE)/processes/hadronic/models/cascade/cascade/include \
              -I$(G4BASE)/processes/hadronic/models/cascade/evaporation/include \
              -I$(G4BASE)/processes/hadronic/models/cascade/utils/include \
              -I$(G4BASE)/processes/hadronic/models/chiral_inv_phase_space/body/include \
              -I$(G4BASE)/processes/hadronic/models/chiral_inv_phase_space/interface/include \
	      -I$(G4BASE)/processes/hadronic/models/chiral_inv_phase_space/cross_sections/include \
	      -I$(G4BASE)/processes/hadronic/models/chiral_inv_phase_space/fragmentation/include \
	      -I$(G4BASE)/processes/hadronic/models/chiral_inv_phase_space/processes/include \
              -I$(G4BASE)/processes/hadronic/models/coherent_elastic/include \
              -I$(G4BASE)/processes/hadronic/models/de_excitation/ablation/include \
              -I$(G4BASE)/processes/hadronic/models/de_excitation/evaporation/include \
              -I$(G4BASE)/processes/hadronic/models/de_excitation/fermi_breakup/include \
              -I$(G4BASE)/processes/hadronic/models/de_excitation/fission/include \
              -I$(G4BASE)/processes/hadronic/models/de_excitation/gem_evaporation/include \
              -I$(G4BASE)/processes/hadronic/models/de_excitation/handler/include \
              -I$(G4BASE)/processes/hadronic/models/de_excitation/management/include \
              -I$(G4BASE)/processes/hadronic/models/de_excitation/multifragmentation/include \
              -I$(G4BASE)/processes/hadronic/models/de_excitation/photon_evaporation/include \
              -I$(G4BASE)/processes/hadronic/models/de_excitation/util/include \
              -I$(G4BASE)/processes/hadronic/models/em_dissociation/include \
              -I$(G4BASE)/processes/hadronic/models/general_finalstate/include \
              -I$(G4BASE)/processes/hadronic/models/high_energy/include \
              -I$(G4BASE)/processes/hadronic/models/im_r_matrix/include \
              -I$(G4BASE)/processes/hadronic/models/incl/include \
              -I$(G4BASE)/processes/hadronic/models/inucl/include \
              -I$(G4BASE)/processes/hadronic/models/isotope_production/include \
              -I$(G4BASE)/processes/hadronic/models/leading_particle/include \
              -I$(G4BASE)/processes/hadronic/models/lll_fission/include \
              -I$(G4BASE)/processes/hadronic/models/low_energy/include \
              -I$(G4BASE)/processes/hadronic/models/management/include \
              -I$(G4BASE)/processes/hadronic/models/neutron_hp/include \
              -I$(G4BASE)/processes/hadronic/models/particle_hp/include \
              -I$(G4BASE)/processes/hadronic/models/parton_string/diffraction/include \
              -I$(G4BASE)/processes/hadronic/models/parton_string/hadronization/include \
              -I$(G4BASE)/processes/hadronic/models/parton_string/management/include \
              -I$(G4BASE)/processes/hadronic/models/parton_string/qgsm/include \
              -I$(G4BASE)/processes/hadronic/models/photolepton_hadron/muon_nuclear/include \
              -I$(G4BASE)/processes/hadronic/models/pre_equilibrium/exciton_model/include \
              -I$(G4BASE)/processes/hadronic/models/qmd/include \
              -I$(G4BASE)/processes/hadronic/models/quark_molecular_dynamics/body/include \
              -I$(G4BASE)/processes/hadronic/models/quark_molecular_dynamics/util/include \
              -I$(G4BASE)/processes/hadronic/models/radioactive_decay/include \
              -I$(G4BASE)/processes/hadronic/models/relativistic_qmd/include \
              -I$(G4BASE)/processes/hadronic/models/relativistic_qmd/uplusplus/include \
              -I$(G4BASE)/processes/hadronic/models/rpg/include \
              -I$(G4BASE)/processes/hadronic/models/theo_high_energy/include \
              -I$(G4BASE)/processes/hadronic/models/lepto_nuclear/include/ \
              -I$(G4BASE)/processes/hadronic/models/inclxx/interface/include/ \
              -I$(G4BASE)/processes/hadronic/models/inclxx/incl_physics/include/ \
              -I$(G4BASE)/processes/hadronic/models/inclxx/utils/include/ \
              -I$(G4BASE)/processes/hadronic/models/util/include \
              -I$(G4BASE)/processes/hadronic/processes/include \
              -I$(G4BASE)/processes/hadronic/stopping/include \
              -I$(G4BASE)/processes/hadronic/util/include \
              -I$(G4BASE)/geometry/management/include \
              -I$(G4BASE)/geometry/solids/CSG/include \
              -I$(G4BASE)/geometry/solids/specific/include \
              -I$(G4BASE)/geometry/solids/BREPS/include \
              -I$(G4BASE)/geometry/solids/Boolean/include \
              -I$(G4BASE)/geometry/divisions/include \
              -I$(G4BASE)/geometry/volumes/include \
              -I$(G4BASE)/geometry/navigation/include \
              -I$(G4BASE)/geometry/magneticfield/include \
              -I$(G4BASE)/geometry/biasing/include \
              -I$(G4BASE)/track/include \
              -I$(G4BASE)/tracking/include \
              -I$(G4BASE)/digits_hits/detector/include \
              -I$(G4BASE)/digits_hits/hits/include \
              -I$(G4BASE)/digits_hits/digits/include \
              -I$(G4BASE)/digits_hits/scorer/include \
              -I$(G4BASE)/digits_hits/utils/include \
              -I$(G4BASE)/persistency/ascii/include \
              -I$(G4BASE)/persistency/gdml/include \
              -I$(G4BASE)/persistency/mctruth/include \
              -I$(G4BASE)/readout/include \
              -I$(G4BASE)/event/include \
              -I$(G4BASE)/error_propagation/include \
              -I$(G4BASE)/graphics_reps/include \
              -I$(G4BASE)/intercoms/include \
              -I$(G4BASE)/externals/clhep/include
  ifdef GAMOS_NO_ROOT
    INCFLAGS  += -DGAMOS_NO_ROOT
  endif
  ifdef BEAMZPOS
    INCFLAGS  += -DBEAMZPOS
  endif
  ifdef G4LIB_USE_G3TOG4
    INCFLAGS += -I$(G4BASE)/g3tog4/include
  endif
 endif
endif

CPPFLAGS += $(INCFLAGS)
LDFLAGS += -L$(G4LIBDIR)

G4TMPDIR := $(G4TMP)/$(G4SYSTEM)/$(G4TARGET)
G4BINDIR := $(G4BIN)/$(G4SYSTEM)

ifdef G4EXLIB
  G4LIBDIR := $(G4TMPDIR)
  LDFLAGS += -L$(G4LIBDIR)
  ifdef SHAREDLIBS
    ifdef G4RUNPATHOPTION
#         For the example dependent directory, keep the 
#         path to the shared lib in the executable.
      LDFLAGS += $(G4RUNPATHOPTION)$(G4LIBDIR)
    endif
  endif
  include $(GAMOSINSTALL)/config/common.gmk
endif

ifdef LDLIBS
  USER_DEFINED_LDLIBS := 1
endif

# Because of the script for granular libraries which replaces part of LDLIBS
# and because of the way user defined LDLIBS was augmented historically I
# have split LDLIBS into 4 parts...
#
ifndef USER_DEFINED_LDLIBS

# TARGETLIB is the library generated for the final user application;
# TARGOBJEXT defines the target object extension
#
ifdef G4EXLIB
  ifeq (,$(findstring WIN32-VC,$(G4SYSTEM)))
  # Unix
    TARGETLIB  := -l$(G4TARGET)
    TARGOBJEXT := o
  else
  # Windows
    TARGETLIB  := lib$(G4TARGET).a   
    TARGOBJEXT := OBJ   
  endif
endif

# LDLIBS1 contains the very high level libraries...
#
LDLIBS1 := $(EXTRALIBS)

# VISLIBS (and UILIBS?) is handled by the granular library script...
#
ifdef GLOBALLIBS
  LDLIBS2 := $(VISLIBS) -lG4interfaces -lG4persistency

  ifdef G4LIB_USE_G3TOG4
    LDLIBS2 += -lG3toG4
  endif

  LDLIBS2 += -L$(G4INSTALL)/lib/Linux-g++ \
  	     -lG4interfaces    -lG4persistency   -lG4analysis   -lG4error_propagation   -lG4readout   -lG4physicslists   -lG4run   -lG4event   -lG4tracking   -lG4parmodels   -lG4processes   -lG4digits_hits   -lG4track   -lG4particles   -lG4geometry   -lG4materials   -lG4graphics_reps   -lG4intercoms   -lG4global   -lG4FR   -lG4GMocren   -lG4OpenGL   -lG4RayTracer   -lG4Tree   -lG4VRML   -lG4clhep   -lG4gl2ps   -lG4visHepRep   -lG4vis_management   -lG4modeling   -lG4zlib
#  	     -lG4interfaces    -lG4persistency   -lG4analysis   -lG4error_propagation   -lG4readout   -lG4physicslists   -lG4run   -lG4event   -lG4tracking   -lG4parmodels   -lG4processes   -lG4digits_hits   -lG4track   -lG4particles   -lG4geometry   -lG4materials   -lG4graphics_reps   -lG4intercoms   -lG4global   -lG4FR   -lG4GMocren   -lG4OpenGL   -lG4RayTracer   -lG4Tree   -lG4VRML   -lG4clhep   -lG4gl2ps   -lG4visHepRep   -lG4visXXX   -lG4vis_management   -lG4modeling   -lG4zlib
else
# The next lines specifically should read LDLIBS2 = , not LDLIBS2 :=, so
# that it is not expanded until the directory G4TMPDIR is created.
  ifeq ($(G4INCLUDE_EXEC),1)
    LDLIBS2 = $(shell \
        G4TMP=$(G4TMP); export G4TMP; \
        if [ \( -f $(G4LIB)/$(G4SYSTEM)/liblist \
          -a -f $(G4LIB)/$(G4SYSTEM)/libname.map \) ]; then \
        $(G4LIB)/$(G4SYSTEM)/liblist \
          -m $(G4LIB)/$(G4SYSTEM) \
          < $(G4LIB)/$(G4SYSTEM)/libname.map; fi)
  else
    LDLIBS2 = $(shell \
        G4TMP=$(G4TMP); export G4TMP; \
        if [ \( -f $(G4LIB)/$(G4SYSTEM)/liblist \
          -a -f $(G4LIB)/$(G4SYSTEM)/libname.map \) ]; then \
        $(G4LIB)/$(G4SYSTEM)/liblist \
          -d $(G4TMPDIR) \
          < $(G4LIB)/$(G4SYSTEM)/libname.map; fi)
# Warning: do not add to LDLIBS2 with += because this causes it to be
# expanded too soon.  Hence extra libraries must have a different name...
# Extra libraries to resolve remaining circular dependencies...
#  LDLIBS2EXTRA = 
  endif
endif

ifdef G4LIB_USE_ZLIB
#  LDLIBS2 += -lG4zlib
endif

# LDLIBS3 contains the first set of low level libraries...
#
ifdef G4LIB_USE_GDML
  LDLIBS3 += $(GDMLLIBS)
endif
LDLIBS3 += $(INTYLIBS)
LDLIBS3 += $(ANALYSISLIBS)

endif # ifndef USER_DEFINED_LDLIBS

# LDLIBS4 contains the next set of low level libraries which historically
# (why?) the user is not supposed to be able to define...
#
LDLIBS4 += $(LOADLIBS)

# Finally assemble libraries...
#
ifdef USER_DEFINED_LDLIBS
  LDLIBS_PREFINAL := $(LDLIBS)
  LDLIBS_KERNEL   := $(LDLIBS)
else
# Again, do not use := or +=.  See note on LDLIBS2 above.
  LDLIBS_PREFINAL = $(LDLIBS1) $(LDLIBS2) $(LDLIBS2EXTRA) $(LDLIBS3)
# Version leaving out EXTRALIBS 
  LDLIBS_KERNEL   = $(LDLIBS2) $(LDLIBS2EXTRA) $(LDLIBS3)
endif
LDLIBS_PREFINAL += $(LDLIBS4)
LDLIBS_KERNEL   += $(LDLIBS4)

ifneq (,$(findstring WIN32-VC,$(G4SYSTEM)))
  # Windows
  WIN32TMP := $(patsubst -L%,$(LIB_PATH)%,$(LDFLAGS))
  LDFLAGS  = $(patsubst /,$(PATH_DEL),$(WIN32TMP))
  WIN32TMP2 := $(patsubst -L%,$(LIB_PATH)%,$(LDLIBS_PREFINAL))
  WIN32TMP3 := $(patsubst -L%,$(LIB_PATH)%,$(LDLIBS1))
  WIN32TMP4 := $(patsubst -L%,$(LIB_PATH)%,$(LDLIBS_KERNEL))
  ifdef SHAREDLIBS
    ifdef G4LIB_USE_DLL
        LDLIBS = $(TARGETLIB) $(patsubst -l%,lib%.a,$(WIN32TMP3)) $(patsubst -l%,lib%.lib,$(WIN32TMP4))
    else
        LDLIBS = $(TARGETLIB) $(patsubst -l%,lib%.lib,$(WIN32TMP2))
    endif
  else
    LDLIBS = $(TARGETLIB) $(patsubst -l%,lib%.a,$(WIN32TMP2))
  endif
  OUT   := /Fe
else
  # Unix
  LDLIBS = $(TARGETLIB) $(LDLIBS_PREFINAL)
  OUT   := -o 
endif

ifdef transform-RPath
# Invoke system specific transformation of 
include $(G4INSTALL)/config/sys/$(G4SYSTEM)-runpath.gmk
endif

sources := $(wildcard $(G4TARGET).cc)
objects := $(patsubst %.cc,$(G4TMPDIR)/exe/%.$(TARGOBJEXT),$(sources))
dependencies := $(patsubst %.cc,$(G4TMPDIR)/exe/%.d,$(sources))

LINK_DEPENDENCIES := $(G4TMPDIR)/exe/obj.last $(EXTRA_LINK_DEPENDENCIES)

.PHONY: bin clean clean_bin debug

# Make $(G4TARGET) executable.

bin: $(G4BINDIR)/$(G4TARGET)

$(G4BINDIR)/$(G4TARGET): $(LINK_DEPENDENCIES)
ifndef USER_DEFINED_LDLIBS
  ifndef GLOBALLIBS
	@echo "Using granular libraries ...  $(G4LIB)/$(G4SYSTEM)/liblist " 
	@if [ ! \( -f $(G4LIB)/$(G4SYSTEM)/liblist \
	  -a -f $(G4LIB)/$(G4SYSTEM)/libname.map \) ]; then \
	  echo "ERROR: No liblist program or library map file."; \
	  echo "       These are needed for building with granular"; \
	  echo "       libraries."; \
	  echo '         cd $$G4INSTALL/source'; \
	  echo "         gmake"; \
	  echo "       or if you are sure you have already made all the"; \
	  echo "       granular libraries:"; \
	  echo "         gmake libmap"; \
	  exit 1; fi
  else
	@echo "Using global libraries ..."
  endif
endif
	@if [ ! -d $(G4BINDIR) ] ; then mkdir $(G4BINDIR) ;fi
ifdef CPPVERBOSE
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) \
	  $(OUT)$(G4BINDIR)/$(G4TARGET) $(objects) $(LDFLAGS) \
	  $(TARGETLIB) $(EXTRALIBS) $(LDLIBS)
#	  $(LDLIBS)
else
	@echo Linking $(G4TARGET) ...
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) \
	  $(OUT)$(G4BINDIR)/$(G4TARGET) $(objects) $(LDFLAGS) \
	  $(TARGETLIB) $(EXTRALIBS) $(LDLIBS)
#	  $(LDLIBS)
	@if [ \( -f $(G4BINDIR)/$(G4TARGET) \) ]; then \
	  echo "... Done!"; fi
endif

# Touch the versioning file
ifdef G4EXLIB
$(G4TMPDIR)/exe/obj.last: lib $(objects)
else
$(G4TMPDIR)/exe/obj.last: $(objects)
endif
	@$(TOUCH) $@

# Make the $(G4TARGET).$(TARGOBJEXT) file.
$(G4TMPDIR)/exe/$(G4TARGET).$(TARGOBJEXT) : $(G4TARGET).cc
ifdef CPPVERBOSE
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(APPFLAGS) \
	 -c $(OUT_OBJ)$(G4TMPDIR)/exe/$(G4TARGET).$(TARGOBJEXT) $(G4TARGET).cc
else
	@echo Compiling $(G4TARGET).cc ...
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(APPFLAGS) \
	 -c $(OUT_OBJ)$(G4TMPDIR)/exe/$(G4TARGET).$(TARGOBJEXT) $(G4TARGET).cc
endif

GPPFLAGS := "-M"

# Make the $(G4TARGET).d file and include it.

# The ideas for this come from the GNU Make Manual, Section 4.12,
# Generating Prerequisites Automatically.  The g++ compiler has an
# option -M or -MM to write to standard output a list of dependencies
# based on the #include statements.  The "sed" adds the dependency
# file itself as a second target.  The result is a mini-makefile which
# specifies the .o and .d files as targets which depend on all the
# files found through the #include statements.  This file is then
# included, causing GNU Make to honour these dependencies.

# The "set -e" causes the shell to exit with an error when the "g++"
# fails (otherwise it would only notice the last command in the
# pipeline, namely "sed").  GNU Make notices the error and exits
# sooner than it otherwise would (still not as soon as I expect,
# though!).  Even then, an empty file is made, so "[ -s $@ ] || rm -f
# $@" removes it ([ -s filename ] gives zero exit code only if file
# exists and has a size greater than zero).  This avoids making
# corrupt .d files which would play havoc with your next build.

$(G4TMPDIR)/exe/$(G4TARGET).d: $(G4TARGET).cc
	@if [ ! -d $(G4TMP)/$(G4SYSTEM) ] ; then mkdir $(G4TMP)/$(G4SYSTEM) ;fi
	@if [ ! -d $(G4TMPDIR) ] ; then mkdir $(G4TMPDIR) ;fi
	@if [ ! -d $(G4TMPDIR)/exe ] ; then mkdir $(G4TMPDIR)/exe ;fi
	@echo Making dependency for file $< ...
	@set -e;\
	g++ $(GPPFLAGS) $(CPPFLAGS) -w $< |\
	sed 's!$(G4TARGET)\.$(TARGOBJEXT)!$(G4TMPDIR)/exe/& $@!' >$@;\
	[ -s $@ ] || rm -f $@
ifneq ($(dependencies),)
ifneq ($(MAKECMDGOALS),clean)
-include $(dependencies)
endif
endif

clean::
	@echo Cleaning up ...
	@rm -f ./core
	@rm -rf $(G4TMPDIR)
	@rm -f $(G4LIBDIR)/lib$(G4TARGET).a
	@rm -f $(G4LIBDIR)/lib$(G4TARGET).$(SHEXT)
	@rm -f $(G4BINDIR)/$(G4TARGET)
ifdef CFRONT
	@rm -rf $(G4TREP)/exec
endif

clean_bin::
	@echo Removing application $(G4TARGET) ...
	@rm -f ./core
	@rm -f $(G4BINDIR)/$(G4TARGET)
