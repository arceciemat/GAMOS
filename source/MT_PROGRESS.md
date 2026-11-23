# GAMOS multithreading port (source.MT / analysis.MT)

Goal: GAMOS running with G4MTRunManager / G4TaskRunManager exactly as Geant4 does
(`/run/numberOfThreads N`, env `G4RUN_MANAGER_TYPE`, `G4FORCENUMBEROFTHREADS`).
All changes are guarded by `#ifdef G4MULTITHREADED`, so the same tree still builds sequentially.

## Geant4
- MT build of the patched Geant4: `/datan1/gamos/external/geant4/geant4.11.4.p03.gamos.MT`
  (copy of geant4.11.4.p03.gamos sources, `GEANT4_BUILD_MULTITHREADED=ON`, build/ inside).

## Design
1. **Command replay** (Geant4 mechanism): every UI command executed on the master that is
   `ToBeBroadcasted()` is stacked and re-executed on each worker before its first BeamOn.
   So each worker creates its own GAMOS objects (user actions, filters, classifiers,
   scorers, SDs, histograms, generator) simply by replaying the user's macro.
   - Worker threads create the GAMOS messengers in `GmActionInitialization::Build()`.
   - Commands that act on shared objects (geometry, physics list, materials, cuts,
     regions, user limits, `/gamos/setParam`, random seeds...) are `SetToBeBroadcasted(false)`:
     they run only on the master.
2. **Thread-local singletons**: managers holding per-event/per-run state get
   `static G4ThreadLocal X* theInstance`; a non-thread-local `theMasterInstance` pointer is kept
   where workers must merge into the master.
   Shared (master-only, read by workers): GmParameterMgr, geometry/physics/cut managers.
3. **User actions**: on the master only the run-action list is given to G4MTRunManager;
   event/stacking/tracking/stepping lists only on workers.
   On the master, UAs' Begin/EndOfRunAction are called only if `IsRunOnMaster()`
   (GmScoringUA, ...); on workers all are called.
4. **Merging** (at worker end of run, under mutex):
   - scorers: `GmScoringRun::Merge` (G4Run::Merge, called by G4WorkerRunManager)
   - histograms: GmAnalysisMgr worker histos added into master histos with same file/name;
     only the master writes files.
5. Output files written by workers: thread suffix `.thN` in the name (GmFileNameUtils helper).

## Build / run
- Separate install tree `/datan1/gamos/GAMOS.7.0.1.MT` (source -> GAMOS.7.0.1/source.MT, analysis -> analysis.MT,
  own config/lib/tmp/bin/module). Config: `source /datan1/gamos/GAMOS.7.0.1.MT/config/confgamos.sh` (or .csh):
  G4INSTALL = geant4.11.4.p03.gamos.MT and `G4MULTITHREADED=1` (needed: GNUmake build uses -DG4GMAKE, which
  skips G4GlobalConfig.hh).
- ALWAYS source the config with an absolute path: `source` is a symlink, `../config` from inside it resolves to
  the sequential GAMOS.7.0.1/config (this happened once on 2026-11-19 and overwrote the sequential libs; they were
  rebuilt with `make -B` from GAMOS.7.0.1/source, stale .d files and GmActionInitialization.o removed, tested OK).
- Run: `/run/numberOfThreads N` in the macro (before /run/initialize) or `G4FORCENUMBEROFTHREADS=N`;
  `G4RUN_MANAGER_TYPE=Serial|MT|Tasking` (Geant4 default: Tasking).

## Done (2026-11-19)
- GmRunManager: G4RunManagerFactory, GmActionInitialization (Build -> GmRunManager::BuildForWorker),
  CreateMessengers(bMaster). Generator set only on worker RMs.
- Messengers on workers: Factories (geometry/physicsList/verbosity guarded), SD, Geometry (only /gamos/field),
  Biasing, VarianceReduction, Scoring, ExtraPhysics (Idle-state process construction must be replayed: worker
  processes are matched to master ones by position), UserLimits (workers only add GmStepLimiter/GmUserSpecialCuts
  processes; regions/cuts master only). Master only: Analysis (static settings), Base (/gamos/setParam),
  Run (random), DeprecatedCommands.
- Thread-local singletons: GmUserActionMgr GmStepMgr GmFilterMgr GmClassifierMgr GmDistributionMgr GmGetParticleMgr
  GmHitsEventMgr GmVSDList GmDataMgr GmScoringMgr GmPSPrinterMgr GmPDSInvertGeantinoStackUA GmGeneratorMgr
  GmIsotopeMgr RTPlanMgr GmTimeStudyMgr CCIOMgr PETProjDataMgr PETIOMgr PETIOMgrTOF PETIOMgrCrystalID SPECTIOMgr
  SPECTProjDataMgr RTExtraInfoMgr SHMetastableIsotMgr GmCutsStudyMgr GmBiasingMgr GmMovementMgr, GmAnalysisMgr instances.
