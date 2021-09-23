//<<<<<< INCLUDES                                                       >>>>>>

#include "PluginManager/PluginManager.h"
#include "PluginManager/PluginInfo.h"
#include "PluginManager/PluginFactoryBase.h"
#include "PluginManager/ModuleCache.h"
#include "PluginManager/Module.h"
#include "debug.h"
#include "PluginManager/ModuleDescriptor.h"
#include "SealBase/SystemError.h"
#include "SealBase/Log.h"
#include <algorithm>
#include <functional>
#include <typeinfo>

namespace seal {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>

/// Locate a #ModuleCache by it's file name.
class DirectoryByName {
    Filename m_which;
public:
    DirectoryByName (Filename name) : m_which (name) {}
    bool operator() (const ModuleCache *dir) {
	return dir->directory () == m_which;
    }
};

/// Simple class to configure optional destruction of plug-in manager on exit.
class PluginManagerDestructor {
    PluginManager	*m_manager;
    bool		m_destroy;

public:
    PluginManagerDestructor (void) : m_manager (0), m_destroy (false) {}
    ~PluginManagerDestructor (void)
    {
	if (m_destroy && m_manager)
	{
	    LOG (0, trace, LFplugin_manager, "destroying plug-in manager\n" << indent);
	    delete m_manager;
	    m_manager = 0;
	    LOG (0, trace, LFplugin_manager, undent);
	}
    }

    void policy (bool destroy) { m_destroy = destroy; }
    void object (PluginManager *manager) { m_manager = manager; }
};

//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>

static PluginManagerDestructor s_destructor;

//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

/** Get the plug-in manager.  This retrieves a pointer to a global
    plug-in manager object.  The manager is automatically created on
    first call to this function, and initialised to use @c SEAL_PLUGINS
    environment variable as the search path.  The manager is not fully
    initialised until someone calls #initialise() method.  That is, it
    is functional but will not scan for plug-ins until #initialise() is
    called.  Otherwise the manager is fully functional.  Once the init
    has been done, the manager also knows about the plug-ins.  */
PluginManager *
PluginManager::get (void)
{
    // FIXME: Thread safety
    static PluginManager           *s_db = 0;
    if (! s_db)
    {
	const char *path = getenv ("SEAL_PLUGINS");
	if (! path) path = "";
	LOG (0, trace, LFplugin_manager,
	     "initialising plugin manager with path <" << path << ">");
	s_db = new PluginManager (SearchPath (path ? path : ""));
	s_destructor.object (s_db);
    }
    return s_db;
}

/** Configure the destruction of the plug-in manager.  By default the
    manager isn't destructed.  This function allows the application to
    set the destruction policy, either to force the destruction or to
    prevent it.  In general, it would be a good idea to destroy the
    manager, but you will want to ensure your compiler and language
    run-times are implementing correct destruction mechanism before
    enabling the functionality.
    
    Note that this method only sets the destruction policy @em if a
    #PluginManager is constructed.  The method itself neither creates
    nor destroys the manager.  */
void
PluginManager::destroyOnExit (bool destroy)
{ s_destructor.policy (destroy); }

/** Construct a plug-in manager using the @a path as the list of module
    definition directories.  This constructor is private; use #get()
    instead.  */

