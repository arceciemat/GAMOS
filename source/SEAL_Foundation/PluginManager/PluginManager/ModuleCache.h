#ifndef PLUGIN_MANAGER_MODULE_CACHE_H
# define PLUGIN_MANAGER_MODULE_CACHE_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "PluginManager/config.h"
# include "SealBase/Filename.h"
# include <iosfwd>
# include <utility>
# include <string>
# include <list>

namespace seal {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class PluginManager;
class ModuleDescriptor;
class ModuleReg;
class Module;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Automatically cached directory of module registrations.

    The #PluginManager automatically instantiates a #ModuleCache for
    every module registration directory.  The purpose of this class is
    to maintain an automatically updated cache about all the modules
    in the directory, including all the plug-ins registered in those
    modules.  The cache object automatically refreshes the cache with
    respect to the on-disk registrations when it is created.  Clients
    can also force an update by calling #refresh(); note however that
    calling it on already-loaded modules whose library has been
    updated while the program is running is not a good idea.

    The cache maintains information about all the registrations in the
    directory, including the time-stamp of each file.  The cache is
    updated if the file time stamp is different from what it was when
    the data was cached.  The cache also records time-stamps for bad
    files: files the cache does not understand or files that describe
    modules that fail to load for some reason.  Such files are ignored
    until the on-disk file time stamp changes.

    The #ModuleCache updates the on-disk cache automatically if it
    can, i.e. if it has write permissions on the directory.  It never
    modifies the original registration files.  If the cache file
    cannot be written, the errors are silently ignored and class
    proceeds with just the information in memory; the canonical data
    it maintains is the part that is in memory, the data on disk is
    merely an optimisation that avoids having to query all the modules
    all the time.

    Note that if the cache cannot be written but needs to be updated,
    there is a performance penalty because the client programs will
    continuously refresh their caches.  Thus when new modules are
    installed, it is important to refresh the cache before the
    directories are frozen.

    The cache data is maintained in string form as #ModuleDescriptor
    objects.  The #ModuleCache automatically creates #Module objects
    as needed; the mapping from registration files to modules defined
    in them is maintained in #ModuleReg objects.  When restoring a
    cache the information is passed as #ModuleDescriptor to the
    #Module objects, which passes it to the #PluginManager, which
    passes it to the #PluginFactory categories as necessary.  When
    writing the #ModuleReg objects ask the #Module objects, which
    maintain their own caches of the #PluginInfo items registered with
    them, to produce #ModuleDescriptors for output.  */
class ModuleCache
{
    typedef std::list<Module *>			ModuleList;
public:
    /// Iterator over #Module objects in this cache
    typedef ModuleList::const_iterator		Iterator;

    ModuleCache (PluginManager *manager, const Filename &directory);
    ~ModuleCache (void);

    // cache definition
    PluginManager *	manager (void) const;
    Filename		directory (void) const;

    // cache management
    void		refresh (void);

    // module access
    Iterator		begin (void) const;
    Iterator		end (void) const;

protected:
    void		load (void);
    void		rebuild (void);
    void		notify (void);
    void		update (void);

private:
    struct CacheContext;
    struct ParseContext;
    class FileByName;

    friend struct CacheContext;
    friend struct ParseContext;
    friend class FileByName;

    /** Module registrations (each of which maintains its list of modules).  */
    typedef std::list<ModuleReg *>	RegList;
    /** Iterator over module registations.  */
    typedef RegList::iterator		RegIterator;

    /** Different cache states.  The progression goes from first to last.  */
    enum CacheStatus {
	CACHE_MISSING	= 1,		//< No cache
	CACHE_OLD	= 2,		//< Cache exists but is outdated
	CACHE_VALID	= 3		//< Cache exists and is up-to-date
    };

    /** Flags that describe the current cache status.  */
    struct CacheContext {
	CacheStatus	status  : 3;	//< Cache status
	bool		added   : 1;	//< Flag for new modules
	bool		removed : 1;	//< Flag for removed modules
	bool		changed : 1;	//< Flag for changed modules
	bool		skipped : 1;	//< Flag for skipped modules or files
    };

    /** Structure used as temporary storage and result by #parse() and
        #reconstruct().  At the beginning of parse the data members
	need to be clean, output is in #result at the end.  */
    struct ParseContext {
	RegList		result;		//< Reconstructed read result
	ModuleReg	*file;		//< Parse stack: current .reg file
	Module		*module;	//< Parse stack: current module spec
    };

    bool		parse (const Filename &filename,
			       ModuleDescriptor *root = 0);
    static ModuleDescriptor *makeBad (const std::string &file,
		    		      const std::string &time);
    CacheStatus		scanModules (RegList &cache);
    void		reconstruct (ModuleDescriptor *info);
    void		write (std::ostream &s,
			       ModuleDescriptor *doc,
			       int level = 0);

    PluginManager	*m_manager;	//< Manager that owns this cache
    Filename		m_directory;	//< Module directory
    RegList		m_cache;	//< Raw cache contents
    CacheContext	m_state;	//< Cache status flags
    ParseContext	m_parse;	//< Parsing working storage
    ModuleList		m_modules;	//< List of modules in this directory

    static const char	s_cacheFile [];	//< Cache file name
    static const char	s_cacheTag [];	//< Descriptor tag for the cache root
    static const char	s_fileTag [];	//< Descriptor tag for the file tree
    static const char	s_moduleTag [];	//< Descriptor tag for a module tree
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace seal
#endif // PLUGIN_MANAGER_MODULE_CACHE_H
