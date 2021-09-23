#ifndef SEAL_BASE_FILE_NAME_LOCK_H
# define SEAL_BASE_FILE_NAME_LOCK_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "SealBase/Filename.h"
# include "SealBase/Time.h"
# include <string>

namespace seal {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class Filename;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class FileNameLock
{
public:
    enum Kind {
	LockNone,
	LockSelf,
	LockOther,
	LockUnknown
    };

    enum Success {
	LockCreated,
	LockExists,
	LockFailure
    };

    class Info
    {
    public:
	Info (void);
	// implicit copy constructor
	// implicit destructor
	// implicit assignment operator

    private:
    };

    FileNameLock (void);
    FileNameLock (const Filename &name);
    ~FileNameLock (void);

    // Locking (see also filename autosave, backups)
    Kind		tryLock (const Filename &name);
    static bool		isLocked (const Filename &name, FileNameLock **lock);

    void		lock (const Filename &name);
    void		unlock (void);

    // Lock details
    bool		valid (void) const;
    const std::string &	user (void) const;
    const std::string &	host (void) const;
    pid_t		pid (void) const;
    Time		time (void) const;

private:
    void		unlock (const Filename &name);
    // void		unlockAll (const Filename &name) const;

    static Filename	lockname (const Filename &name);
    Kind		locker (const Filename &name);
    Success		tryLock (const Filename &name, bool force);

    void		reset (void);

    FileNameLock &	now (void);
    Filename		format (void) const;
    bool		parse (const Filename &from);

    Filename		m_filename;
    std::string		m_user;
    std::string		m_host;
    pid_t		m_pid;
    Time		m_time;

    // undefined, no semantics (yet?)
    FileNameLock (const FileNameLock &);
    FileNameLock &operator= (const FileNameLock &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace seal
#endif // SEAL_BASE_FILE_NAME_LOCK_H
