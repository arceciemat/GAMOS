#include <iostream>
#ifndef PLUGIN_MANAGER_PLUGIN_DB_VIEW_H
# define PLUGIN_MANAGER_PLUGIN_DB_VIEW_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "PluginManager/MapUniqueIterator.h"
# include "PluginManager/PluginFactoryBase.h"
# include "PluginManager/ModuleDef.h"
# include "PluginManager/Module.h"
# include "SealBase/DebugAids.h"
# include <map>

namespace seal {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

/** Plug-in factory base class.

    The plug-in manager is used through factories derived from this
    class.  Each factory represents a category of objects that can be
    created as a plug-in object.  The plug-in manager is in some sense
    merely a set of factories.  The clients of a factory of type T can
    create instances derived from T, just as in any factory.  Only
    with the plug-in manager the implementations for those classes can
    be loaded on demand from a shared library behind the scenes.
    Clients of the factory are thus decoupled from the physical
    decomposition of the application.

    To define a new plug-in category for some type T, with constructor
    arguments A and B, inherit your factory from PluginFactory<T *(A,
    B)>.  This base class provides all the functionality for
    integration to #PluginManager, managing modules and available
    plug-in objects, iteration over registered plug-ins and the @c
    create() methods to instantiate them.  Please refer to the
    separate how-to documentation for a more detailed example.

    The plug-in manager restricts in no way the types of objects that
    can be created through the factories.  It is possible (though not
    very meaningful) to create instances of the C++ scalar types like
    @c int via the plug-in mechanism.

    Each factory is a globally constructed singleton object like the
    plug-in manager itself (and not a pointer, nor a function-local
    static object).  This strategy is required because shared-library
    loading is an application-global operation and to ensure that the
    factory is always valid if it exists: the factories may come and
    go several times during a single program execution if modules are
    loaded and unloaded, for example during module queries by the
    caching mechanism.  Factories do not have an abstract interface
    unlike plug-ins, they are normal types that are always accessed
    through a static interface.  Clients using a particular factory
    should just use it directly without intermediaries.  It makes no
    sense to try to abstract the factory away; an attempt to do so is
    most likely a misunderstanding.  */
template <class Proto> class PluginFactory;
template <class Proto> class PluginFactoryImpl;
template <class Proto> class PluginFactoryImplTypes;
template <class Types> class PluginFactoryImplBase;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Specialisations of PluginInfo for various argument combinations.  Up
// to three arguments are supported now.  Arbitrary many *could* be
// supported, it's just a question of tedious work to make it happen.
// (See boost absolute and total preprocessor abuse package for another
// solution that scales up to 256 arguments -- it results in totally
// incomprehensible code, both before and after expansion.)
//
// The PluginFactoryImplTypes defines helper types needed to implement
// a particular plug-in factory kind.  The template argument is a
// function prototype (return value and argument list), or just "void"
// for factory that never creates anything, only loads modules.
template <>
class PluginFactoryImplTypes<void>
{
public:
    class Info : public PluginInfo
    {
    public:
	Info (Module *module, const std::string &name, const std::string &tag);
	Info (Module *module, ModuleDescriptor *details, const std::string &tag);
	~Info (void);
    };
};

template <class R>
class PluginFactoryImplTypes<R * (void)>
{
public:
    typedef R Object;
    class Factory {
    public:
	virtual ~Factory (void);
	virtual R *create (void) = 0;
    };

    template <class T> class AutoFactory : public Factory
    {
    public:
	virtual R *create (void);
    };

    class Info : public PluginInfo
    {
    public:
	Info (Module *module, const std::string &name, const std::string &tag);
	Info (Module *module, ModuleDescriptor *details, const std::string &tag);
	~Info (void);

	R *		create (void);
	void		attach (Factory *factory);
	virtual void	detach (void);
	

    private:
	Factory *	factory (void) const;
	Factory		*m_factory;	/*< Factory for creating instances
					    of this type, set when the
					    module is attached.  */
    };
};

//////////////////////////////////////////////////////////////////////
template <class R, class T1>
class PluginFactoryImplTypes<R * (T1)>
{
public:
    typedef R Object;
    class Factory {
    public:
	virtual ~Factory (void);
	virtual R *create (T1 a1) = 0;
    };

    template <class T> class AutoFactory : public Factory
    {
    public:
	virtual R *create (T1 a1);
    };

