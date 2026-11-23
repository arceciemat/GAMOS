#ifndef GmMTMergeMgr_h
#define GmMTMergeMgr_h

// GAMOS multithreading utilities
//  - registry of functions that merge the results of a worker thread into the master objects;
//    they are called at the end of each run in each worker thread, one thread at a time
//    (after the EndOfRunAction of the worker user actions and before the master EndOfRunAction)
//  - helpers to know the thread and to build per-thread file names

#include "globals.hh"
#include "G4Threading.hh"
#include <functional>
#include <map>

class GmMTMergeMgr
{
public:
  // Register a merger (only the first registration of a given name is kept)
  static void RegisterMerger( const G4String& name, std::function<void()> merger );
  // Called in each worker thread at end of run
  static void MergeAllToMaster();

  // Register an action to be executed in the master thread at end of run, after all workers have merged
  // (e.g. writing files with the merged results). Only first registration of a given name is kept;
  // the action is executed at the end of each run while it remains registered
  static void RegisterMasterEndOfRunAction( const G4String& name, std::function<void()> action );
  // Called by GmUserRunActionList::EndOfRunAction in the master thread
  static void RunMasterEndOfRunActions();

  // true if running with G4MTRunManager/G4TaskRunManager and this is a worker thread
  static G4bool IsWorkerThread() {
    return G4Threading::IsMultithreadedApplication() && !G4Threading::IsMasterThread();
  }
  // true if running multithreaded and this is the master thread
  static G4bool IsMTMaster() {
    return G4Threading::IsMultithreadedApplication() && G4Threading::IsMasterThread();
  }
  // In worker threads returns the file name with ".thN" added before the extension
  // (fileName.ext -> fileName.thN.ext); in sequential mode or master thread returns it unchanged
  // The thread file is registered, and at the end of the job (MergeThreadFiles) the files of all threads are merged
  // into fileName, according to the merge mode of the file (default: concatenation in thread order)
  static G4String GetThreadFileName( const G4String& fileName );

  enum ThreadFileMergeMode { TFMConcatenate = 0, // independent records (text lines or fixed-size binary records)
			     TFMSkipHeaderLines,  // text with header lines: only those of the first file are kept
			     TFMEGSPhsp,          // EGS phase space: header counters summed, records appended
			     TFMNoMerge };        // not mergeable by concatenation (e.g. binary header with counters)
  // Set the merge mode of a file given by its (thread) file name, as returned by GetThreadFileName
  static void SetThreadFileMergeMode( const G4String& threadFileName, ThreadFileMergeMode mode, G4int nHeaderLines = 0 );
  // Called by the main program at the end of the job, after the worker threads are deleted (files closed):
  // merges the thread files of each registered file and deletes them (unless bKeepThreadFiles)
  static void MergeThreadFiles( G4bool bKeepThreadFiles = false );

private:
  static std::map<G4String,std::function<void()> >& GetMergers();
  static std::map<G4String,std::function<void()> >& GetMasterEndOfRunActions();
};

#endif