- Thread-local statics: GmCheckOriginalGamma maps, GmVHistoBuilder::theHistoNumbers, GmGenerDistTimeDecay::theCurrentActivity,
  GmVSD::bFirstSDBegin/End, GmG4Utils maps, GmFileIn::theInstances, GmImportanceSamplingProcess::theInheritTrackInfoUA,
  GmTimeStudyMgr::theG4Verbose, RTVExtraInfoProviderLong::theWords, SHUtils maps, GmCSTrackInfo::theEnergy2Range,
  GmPDSScore::theNumberOfScores, DicomNestedPhantomParameterisation::ComputeMaterial local `mate`.
- User action lists: event/stacking/tracking/stepping lists not given to G4MTRunManager; run list on master calls
  only UAs with IsRunOnMaster() (GmScoringUA). New GmUserAction::SetRunOnMaster().
- GmMTMergeMgr (GamosUtils): merger registry called at worker end of run (mutex), GetThreadFileName(), IsWorkerThread().
- GmAnalysisMgr: thread-local instances + master instances; worker histos added to master by name (cloned if missing),
  reset after merge; ROOT::EnableThreadSafety + TH1::AddDirectory(false). Only master writes files.
- Scoring: GmScoringRun::Merge (hits maps) + GmVPrimitiveScorer::MergeToMaster (SumV2, NFilled, spectra);
  GmScoringUA prints only on master; SqdoseEachNEvents ignored in MT.

- G4tgrEvaluator (shared, not thread safe): GmGenUtils::GetValue/GetInteger locked; GmGenUtils::GetDoubleTgr() used in generator.
- SEAL PluginManager: global recursive mutex in PluginFactory::create (lazy module loading crashed with threads).
- GmIOMgr::SetFileNameOut adds .thN in workers. GmNumberOfEvent thread local + merged to master.

## First validation (2026-11-19), testMT/test2.in (e- 6 MeV, water box, 2 scorers, GmTrackDataHistosUA, 20000 ev)
- seq:  dose 4.66092e-12 Gy +-0.36%   MT 4 threads: 4.6671e-12 Gy +-0.37%  -> agree; errors agree (SumV2 merge OK)
- histogram entries 138131 (seq) vs 137719 (MT): statistical. Run: G4RUN_MANAGER_TYPE=MT; always `< /dev/null`
  (a SEAL ASSERT prompt looped and wrote a 30 GB log once).
- Not yet tested with G4RUN_MANAGER_TYPE=Tasking (Geant4 default).

## Session 2 (2026-11-19)
- Tasking (Geant4 default): G4WorkerTaskRunManager::ProcessUI() re-applied the whole command stack (GAMOS objects
  created twice -> "No active source"). GmWorkerTaskRunManager overrides ProcessUI() (commands already applied by
  G4TaskRunManager); GmUserTaskThreadInitialization creates it. Tested: 2 x beamOn, same results as sequential.
  Note: scorer errors in 2nd run are larger also in SEQUENTIAL (SumV2 not reset between runs) -> existing GAMOS behaviour.
- RTIAEASharedReader: IAEA phase space read by all threads with one shared file position (mutex): each particle read
  once, skip at start of each run and recycling global (recycle counter starts as in generator, theNRecycled=1).
  Used by RTGeneratorPhaseSpace and RTGeneratorPhaseSpaceRTPlan. EGS and Penelope generators: FatalException in MT.
  Test RT exercise3b: dose 1.0884e-14+-1.37% (MT) vs 1.0931e-14+-1.38% (seq); all threads stop at end of file.
- RTPhaseSpaceUA writes NAME.thN.IAEAphsp/.IAEAheader; analysis.MT/pythonUtilities/mergeIAEAphsp.py merges them
  (sums counts and ORIG_HISTORIES, checksum, statistics). Test RT exercise2b OK.
  UPDATE (session 3): merged automatically in C++ at end of run. Each worker, after closing its files, appends its
  .IAEAphsp to NAME.IAEAphsp and adds its header counters (iaea_header_type::add_counters) under a mutex; a master
  end-of-run action (GmMTMergeMgr) writes NAME.IAEAheader. Thread files deleted unless
  /gamos/setParam RTPhaseSpaceUA:KeepThreadFiles 1. Records are in thread-completion order (not th0,th1,...).
  Test RT exercise2b (6 Z stops, 4 threads): merged = concatenation of thread files, header = mergeIAEAphsp.py one
  (except <E> printed with fewer digits); merged file read by RTGeneratorPhaseSpace OK. mergeIAEAphsp.py kept for
  thread files kept with KeepThreadFiles or merging several jobs.
  Also fixed (seq too): header file name ignored RTPhaseSpaceUA:ZStopInFileName (NAME.IAEAheader vs NAME_Z.IAEAphsp
  with only one Z stop); now RTPhaseSpaceUA::GetPSFileName is used for both.
- GetThreadFileName in GmShowerShapeUA, GmDNATimeStepAction, GmVMicroyzUA, RTPhaseSpace_EGSUA.
- GmMTMergeMgr::RegisterMasterEndOfRunAction: run on master after all merges (GmUserRunActionList).
- PETProjDataMgr / SPECTProjDataMgr: sinograms merged to master instance, written by master. Test PET exercise3a OK
  (coincidences 954 vs 1008, all histogram files equivalent).
