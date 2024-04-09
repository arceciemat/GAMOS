#ifndef PLUGIN_MANAGER_PLUGIN_PARSER_ERROR_H
# define PLUGIN_MANAGER_PLUGIN_PARSER_ERROR_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "PluginManager/config.h"
# include "SealBase/Error.h"

namespace seal {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

class PLUGIN_MANAGER_API PluginParserError : public Error
{
public:
    PluginParserError (const std::string &reason);
    // implicit copy constructor
    // implicit assignment operator
    // implicit destructor

    virtual std::string	explainSelf (void) const;
    virtual Error *	clone (void) const;
    virtual void	rethrow (void);

private:
    std::string		m_reason;
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>
//<<<<<< INLINE MEMBER FUNCTIONS                                        >>>>>>

} // namespace seal
#endif // PLUGIN_MANAGER_PLUGIN_PARSER_ERROR_H
