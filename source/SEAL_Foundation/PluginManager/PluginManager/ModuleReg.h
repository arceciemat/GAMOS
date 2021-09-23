#ifndef PLUGIN_MANAGER_MODULE_REG_H
# define PLUGIN_MANAGER_MODULE_REG_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "PluginManager/config.h"
# include "SealBase/Filename.h"
# include <list>

namespace seal {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>

class Module;
class ModuleDescriptor;

//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Cache module registration entry.

    This class represents a registration file in the #ModuleCache's
    directory.  It keeps track of each module registered in the file
    (normally just one, but the cache itself of course has many).

    The registration object remembers the name of the registration
    file, the time stamp of the file when it was last examined, and
    whether it is good or bad.  "Bad" means either that the file is
    corrupt (garbage not understood by #ModuleCache), or that the
    modules described in it cannot be loaded for some reason.  */
class ModuleReg
{
public:
    typedef std::list<Module *>::iterator Iterator;
    typedef std::list<Module *>::const_iterator ConstIterator;

    ModuleReg (const Filename &regfile, unsigned time, bool bad);
    ModuleReg (const Filename &regfile,
	       const std::string &time,
	       const std::string &bad);
    ~ModuleReg (void);
    // implicit copy constructor
    // implicit assignment operator

    Filename		file (void) const;
    unsigned		time (void) const;

    Iterator		begin (void);
    Iterator		end (void);
    void		add (Module *module);
    Iterator		erase (Iterator pos);

    void		cache (ModuleDescriptor *to) const;

    bool		bad (void) const;
    void		bad (bool value);

    static const char *	tag (void);

private:
    Filename		m_file;		//< Registration file name (no directory)
    unsigned		m_time;		//< Registration file time stamp
    bool		m_bad;		//< Flag to mark if the file was bad
    std::list<Module *>	m_modules;	//< Modules declared in this file

    static const char	s_tag [];	//< Cache descriptor tag.
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

/** Get the registration file name without the directory part.  */
inline Filename
ModuleReg::file (void) const
{ return m_file; }

/** Return the registration file time stamp.  */
inline unsigned
ModuleReg::time (void) const
{ return m_time; }

//////////////////////////////////////////////////////////////////////
/** Return iterator to the beginning of the module list.  */
inline ModuleReg::Iterator
ModuleReg::begin (void)
{ return m_modules.begin (); }

/** Return iterator one past the end of the module list.  */
inline ModuleReg::Iterator
ModuleReg::end (void)
{ return m_modules.end (); }

//////////////////////////////////////////////////////////////////////
/** Return the cache descriptor tag for this object type.  */
inline const char *
ModuleReg::tag (void)
{ return s_tag; }

} // namespace seal
#endif // PLUGIN_MANAGER_MODULE_REG_H