- GmGeometryUtils thread local (shared G4TouchableHistory in BuildTouchableName crashed navigation; lazy caches).
- GmParameterMgr: usage bookkeeping (map ++ and set insert on every Get) under mutex.
- GmAnalysisMgr: histogram files booked only in workers are now saved by master.

- GmUserAction master registry: GmRunManager::SelectUserAction registers master instances by name;
  GmUserAction::GetMasterUserAction(name), GetMergeMutex(). Pattern for UAs with end-of-run summaries:
  SetRunOnMaster(true); worker EndOfRunAction adds its data to the master instance (under mutex) and returns;
  master EndOfRunAction prints. Done for GmCountProcessesUA (per thread if it has a classifier), GmPhantomDoseUA,
  PET/SPECT classifier UAs (table from merged histograms, merged sinogram).
- GmPDSUA (point detectors): scores of all workers added to those of the first worker of the run, printed by master
  (GmMTMergeMgr master end-of-run action) with that worker's helper.
- GmBaseMessenger (/gamos/filter, /gamos/classifier, /gamos/distribution) now created on workers (SVG cmds master only).
- Biasing: createOperator/addParticle*/addProcesses2Oper/EWBS commands allowed in Idle (workers replay in Idle state);
  physics constructor registered and configured only by master; operators per thread.
- Tutorial sweep (testMT/tut, testMT/runtut.sh, runtutseq.sh; results mt.txt, seq.txt): 177 macros.

- Classifiers: indices assigned in order of appearance (ByLogicalVolume, ByParticle, ByProcess, ByMaterial, ByRegion,
  ByPhysicalVolume(Replicated), ByParallelLogicalVolume, ByCreatorProcess, ByPrimaryParticle, ByParticleProcess,
  BySecondariesList(Exclude), ByHadronicReaction, ByStringData) were THREAD DEPENDENT -> merged results by index wrong.
  Now GmVClassifier::AssignNewIndex(): worker asks the master classifier of the same name (mutex, only for new keys),
  caches the index. ByParticleProcess keys by master process. ByAncestors gives its index names to the master.
  Master classifiers registered in GmClassifierMgr::AddClassifier.
- G4tgrFileIn::GetInstance is shared by all threads: replaced by GmFileIn (thread local) in GmG4Utils (element names,
  "Z value not found" crash), SHUtils, GmChangeEnergies.
- GmConvergenceTester::Merge (histories of workers appended to master). FOM in MT uses the sum of CPU times of all threads.
- FatalException in MT: GmGeneratorChangeEnergyAndMaterial, GmVChangeWorldAndSource (change shared geometry between events),
  GmMovementMgr (geometry movements), RTGeneratorPhaseSpace_EGS, RTGeneratorPSPenelope.
- Tutorial sweep after fixes: all tutorials that run sequentially also run in MT, except PlugInTutorial (user plug-ins
  must be compiled against the MT tree) and the 3 GmGeneratorChangeEnergyAndMaterial macros (refused on purpose).

## Session 3 (2026-11-19)
- RANDOM SEEDS (bug, MT only): /gamos/random/restoreSeeds and /gamos/random/setEngine replace the CLHEP engine
  (setTheEngine(new ...)), but G4MTRunManager/G4TaskRunManager seed the workers from the engine they had when created
  (masterRNGEngine) -> all jobs gave the SAME results whatever the seeds. GmRunManager::UpdateMTMasterRandomEngine()
  sets masterRNGEngine (both copies, G4MTRunManager and G4TaskRunManager) to the new engine; called after both commands.
- GmHistosGammaAtSD ($$GSD table): worker counts merged into the master instance, printed once by the master.
- RTGeneratorPSPenelope: one shared input file for all threads (mutex), header/skip/recycling global; MT FatalException removed.
  Fixed also in sequential: theNEventsSkip never initialised (now parameter RTGeneratorPSPenelope:NEventsSkip, default 0),
  theParticleDef/theHistos not initialised, MaxNReuse=-1 (default) gave weight -w (negative doses) -> now 1,
  after recycling the last (eof) line was used and the job crashed -> first line of the new pass is read.
  Test (testMT/pentest, synthetic 20000 particles file), dose Serial / MT 4 threads:
  10000 ev 4.427e-12/4.435e-12 (+-0.75%); 20000 ev 4.419e-12/4.411e-12; 30000 ev no recycle (stops at end of file)
  4.419e-12/4.407e-12; 30000 ev 2 passes 4.413e-12/4.428e-12 (+-0.44%). (In Tasking "events processed" after the
  file is exhausted includes the aborted events, the dose normalisation is right.)
