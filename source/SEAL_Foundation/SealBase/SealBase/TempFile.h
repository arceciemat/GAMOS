#ifndef SEAL_BASE_TEMP_FILE_H
# define SEAL_BASE_TEMP_FILE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "SealBase/Filename.h"
# include "SealBase/File.h"

namespace seal {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Temporary file name handling.  */
class TempFile
{
public:
    // implicit copy constructor
    // implicit destructor
    // implicit assignment operator

    static Filename	tempdir (void);
    static File *	file (Filename &name);
    static void		dir (Filename &name);

private:
    // Implementation-specific functions
    static bool		tryTempDir (Filename &name, Filename candidate);

    static bool		domktemp (char *path, IOFD *fd, bool dodir, int suffixlen);
    static char *	domkdtemp (char *path);
    static char *	domktemp (char *path);
    static IOFD		domkstemp (char *path);
    static IOFD		domkstemps (char *path, int suffixlen);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace seal
#endif // SEAL_BASE_TEMP_FILE_H
