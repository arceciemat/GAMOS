#ifndef PLUGIN_MANAGER_MODULE_DEF_H
# define PLUGIN_MANAGER_MODULE_DEF_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "PluginManager/PluginDef.h"
# include "SealBase/Callback.h"
# include <string>
# include <list>

namespace seal {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>

# if 0 // For doxygen
/** Determines whether plug-ins are in libraries or in the program.

    This macro controls the behaviour of #DEFINE_SEAL_MODULE and
    #DEFINE_SEAL_PLUGIN macros.  If set, the macros produce static
    initialisers to register to #Module::builtin().  If unset, the
    macros generate a shared library entry point through which the
    module definition can be accessed by #Module.  */
#  define SEAL_BUILTIN_PLUGINS 0
# endif // 0 for doxygen

/** @def DEFINE_SEAL_MODULE()
  @brief Define module entry point to register this #Module.
  @sa    #SEAL_BUILTIN_PLUGINS.  */

/** @def DEFINE_SEAL_PLUGIN(factory,type,name)
  @brief Register @a type into @a factory as @a name.
  @sa    #SEAL_BUILTIN_PLUGINS.  */

/** @def DEFINE_SEAL_PLUGIN_DEFAULT_NAME(factory,type)
  @brief Register @a type into @a factory as stringized @a type name.
  @sa    #SEAL_BUILTIN_PLUGINS.  */

/** @def DEFINE_SEAL_PLUGIN_NAME(factory,name)
  @brief Register @a name into @a factory without any object type.
  @sa    #SEAL_BUILTIN_PLUGINS.  */

/** @def SEAL_PLUGIN_API
  @brief Windows DLL API used for module entry point.
         Automatically determined from #SEAL_BUILTIN_PLUGINS.  */


# define DEFINE_SEAL_PLUGIN(factory,type,name)			\
     SEAL_MODULE_QUERY_MDACTION					\
	((&seal::PluginDef<factory, type>::declareName, std::string (name)));\
     SEAL_MODULE_ATTACH_MDACTION				\
	((&seal::PluginDef<factory, type>::installFactoryName,std::string (name)))

# define DEFINE_SEAL_PLUGIN_DEFAULT_NAME(factory,type)		\
     SEAL_MODULE_QUERY_MDACTION					\
	((&seal::PluginDef<factory, type>::declareName, std::string (#type)));\
     SEAL_MODULE_ATTACH_MDACTION				\
	((&seal::PluginDef<factory, type>::installFactoryName,std::string (#type)))

# define DEFINE_SEAL_PLUGIN_NAME(factory,name)			\
     SEAL_MODULE_QUERY_MDACTION					\
	((&seal::PluginDef<factory, void>::declareName, std::string (name)))

# define SEAL_MODULE_SYM(x,y)	SEAL_MODULE_SYM2(x,y)
# define SEAL_MODULE_SYM2(x,y)	x ## y

# ifndef SEAL_BUILTIN_PLUGINS
#  ifdef _WIN32
#   define SEAL_PLUGIN_API __declspec(dllexport)
#  else
#   define SEAL_PLUGIN_API
#  endif

#  define DEFINE_SEAL_MODULE()						\
     namespace { seal::SimpleModuleDef SEAL_THIS_MODULE; }		\
     extern "C" SEAL_PLUGIN_API seal::ModuleDef *SEAL_MODULE (void)	\
	{ return &SEAL_THIS_MODULE; }					\
     extern "C" SEAL_PLUGIN_API seal::ModuleDef *SEAL_MODULE (void)

#  define SEAL_MODULE_QUERY_ACTION(cbargs)				\
     static bool SEAL_MODULE_SYM (moduleInitQueryAction, __LINE__) =	\
	(SEAL_THIS_MODULE.onQuery.push_back(seal::CreateCallback cbargs), true)
#  define SEAL_MODULE_QUERY_MDACTION(cbargs)				\
     static bool SEAL_MODULE_SYM (moduleInitQueryAction, __LINE__) =	\
	(SEAL_THIS_MODULE.onQueryMD.push_back(seal::CreateCallback cbargs), true)

#  define SEAL_MODULE_ATTACH_ACTION(cbargs)				\
     static bool SEAL_MODULE_SYM (moduleInitAttachAction, __LINE__) =	\
	(SEAL_THIS_MODULE.onAttach.push_back(seal::CreateCallback cbargs), true)
#  define SEAL_MODULE_ATTACH_MDACTION(cbargs)				\
     static bool SEAL_MODULE_SYM (moduleInitAttachAction, __LINE__) =	\
	(SEAL_THIS_MODULE.onAttachMD.push_back(seal::CreateCallback cbargs), true)

#  define SEAL_MODULE_DETACH_ACTION(cbargs)				\
     static bool SEAL_MODULE_SYM (moduleInitDetachAction, __LINE__) =	\
	(SEAL_THIS_MODULE.onDetach.push_back(seal::CreateCallback cbargs), true)
#  define SEAL_MODULE_DETACH_MDACTION(cbargs)				\
     static bool SEAL_MODULE_SYM (moduleInitDetachAction, __LINE__) =	\
	(SEAL_THIS_MODULE.onDetachMD.push_back(seal::CreateCallback cbargs), true)

# else

#  define DEFINE_SEAL_MODULE()						\
     namespace { seal::SimpleModuleDef SEAL_THIS_MODULE; }		\
     static bool SEAL_MODULE_SYM (moduleInitAction, __LINE__) =		\
	(SEAL_THIS_MODULE.bind (Module::builtin ()), true)

#  define SEAL_MODULE_QUERY_ACTION(cbargs)				\
     static bool moduleInitQueryAction ## __LINE__ =			\
	(seal::CreateCallback cbargs->call (), true)
#  define SEAL_MODULE_QUERY_MDACTION(cbargs)				\
     static bool SEAL_MODULE_SYM (moduleInitQueryAction, __LINE__) =	\
	(seal::CreateCallback cbargs->call (&SEAL_THIS_MODULE), true)

#  define SEAL_MODULE_ATTACH_ACTION(cbargs)				\
     static bool SEAL_MODULE_SYM (moduleInitAttachAction, __LINE__) =	\
	(seal::CreateCallback cbargs->call (), true)
#  define SEAL_MODULE_ATTACH_MDACTION(cbargs)				\
     static bool SEAL_MODULE_SYM (moduleInitDetachAction, __LINE__) =	\
	(seal::CreateCallback cbargs->call (&SEAL_THIS_MODULE), true)

#  define SEAL_MODULE_DETACH_ACTION(cbargs)   // FIXME: Execute on destruction
#  define SEAL_MODULE_DETACH_MDACTION(cbargs) // FIXME: Execute on destruction
# endif

//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class Module;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** A base class for modules.

    This class is a bridge between #Module and a dynamically loaded
    shared library.  A #Module exists for a shared library even when
    it is not resident in memory.  A #ModuleDef exists for each module
    that is loaded in memory, usually just the simple proxy generated
    with #DEFINE_SEAL_MODULE().

    The purpose of this class is to allow a module to appear as an
    object.  The class is not meant to be generally visible however,
    #DEFINE_SEAL_MODULE() and #DEFINE_SEAL_PLUGIN() macros should be
    sufficient to most users.

    A module has three entry points each of which is a virtual method
    in this class: #attach(), #detach() and #query().  The methods are
    invoked by respective methods in the #Module, thus causing them to
    be executed in the context of the loaded shared library.

    Subclasses should override at least #query() to describe the
    properties of the module.  The implementation should invoke @c
    declare() in the appropriate #PluginFactory subclasses to register
    the capabilities of the module.  #DEFINE_SEAL_PLUGIN() arranges
    such calls to take place.

    The subclass can also override the #attach() and #detach() methods
    to execute actions.  Plug-ins registered in #query() must be given
    factories in #attach() by invoking the @c installFactory() method
    in the appropriate #PluginFactory.  #DEFINE_SEAL_PLUGIN() arranges
    that to happen.  Additional actions can be arranged to execute at
    #attach() time with #SEAL_MODULE_ATTACH_ACTION(), for example to
    initialize external third-party libraries.  There is no normally
    no reas to override #detach(); it's purpose is to undo #attach(),
    but as factories don't need to be removed, there is often little
    to do.  Use #SEAL_MODULE_DETACH_ACTION() to register actions to be
    executed in #detach().

    Subclasses should not define a constructor or a destructor, or
    should at the very least make them very simple.  */
class ModuleDef
{
public:
    ModuleDef (void);
    virtual ~ModuleDef (void);

    // Module implementation interface
    virtual void	query (void);
    virtual void	attach (void);
    virtual void	detach (void);

public:
    // Plug-in architecture interface: for internal use only!  (Public
    // because DEFINE_SEAL_MODULE currently needs access to these.)
    void		bind (Module *module);
    void		release (void);

    // For use by factories.
    Module *		module (void) const;

private:
    Module		*m_module; //< The module owning me.

    // undefined semantics
    ModuleDef (const ModuleDef &);
    ModuleDef &operator= (const ModuleDef &);
};

/** #ModuleDef used by #DEFINE_SEAL_MODULE().  */
class SimpleModuleDef : public ModuleDef
{
public:
    typedef Callback1<ModuleDef *>	MDAction;
    typedef std::list<MDAction>		MDActionList;
    typedef Callback			Action;
    typedef std::list<Action>		ActionList;

    virtual void	query (void);
    virtual void	attach (void);
    virtual void	detach (void);

    MDActionList	onQueryMD;
    MDActionList	onAttachMD;
    MDActionList	onDetachMD;
    ActionList		onQuery;
    ActionList		onAttach;
    ActionList		onDetach;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace seal
#endif // PLUGIN_MANAGER_MODULE_DEF_H