- EGS phase space (BUG CONFIRMED, also sequential): EGSPhspParticle::RefreshParticle never advanced theFilePointer ->
  the FIRST RECORD WAS READ FOR EVERY PARTICLE (synthetic test: old code 5.62e-12 Gy, all primaries 4.70893 MeV at the
  same x,y; fixed 4.407e-12 Gy = PENELOPE file with the same particles 4.419e-12). Also fixed: the charge used to subtract
  the electron mass was the one of the previous record; MODE2 files (32-byte records) now supported; after recycling the
  data of the last record were used again. RTGeneratorPhaseSpace_EGS: shared file position for all threads (mutex),
  skip at start of each run and recycling global, automatic MaxNReuse per run in each thread; MT FatalException removed.
  Test Serial / MT 4 thr: 10000 ev 4.410e-12/4.406e-12; 20000 ev 4.407e-12/4.421e-12; 30000 ev no recycle (stops at
  end of file) 4.407e-12/4.398e-12; 30000 ev 2 passes 4.400e-12/4.435e-12 (+-0.43%).
- GmCountTracksAndStepsUA: worker counts merged into the master, printed once (classifier names from master classifier;
  tested with GmClassifierByParticle, MT = Serial within statistics).
- GmDataTTreeUA: each worker writes NAME_tree_INDEX.thN.root (all threads wrote the same file); merge with ROOT hadd.
- GmDumpMateG4dcmUA, GmBuildPhantomStructuresUA: run once on the master (all workers wrote the same file).
- analysis.MT/pythonUtilities/mergeThreadFiles.py: concatenates NAME.thN.EXT -> NAME.EXT (data text/bin files without
  header options, hits, PET/SPECT/CC list mode, ...); --skip-lines N for text headers. Tested GmTrackData{Text,Bin}FileUA.
- GmCountProcessesUA: now merged into the master also with a classifier (it printed per thread): classifier indices are
  the same in all threads (master AssignNewIndex, or computed / user-given indices). Tested GmClassifierByRegion.
- SHNthValueLayerUA: counts per layer and number of events merged into the master, one table and one fit
  (ShieldingTutorial exercise1a1 MT: layer0 0.7801 layer4 0.2843; seq 0.7795 / 0.2821).
- RTVZRLimitsUA/RTZRLimitsUA (killed particles per Z) and GmHistosAtCavityUA ($$HAC counters): merged into the master,
  printed once (RTZRLimitsUA tested, testMT/pentest/zr*.in).
- GmRunData{Cout,Histos,TextFile,BinFile}UA: workers only accumulate; accumulated run data (all are sums) added to the
  master instance (GmVDataUser::MergeRunDataToMaster, GmVData::AddDataAccumulated), which prints / fills / writes once
  (before: one line/entry per thread). Tested: one histogram entry, one text line, MT = Serial within statistics.
- Merged into the master, printed once: SHActivationUA (ions summed; job time = SUM of the time spans of the threads,
  as each thread has its own source clock at the full activity -> events/s as sequential; ShieldingTutorial exercise4a:
  1e8 ev/s both, Cr50 495/501, Cu66 432/420), SHStudyDecayChannelsUA (set of particles), RTReuseAtZPlaneWithNVoxelUA
  (sums; theRatioSum/theRatioSum2 were not initialised), GmVMicroyzUA (sums Y,Y2,Y4,Z,Z2,Z4,Wei,Wei2; the master writes
  no file; Y4,Z4,Wei2 were not initialised). examples/DNA/microyz 4000 ev, 4 seeds: y_F serial 0.1315+-0.0012,
  MT 0.1304+-0.0023; y_D 0.299+-0.010 / 0.295+-0.006.
- FOUND, NOT CHANGED (also sequential, for the authors):
  * SHRadDecayChainSet::BuildPossibleDecayChains: chains with the same name are merged adding the BR but NOT the
    NATOMS_IN_JOB: e.g. Cu64 (833 atoms) and Cu64[1594.19] (101 atoms) give one line with only one of the two counts,
    which one depends on the order of the G4Ions pointers (seq printed 833, MT 67).
  * GmVMicroyzUA::EndOfRunAction: '1/nev' is an integer division (=0) in the error formulas: the printed relative errors
    (~0.03%) are ~30 times smaller than the spread between seeds (~1%).
- Still printed per thread: SHActivationFromFluxUA (uses the run map of a worker scorer), SHGetHadronicChannelXSUA,
  GmProdCutsStudyUA, GmMinRangeLimitsStudyUA (study tools with complex per-run structures).
- Check of physics (CC tutorial geometry, 141 keV gammas, 1e6 events): photoelectric counts Serial 71734/71756,
  MT 8 thr 72261/72142 (1.7 sigma, opposite sign to the QA difference) -> no physics difference. The MT build run with
  G4RUN_MANAGER_TYPE=Serial gives EXACTLY the same counts as the sequential GAMOS.7.0.1 with the same seeds.
- EVENT-FILE GENERATORS (BUG, MT only): each worker opened its own copy of the input file and read it from the start
  -> every event of the file was generated once PER THREAD. Fixed with one file position for all threads:
  GmGeneratorFromTextFile (lines of one event read together under a mutex; no stale extra event at end of file),
  GmGeneratorFromBinFile, GmGeneratorFromTextFileWithParent (the first instance reads for all threads, under a mutex),
  GmFromTextFileSource (/gamos/generator/addFromTextFileSource: one reader per source name). ShieldingTutorial
  exercise4b QA text tests: 21 FAILED -> 1 (p=0.03). Files written per thread by MT (e.g. exercise4a ->
  NAME.out.thN.exercise4a) must be merged with mergeThreadFiles.py before being read (event lines stay contiguous).
