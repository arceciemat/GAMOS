INCFLAGS += -I$(GAMOSINSTALL)/source 
INCFLAGS += -I$(G4INSTALL)/source/externals/ptl/include

ifndef EXTRALIBS
EXTRALIBS := -L$(G4INSTALL)/lib/$(G4SYSTEM)
else 
EXTRALIBS += -L$(G4INSTALL)/lib/$(G4SYSTEM)
endif

# cmake
#EXTRALIBS += -lG4FR   -lG4digits_hits      -lG4interfaces -lG4ptl     -lG4tracking -lG4GMocren -lG4error_propagation  -lG4materials     -lG4visHepRep -lG4OpenGL   -lG4event      -lG4modeling    -lG4vis_management -lG4RayTracer  -lG4geometry      -lG4parmodels  -lG4readout -lG4zlib -lG4Tree   -lG4gl2ps      -lG4particles  -lG4run    -lG4VRML   -lG4global      -lG4persistency -lG4tasking -lG4analysis -lG4graphics_reps   -lG4physicslists  -lG4tools -lG4clhep   -lG4intercoms      -lG4processes  -lG4track

EXTRALIBSS +=  -lG4accumulables -lG4phys_lists -lG4phys_ctor_glnuclear -lG4phys_ctor_helastic -lG4phys_ctor_em -lG4phys_ctor_limiters -lG4phys_ctor_hinelastic -lG4phys_ctor_ions -lG4phys_ctor_stopping -lG4phys_ctor_decay  -lG4phys_builders -lG4emdna-processes -lG4biasing_imp -lG4phys_ctor_factory -lG4RayTracer -lG4emdna-models -lG4UIbasic -lG4UIcommon -lG4visHepRep -lG4geombias -lG4geomtext -lG4GMocren -lG4FR -lG4Tree -lG4hadronic_proc -lG4hadronic_inclxx_interface -lG4physlist_util -lG4emdna-utils -lG4optical -lG4VRML -lG4analysismng -lG4decay -lG4emhighenergy -lG4tools -lG4hadronic_radioactivedecay -lG4biasing_gen -lG4emdna-moltypes -lG4emdna-molman -lG4emdna-man -lG4xrays -lG4emlowenergy -lG4vis_management -lG4hadronic_coherent_elastic -lG4parameterisation -lG4modeling -lG4had_gamm_nuclear -lG4hadronic_abla_abla -lG4hadronic_ablation -lG4hadronic_abrasion -lG4had_lend -lG4tasking -lG4had_lept_nuclear -lG4hadronic_stop -lG4hadronic_binary -lG4had_fission -lG4hadronic_bert_cascade -lG4had_part_hp -lG4hadronic_qmd -lG4had_preequ_exciton -lG4had_string_diff -lG4hadronic_deex_handler -lG4hadronic_deex_evaporation -lG4hadronic_deex_photon_evaporation -lG4hadronic_deex_gem_evaporation -lG4hadronic_deex_fermi_breakup -lG4run -lG4hadronic_deex_multifragmentation -lG4had_string_frag -lG4had_theo_max -lG4hadronic_qgstring -lG4hadronic_quasi_elastic -lG4had_string_man -lG4had_im_r_matrix -lG4geomBoolean -lG4hadronic_deex_fission -lG4hadronic_inclxx_physics -lG4shortlived -lG4hadronic_inclxx_utils -lG4hadronic_deex_management -lG4hadronic_deex_util -lG4hadronic_mgt -lG4hadronic_xsect -lG4transportation -lG4hadronic_util -lG4detutils -lG4hadronic_util -lG4geomdivision -lG4scoring -lG4muons -lG4emstandard -lG4biasing_mgt -lG4mesons -lG4detscorer -lG4event -lG4tracking -lG4detector -lG4emutils -lG4baryons -lG4ions -lG4leptons -lG4bosons -lG4cuts -lG4procman -lG4digits -lG4navigation -lG4track -lG4volumes -lG4magneticfield -lG4specsolids -lG4csg -lG4hepnumerics -lG4hits -lG4partman -lG4geometrymng -lG4heprandom  -lG4materials -lG4graphics_reps -lG4intercoms -lG4globman -lG4ptl -lG4clhep -lexpat -lm -lstdc++ -lpthread

ifdef G4VIS_USE_OPENGL
EXTRA-LS += -lG4OpenGL -lG4gl2ps -lGLU -lGL -lXmu -lXt -lXext -lX11 -lXi -lSM -lICE 
endif
