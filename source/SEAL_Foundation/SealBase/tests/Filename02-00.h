#include "SealBase/Filename.h"
#include "SealBase/FileNameLock.h"
#include "SealBase/ShellEnvironment.h"
#include "SealBase/Signal.h"
#include "SealBase/DebugAids.h"
#include <iostream>
#include <iomanip>


namespace seal_test { 
  extern std::ostream & out;
}



using namespace seal;

inline void
dump (const char *scope, const Filename &x)
{
    Filename target;
    seal_test::out << "### " << scope << ": " << x
	      << "\nisAbsolute?     = " << x.isAbsolute ()
	      << "\nisRelative?     = " << x.isRelative ()
	      << "\n"

	      << "\nexists?         = " << x.exists ()
	      << "\nisDirectory?    = " << x.isDirectory ()
	      << "\nisRegular?      = " << x.isRegular ()
	      << "\nisSymlink?      = " << x.isSymlink (&target)
	      << "\n  target        = " << target
	      << "\nisAccessibeDir? = " << x.isAccessibleDirectory ()
	      << "\nisOwnershipPr?  = " << x.isOwnershipPreserved ()
	      << "\nisReadable?     = " << x.isReadable ()
	      << "\nisWritable?     = " << x.isWritable ()
	      << "\nisExecutable?   = " << x.isExecutable ()
	      << "\n"

	// FIXME: status, setStatus, times

	      << "\nasDirectory     = " << x.asDirectory ()
	      << "\nasFile          = " << x.asFile ()
	      << "\n"

	// << "\ndrive           = " << x.drive ()
	// << "\nshare           = " << x.share ()
	      << "\ndirectory       = " << x.directory ()
	      << "\nnondirectory    = " << x.nondirectory ()
	      << "\nextension       = " << x.extension ()
	      << "\n.extension      = " << x.extension (0, true)
	      << "\nsansExtension   = " << x.sansExtension ()
	      << "\n"

	// FIXME: append
	
	      << "\ntruename        = " << x.truename ()
	      << "\nchaseLinks      = " << x.chaseLinks ()
	      << "\n"

	      << "\nautosaveName    = " << x.autosaveName ()
	      << "\nisAutosaveName? = " << x.isAutosaveName ()
	      << "\n"

	// FIXME: findBackupName
	      << "\nbackupName      = " << x.backupName ()
	      << "\nisBackupName?   = " << x.isBackupName ()
	      << "\nsansVersions    = " << x.sansVersions ()
	      << "\n"

	      << "\nexpand          = " << x.expand ()
	      << "\nabbreviate      = " << x.abbreviate ()

	      << "\n";
}

inline void
describe (bool locked, FileNameLock *lock)
{
    seal_test::out << "\nisLocked?       = ";

    if (locked)
	if (! lock)
	    seal_test::out << "by myself";
	else
	    seal_test::out << "user = " << lock->user ()
		      << ", host = " << lock->host ()
		      << ", pid = " << lock->pid ()
		      << ", time = " << lock->time ()
		      << std::endl;
    else
	seal_test::out << "no\n";
}

inline void
dumpl (const char *scope, const Filename &x)
{
    dump (scope, x);

    if (! x.isDirectory ())
    {
	FileNameLock	*l = 0;
	bool		locked;

	locked = FileNameLock::isLocked (x, &l);
	describe (locked, l);
	delete l;
	l = 0;

	FileNameLock lock;

	lock.lock (x);
	locked = FileNameLock::isLocked (x, &l);
	describe (locked, l);
	delete l;
	l = 0;

	lock.unlock ();
	locked = FileNameLock::isLocked (x, &l);
	describe (locked, l);
	delete l;
	l = 0;
    }
}
