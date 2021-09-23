//<<<<<< INCLUDES                                                       >>>>>>

#include "PluginManager/Module.h"
#include "PluginManager/ModuleDef.h"
#include "PluginManager/ModuleDescriptor.h"
#include "PluginManager/PluginCapabilities.h"
#include "PluginManager/PluginManager.h"
#include "PluginManager/PluginInfo.h"
#include "PluginManager/PluginError.h"
#include "debug.h"
#include "SealBase/SharedLibrary.h"
#include "SealBase/SharedLibraryError.h"
#include "SealBase/ShellEnvironment.h"
#include "SealBase/Filename.h"
#include "SealBase/DebugAids.h"
#include "SealBase/Log.h"
#include <algorithm>

namespace seal {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>

const char Module::s_modEntryPoint [] = "SEAL_MODULE";
const char Module::s_capEntryPoint [] = "SEAL_CAPABILITIES";
const char Module::s_tag [] = "module";

//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

Module *
Module::builtin (void)
{
    static Module *myself = new Module (PluginManager::get (), "");
    return myself;
}

Module::Module (PluginManager *manager, const Filename &libraryName)
    : m_manager (manager),
      m_libraryName (libraryName),
      m_library (0),
      m_cache (0),
      m_attached (libraryName.empty ()),
      m_definition (0)
{
    LOG (0, trace, LFplugin_manager,
	 "constructing module for library `" << m_libraryName << "'\n");
    makeCache ();
}

Module::~Module (void)
{
    LOG (0, trace, LFplugin_manager,
         "destroying module for library `" << m_libraryName << "'\n");

    unload ();
    ASSERT (! m_library);

    while (! m_infos.empty ())
	delete *m_infos.begin ();

    delete m_cache;
}

//////////////////////////////////////////////////////////////////////
PluginManager *
Module::manager (void) const
{ return m_manager; }

const Filename &
Module::libraryName (void) const
{ return m_libraryName; }

SharedLibrary *
Module::library (void) const
{
    // FIXME: return SharedLibrary::self if m_libraryName is empty?
    if (m_libraryName.empty ())
	return 0;

    if (! m_library)
    {
	LOG (0, trace, LFplugin_manager,
	     "loading shared library `" << m_libraryName << "'\n");

	try
	{
	    m_manager->feedback (PluginManager::StatusLoading, m_libraryName);
	    // FIXME: remove cast when filename is in the same namespace/gcc bug fixed
	    m_library = SharedLibrary::load ((const std::string &) m_libraryName);
	}
	catch (Error &e)
	{
	    m_manager->feedback (PluginManager::ErrorLoadFailure, m_libraryName, &e);
	    throw PluginError (e.clone ());
	}
    }

    ASSERT (m_library);
    return m_library;
}

bool
Module::loaded (void) const
{ return m_libraryName.empty () || m_library;}

void
Module::load (void)
{
    if (m_libraryName.empty ())
	return;

    LOG (0, trace, LFplugin_manager, "loading `" << m_libraryName << "'\n");

    VERIFY (library ());
    VERIFY (checkEntryPoints ());
}

void
Module::unload (void)
{
    if (m_libraryName.empty ())
	return;

    detach ();

    // Do not unload libraries on platforms with broken handling of
    // global destructors in shared libraries (e.g. KCC 4.0f on
    // Linux).  The problem is that global destructors are not
    // deregistered on library unload, causing instability with
    // subsequent dynamic loading operations, program exit etc.
    if (m_library)
    {
        LOG (0, trace, LFplugin_manager, "unloading `" << m_libraryName << "'\n");

	if (! ShellEnvironment ().has ("SEAL_KEEP_MODULES"))
	{
	    m_manager->feedback (PluginManager::StatusUnloading, m_libraryName);
	    m_library->release ();
	}
	else
	    m_library->abandon ();

	m_library = 0;
    }
}

//////////////////////////////////////////////////////////////////////
void
Module::makeCache (void)
{ m_cache = new ModuleDescriptor (0, s_tag, std::string (m_libraryName)); }

void
Module::clearCache (void)
{ delete m_cache; makeCache (); }

ModuleDescriptor *
Module::cacheRoot (void) const
{ return m_cache; }

//////////////////////////////////////////////////////////////////////
Module::InfoIterator
Module::beginInfos (void) const
{ return m_infos.begin (); }

Module::InfoIterator
Module::endInfos (void) const
{ return m_infos.end (); }

void
Module::addInfo (PluginInfo *info, bool needCache)
{
    if (needCache)
	// This info isn't cached yet -- add it.
	info->cache (cacheRoot ());

    m_infos.push_back (info);
    m_manager->addInfo (info);
}

void
Module::detachInfo (PluginInfo *info)
{
    // Remove from the list but keep the details in the cache.  Also
    // notify the plug-in database so that this gets removed from the
    // factories.
    InfoList::iterator pos = std::find (m_infos.begin (), m_infos.end (), info);
    ASSERT (pos != m_infos.end ());
    m_infos.erase (pos);
    m_manager->removeInfo (info);
}

//////////////////////////////////////////////////////////////////////
void
Module::restore (ModuleDescriptor *from)
{
    // FIXME: If factory is active, this will result in creation of a new
    // info, which we'll want to add to our cache.  Make sure we don't
    // make duplicates...
    from->dup (m_cache);
    m_manager->restore (this, from);
}

void
Module::cache (ModuleDescriptor *to) const
{
    LOG (0, trace, LFplugin_manager, "caching stuff\n"); // FIXME: improve

    // Internal cache is already in sync (we update it every time a
    // new info item is created), so let base class do all the work.
    m_cache->dup (to);
}

//////////////////////////////////////////////////////////////////////
bool
Module::checkEntryPoints (void) const
{
    // Check for module entry points.  It's legal to have just one or both
    // of the normal and capability entry points, but not to have neither.
    // First check for the capability entry point, then for the standard one.
    // If either succeeds, we declare success.  If both fail, we record the
    // error from the standard entry point for more meaningful errors.
    m_manager->feedback (PluginManager::StatusEntries, m_libraryName);

    try
    {
	if (library ()->function (s_capEntryPoint))
	    return true;
    }
    catch (Error &)
    {
    }

    try
    {
	return library ()->function (s_modEntryPoint);
    }
    catch (Error &e)
    {
	m_manager->feedback (PluginManager::ErrorEntryFailure, m_libraryName, &e);
	throw PluginError (e.clone ());
    }
}

ModuleEntry
Module::moduleEntryPoint (void)
{
    SharedLibrary::Function entry = 0;

    if (! libraryName ().empty ()) 
    {
	// Swallow errors, just return null if it fails
	try { entry = library ()->function (s_modEntryPoint); }
	catch (Error &) {}

	LOG (0, trace, LFplugin_manager, "module entry point ("
	     << s_modEntryPoint << ") = " << (void *) entry << '\n');
    }

    return reinterpret_cast<ModuleEntry> (entry);
}

ModuleCapEntry
Module::capabilityEntryPoint (void)
{
    SharedLibrary::Function entry = 0;

    if (! libraryName ().empty ()) 
    {
	// Swallow errors, just return null if it fails
	try { entry = library ()->function (s_capEntryPoint); }
	catch (Error &) {}

	LOG (0, trace, LFplugin_manager, "capability entry point ("
	     << s_capEntryPoint << ") = " << (void *) entry << '\n');
    }

    return reinterpret_cast<ModuleCapEntry> (entry);
}

void
Module::attach (void)
{
    if (! m_attached && ! libraryName ().empty ())
    {
	ASSERT (! m_definition);

	LOG (0, trace, LFplugin_manager,
	     "attaching module " << libraryName () << '\n' << indent);

	// Capability-only modules don't have a normal entry; we
	// don't need a definition for them either.
	if (ModuleEntry entry = moduleEntryPoint ())
	{
	    m_definition = (*entry) ();
	    ASSERT (m_definition);

	    m_manager->feedback (PluginManager::StatusAttaching, m_libraryName);
	    m_definition->bind (this);
	    m_definition->attach ();
	}

	m_attached = true;
	LOG (0, trace, LFplugin_manager, undent);
    }
}

void
Module::detach (void)
{
    if (loaded () && m_attached && ! libraryName ().empty ())
    {
	LOG (0, trace, LFplugin_manager,
	     "detaching module " << libraryName () << '\n' << indent);

	m_manager->feedback (PluginManager::StatusDetaching, m_libraryName);
	for (InfoIterator i = m_infos.begin (); i != m_infos.end (); ++i)
	    (*i)->detach ();

	// Capability-only modules don't have a definition.
	if (m_definition)
	{
	    m_definition->detach ();
	    m_definition->release ();
	    m_definition = 0;
	}
	LOG (0, trace, LFplugin_manager, undent);
    }
}

void
Module::query (void)
{
    if (! libraryName ().empty ()) 
    {
	ASSERT (! m_definition);

	LOG (0, trace, LFplugin_manager,
	     "querying module " << libraryName () << '\n' << indent);

	// Blast away existing infos and the cache in order to rebuild
	// a clean state from the newly created infos.
	clearCache ();
	while (! m_infos.empty ())
	    delete *m_infos.begin ();

	// Now invoke the standard entry point.
	if (ModuleEntry entry = moduleEntryPoint ())
	{
	    m_definition = (*entry) ();
	    ASSERT (m_definition);

	    m_manager->feedback (PluginManager::StatusQuerying, m_libraryName);
	    m_definition->bind (this);
	    m_definition->query ();
	    m_definition->release ();
	    m_definition = 0;
	}

	// Repeat with capabilities entry, processing it manually as
	// if the code was in the entry point in the module library.
	// This allows libraries to define "caability" modules with
	// pure string labels without linking against us -- but not
	// anything that needs a factory.  These would normally be
	// used for side-effects or libraries that self-register to
	// some other factory.  NB: The code below creates a module
	// definition even if the module has a standard entry point
	// and thus an existing definition.  Since this is a query,
	// different defs don't matter, and attach will never use
	// the def anyway.
	if (ModuleCapEntry capEntry = capabilityEntryPoint ())
	{
	    ModuleDef	*def = new ModuleDef;
	    const char	**names = 0;
	    int		n = -1;

	    (*capEntry) (names, n);

	    def->bind (this);
	    for (int i = 0; names && names [i] && (n < 0 || i < n); ++i)
		PluginCapabilities::get ()->declare (def, names [i]);
	    def->release ();
	    delete def;
	}

	LOG (0, trace, LFplugin_manager, undent);
    }
}

bool
Module::attached (void)
{ return m_attached; }

} // namespace seal
