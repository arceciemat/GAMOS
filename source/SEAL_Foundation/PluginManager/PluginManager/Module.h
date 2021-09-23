#ifndef PLUGIN_MANAGER_PLUGIN_H
# define PLUGIN_MANAGER_PLUGIN_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "PluginManager/config.h"
# include "PluginManager/PluginInfo.h"
# include "PluginManager/ModuleDescriptor.h"
# include "SealBase/Filename.h"
# include <string>
# include <list>

//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

namespace seal {
class Module;
class ModuleDef;
class ModuleCache;
class ModuleReg;
class ModuleDescriptor;
class SharedLibrary;
class PluginManager;

/** The type of the standard entry point function in a module library.  */
extern "C" { typedef ModuleDef *(*ModuleEntry) (void); }

/** The type of the capability entry point function in a module library.  */
extern "C" { typedef void (*ModuleCapEntry) (const char **&names, int &n); }

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Module proxy.

    This class maintains the list of the capabilities provided by a
    plug-in module, but only in a generic manner.  Custom plug-in
    factories provide access to specific information.  The two are
    coordinated so that as new modules enter and leave the system the
    factory views are synchronised, and as new factories enter and
    leave, the information is cached and uncached automatically.

    A #Module exists for each module in an #ModuleCache regardless
    of whether the module library is presently loaded.  The module is
    queried for the list of properties it provides (see #ModuleDef,
    #DEFINE_SEAL_MODULE, #DEFINE_SEAL_PLUGIN).  #ModuleDirectory
    caches the information for later accesses without having to query
    the module. If the module changes, it us automatically queried
    again to update the cache.

    #Module can also treat the program itself as a module through
    the #builtin() method.  This allows dynamic loading to be bypassed
    as long as clients always use custom #PluginManager factories and
    #PluginLoader.  Defining #SEAL_BUILTIN_PLUGINS preprocessor
    macro causes #DEFINE_SEAL_MODULE() and #DEFINE_SEAL_PLUGIN()
    to a variant that automatically does so.

    It is not safe to detach a module if there are any references
    remaining to it.  This means there can be no live objects whose
    implementation for any part, including but not limited to virtual
    function tables, methods, template functions or static data,
    belong to the library detached.  Note: this applies to any object
    created by the library, not just the plug-ins created by it!  */
class Module
{
    typedef std::list<PluginInfo *>	InfoList;
public:
    typedef InfoList::const_iterator	InfoIterator;

    Module (PluginManager *manager, const Filename &libraryName);
    virtual ~Module (void);

    // module management interface
    PluginManager *	manager (void) const;
    const Filename &	libraryName (void) const;

    virtual void	load (void);
    virtual void	unload (void);
    virtual void	attach (void);
    virtual void	detach (void);
    virtual void	query (void);

    virtual bool	attached (void);

    // program built-in module (= program itself)
    static Module *	builtin (void);

    // info access
    InfoIterator	beginInfos (void) const;
    InfoIterator	endInfos (void) const;
    void		addInfo (PluginInfo *info, bool cache);
    void		detachInfo (PluginInfo *info);
    ModuleDescriptor *cacheRoot (void) const;

protected:
    friend class ModuleCache;
    friend class ModuleReg;
    void		makeCache (void);
    void		clearCache (void);
    virtual void	restore (ModuleDescriptor *info);
    virtual void	cache (ModuleDescriptor *to) const;

    virtual bool	checkEntryPoints (void) const;
    ModuleEntry		moduleEntryPoint (void);
    ModuleCapEntry	capabilityEntryPoint (void);

    SharedLibrary *	library (void) const;
    bool		loaded (void) const;

    static const char *	tag (void);

private:
    PluginManager	*m_manager;
    Filename		m_libraryName;
    mutable SharedLibrary *m_library;
    ModuleDescriptor	*m_cache;

    bool		m_attached;	//< Set when library has been attached.
    ModuleDef		*m_definition;	//< My definition proxy.
    InfoList		m_infos;	//< Info items for this module.
    static const char	s_modEntryPoint [];//< Name of the normal library entry point.
    static const char	s_capEntryPoint [];//< Name of the capability library entry point.
    static const char	s_tag [];

    // undefined semantics
    Module (const Module &);
    Module &operator= (const Module &);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

inline const char *
Module::tag (void)
{ return s_tag; }

} // namespace seal
#endif // PLUGIN_MANAGER_PLUGIN_H