- SHActivationFromFluxUA: flux-scorer run values matched BY ION (indices depend on the thread) and added to the master;
  time span summed as in SHActivationUA. Compiled only (no tutorial/example uses it).
  Note (also sequential): CalculateErrors() is never called, so the printed relative errors are always 0.
- Not merged (study tools): GmProdCutsStudyUA, GmMinRangeLimitsStudyUA (their histograms merge by name; only the
  printed minimum ranges are per thread, keyed by thread-local G4VProcess*), SHGetHadronicChannelXSUA (only used with
  GmGeneratorChangeEnergyAndMaterial, sequential only).
- QA seq vs MT, all 74 exercises (qa/qa_results_*.txt): no MT physics difference left. Explained failures:
  statTest artifacts (empty histograms, infinite integrals, correlated entries per primary: gammaSD distances, PT dose
  profiles), small-count tails (p~0.02-0.05), qaref text lines not found in either version (RT 5b, Shielding 5f, CC 4b),
  SD measuring-time edge effect (PET 4a). Shielding 5a/5e/5g: the SEQUENTIAL Geant4 tree
  (geant4.11.4.p03.gamos) had its ParticleHP sources modified and rebuilt on 2026-11-19 09:34-13:27
  (G4HadronPhysicsQGSP_BIC_AllHP.cc, G4ParticleHPManager.hh, G4ParticleHPInelasticCompFS.cc, G4ParticleHPProduct.cc),
  not the MT tree: different AllHP proton final states. The MT build in Serial and in MT mode agree with each other.
- QA findings that are NOT bugs: (1) statTest marks FAILED a histogram empty in one version (e.g. 2 hits vs 0 in
  GammaSpectrometry exercise2d); (2) gammaSD "Distance between interactions" histograms fail also seq-vs-GAMOS.7.0
  (correlated entries); (3) LIMITATION: SD:MeasuringTime / DeadTime are applied to the time sequence of each thread and
  the hits in the last measuring window of each thread are lost (no flush at end of run, also in sequential but once):
  PET exercise4a (1 us window, 10 us runs) hits -5% with 2 threads, -16% with 4, OK with 1 thread. Negligible when the
  run time per thread >> measuring/dead time.
- SHNthValueLayerUA: worker counts/NEvents merged into the master, printed and fitted once (ShieldingTutorial exercise1a1:
  NSTEPS shield:0..3 = 0.780/0.606/0.469/0.364 MT vs 0.779/0.605/0.470/0.365 Serial; ancestors classifier names OK).
- GmCountProcessesUA with a classifier: now also merged into the master (tested GmClassifierByRegion, same lines as Serial).
- QA status (qa/qa_results_part1.txt, qa_results_cc.txt): CC, GammaSpectrometry, HS, PET, PT 1a/2a/2c/3a compared, no MT
  bug found. PT dose-profile / exit-spectrum KS failures (e.g. exercise3a X/Y/Z_Merged, BraggPeak_*) also fail, more
  strongly, in the seq GAMOS.7.0.1 vs GAMOS.7.0 QA (correlated deposits in many bins -> KS significance overestimated).
  Remaining exercises (PT 3c.., RT, Shielding, XRay) still running in qa/ (seq side), compare with qacmp.sh.
- QA seq vs MT with the tutorial QA tools: testMT/qa (copies qa/seq and qa/mt of the tutorials; runNx.sh MODE TUT DIR EXER
  = runN.sh with 10 jobs, MT with 2 threads per job; qacmp.sh TUT DIR EXER compares with StatTest/statTest.py using
  EXER.qaref; runall.sh MODE LIST). Results in qa/qa_results.txt.

## Session 4 (2026-11-19)
- Geant4 MT tree: copied the ParticleHP changes of the sequential tree (G4HadronPhysicsQGSP_BIC_AllHP.cc,
  G4ParticleHPManager.hh (+ installed include/Geant4 copy), G4ParticleHPInelasticCompFS.cc, G4ParticleHPProduct.cc),
  rebuilt. Default multiplicity method: Poisson (PHP_USE_POISSON{true}; BetweenInts only with
  G4PHP_MULTIPLICITY_METHOD=BetweenInts, not set in the configs); "PHP use Poisson 1" printed in seq, Serial and MT.
  MT build in Serial mode now gives exactly the same AllHP proton products as the sequential install.
