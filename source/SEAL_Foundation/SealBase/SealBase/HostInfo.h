#ifndef SEAL_BASE_HOST_INFO_H
# define SEAL_BASE_HOST_INFO_H

//<<<<<< INCLUDES                                                       >>>>>>

# include "SealBase/config.h"
# include <string>

namespace seal {
//<<<<<< PUBLIC DEFINES                                                 >>>>>>
//<<<<<< PUBLIC CONSTANTS                                               >>>>>>
//<<<<<< PUBLIC TYPES                                                   >>>>>>
//<<<<<< PUBLIC VARIABLES                                               >>>>>>
//<<<<<< PUBLIC FUNCTIONS                                               >>>>>>
//<<<<<< CLASS DECLARATIONS                                             >>>>>>

/** Describe a host, and by default the current host.  */
class HostInfo
{
public:
    static std::string	name (void);
    static std::string	dnsname (void);
    static std::string	dnsdomain (void);
    static std::string	fqdn (void);

    // FIXME: for host name x?
    // FIXME: gethostbyname, gethostbyaddr, ...
private:
    static bool		isfqdn (const char *name);
};

//<<<<<< INLINE PUBLIC FUNCTIONS                                        >>>>>>

} // namespace seal
#endif // SEAL_BASE_HOST_INFO_H
