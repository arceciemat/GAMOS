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
    std::for_each (onQueryMD.begin (), onQueryMD.end (),
		   std::bind2nd (std::mem_fun_ref (&MDAction::operator()), this));
    std::for_each (onQuery.begin (), onQuery.end (),
		   std::mem_fun_ref (&Action::operator()));
}

void
SimpleModuleDef::attach (void)
{
    std::for_each (onAttachMD.begin (), onAttachMD.end (),
		   std::bind2nd (std::mem_fun_ref (&MDAction::operator()), this));
    std::for_each (onAttach.begin (), onAttach.end (),
		   std::mem_fun_ref (&Action::operator()));
}

void
SimpleModuleDef::detach (void)
{
    std::for_each (onDetachMD.begin (), onDetachMD.end (),
		   std::bind2nd (std::mem_fun_ref (&MDAction::operator()), this));
    std::for_each (onDetach.begin (), onDetach.end (),
		   std::mem_fun_ref (&Action::operator()));
}

} // namespace seal