    class Info : public PluginInfo
    {
    public:
	Info (Module *module, const std::string &name, const std::string &tag);
	Info (Module *module, ModuleDescriptor *details, const std::string &tag);
	~Info (void);

	R *		create (T1 a1);
	void		attach (Factory *factory);
	virtual void	detach (void);

    private:
	Factory *	factory (void) const;
	Factory		*m_factory;	/*< Factory for creating instances
					    of this type, set when the
					    module is attached.  */
    };
};

//////////////////////////////////////////////////////////////////////
template <class R, class T1, class T2>
class PluginFactoryImplTypes<R * (T1, T2)>
{
public:
    typedef R Object;
    class Factory {
    public:
	virtual ~Factory (void);
	virtual R *create (T1 a1, T2 a2) = 0;
    };

    template <class T> class AutoFactory : public Factory
    {
    public:
	virtual R *create (T1 a1, T2 a2);
    };

    class Info : public PluginInfo
    {
    public:
	Info (Module *module, const std::string &name, const std::string &tag);
	Info (Module *module, ModuleDescriptor *details, const std::string &tag);
	~Info (void);

	R *		create (T1 a1, T2 a2);
	void		attach (Factory *factory);
	virtual void	detach (void);

    private:
	Factory *	factory (void) const;
	Factory		*m_factory;	/*< Factory for creating instances
					    of this type, set when the
					    module is attached.  */
    };
};

//////////////////////////////////////////////////////////////////////
template <class R, class T1, class T2, class T3>
class PluginFactoryImplTypes<R * (T1, T2, T3)>
{
public:
    typedef R Object;
    class Factory {
    public:
	virtual ~Factory (void);
	virtual R *create (T1 a1, T2 a2, T3 a3) = 0;
    };

    template <class T> class AutoFactory : public Factory
    {
    public:
	virtual R *create (T1 a1, T2 a2, T3 a3);
    };

    class Info : public PluginInfo
    {
    public:
	Info (Module *module, const std::string &name, const std::string &tag);
	Info (Module *module, ModuleDescriptor *details, const std::string &tag);
	~Info (void);

	R *		create (T1 a1, T2 a2, T3 a3);
	void		attach (Factory *factory);
	virtual void	detach (void);

    private:
	Factory *	factory (void) const;
	Factory		*m_factory;	/*< Factory for creating instances
					    of this type, set when the
					    module is attached.  */
    };
};

//////////////////////////////////////////////////////////////////////
template <class R, class T1, class T2, class T3, class T4>
class PluginFactoryImplTypes<R * (T1, T2, T3, T4)>
{
public:
    typedef R Object;
    class Factory {
    public:
	virtual ~Factory (void);
	virtual R *create (T1 a1, T2 a2, T3 a3, T4 a4) = 0;
    };

    template <class T> class AutoFactory : public Factory
    {
    public:
	virtual R *create (T1 a1, T2 a2, T3 a3, T4 a4);
    };

    class Info : public PluginInfo
    {
    public:
	Info (Module *module, const std::string &name, const std::string &tag);
	Info (Module *module, ModuleDescriptor *details, const std::string &tag);
	~Info (void);

	R *		create (T1 a1, T2 a2, T3 a3, T4 a4);
	void		attach (Factory *factory);
	virtual void	detach (void);

    private:
	Factory *	factory (void) const;
	Factory		*m_factory;	/*< Factory for creating instances
					    of this type, set when the
					    module is attached.  */
    };
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Definition on the main factory implementation base class.  This
// class defines all the utilities for maintaining info objects,
// including their declarations from modules, but includes no support
// for creating objects or accessing object factories.

template <class Types>
class PluginFactoryImplBase : public PluginFactoryBase
{
public:
    /** The info type.  */
    typedef typename Types::Info		Info;

    /** Info table type.  Keyed by the plug-in registration name.  The table
	is always built such that modules earlier in the module search path
	occur earlier in this table; the first interesting info item to use
	is the first of the multiple entries.  */
    typedef std::multimap<std::string, Info *>	Map;

    /** Type of the element in the info table.  */
    typedef typename Map::value_type		MapValue;

    /** Iterator over module data, filters out duplicate entries if
        the same plug-in is available from multiple modules, such as
        released and privately re-built modules. */
    typedef MapUniqueIterator<Map>		Iterator;

    PluginFactoryImplBase (const std::string &tag);
    ~PluginFactoryImplBase (void);

