//<<<<<< INCLUDES                                                       >>>>>>

#include "PluginManager/ModuleDef.h"
#include "PluginManager/Module.h"
#include "debug.h"
#include "SealBase/DebugAids.h"
#include <algorithm>
#include <functional>

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

ModuleDef::ModuleDef (void)
    : m_module (0)
{}

ModuleDef::~ModuleDef (void)
{
    if (m_module)
	LOG (0, warning, LFplugin_manager, "Attached ModuleDef destroyed\n");
}

void
ModuleDef::attach (void)
{}

void
ModuleDef::detach (void)
{}

void
ModuleDef::query (void)
{}

void
ModuleDef::bind (Module *module)
{
    ASSERT (! m_module);
    ASSERT (module);
    m_module = module;
}

void
ModuleDef::release (void)
{
    ASSERT (m_module);
    m_module = 0;
}

Module *
ModuleDef::module (void) const
{
    ASSERT (m_module);
    return m_module;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
SimpleModuleDef::query (void)
{
    for (auto &action : onQueryMD)
	action (this);
    for (auto &action : onQuery)
	action ();
}

void
SimpleModuleDef::attach (void)
{
    for (auto &action : onAttachMD)
	action (this);
    for (auto &action : onAttach)
	action ();
}

void
SimpleModuleDef::detach (void)
{
    for (auto &action : onDetachMD)
	action (this);
    for (auto &action : onDetach)
	action ();
}

} // namespace seal