- END-OF-JOB MERGE OF THREAD FILES (C++): GmMTMergeMgr::GetThreadFileName registers each thread file; the gamos main
  calls GmMTMergeMgr::MergeThreadFiles() after deleting the run manager (worker files closed/flushed): NAME.thN.EXT
  concatenated in thread order into NAME.EXT and deleted (/gamos/setParam GmMTMergeMgr:KeepThreadFiles 1 keeps them).
  Modes: concatenation (default), text header lines kept only from the first file (data text files with
  WriteHeaderData), EGS phase space (header counters summed, max/min E, records appended), no merge (data binary
  files with header counters: message, files kept), .root (message with the hadd command), IAEA (merged by
  RTPhaseSpaceUA). Tested: GmTrackData{Text,Bin}FileUA merged = byte-identical concatenation; Shielding exercise4a
  output read by exercise4b; EGS file written in MT read back. mergeThreadFiles.py only for kept files.
- Fixed in BOTH trees (source and source.MT):
  * Shielding HP printers: SHPrintPHPChannelThickYieldUA, SHPrintPHPChannelXSUA, SHPlotPHPDBSpectraUA tested the
    model NAME "ParticleHPInelastic" / "ParticleHPCapture"; in Geant4 11.4 the names are "ProtonHPInelastic",
    "NeutronHPInelastic", ... -> they printed NOTHING (ShieldingTutorial exercise5a/5c/5f). Now dynamic_cast.
  * SHRadDecayChainSet: chains with the same name from different first ions (Cu64[1594.19] -> Cu64) now add their
    atoms (N*BR conserved); before one count was lost (ex4a Cu64: 902 = 838 + 64 now).
  * GmVMicroyzUA: error of the means was too small by ~sqrt(N) (wrong denominator, and 1/nev integer); now
    sum(w(x-m)^2)/(W-W2/W)*W2/W^2. microyz example 4000 ev: y_F rel. error 1.84% (= computed from the event values).
    Sums Y4/Z4/Wei2 initialised.
  * SHActivationFromFluxUA: CalculateErrors() called (errors were always 0).
  * RTPhaseSpace_EGSUA: positions written in mm (EGS format and reader: cm) and kinetic energy for e+- (format and
    reader: total energy); after writing the header at end of run the file position was not restored (a next run
    overwrote the first records); header flushed. Text file (TextFileName) flushed at end of run (RTPhaseSpaceUA too).
  * Ported to the sequential tree the fixes that were only in MT: EGS reader (first record always read, charge,
    MODE2, recycling), PENELOPE reader (negative weights, uninitialised variables, crash after recycling),
    RTReuseAtZPlaneWithNVoxelUA initialisation, RTPhaseSpaceUA header name with ZStopInFileName.
- GmShowerShapeUA (MT): thread file name was suffixed again at each run; binary file flushed each event.
- QA tools: lines printed by workers start with "G4WTn > ": removed in qa/runNx.sh for MT. Remaining Shielding 5
  differences are .qaref problems: 5a uses a printed cross-section value as part of the line key (last digit
  varies), 5e has ErrorCol pointing to a word (statTest crashes); MT and seq outputs of 5e are identical.
  Note: thick-target yields of 5f are much smaller than with GAMOS 7.0 (e.g. G4_Al gamma 0.00173 vs 0.0645): Geant4
  11.4 ParticleHP/data, not GAMOS.

## Session 5 (2026-11-19)
- qaref fixed (tutorials): RT exercise5b (no dose printer in the macro: Text block and RTPSPDoseHistos histos removed),
  CC exercise4b (empty Text block removed), Shielding exercise5a (rebuilt; value = macroscopic XS, line key without
  printed numbers; buildQA.exercise5a.sh shift 4), Shielding exercise5e (rebuilt with ErrorCol = ValueCol+2).
  QA: 5e 20/20 pass, 5a ~4% NormalCDF failures (HP cross sections vary with the seed).
- Microdosimetry vs Geant4 example microyz (testMT/g4dna/yz: 1 keV e-, 2 nm, dna_opt2, 10 jobs x 1000 ev):
  Geant4 yF 12.94 (spread 2.4%) yD 23.28 (2.7%); GAMOS GmMicroyzUA yF 13.10 (2.6%) yD 23.50 (2.1%); MT 12.95 / 23.18.
  ERRORS (both trees): delta method for ratios of sums, new sums Sum(w2 y^k) (GmVMicroyzUA::AddEntry used by the 4 UAs,
  MT merge included); printed errors yF 2.46%, yD 2.45% = spread of the jobs. The yD error printed before was that of <y^2>.
  GmMicroyzG4UA: protection for events without hits. NOTE: GmMicroyzG4UA uses weight nbEdep/nHits, GmMicroyzUA and the
  Geant4 example plot.C use nHits/nbEdep (not changed, to be checked with the author).
