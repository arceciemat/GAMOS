//<<<<<< INCLUDES                                                       >>>>>>

#include "PluginParserError.h"

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

PluginParserError::PluginParserError (const std::string &reason)
    : m_reason (reason)
{}

std::string
PluginParserError::explainSelf (void) const
{ return m_reason; }

/** @reimp */
Error *
PluginParserError::clone (void) const
{ return new PluginParserError (*this); }

/** @reimp */
void
PluginParserError::rethrow (void)
{ throw *this; }

} // namespace seal
