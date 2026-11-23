#ifndef SEAL_BASE_FILE_LOCK_H
# define SEAL_BASE_FILE_LOCK_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "SealBase/sysapi/IOTypes.h"

namespace seal {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class File;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class FileLock
{
public:
    FileLock (void);
    FileLock (File *file, IOOffset pos, IOOffset len,
	      bool write = true, bool wait = false);
    ~FileLock (void);

    void		lock (File *file,
			      IOOffset pos,
			      IOOffset length,
			      bool write = true,
			      bool wait = false);
    void		unlock (void);
    static bool		locked (File *file,
				IOOffset pos,
				IOOffset length,
				bool write = true);

private:
    static void		unlock (File *file, IOOffset pos, IOOffset length);

    File		*m_file;
    IOOffset		m_offset;
    IOOffset		m_length;

    // undefined, no semantics (yet?)
    FileLock (const FileLock &);
    FileLock &operator= (const FileLock &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace seal
#endif // SEAL_BASE_FILE_LOCK_H