- microtrack (examples/DNA/microtrack plug-in MUTTrackerSD) vs Geant4 example microtrack (testMT/g4dna/mt):
  BUGS fixed in MUTTrackerSD: hit selection region derived from the world HALF lengths with formulas for full lengths
  (100 um x 4 um -> 48 um x 50 um); now HITSELREG_XY/HITSELREG_Z read from the geometry (as SITE_RADIUS);
  specific energy histograms (31-33) filled with log10(y) instead of log10(z).
  microtrack.in aligned with the Geant4 run.mac: 20.13 MeV protons from Z0=-50.99 um (was 20 MeV at -3.01 um, no
  electron equilibrium), applyCuts e-/gamma; MicroTrack:SiteRadius was not used (SITE_RADIUS of the geometry is).
  10 x 100 ev: yF Geant4 2.30+-0.12, GAMOS 2.19+-0.13, MT 2.20+-0.07; yD 3.67+-0.14 / 3.75+-0.11 / 3.43+-0.07;
  eps1F 1.54+-0.08 / 1.46+-0.08 / 1.47+-0.05 keV (GAMOS values from the histogram sums: yF=sum(yfy)/sum(fy), ...).
- dnaphysics vs Geant4 example (testMT/g4dna/dp; 100 keV p, 100 um water, DNA_Opt2, cut 1 um, 10 x 100 ev):
  number of interactions per process for e-, p, H (14 processes) agree Geant4 / GAMOS / GAMOS MT within 1.5 sigma
  (largest 2.6 sigma, H ionisation MT). Note: the GAMOS example macro uses its DNAPPhysicsList with DNA_Opt0,
  the Geant4 example DNA_Opt2 + raddecay.
- dnadamage1 vs Geant4 example (testMT/g4dna/dd, 5 keV p, 10 jobs x 5 ev):
  * examples/DNA/dnadamage1/VoxelStraight.fab2g4dna is NOT the Geant4 file: 50 nm voxel, 24 nucleosomes, 4860 bp
    (the Geant4 example downloads a 40 nm voxel, 18 nucleosomes, 3640 bp). With the GAMOS file BOTH Geant4 and GAMOS
    fail in the navigation of the chemistry stage (G4SubtractionSolid "solidCut and solid_box_for_cut", NaN
    positions): millions of warnings, GB of output per job (it filled the disk: jobs killed, logs emptied).
    With the official file no NaN in Geant4 (seq and MT).
  * BUG fixed (both trees): GmDNATimeStepAction default damaging radical "OH" was resolved by GetG4Particle to the
    hydroxide ion OH^-1 (named "OH"), not the hydroxyl radical °OH (G4OH): NO damaging reaction was ever written
    (DamagingChemicalReactions file empty). Now "OH"/"°OH" -> G4OH::Definition(), as in the Geant4 example.
  * Comparison with the official geometry, 5 keV p, same physics (G4EmDNAPhysics and option2 give identical results
    here), damage = Damaged_Deoxyribose by °OH as Geant4; 20 jobs x 5 ev each (per job): energy in DNA Geant4 583+-45,
    GAMOS 644+-65 eV; damaging reactions 1.1+-0.3 / 0.9+-0.2; deposits per volume within 2.1 sigma, but GAMOS higher in
    the 4 backbone volumes (e.g. phosphate1 21.5+-2.6 vs 14.6+-1.9) and lower in histone (76+-15 vs 115+-25):
    not significant, to be checked with higher statistics (GmDNAGeometry is a port of the Geant4 DNAParser).
  * BUG fixed (both trees): GmDNASteppingAction wrote the copy number of the POST-step volume (next volume for steps
    limited by a boundary) while the volume type is the pre-step one; now pre-step copy number, as in Geant4.

## Session 6 (2026-11-19)
- examples/DNA/dnadamage1/VoxelStraight.fab2g4dna replaced by the file of the Geant4 example (40 nm voxel), which
  CMake downloads at configuration from https://cern.ch/geant4-data/examples/dna/dnadamage1/VoxelStraight.fab2g4dna
  (it is not in the Geant4 release; MD5 95fbae36cae954bbae7bc3b6aef491e0, while the CMakeLists.txt quotes
  3e28151dc4c4647af3ae37d0385fc443 without checking it). Old file kept as VoxelStraight.fab2g4dna.GAMOS50nm.
- examples/DNA/dnaphysics/dnaphysics.in: /process/em/AddDNARegion World DNA_Opt2 (was DNA_Opt0).
- Users Guide: new chapter /datan1/gamos/doc/v.7.0-sphinx/source/Multithreading/Multithreading.rst (after
  OptimisingCPU in GamosUsersGuide.rst). Sphinx -W and pdflatex build OK (chapter 28).
- MT build logs moved from GAMOS.7.0.1/ to GAMOS.7.0.1.MT/buildlogs/ (they were written through the source symlink).

## Session 7 (2026-11-20): CMake build (Linux tested, Windows prepared)
- The CMake build of GAMOS (GAMOS/CMakeLists.txt, GAMOS/cmake/Modules, sources.cmake in each module) works for the
  sequential tree and now for the MT tree. GAMOS.7.0.1.MT is the new reference: its CMakeLists.txt and cmake/ are
  real copies (2026-11-20; they were symlinks to GAMOS.7.0.1/GAMOS); edit them there from now on. Tested: sequential (build tree and install tree) test2.in dose 4.66092e-12 = GNUmake;
  MT 4 threads 4.6671e-12 = GNUmake MT. Build dirs used for the tests: scratchpad/cmake_seq, cmake_mt.
