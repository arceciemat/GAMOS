//<<<<<< INCLUDES                                                       >>>>>>

#include "PluginManager/ModuleCache.h"
#include "PluginManager/ModuleDescriptor.h"
#include "PluginManager/ModuleReg.h"
#include "PluginManager/Module.h"
#include "PluginManager/PluginManager.h"
#include "PluginManager/PluginError.h"
#include "PluginParserError.h"
#include "debug.h"
#include "SealBase/DebugAids.h"
#include "SealBase/Log.h"
#include "SealBase/DirIterator.h"
#include "SealBase/SharedLibrary.h"
#include "SealBase/StringFormat.h"
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cerrno>
#include <sys/stat.h>
#include <sstream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstring>

namespace seal {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>

static const char SAFE [] = { "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			      "abcdefghijklmnopqrstuvwxyz"
			      "01234567890.,/_-" };
static const char HEX [] = { "0123456789abcdef" };

//<<<<<< PRIVATE TYPES                                                  >>>>>>

class ModuleCache::FileByName
{
    Filename m_name;
public:
    FileByName (const Filename &name) : m_name (name) {}
    bool operator() (const ModuleReg *info) { return info->file() == m_name; }
};

//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

const char		ModuleCache::s_cacheFile [] = ".cache";
const char		ModuleCache::s_cacheTag  [] = "cache";

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>

/** Encode string @a input for registry.  Replaces all unsafe characters
    with "%XX" where "XX" is the hex of the character numeric value.  */
static std::string
encode (const std::string &input)
{
    std::string result;
    result.reserve (input.size ());
    for (const char *s = input.c_str (); *s; ++s)
	if (strchr (SAFE, *s))
	    result += *s;
	else
	{
	    result += '%';
	    result += HEX [((unsigned char) *s >> 4) & 0xf];
	    result += HEX [((unsigned char) *s     ) & 0xf];
	}
    return result;
}

/** Decode registry string @a input.  Replaces all occurances of "%XX"
    with the character of interpreting the hex value "XX", and at the
    same time checks only safe characters are used in @a input.  */
static std::string
decode (const std::string &input)
{
    std::string result;
    result.reserve (input.size ());
    for (const char *s = input.c_str (); *s; ++s)
      if (strchr (SAFE, *s)) 
	{
	result += *s;
	} 
      else if (*s == '+' ) 
	{
	  result += *s;
	}	
      else if (*s == '%' && isxdigit (s[1]) && isxdigit (s[2]))
	{
	    result += (char) (((strchr (HEX, tolower (s[1])) - HEX) << 4)
			      + ((strchr (HEX, tolower (s[2])) - HEX)));
	    s += 2;
	}
	else
	    throw PluginError (new PluginParserError ("bad character"));

    return result;
}


//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Initialise a module cache in @a directory.  Initialises the cache
    to clear state and then invokes #refresh() to rebuild; this causes
    for instance #Module objects to be created.  */
ModuleCache::ModuleCache (PluginManager *manager, const Filename &directory)
    : m_manager (manager),
      m_directory (directory.asDirectory ())
{
    ASSERT (! m_directory.empty ());

    m_state.status = CACHE_MISSING;
    m_state.added  = m_state.removed
		   = m_state.changed
		   = m_state.skipped
		   = false;

    m_parse.file   = 0;
    m_parse.module = 0;
    refresh ();
}

/** Destroy the module cache.  Destroys all modules owned by the cache.  */
ModuleCache::~ModuleCache (void)
{
    LOG (0, trace, LFplugin_manager,
         "destroying module cache for `" << m_directory << "'\n" << indent);
    ASSERT (! m_directory.empty ());
    for (RegIterator r = m_cache.begin (); r != m_cache.end (); ++r)
	delete *r;

    LOG (0, trace, LFplugin_manager, undent);
}

/** Return the module cache directory name.  */
Filename
ModuleCache::directory (void) const
{ return m_directory; }

/** Return an iterator to the first #Module in the cache.  */
ModuleCache::Iterator
ModuleCache::begin (void) const
{ return m_modules.begin (); }

/** Return an iterator one past the last #Module in the cache.  */
ModuleCache::Iterator
ModuleCache::end (void) const
{ return m_modules.end (); }

/** Refresh the module cache.  This causes the old ".cache" file to be
    loaded and to be compared with the current state of the directory.
    If there are any differences, a new cache will be written out.  */
void
ModuleCache::refresh (void)
{
    load ();
    rebuild ();
    notify ();
    update ();
}

/** Parse a module cache file (".cache" or one of the "*.reg" files).  This
    both reads the file and reconstructs its contents as children of @a root;
    the descriptor tree is destroyed before the method returns.  If reading
    in ".cache", @a root should be null, if reading in a single registration
    it should be contain the initial cache structure, otherwise reconstruct
    will fail.  The reconstructed cache will be put into #m_parse.result.
    Returns @c true if the cache was successfully read and reconstructed,
    @c false otherwise.  */
bool
ModuleCache::parse (const Filename &file, ModuleDescriptor *root /* = 0 */)
{
    // FIXME: Use platform independent facilities for all file stuff (actually, registry)
    struct stat		statbuf;
    Filename		filename (m_directory, file);
    bool		errors = false;
    ModuleDescriptor	*regdata = root;

    if (::stat (filename, &statbuf) == -1)
    {
	delete regdata;
	return true;
    }

    LOG (0, trace, LFplugin_manager, "parsing file: " << file << "\n");

    ASSERT (m_parse.result.empty ());
    ASSERT (! m_parse.file);
    ASSERT (! m_parse.module);

    // Parse the registry file
    try {
	// FIXME: use an object so we don't need to fclose() it.
      //      std::cout << " OPEN FILE " << filename << std::endl; //GDEB
      FILE *f = fopen (filename, "r");
	if (! f)
	    throw PluginError (new PluginParserError ("can't read the file"));

	typedef std::vector<ModuleDescriptor *> ContextStack;
	ContextStack	stack;
	int		offset = 0;
	int		ch;

	// Bias the stack in case we were give a root preamble.  In
	// that case we simply descend as far as we can into the first
	// children of the preamble, and insert parse results there.
	// `regdata' is assinged
	while (root)
	{
	    ++offset;
	    stack.push_back (root);
	    root = root->children () ? root->child (0) : 0;
	}

	// Process the file
	do {
	    std::string	token;
	    int		level = offset;

	    //	    while ((ch = fgetc (f)) != EOF && ch == ' ')
	    while ((ch = fgetc (f)) != -1 && ch == ' ')
		++level;

	    //	    if (feofch == EOF)
	    if (feof(f))
		break;

	    if (level == 0 && regdata)
	    {
		fclose (f);
		throw PluginError (new PluginParserError ("duplicate top level line"));
	    }

	    stack.resize (level+1);
	    ModuleDescriptor *doc
		= new ModuleDescriptor (level ? stack [level-1] : 0);
	    stack [level] = doc;
	    if (level == 0)
		regdata = doc;

	    for ( ; ch != EOF; ch = fgetc (f))
		if (ch == '\n')
		{
		    if (! token.empty ())
			doc->addToken (decode (token));

		    if (doc->tokens () == 0)
		    {
			fclose (f);
			throw PluginError (new PluginParserError ("empty line"));
		    }

		    break;
		}
		else if (ch == ' ')
		{
		    if (! token.empty ())
			doc->addToken (decode (token));
		    token = "";
		}
		else
		    token += ch;
	} while (ch != EOF);

	fclose (f);
	reconstruct (regdata);

	ASSERT (! m_parse.file);
	ASSERT (! m_parse.module);
    } catch (Error &e) {
	m_manager->feedback (PluginManager::ErrorBadCacheFile, file, &e);

	// FIXME: Notify owner
	LOG (0, error, LFplugin_manager,
	     file << ": error, file ignored (" << e.explain() << ")\n");

	errors = true;
    } catch (...) {
	m_manager->feedback (PluginManager::Other, file);

	// FIXME: Notify owner
	LOG (0, error, LFplugin_manager,
	     file << ": unknown error, file ignored\n");

	errors = true;
    }

    delete regdata;

    if (errors)
    {
	delete m_parse.file;
	m_parse.file = 0;

	delete m_parse.module;
	m_parse.module = 0;

	for (RegIterator r = m_parse.result.begin ();
	     r != m_parse.result.end (); ++r)
	    delete *r;

	m_parse.result.clear ();
    }

    return !errors;
}

/** Load the module cache file ".cache".  Attempts to load and rebuild
    the cache from the file.  The cache is first set to #CACHE_MISSING
    status and all #m_state flags are cleared.  If the file exists and
    can be parsed and reconstructed successfully, #m_cache is set to
    the registrations (of #ModuleReg) and the cache status is set to
    #CACHE_OLD.  Otherwise #m_cache is left untouched.  */
void
ModuleCache::load (void)
{
    LOG (0, trace, LFplugin_manager, "loading cache "
	 << Filename (m_directory, s_cacheFile) << '\n' << indent);

    m_state.status = CACHE_MISSING;
    m_state.added  = m_state.removed
		   = m_state.changed
		   = m_state.skipped
		   = false;

      //GAMOS
    /*    char* path = getenv( "GAMOS_SEAL_NOT_CACHE" );
    if( !path || *path == '0' ) 
    { */
    m_state.status = CACHE_OLD;
    
    // Clear the cache before adding to it (in case we already
    // have something, i.e. refresh() was already invoked).
    for (RegIterator r = m_cache.begin (); r != m_cache.end (); ++r)
      delete *r;
    
    m_cache.clear ();
    m_cache.splice (m_cache.end (), m_parse.result);
    LOG (0, trace, LFplugin_manager, undent << "ModuleCache load done\n");
    //    LOG (0, trace, LFplugin_manager, undent << "PATH " << *path << "ModuleCache load done\n");
	//  }
}

/** Construct a module descriptor for a bad @a file with timestamp @a time.  */
ModuleDescriptor *
ModuleCache::makeBad (const std::string &file, const std::string &time)
{
    PluginManager::get ()->feedback (PluginManager::ErrorBadCacheFile, file);
    ModuleDescriptor *top = new ModuleDescriptor (0, s_cacheTag);
    new ModuleDescriptor (top, ModuleReg::tag (), file, time, "0");
    return top;
}

/** Actually scan the module directory, compare it with the current
    #m_cache, and build a new @a cache that reflects the changes.
    Returns a cache status code to reflect the new state.  */
ModuleCache::CacheStatus
ModuleCache::scanModules (RegList &cache)
{
    LOG (0, trace, LFplugin_manager,
	 "scanning `" << m_directory << "'\n" << indent);

    CacheStatus	result = CACHE_MISSING;
    DirIterator	file (m_directory);
    DirIterator	end;

    ASSERT (cache.empty ());
    for (; file != end; ++file)
    {
	// Ignore the cache file.
	if (*file == s_cacheFile)
	    continue;

	// Check, read and validate the module definition file.
	// FIXME: Use platform-independent facilities!
	struct stat	statbuf;
	Filename	filename (m_directory, *file);
	int		status = stat (filename, &statbuf);

	// Ignore files we cannot stat
	if (status == -1)
	{
	    LOG (0, error, LFplugin_manager,
		 "stat() failed with error " << errno << ", ignoring `"
		 << *file << "'\n");
	    m_state.skipped = true;
	    continue;
	}

	// Check whether the module reg fragment has changed by
	// comparing the file and cached timestamps.
	RegIterator match
	    = std::find_if (m_cache.begin(), m_cache.end(), FileByName (*file));

	if (match == m_cache.end ())
	{
	    LOG (0, trace, LFplugin_manager,
		 "==> new module definition `" << *file << "'\n");
	    m_state.added = true;
	}
	else if ((*match)->time () != (unsigned long) statbuf.st_mtime)
	{
	    LOG (0, trace, LFplugin_manager,
		 "==> modified module definition `" << *file << "' (file is "
		 << statbuf.st_mtime << ", cached was " << (*match)->time ()
		 << ")\n");
	    m_state.changed = true;
	}
	else
	{
	    // Copy the old cache to the new cache.  This relies on
	    // the knowledge that the old cache will be thrown away
	    // and so it is ok to steal the entry from the old one to
	    // the new one.
	    LOG (0, trace, LFplugin_manager,
		 "==> unchanged definition `" << *file << "'\n");
	    cache.splice (cache.end (), m_cache, match);
	    continue;
	}

	// Construct a fake registry document root for this file
	std::string time = StringFormat ("%1").arg (statbuf.st_mtime);
	ModuleDescriptor *top = new ModuleDescriptor (0, s_cacheTag);
	new ModuleDescriptor (top, ModuleReg::tag (), *file, time, "1");

	// Mark bad files we cannot `parse' (errors are already logged).
	// We already tried to parse the file itself, now make a dummy
	// proxy that marks the file bad and forget about the contents.
	if (! parse (*file, top) || m_parse.result.empty ())
	{
	    reconstruct (makeBad (*file, time));
	    m_state.skipped = true;
	}

	// Try loading the modules to check that they are well-formed:
	// ensure that each module loads and resolves the required
	// entry points.  If this fails, mark the whole registration
	// file bad so that it will be ignored until the time-stamp
	// changes.  This leaves the cache always in valid state,
	// remembering anything bad.
	ASSERT (m_parse.result.size () <= 1);
	for (RegIterator r = m_parse.result.begin () ; r != m_parse.result.end (); ++r)
	    for (ModuleReg::Iterator m = (*r)->begin (); m != (*r)->end (); ++m)
		try
		{
		    (*m)->load (); // checks for entry points
		    (*m)->query ();
		    (*m)->unload ();
		}
		catch (Error &e)
		{
		    m_manager->feedback (PluginManager::ErrorBadModule,
			 		 (*m)->libraryName (), &e);

		    // FIXME: Notify to owner
		    LOG (0, error, LFplugin_manager,
			 *file << ": bad module library `"
			 << (*m)->libraryName ()
			 << "', module ignored\n (error was: "
			 << e.explain () << ")\n");
		    m_state.skipped = true;
		    (*r)->bad (true);
		    break;
		}

	// Transfer into cache.
	cache.splice (cache.end (), m_parse.result);
    }

    // See if cache contains info on removed modules.
    for (RegIterator r = m_cache.begin (); r != m_cache.end (); ++r)
	if (std::find_if (cache.begin (), cache.end (),
			  FileByName ((*r)->file ())) == cache.end ())
	{
	    LOG (0, trace, LFplugin_manager,
		 "==> removed module `" << (*r)->file () << "'\n");
	    m_state.removed = true;
	}

    // Update status codes.  Note that CACHE_VALID means .cache is
    // valid, CACHE_OLD means it needs updating.
    result = (m_state.added || m_state.removed || m_state.changed
	      || m_state.skipped) ? CACHE_OLD : CACHE_VALID;

    LOG (0, trace, LFplugin_manager,
	 undent << "done (" << (int) result << ")\n");
    return result;
}

/** Rebuild the cache comparing it to the actual directory contents.
    Assumes any existing cache is already loaded.  Scans the directory
    contents and compares with the cache, updating it to the new state
    of things.  Updates #m_state to reflect what happened.  */
void
ModuleCache::rebuild (void)
{
  LOG (0, trace, LFplugin_manager,
	 "rebuilding cache in `" << m_directory << "'\n" << indent);

    // Build a new cache and stealing bits from the old one as the new
    // one is rebuilt.  Then replace the cache with the new one and
    // delete whatever was left over from the old cache (creating the
    // new one may steal bits of the old).
    RegList cache;
    m_state.status = scanModules (cache);
    m_cache.swap (cache);
    for (RegIterator r = cache.begin (); r != cache.end (); ++r)
	delete *r;

    LOG (0, trace, LFplugin_manager,
	 undent << "done (" << (int) m_state.status
	 << ", added = " << m_state.added
	 << ", removed = " << m_state.removed
	 << ", changed = " << m_state.changed
	 << ", skipped = " << m_state.skipped
	 << ")\n");
  
}

/** Update #m_modules from the current state of the cache (#m_cache).  */
void
ModuleCache::notify (void)
{
    LOG (0, trace, LFplugin_manager, "collecting modules\n" << indent);
    for (RegIterator r = m_cache.begin (); r != m_cache.end (); ++r)
	if (! (*r)->bad ())
	    for (ModuleReg::Iterator p = (*r)->begin (); p != (*r)->end (); ++p)
		m_modules.push_back (*p);
    LOG (0, trace, LFplugin_manager, undent);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Dump the module descriptor @a doc and all it's children into the
    stream @a s.  Output should happen at nesting @a level; omit this
    argument on the first call, it is meant to be used only in the
    internal implementation.  */
void
ModuleCache::write (std::ostream &s, ModuleDescriptor *doc, int level/*=0*/)
{
    s << std::string (level, ' ') << encode (doc->token (0));
    for (unsigned i = 1; i < doc->tokens (); ++i)
	s << ' ' << encode (doc->token (i));
    s << std::endl;

    for (unsigned i = 0; i < doc->children (); ++i)
	write (s, doc->child (i), level+1);
}

/** Update the ".cache" file on the disk if it is writable.  */
void
ModuleCache::update (void)
{
    if (m_state.status == CACHE_VALID)
	return;

    // Regenerate cache file from information we have in memory
    LOG (0, trace, LFplugin_manager,
	 "updating cache in `" << m_directory << "'\n" << indent);

    // Build a descriptor tree and dump it out
    ModuleDescriptor *all = new ModuleDescriptor (0, s_cacheTag);
    for (RegIterator r = m_cache.begin (); r != m_cache.end (); ++r)
	(*r)->cache (all);

    std::ostringstream buf;
    write (buf, all);
    delete all;

    Filename	filename (m_directory, s_cacheFile);
    FILE	*file = STDC::fopen (filename, "wb");

    if (! file)
    {
	LOG (0, error, LFplugin_manager,
	     filename << ": error " << errno << ", unable to open the cache"
	     " for writing -- cache not updated\n" << undent);
	return;
    }

    std::string data (buf.str ());
    if (STDC::fwrite (data.c_str (), 1, data.size (), file) != data.size ())
    {
	LOG (0, error, LFplugin_manager,
	     filename << ": error " << errno << " while writing cache out"
	     " -- forcing file truncation\n");

	file = STDC::freopen (filename, "wb", file);
    }
    else
	m_state.status = CACHE_VALID;

    STDC::fclose (file);

    LOG (0, trace, LFplugin_manager, undent << "done\n");
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Validate and reconstruct descriptor proxies from @a info.  */
void
ModuleCache::reconstruct (ModuleDescriptor *info)
{
    // Protect against random junk, e.g. empty files
    if (! info || ! info->tokens ())
	throw PluginError (new PluginParserError ("empty descriptor"));

    // Make sure the root is either a cache...
    if (info->token (0) == s_cacheTag)
    {
	if (info->tokens () != 1)
	    throw PluginError (new PluginParserError
			       ("`cache' does not take arguments"));
	else if (info->parent ())
	    throw PluginError (new PluginParserError
			       ("`cache' must be at the root"));

	for (unsigned i = 0; i < info->children (); ++i)
	    reconstruct (info->child (i));

	ASSERT (! m_parse.file);
	ASSERT (! m_parse.module);
    }
    // ... or a module registration
    else if (info->token (0) == ModuleReg::tag ())
    {
	if (info->tokens () != 4)
	    throw PluginError (new PluginParserError
			       ("`file' must have three arguments"));
	if (! info->parent () || info->parent ()->token (0) != s_cacheTag)
	    throw PluginError (new PluginParserError
			       ("`file' must be under `cache'"));
	if (info->token (3) != "1" && info->token (3) != "0")
	    throw PluginError (new PluginParserError
			       ("`file' last argument must be 1 or 0"));

	ASSERT (! m_parse.file);
	m_parse.file = new ModuleReg (info->token (1),
				      info->token (2),
				      info->token (3));

	for (unsigned i = 0; i < info->children (); ++i)
	    reconstruct (info->child (i));

	ASSERT (m_parse.file);
	ASSERT (! m_parse.module);
	m_parse.result.push_back (m_parse.file);
	m_parse.file = 0;
    }
    // ... or a module itself
    else if (info->token (0) == Module::tag ())
    {
	if (info->tokens () != 2)
	    throw PluginError (new PluginParserError
			       ("`module' must have one argument"));
	if (! info->parent() || info->parent ()->token(0) != ModuleReg::tag())
	    throw PluginError (new PluginParserError
			       ("`module' must be under a `file'"));

	ASSERT (m_parse.file);
	ASSERT (! m_parse.module);
	m_parse.module = new Module (m_manager, info->token (1));

	for (unsigned i = 0; i < info->children (); ++i)
	    reconstruct (info->child (i));

	ASSERT (m_parse.file);
	ASSERT (m_parse.module);
	m_parse.file->add (m_parse.module);
	m_parse.module = 0;
    }
    // ... or junk we don't understand
    else
    {
	if (! m_parse.module)
	    throw PluginError (new PluginParserError
			       (info->token (0) + " must be under a `module'"));

	ASSERT (m_parse.file);
	m_parse.module->restore (info);
    }
}

} // namespace seal
