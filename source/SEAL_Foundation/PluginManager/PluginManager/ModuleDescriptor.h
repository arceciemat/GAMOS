#ifndef PLUGIN_MANAGER_MODULE_DESCRIPTOR_H
# define PLUGIN_MANAGER_MODULE_DESCRIPTOR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "PluginManager/config.h"
# include <vector>
# include <string>

namespace seal {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Cache data representation: a tree of vectors of strings.  */
class ModuleDescriptor
{
public:
    ModuleDescriptor (ModuleDescriptor *parent = 0,
		      const std::string &t1 = "",
		      const std::string &t2 = "",
		      const std::string &t3 = "",
		      const std::string &t4 = "",
		      const std::string &t5 = "",
		      const std::string &t6 = "",
		      const std::string &t7 = "",
		      const std::string &t8 = "");
    ModuleDescriptor (ModuleDescriptor *parent,
		      const std::vector<std::string> &tokens);
    ~ModuleDescriptor (void);
    // implicit copy constructor
    // implicit assignment operator

    ModuleDescriptor *			parent (void) const;
    unsigned				children (void) const;
    ModuleDescriptor *			child (unsigned index) const;
    void				addChild (ModuleDescriptor *child);

    unsigned				tokens (void) const;
    const std::string &			token (unsigned index) const;
    void				addToken (const std::string &text);

    void				dup (ModuleDescriptor *to) const;

private:
    ModuleDescriptor			*m_parent;	//< Up in the tree.
    std::vector<ModuleDescriptor *>	m_children;	//< Down in the tree.
    std::vector<std::string>		m_tokens;	//< Tokens.
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace seal
#endif // PLUGIN_MANAGER_MODULE_DESCRIPTOR_H