- Fixes for CMake:
  * gamos.sh/csh/bat: Geant4 data variables from the Geant4 data set list (if not set), or from the new cache
    variable GAMOS_GEANT4_DATA_DIR; extra data sets of confgamos.sh (TENDL, NuDEX, URRPT) if found there.
  * GmGenUtils::ReadUnits (both trees): CLHEP SystemOfUnits.h also searched in the installed Geant4 headers
    (GAMOS_G4_INCLUDE_DIR, defined by CMake) and in $G4INSTALL/include; G4INSTALL undefined crashed GAMOS.
  * GmWorkerTaskRunManager.hh (MT): includes G4Types.hh before testing G4MULTITHREADED (with CMake it comes from
    G4GlobalConfig.hh, not from the compiler flags): the class was compiled out (undefined vtable).
  * LD_LIBRARY_PATH: Geant4 libraries use RUNPATH (searched after LD_LIBRARY_PATH); another Geant4 in LD_LIBRARY_PATH
    (e.g. 11.01 in the login environment) mixed versions: gamos.sh and the plugin listing put the Geant4 lib dir first.
  * SealPluginDump linked with NO_AS_NEEDED (both trees): with an MT Geant4 (initial-exec TLS) the Geant4 libraries
    could not be dlopen-ed ("cannot allocate memory in static TLS block") and plugin.lis was empty.
- Windows (NOT TESTED, no Windows machine): CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS, MSVC definitions (WIN32,
  _USE_MATH_DEFINES, NOMINMAX, _CRT_SECURE_NO_WARNINGS), DLLs in bin (RUNTIME) and import libraries in lib
  (ARCHIVE), fixed output dirs for Visual Studio ($<0:>), .reg files pointing to the DLLs, gamos.bat (build and
  install trees). SEAL has Windows code (LoadLibrary/GetProcAddress); GAMOS_SEARCH_PATH uses ';' on Windows.
  Expected risk: static data used across DLLs (not exported by WINDOWS_EXPORT_ALL_SYMBOLS).
- Users Guide (GettingStarted, Compiling GAMOS with CMake): GAMOS_GEANT4_DATA_DIR, multithreading, Windows section.
- The GNUmake builds are kept and still work (both trees rebuilt and tested).

## Final tutorial sweep (2026-11-19, testMT/mt_final.txt vs seq.txt, 177 macros, 4 threads)
- MT fails where sequential works only for: PlugInTutorial (user plug-ins not compiled for the MT tree) and the 8 macros
  using GmGeneratorChangeEnergyAndMaterial (refused in MT on purpose).
- Several MT runs finish where sequential hit the 600 s limit (e.g. RT exercise4b.UBS 444 s, PT exercise4d 154 s,
  GammaSpectrometry exercise2e 525 s); RT exercise4b.EWBS 129 s (MT) vs 531 s (seq).
- Only crashes/exceptions were compared for the whole sweep; numerical comparison done for: test1/test2 (dose),
  RT exercise2b/3b (phase space), PET exercise3a (histograms, sinogram), Primer 0a (process counts).

## Next steps
1. GmGeneratorChangeEnergyAndMaterial / GmVChangeWorldAndSource: NOT to be done (decision 2026-11-19): they stay
   sequential only (FatalException in MT). EGS and Penelope did not need MT either (MT support was already done).
2. DONE (session 3): EGS and Penelope readers shared by threads; EGS first-record bug fixed (see Session 3).
3. DONE (session 3): QA seq vs MT for the 74 tutorial QA exercises (testMT/qa). To redo after syncing the Geant4
   ParticleHP changes into geant4.11.4.p03.gamos.MT: Shielding exercise5.
4. DONE (session 3): end-of-run summaries merged into the master for all user actions used in the tutorials/examples
   (see Session 3); left per thread only GmProdCutsStudyUA, GmMinRangeLimitsStudyUA, SHGetHadronicChannelXSUA.
5. DONE: analysis.MT/pythonUtilities/mergeThreadFiles.py; GmDataTTreeUA per-thread ROOT files (hadd).
6. PlugInTutorial: user plug-ins must be built with the MT configuration (source /datan1/gamos/GAMOS.7.0.1.MT/config/
   confgamos.sh, G4MULTITHREADED=1), otherwise the plug-in library (built against the sequential Geant4/GAMOS) fails to
   load or crashes. User actions must follow the MT rules: thread-local data, merge to master (GmUserAction::
   GetMasterUserAction + SetRunOnMaster), per-thread file names (GmMTMergeMgr::GetThreadFileName).

## Known limitations / TODO
- UAs that print text summaries at EndOfRunAction print one per worker thread (not merged) unless they implement merging.
- Output files written by worker UAs (phase space, TTrees, PET/SPECT/CC data, hits, ...) need GmMTMergeMgr::GetThreadFileName.
- GmMovementMgr (geometry movement between events) cannot work in MT (geometry is shared).
- CLHEP theNCalls counter (GAMOS patch) not thread safe.
- /gamos/setParam values are read by workers at the end (all master commands are executed before workers replay).