    Iterator		begin (void) const;
    Iterator		end (void) const;
    Iterator		locate (const std::string &name) const;
    Info *		info (const std::string &name) const;

    virtual void	declare (ModuleDef *def, std::string name);

    // Notifications from PluginManager.
    virtual void	rebuild (void);
    virtual void	restore (Module *module, ModuleDescriptor *info);
    virtual void	addInfo (PluginInfo *info);
    virtual void	removeInfo (PluginInfo *info);

private:
    Map			m_map;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Intermediate implementation base class for factories that actually
// instantiate objects.  Adds factories atop PluginFactoryImplBase.

template <class Proto>
class PluginFactoryImpl
    : public PluginFactoryImplBase< PluginFactoryImplTypes<Proto> >
{
public:
    /** Alias type definition helper for more convenient access.  */
    typedef PluginFactoryImplTypes<Proto>	Types;

    /** The object we create.  */
    typedef typename Types::Object		Object;

    /** The basic factory type. */
    typedef typename Types::Factory		Factory;

    /** The info type.  */
    typedef typename Types::Info		Info;

    PluginFactoryImpl (const std::string &name);

    virtual void	installFactory (ModuleDef *def,
					std::string name,
					Factory *factory);
};

//////////////////////////////////////////////////////////////////////
// Custom specialisation for "void", i.e. no factory.

template <>
class PluginFactoryImpl<void>
    : public PluginFactoryImplBase< PluginFactoryImplTypes<void> >
{
public:
    typedef PluginFactoryImplTypes<void>	Types;

    PluginFactoryImpl (const std::string &name);
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Final client-visible factory base classes, specialised for various
// constructor argument types.  These add the actual load()/create()
// method signatures and thus have a specialisation for every possible
// number of arguments (currently zero to three; see above for comments
// on why this is not expanded to more arguments).

template <>
class PluginFactory<void> : public PluginFactoryImpl<void>
{
public:
    PluginFactory (const std::string &name);

    virtual void	load (Iterator info) const;
    virtual void	load (const std::string &name) const;
};

//////////////////////////////////////////////////////////////////////
template <class R>
class PluginFactory<R * (void)> : public PluginFactoryImpl<R * (void)>
{
 public:
    typedef typename PluginFactoryImpl<R * (void)>::Iterator Iterator;
    PluginFactory (const std::string &name);

    virtual R *		create (Iterator info) const;
    virtual R *		create (const std::string &name) const;
};

//////////////////////////////////////////////////////////////////////
template <class R, class T1>
class PluginFactory<R * (T1)> : public PluginFactoryImpl<R * (T1)>
{
 public:
    typedef typename PluginFactoryImpl<R * (T1)>::Iterator Iterator;
    PluginFactory (const std::string &name);

    virtual R *		create (const std::string &name, T1 a1) const;
    virtual R *		create (Iterator item, T1 a1) const;
};

//////////////////////////////////////////////////////////////////////
template <class R, class T1, class T2>
class PluginFactory<R * (T1, T2)> : public PluginFactoryImpl<R * (T1, T2)>
{
 public:
    typedef typename PluginFactoryImpl<R * (T1, T2)>::Iterator Iterator;
    PluginFactory (const std::string &name);

    virtual R *		create (const std::string &name, T1 a1, T2 a2) const;
    virtual R *		create (Iterator item, T1 a1, T2 a2) const;
};

//////////////////////////////////////////////////////////////////////
template <class R, class T1, class T2, class T3>
class PluginFactory<R * (T1, T2, T3)> : public PluginFactoryImpl<R * (T1, T2, T3)>
{
 public:
    typedef typename PluginFactoryImpl<R * (T1, T2, T3)>::Iterator Iterator;
    PluginFactory (const std::string &name);

    virtual R *		create (const std::string &name, T1 a1, T2 a2, T3 a3) const;
    virtual R *		create (Iterator item, T1 a1, T2 a2, T3 a3) const;
};

//////////////////////////////////////////////////////////////////////
template <class R, class T1, class T2, class T3, class T4>
class PluginFactory<R * (T1, T2, T3, T4)> : public PluginFactoryImpl<R * (T1, T2, T3, T4)>
{
 public:
    typedef typename PluginFactoryImpl<R * (T1, T2, T3, T4)>::Iterator Iterator;
    PluginFactory (const std::string &name);

    virtual R *		create (const std::string &name, T1 a1, T2 a2, T3 a3, T4 a4) const;
    virtual R *		create (Iterator item, T1 a1, T2 a2, T3 a3, T4 a4) const;
};

//////////////////////////////////////////////////////////////////////
template <class I>
class PluginFactory : public PluginFactoryBase
{
public:
    typedef I					Info;

    typedef typename I::Factory			Factory;

    typedef std::multimap<std::string, Info *>	Map;

    typedef typename Map::value_type		MapValue;
 
    typedef MapUniqueIterator<Map>		Iterator;

    PluginFactory (const std::string &tag);
    ~PluginFactory (void);

    Iterator		begin (void) const;
    Iterator		end (void) const;
    Iterator		locate (const std::string &name) const;
    Info *		info (const std::string &name) const;

    virtual void	declare (ModuleDef *def, std::string name);
    virtual void	installFactory (ModuleDef *def,
					std::string name,
					Factory *factory);

    virtual void	rebuild (void);
    virtual void	restore (Module *module, ModuleDescriptor *info);
    virtual void	addInfo (PluginInfo *info);
    virtual void	removeInfo (PluginInfo *info);

private:
    Map			m_map;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Construct a new plug-in factory.  Derived class should pass the
    key by which items cached by this category are identified in the
    plug-in manager cache.  It should be unique across the entire
    software system, so if you provide a library, please make an
    effort to pick a suitably unique tag label. */
template <class Types>
inline
PluginFactoryImplBase<Types>::PluginFactoryImplBase (const std::string &tag)
    : PluginFactoryBase (tag)
{
    ASSERT (! tag.empty ());
    rebuild ();
}

/** Destroy the factory.  */
template <class Types>
inline
PluginFactoryImplBase<Types>::~PluginFactoryImplBase (void)
{
    // Remove all my infos.
    while (! m_map.empty ())
	delete m_map.begin ()->second;
}

template <class Types>
inline typename PluginFactoryImplBase<Types>::Iterator
PluginFactoryImplBase<Types>::begin (void) const
{ return Iterator (m_map.begin (), &m_map); }

template <class Types>
inline typename PluginFactoryImplBase<Types>::Iterator
PluginFactoryImplBase<Types>::end (void) const
{ return Iterator (m_map.end (), &m_map); }

template <class Types>
inline typename PluginFactoryImplBase<Types>::Iterator
PluginFactoryImplBase<Types>::locate (const std::string &name) const
{
  //  std::cout << " PluginFactory m_map " << m_map.size() << std::endl; //GDEB
  return Iterator (m_map.find (name), &m_map); }

template <class Types>
inline typename PluginFactoryImplBase<Types>::Info *
PluginFactoryImplBase<Types>::info (const std::string &name) const
{ Iterator pos (locate (name)); return pos != end () ? *pos : 0; }

template <class Types>
inline void
PluginFactoryImplBase<Types>::declare (ModuleDef *def, std::string name)
{
    // This will eventually come back to us, see comments in add().
    ASSERT (def);
    ASSERT (def->module ());
    ASSERT (! name.empty ());
    new Info (def->module (), name, category ());
}

template <class Types>
inline void
PluginFactoryImplBase<Types>::rebuild (void)
{
    // First remove all my infos
    while (! m_map.empty ())
	delete m_map.begin ()->second;

    // And now regenerate them from cache contents
    PluginFactoryBase::rebuild ();
}

template <class Types>
inline void
PluginFactoryImplBase<Types>::restore (Module *module, ModuleDescriptor *info)
{
    // The info item registers itself in the module on creation,
    // causing PluginManager to be notified, and us getting notified
    // again via addInfo() below.  On exit the info item already
    // exists in our map.
    new Info (module, info, category ());
}

template <class Types>
inline void
PluginFactoryImplBase<Types>::addInfo (PluginInfo *info)
{
    // We insert entries in a multi-map because similar entries might
    // get inserted from multiple modules.  When modules are removed
    // we still have a list of previous choices.  The infos are used
    // in the order inserted, relying on the knowledge that PluginManager
    // always processes modules in preference order, i.e. first those
    // earlier in the search path, putting them first on our map's
    // list of values.
    ASSERT (dynamic_cast<Info *> (info));
    m_map.insert (MapValue (info->name (), static_cast<Info *> (info)));
}

template <class Types>
inline void
PluginFactoryImplBase<Types>::removeInfo (PluginInfo *info)
{
    // Remove the info from the appropriate position.
    typename Map::iterator pos = m_map.find (info->name ());
    while (pos != m_map.end ()
	   && pos->first == info->name ()
	   && pos->second != info)
	++pos;

    ASSERT (pos != m_map.end ());
    ASSERT (pos->second == info);
    m_map.erase (pos);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
template <class Proto>
inline
PluginFactoryImpl<Proto>::PluginFactoryImpl (const std::string &tag)
    : PluginFactoryImplBase<Types> (tag)
{}

template <class Proto>
inline void
PluginFactoryImpl<Proto>::installFactory (ModuleDef *def,
					  std::string name,
					  Factory *factory)
{
    ASSERT (def);

    Info *info = this->info (name);

    ASSERT (info);
    ASSERT (info->module ());
    ASSERT (info->module () == def->module ());
    ASSERT (! info->name ().empty ());
    ASSERT (info->name () == name);
    ASSERT (factory);

    info->attach (factory);
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
inline
PluginFactoryImpl<void>::PluginFactoryImpl (const std::string &tag)
    : PluginFactoryImplBase<Types> (tag)
{}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
inline
PluginFactory<void>::PluginFactory (const std::string &tag)
    : PluginFactoryImpl<void> (tag)
{}

inline void
PluginFactory<void>::load (Iterator info) const
{ if (info != this->end ()) (*info)->module ()->attach (); }

inline void
PluginFactory<void>::load (const std::string &name) const
{ load (this->locate (name)); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
template <class R>
inline
PluginFactory<R * (void)>::PluginFactory (const std::string &tag)
    : PluginFactoryImpl<R * (void)> (tag)
{}

template <class R>
inline R *
PluginFactory<R * (void)>::create (Iterator info) const
{ return info == this->end () ? 0 : (*info)->create (); }

template <class R>
inline R *
PluginFactory<R * (void)>::create (const std::string &name) const
{ return create (this->locate (name)); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
template <class R, class T1>
inline
PluginFactory<R * (T1)>::PluginFactory (const std::string &tag)
    : PluginFactoryImpl<R * (T1)> (tag)
{}

template <class R, class T1>
inline R *
PluginFactory<R * (T1)>::create (Iterator info, T1 a1) const
{ return info == this->end () ? 0 : (*info)->create (a1); }

template <class R, class T1>
inline R *
PluginFactory<R * (T1)>::create (const std::string &name, T1 a1) const
{ return create (this->locate (name), a1); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
template <class R, class T1, class T2>
inline
PluginFactory<R * (T1, T2)>::PluginFactory (const std::string &tag)
    : PluginFactoryImpl<R * (T1, T2)> (tag)
{}

template <class R, class T1, class T2>
inline R *
PluginFactory<R * (T1, T2)>::create (Iterator info, T1 a1, T2 a2) const
{ return info == this->end () ? 0 : (*info)->create (a1, a2); }

template <class R, class T1, class T2>
inline R *
PluginFactory<R * (T1, T2)>::create (const std::string &name, T1 a1, T2 a2) const
{ return create (this->locate (name), a1, a2); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
template <class R, class T1, class T2, class T3>
inline
PluginFactory<R * (T1, T2, T3)>::PluginFactory (const std::string &tag)
    : PluginFactoryImpl<R * (T1, T2, T3)> (tag)
{}

template <class R, class T1, class T2, class T3>
inline R *
PluginFactory<R * (T1, T2, T3)>::create (Iterator info, T1 a1, T2 a2, T3 a3) const
{ return info == this->end () ? 0 : (*info)->create (a1, a2, a3); }

template <class R, class T1, class T2, class T3>
inline R *
PluginFactory<R * (T1, T2, T3)>::create (const std::string &name, T1 a1, T2 a2, T3 a3) const
{ return create (this->locate (name), a1, a2, a3); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
template <class R, class T1, class T2, class T3, class T4>
inline
PluginFactory<R * (T1, T2, T3, T4)>::PluginFactory (const std::string &tag)
    : PluginFactoryImpl<R * (T1, T2, T3, T4)> (tag)
{}

template <class R, class T1, class T2, class T3, class T4>
inline R *
PluginFactory<R * (T1, T2, T3, T4)>::create (Iterator info, T1 a1, T2 a2, T3 a3, T4 a4) const
{ return info == this->end () ? 0 : (*info)->create (a1, a2, a3, a4); }

template <class R, class T1, class T2, class T3, class T4>
inline R *
PluginFactory<R * (T1, T2, T3, T4)>::create (const std::string &name, T1 a1, T2 a2, T3 a3, T4 a4) const
{ return create (this->locate (name), a1, a2, a3, a4); }

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
inline
PluginFactoryImplTypes<void>::Info::Info (Module *module,
					  const std::string &name,
					  const std::string &tag)
    : PluginInfo (module, name, tag)
{ finish (true); }

inline
PluginFactoryImplTypes<void>::Info::Info (Module *module,
					  ModuleDescriptor *details,
					  const std::string &tag)
    : PluginInfo (module, details->token (1), tag)
{ ASSERT (details->tokens () == 2); finish (false); }

inline
PluginFactoryImplTypes<void>::Info::~Info (void)
{ detach (); }

//////////////////////////////////////////////////////////////////////
template <class R>
inline
PluginFactoryImplTypes<R * (void)>::Factory::~Factory (void)
{}

template <class R>
template <class T>
inline R *
PluginFactoryImplTypes<R * (void)>::AutoFactory<T>::create (void)
{ return new T; }

template <class R>
inline
PluginFactoryImplTypes<R * (void)>::Info::Info (Module *module,
						const std::string &name,
						const std::string &tag)
    : PluginInfo (module, name, tag),
      m_factory (0)
{ finish (true); }

template <class R>
inline
PluginFactoryImplTypes<R * (void)>::Info::Info (Module *module,
						ModuleDescriptor *details,
						const std::string &tag)
    : PluginInfo (module, details->token (1), tag),
      m_factory (0)
{ ASSERT (details->tokens () == 2); finish (false); }

template <class R>
inline
PluginFactoryImplTypes<R * (void)>::Info::~Info (void)
{ detach (); }

template <class R>
inline R *
PluginFactoryImplTypes<R * (void)>::Info::create (void)
{
    ASSERT (module ());
    ASSERT (! name ().empty ());
    return factory ()->create ();
}

template <class R>
inline void
PluginFactoryImplTypes<R * (void)>::Info::attach (Factory *factory)
{
    ASSERT (module ());
    ASSERT (! name ().empty ());
    ASSERT (! m_factory);
    ASSERT (factory); 
    m_factory = factory;
}

template <class R>
inline void
PluginFactoryImplTypes<R * (void)>::Info::detach (void)
{
    ASSERT (module ());
    ASSERT (! name ().empty ());
    delete m_factory;
    m_factory = 0;
}

template <class R>
inline typename PluginFactoryImplTypes<R * (void)>::Factory *
PluginFactoryImplTypes<R * (void)>::Info::factory (void) const
{
    ASSERT (module ());
    ASSERT (! name ().empty ());

    if (! m_factory)
	module ()->attach ();

    if (! m_factory)
	noFactory ();

    return m_factory;
}

//////////////////////////////////////////////////////////////////////
template <class R, class T1>
inline
PluginFactoryImplTypes<R * (T1)>::Factory::~Factory (void)
{}

template <class R, class T1>
template <class T>
inline R *
PluginFactoryImplTypes<R * (T1)>::AutoFactory<T>::create (T1 a1)
{ return new T (a1); }

template <class R, class T1>
inline
PluginFactoryImplTypes<R * (T1)>::Info::Info (Module *module,
					      const std::string &name,
					      const std::string &tag)
    : PluginInfo (module, name, tag),
      m_factory (0)
{ finish (true); }

template <class R, class T1>
inline
PluginFactoryImplTypes<R * (T1)>::Info::Info (Module *module,
					      ModuleDescriptor *details,
					      const std::string &tag)
    : PluginInfo (module, details->token (1), tag),
      m_factory (0)
{ ASSERT (details->tokens () == 2); finish (false); }

template <class R, class T1>
inline
PluginFactoryImplTypes<R * (T1)>::Info::~Info (void)
{ detach (); }

template <class R, class T1>
inline R *
PluginFactoryImplTypes<R * (T1)>::Info::create (T1 a1)
{
    ASSERT (module ());
    ASSERT (! name ().empty ());
    return factory ()->create (a1);
}

template <class R, class T1>
inline void
PluginFactoryImplTypes<R * (T1)>::Info::attach (Factory *factory)
{
    ASSERT (module ());
    ASSERT (! name ().empty ());
    ASSERT (! m_factory);
    ASSERT (factory); 
    m_factory = factory;
}

template <class R, class T1>
inline void
PluginFactoryImplTypes<R * (T1)>::Info::detach (void)
{
    ASSERT (module ());
    ASSERT (! name ().empty ());
    delete m_factory;
    m_factory = 0;
}

template <class R, class T1>
inline typename PluginFactoryImplTypes<R * (T1)>::Factory *
PluginFactoryImplTypes<R * (T1)>::Info::factory (void) const
{
    ASSERT (module ());
    ASSERT (! name ().empty ());

    if (! m_factory)
	module ()->attach ();

    if (! m_factory)
	noFactory ();

    return m_factory;
}

//////////////////////////////////////////////////////////////////////
template <class R, class T1, class T2>
inline
PluginFactoryImplTypes<R * (T1, T2)>::Factory::~Factory (void)
{}

template <class R, class T1, class T2>
template <class T>
inline R *
PluginFactoryImplTypes<R * (T1, T2)>::AutoFactory<T>::create (T1 a1, T2 a2)
{ return new T (a1, a2); }

template <class R, class T1, class T2>
inline
PluginFactoryImplTypes<R * (T1, T2)>::Info::Info (Module *module,
						  const std::string &name,
						  const std::string &tag)
    : PluginInfo (module, name, tag),
      m_factory (0)
{ finish (true); }

template <class R, class T1, class T2>
inline
PluginFactoryImplTypes<R * (T1, T2)>::Info::Info (Module *module,
						  ModuleDescriptor *details,
						  const std::string &tag)
    : PluginInfo (module, details->token (1), tag),
      m_factory (0)
{ ASSERT (details->tokens () == 2); finish (false); }

template <class R, class T1, class T2>
inline
PluginFactoryImplTypes<R * (T1, T2)>::Info::~Info (void)
{ detach (); }

template <class R, class T1, class T2>
inline R *
PluginFactoryImplTypes<R * (T1, T2)>::Info::create (T1 a1, T2 a2)
{
    ASSERT (module ());
    ASSERT (! name ().empty ());
    return factory ()->create (a1, a2);
}

template <class R, class T1, class T2>
inline void
PluginFactoryImplTypes<R * (T1, T2)>::Info::attach (Factory *factory)
{
    ASSERT (module ());
    ASSERT (! name ().empty ());
    ASSERT (! m_factory);
    ASSERT (factory); 
    m_factory = factory;
}

template <class R, class T1, class T2>
inline void
PluginFactoryImplTypes<R * (T1, T2)>::Info::detach (void)
{
    ASSERT (module ());
    ASSERT (! name ().empty ());
    delete m_factory;
    m_factory = 0;
}

template <class R, class T1, class T2>
inline typename PluginFactoryImplTypes<R * (T1, T2)>::Factory *
PluginFactoryImplTypes<R * (T1, T2)>::Info::factory (void) const
{
    ASSERT (module ());
    ASSERT (! name ().empty ());

    if (! m_factory)
	module ()->attach ();

    if (! m_factory)
	noFactory ();

    return m_factory;
}

//////////////////////////////////////////////////////////////////////
template <class R, class T1, class T2, class T3>
inline
PluginFactoryImplTypes<R * (T1, T2, T3)>::Factory::~Factory (void)
{}

template <class R, class T1, class T2, class T3>
template <class T>
inline R *
PluginFactoryImplTypes<R * (T1, T2, T3)>::AutoFactory<T>::create (T1 a1, T2 a2, T3 a3)
{ return new T (a1, a2, a3); }

template <class R, class T1, class T2, class T3>
inline
PluginFactoryImplTypes<R * (T1, T2, T3)>::Info::Info (Module *module,
						      const std::string &name,
						      const std::string &tag)
    : PluginInfo (module, name, tag),
      m_factory (0)
{ finish (true); }

template <class R, class T1, class T2, class T3>
inline
PluginFactoryImplTypes<R * (T1, T2, T3)>::Info::Info (Module *module,
						      ModuleDescriptor *details,
						      const std::string &tag)
    : PluginInfo (module, details->token (1), tag),
      m_factory (0)
{ ASSERT (details->tokens () == 2); finish (false); }

template <class R, class T1, class T2, class T3>
inline
PluginFactoryImplTypes<R * (T1, T2, T3)>::Info::~Info (void)
{ detach (); }

template <class R, class T1, class T2, class T3>
inline R *
PluginFactoryImplTypes<R * (T1, T2, T3)>::Info::create (T1 a1, T2 a2, T3 a3)
{
    ASSERT (module ());
    ASSERT (! name ().empty ());
    return factory ()->create (a1, a2, a3);
}

template <class R, class T1, class T2, class T3>
inline void
PluginFactoryImplTypes<R * (T1, T2, T3)>::Info::attach (Factory *factory)
{
    ASSERT (module ());
    ASSERT (! name ().empty ());
    ASSERT (! m_factory);
    ASSERT (factory); 
    m_factory = factory;
}

template <class R, class T1, class T2, class T3>
inline void
PluginFactoryImplTypes<R * (T1, T2, T3)>::Info::detach (void)
{
    ASSERT (module ());
    ASSERT (! name ().empty ());
    delete m_factory;
    m_factory = 0;
}

template <class R, class T1, class T2, class T3>
inline typename PluginFactoryImplTypes<R * (T1, T2, T3)>::Factory *
PluginFactoryImplTypes<R * (T1, T2, T3)>::Info::factory (void) const
{
    ASSERT (module ());
    ASSERT (! name ().empty ());

    if (! m_factory)
	module ()->attach ();

    if (! m_factory)
	noFactory ();

    return m_factory;
}

//////////////////////////////////////////////////////////////////////
template <class R, class T1, class T2, class T3, class T4>
inline
PluginFactoryImplTypes<R * (T1, T2, T3, T4)>::Factory::~Factory (void)
{}

template <class R, class T1, class T2, class T3, class T4>
template <class T>
inline R *
PluginFactoryImplTypes<R * (T1, T2, T3, T4)>::AutoFactory<T>::create (T1 a1, T2 a2, T3 a3, T4 a4)
{ return new T (a1, a2, a3, a4); }

template <class R, class T1, class T2, class T3, class T4>
inline
PluginFactoryImplTypes<R * (T1, T2, T3, T4)>::Info::Info (Module *module,
							  const std::string &name,
							  const std::string &tag)
    : PluginInfo (module, name, tag),
      m_factory (0)
{ finish (true); }

template <class R, class T1, class T2, class T3, class T4>
inline
PluginFactoryImplTypes<R * (T1, T2, T3, T4)>::Info::Info (Module *module,
							  ModuleDescriptor *details,
							  const std::string &tag)
    : PluginInfo (module, details->token (1), tag),
      m_factory (0)
{ ASSERT (details->tokens () == 2); finish (false); }

template <class R, class T1, class T2, class T3, class T4>
inline
PluginFactoryImplTypes<R * (T1, T2, T3, T4)>::Info::~Info (void)
{ detach (); }

template <class R, class T1, class T2, class T3, class T4>
inline R *
PluginFactoryImplTypes<R * (T1, T2, T3, T4)>::Info::create (T1 a1, T2 a2, T3 a3, T4 a4)
{
    ASSERT (module ());
    ASSERT (! name ().empty ());
    return factory ()->create (a1, a2, a3, a4);
}

template <class R, class T1, class T2, class T3, class T4>
inline void
PluginFactoryImplTypes<R * (T1, T2, T3, T4)>::Info::attach (Factory *factory)
{
    ASSERT (module ());
    ASSERT (! name ().empty ());
    ASSERT (! m_factory);
    ASSERT (factory); 
    m_factory = factory;
}

template <class R, class T1, class T2, class T3, class T4>
inline void
PluginFactoryImplTypes<R * (T1, T2, T3, T4)>::Info::detach (void)
{
    ASSERT (module ());
    ASSERT (! name ().empty ());
    delete m_factory;
    m_factory = 0;
}

template <class R, class T1, class T2, class T3, class T4>
inline typename PluginFactoryImplTypes<R * (T1, T2, T3, T4)>::Factory *
PluginFactoryImplTypes<R * (T1, T2, T3, T4)>::Info::factory (void) const
{
    ASSERT (module ());
    ASSERT (! name ().empty ());

    if (! m_factory)
	module ()->attach ();

    if (! m_factory)
	noFactory ();

    return m_factory;
}

} // namespace seal
#endif // PLUGIN_MANAGER_PLUGIN_DB_VIEW_H