  PluginManager::PluginManager (const SearchPath &path)
    : m_initialised (false),
      m_searchPath (path)
{
  /* //  std::cout << " PluginManager::PluginMa PATH " ; //GDEB
  SearchPath::const_iterator dir = m_searchPath.begin ();
    std::cout << *dir;
      while (++dir != m_searchPath.end ())
      std::cout << " " << SearchPath::separator () << *dir;
    */
  //  std::cout << "ZZ " << std::endl; //GDEB
}

/** Destroy the plug-in manager.  Frees all module caches.  */
PluginManager::~PluginManager (void)
{
    // Zap all the module cache directories
    DirectoryIterator dir = beginDirectories ();
    for ( ; dir != endDirectories (); ++dir)
	delete *dir;

    // Zap any factories that still remain.  The ones we still have must
    // be dynamically allocated and thus delete is safe -- otherwise the
    // global destructors would have already destructed them, causing
    // them to unregistered themselves from me.  (This assumes correct
    // library dependency linkage.)
    while (! m_factories.empty ())
	delete *m_factories.begin ();
}

/** Initialise the plug-in manager.  This causes all the plug-in
    directories determined by #get() (from $SEAL_PLUGINS environment
    variable) to be scanned, and new plug-ins potentially be queried.
    It is safe to call this method several times, only the first time
    counts.  The first call should happen at a time when it is safe
    to load and unload libraries: when the program is in sufficiently
    stable state.  If you don't like spending hours of debugging odd
    bugs or don't want to be vulnerable to strange and fragile error
    conditions, translate that to "never call this method from a
    global constructor, directly or indirectly".

    Causes a #ModuleCache to be created for every directory mentioned
    in the search path.  Directories that don't exist or for which
    caching fails for some other reason are ignored (see logging output
    for details).  */
void
PluginManager::initialise (void)
{
    if (m_initialised) return;
    m_initialised = true;

    SearchPath::const_iterator dir = m_searchPath.begin ();
    for ( ; dir != m_searchPath.end (); ++dir)
    {
	// Ignore directories that do not exist.  Also ignore
	// empty directory components ("foo::bar") as they are
	// unlikely to be intended.  This is unlike normal unix
	// behaviour where "::" really means ":.:", but avoids
	// a common problem from appending to SEAL_PLUGINS as
	// $SEAL_PLUGINS:foo when SEAL_PLUGINS is not set.  If
	// user really wants to have "." in the path -- and that
	// is exceedingly unlikely -- they can always do so
	// explicitly.
	if (! dir->empty ())
        {
	    try {
		m_directories.push_back (new ModuleCache (this, *dir));
	    } catch (Error &error) {
		LOG (0, warning, LFplugin_manager, undent << undent
		     << "directory " << *dir << " ignored: "
		     << typeid(error).name () << " ("
		     << error.explain ().c_str () << ")\n");
		continue;
	    }
	}
    }

    rebuild ();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Route a new plug-in info object to the appropriate factory.  */
void
PluginManager::addInfo (PluginInfo *info)
{
    // This method is called by #Module when it receives a new info
    // object.  We find the factory that matches the info object's
    // category, and then route the call to the factory so it can
    // add the info object to its list of available plug-ins.
    //
    // #Module is notified in turn by #PluginInfo constructors, or
    // more specifically, the finish() method that is called by the
    // PluginInfo subclass at the end of its constructor.  The
    // module just routes the call to us.
    //
    // The #PluginInfo can come into existence in one of two ways:
    // created by the module query, or reincarnated from a cache by
    // request of the factory when it is being constructed.
    //
    // The factory matching the info object must already exist, there
    // is no way to get here without that being the case.  However it
    // may still be under construction, which is one reason why the
    // call is propagated through base classes and not directly to the
    // factory from the PluginInfo derived class; another reason is
    // to keep knowledge about factories in the plug-in manager and
    // out of #Module.
    PluginFactoryBase *factory = findFactory (info->category ());
    ASSERT (factory);
    factory->addInfo (info);
}

/* Route a dying plug-in info notification to the appropriate factory. */
void
PluginManager::removeInfo (PluginInfo *info)
{
    // This method is called by #Module when a #PluginInfo object is
    // being destructed, with some processing in #Module first.  We
    // find the factory matching the info object category, and route
    // the call to the factory so it can remove the info from it's
    // list.  Knowledge about the info object remains in raw cached
    // form in #Module unless the purpose is to purge the cache.
    //
    // #PluginInfo objects die in one of two ways: either because the
    // factory is going out of scope -- for instance because it itself
    // was in a dynamically loaded module that is getting unloaded --
    // or because the #Module object deletes it.  The latter can
    // happen for a variety reasons, for example when re-querying a
    // module, when module is marked bad, or when the module object
    // is deleted as a part of a cache refresh.
    //
    // By definition the factory already exists, info objects can be
    // created only with the factory present, and the factory deletes
    // its info objects before dying itself.  The call is routed here
    // in order to keep knowledge about factories here, as well as
    // for symmetry with the other info-related registration methods.
    PluginFactoryBase *factory = findFactory (info->category ());
    ASSERT (factory);
    factory->removeInfo (info);
}

/* Route a request to restore an info object from raw cached form to
   the appropriate factory.  The info item's category, the first token
   in the cached data, is expected to be the factory registration name.  */
void
PluginManager::restore (Module *module, ModuleDescriptor *from)
{
    // This method is called by #Module when reading back a cache
    // to inform a factory to create a #PluginInfo from cached form.
    // We look for a factory matching the info object category; the
    // convention is that the first token of the cached info object
    // data must be the category of the info object and the name of
    // the factory.  If such a factory is registered, we pass it the
    // cache data; it will instantiate an appropriate info object
    // from it, which will then end up calling #addInfo().
    //
    // If no factory is currently registered, the call is ignored.
    // In that case, #Module will retain the raw cached data, and
    // when the factory is instantiated, it will rebuild the info
    // objects from that (see #PluginFactoryBase::rebuild()).  In
    // that case the calls will not go through this method; only
    // #addInfo() is used as the info objects are instantiated.
    ASSERT (from);
    ASSERT (from->tokens () >= 2);
    if (PluginFactoryBase *factory = findFactory (from->token (0)))
	factory->restore (module, from);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Add the @a factory to the plugin manager.  This makes the factory
    participate in the internal cache management scheme to allow data
    about modules and plug-ins to to be cached and restored.  This
    method is called by the #PluginFactoryBase constructor.  */
void
PluginManager::addFactory (PluginFactoryBase *factory)
{
    // FIXME: allow factories to be registered to the database, then
    // notify them on each refresh (and when they are added).  The
    // factories should be accessed via XyzDB::get () which gets the
    // global database and registers an instance of the factory into it.
    m_factories.push_back (factory);
}

/** Remove a @a factory from the plug-in manager.  This method is
    be called from the #PluginFactoryBase destructor so that the
    manager will not try to notify factories that have already been
    unloaded from the program.  */
void
PluginManager::removeFactory (PluginFactoryBase *factory)
{
    FactoryIterator pos = std::find (m_factories.begin (), m_factories.end (), factory);
    ASSERT (pos != m_factories.end ());
    m_factories.erase (pos);
}

/** Find a factory that matches category label @a name.  Internal
    method used to match info item categories to factories.  Returns
    pointer to the matching factory, or null if none is found.  */
PluginFactoryBase *
PluginManager::findFactory (const std::string &name)
{
    FactoryIterator pos = m_factories.begin ();
    while (pos != m_factories.end () && (*pos)->category () != name)
	++pos;

    return pos != m_factories.end () ? *pos : 0;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Internal method to rebuild the module table.  Collects all modules
    currently in all the caches into a map that can be iterated over
    by the clients.  The modules are added in the order seen in the
    search path: a module earlier in the path will hide identically
    named module later in the path.  This allows for instance a module
    in a developer area to override one in a base release area.  */
void
PluginManager::rebuild (void)
{
    ModuleMap			modules;
    ModuleCache::Iterator	module;
    DirectoryIterator		dir;

    // Collect modules into a map
    for (dir = beginDirectories (); dir != endDirectories (); ++dir)
	for (module = (*dir)->begin (); module != (*dir)->end (); ++module)
	    if (! modules.count ((*module)->libraryName ()))
		modules [(*module)->libraryName ()] = *module;

    // Put the information back
    m_modules.swap (modules);

    // There's no need to update custom factories.  Refreshing the modules
    // caused them to be re-queried, which caused each module to blast
    // away their caches and existing infos, which caused all factories to
    // remove their database of those infos, plus the re-query caused
    // all necessary new infos to be created.
}

/** Refresh the plug-in database.  Rescans all module directories,
    re-querying all new or updated modules.  This method can take a
    substantial amount of time to run as it may load and unload a
    large number of modules.  Do not call this method in a program
    that is already running and has loaded some modules in memory --
    they may get unloaded or worse, reloaded with a new, incompatible
    version of the module (if the modules are updated on disk).  In
    general plug-in factories may change their contents considerably,
    and will certainly change somewhat, so beware of invalidating
    outstanding object references.  */
void
PluginManager::refresh (void)
{
    DirectoryIterator dir = beginDirectories ();
    for ( ; dir != endDirectories (); ++dir)
	(*dir)->refresh ();

    rebuild ();
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Get an iterator over all the module definition directories.  */
PluginManager::DirectoryIterator
PluginManager::beginDirectories (void) const
{ return m_directories.begin (); }

/** Get an iterator over all the module definition directories.  */
PluginManager::DirectoryIterator
PluginManager::endDirectories (void) const
{ return m_directories.end (); }

/** Get an iterator to a module definition directory @a name.  */
PluginManager::DirectoryIterator
PluginManager::locateDirectory (const Filename &name) const
{
    return std::find_if (beginDirectories (), endDirectories (),
		    	 DirectoryByName (name));
}

/** Locate a module definition directory @a name.  Returns the
    pointer to the matching #ModuleCache if one is found, null
    otherwise.  */
ModuleCache *
PluginManager::directory (const Filename &name) const
{
    DirectoryIterator pos = locateDirectory (name);
    return pos != endDirectories () ? *pos : 0;
}

//////////////////////////////////////////////////////////////////////
/** Return an iterator over all the modules currently known about.
    Note that several module directories can define a module with the
    same name; only the first is visible in the plug-in manager. */
PluginManager::ModuleIterator
PluginManager::beginModules (void) const
{ return ModuleIterator (m_modules.begin ()); }

/** Return an iterator over all the modules currently known about.
    Note that several module directories can define a module with the
    same name; only the first is visible in the plug-in manager. */
PluginManager::ModuleIterator
PluginManager::endModules (void) const
{ return ModuleIterator (m_modules.end ()); }

/** Return an iterator to a module with shared library @a libraryName.
    Note that several module directories can define a module with the
    same name; only the first is visible in the plug-in manager. */
PluginManager::ModuleIterator
PluginManager::locateModule (const Filename &libraryName) const
{ return ModuleIterator (m_modules.find (libraryName)); }

/** Locate a module with shared library @a libraryName.  Returns the
    pointer to the matching #Module if one is found, null otherwise.
    Note that several module directories can define a module with the
    same name; only the first is visible in the plug-in manager.  */
Module *
PluginManager::module (const Filename &libraryName) const
{
    ModuleIterator pos = locateModule (libraryName);
    return pos != endModules () ? *pos : 0;
}

//////////////////////////////////////////////////////////////////////
/** Add a feedback target.  */
void
PluginManager::addFeedback (FeedbackCB callback)
{ m_feedbacks.push_back (callback); }

/** Remove a feedback target.  */
void
PluginManager::removeFeedback (FeedbackCB callback)
{ m_feedbacks.remove (callback); }

/** Send a feedback notification to all registered targets.  */
void
PluginManager::feedback (FeedbackData data)
{
   FeedbackIterator i;
   for (i = m_feedbacks.begin (); i != m_feedbacks.end (); ++i)
	(*i) (data);
}

/** Send a feedback notification to all registered targets.  */
void
PluginManager::feedback (FeedbackCode code,
			 const Filename &scope,
			 Error *error /* = 0 */)
{ feedback (FeedbackData (code, (std::string) scope, error)); }

/** Send a feedback notification to all registered targets.  */
void
PluginManager::feedback (FeedbackCode code,
			 const std::string &scope,
			 Error *error /* = 0 */)
{ feedback (FeedbackData (code, scope, error)); }

} // namespace seal
