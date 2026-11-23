//<<<<<< INCLUDES                                                       >>>>>>
#include <iostream>

#include "SealBase/SearchPath.h"
#include "SealBase/StringOps.h"
#include "SealBase/DebugAids.h"
#include "SealBase/Log.h"
#include <algorithm>
#include <iterator>

namespace seal {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>

logstream &operator<< (logstream &out, const SearchPath &path)
{
    if (path.empty ())
	return out;

    SearchPath::const_iterator dir = path.begin ();
    out << *dir;
    while (++dir != path.end ())
	out << SearchPath::separator () << *dir;

    return out;
}

//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Return the platform path separator character.  It is colon (":")
    on unixen and semicolon (";") on Windows.  */
const char *
SearchPath::separator (void)
{
#ifdef _WIN32
    return ";";
#else
    return ":";
#endif
}

/** Construct a new search path from the string @a path.  The string
    is split at #separator() characters, and each name is then pushed
    into this object.  If the @a path is empty, no names are added.
    However empty fields in @a path, produced for instance by leading
    or trailing path separators or several consequtive separators,
    are added to the search list and cause files to be searched in the
    current directory. */
SearchPath::SearchPath (const std::string &path)
{
    if (! path.empty ())
    {
	StringList paths (StringOps::split (path, separator ()));
	//	std::cerr << "!!!! GAMOS ERROR SearchPath::SearchPath " << std::endl;
	//	std::cout << " SearchPath pathsN " << paths.size() << std::endl; //GDEB
	for( size_t ii = 0; ii < paths.size(); ii++ ){
	  //	  std::cout << " SearchPath paths " << paths[ii] << std::endl; GDEB
	}
	//	std::string* dd = 0;
	//	dd->c_str();
	//GAMOS std::copy (paths.begin (), paths.end (), std::back_inserter (*this));
	for( StringList::iterator ite = paths.begin(); ite != paths.end(); ite++ ) {
	  m_list.push_back (*ite);
	}
    }
}

/** Search for a file @a name in the search path starting at position
    @a from onwards.  The @a name must be relative, non-empty filename,
    but may include directory components.  The method finds the first
    directory in which the specified file exists and returns an iterator
    to the directory.  The only check that is made is that the filename
    exists, no check is made to ensure it is accessible, nor is the type
    checked -- so the returned name may well be a directory instead of
    a file.  */
SearchPath::const_iterator
SearchPath::search (const_iterator from, const Filename &name) const
{
    ASSERT (! name.empty ());
    ASSERT (! name.isAbsolute ());
    for (const_iterator last = end ();
	 from != last && ! Filename (*from, name).exists ();
	 ++from)
	;

    return from;
}

/** Expand @a name using the path.  Performs a #search() for @a name,
    and combines the resulting directory with @a name.  If @a name is
    not found in the path, returns @a name.

    Like #search(), this method only checks whether the filename
    exists, no check is made to ensure it is accessible, nor is the
    type checked.  The returned name may therefore well be a directory
    instead of a file.  */
Filename
SearchPath::expand (const Filename &name) const
{
    const_iterator pos = search (name);
    if (pos == end ())
	return name;
    else
	return Filename (*pos, name);
}

} // namespace seal
