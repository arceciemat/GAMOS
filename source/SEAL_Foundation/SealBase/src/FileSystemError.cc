//<<<<<< INCLUDES                                                       >>>>>>

#include "SealBase/FileSystemError.h"

namespace seal {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

FileSystemError::FileSystemError (const char *context, int code /* = 0 */)
    : IOError (context, code)
{}

FileSystemError::FileSystemError (const char *context, Error *chain)
    : IOError (context, chain)
{}

FileSystemError::FileSystemError (Error *chain)
    : IOError (chain)
{}

std::string
FileSystemError::explainSelf (void) const
{ return IOError::doexplain ("File system operation"); }

Error *
FileSystemError::clone (void) const
{ return new FileSystemError (*this); }

void
FileSystemError::rethrow (void)
{ throw *this; }

} // namespace seal
