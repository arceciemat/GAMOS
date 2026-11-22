//<<<<<< INCLUDES                                                       >>>>>>

#include "PluginManager/PluginError.h"

namespace seal {
//<<<<<< PRIVATE DEFINES                                                >>>>>>
//<<<<<< PRIVATE CONSTANTS                                              >>>>>>
//<<<<<< PRIVATE TYPES                                                  >>>>>>
//<<<<<< PRIVATE VARIABLE DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC VARIABLE DEFINITIONS                                    >>>>>>
//<<<<<< CLASS STRUCTURE INITIALIZATION                                 >>>>>>
//<<<<<< PRIVATE FUNCTION DEFINITIONS                                   >>>>>>
//<<<<<< PUBLIC FUNCTION DEFINITIONS                                    >>>>>>
//<<<<<< MEMBER FUNCTION DEFINITIONS                                    >>>>>>

PluginError::PluginError (Error *next)
    : Error (next)
{}

std::string
PluginError::explainSelf (void) const
{ return "Plug-in manager failure"; }

/** @reimp */
Error *
PluginError::clone (void) const
{ return new PluginError (*this); }

/** @reimp */
void
PluginError::rethrow (void)
{ throw *this; }

} // namespace seal
