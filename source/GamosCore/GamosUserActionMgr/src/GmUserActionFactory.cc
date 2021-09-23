#ifndef ROOT5
#include "GmUserActionFactory.hh"

GmUserActionFactory GmUserActionFactory::s_instance;

GmUserActionFactory::GmUserActionFactory (void)
    : seal::PluginFactory<GmUserAction* ()> ("GmUserActionFactory")
{}

GmUserActionFactory *
GmUserActionFactory::get (void)
{ return &s_instance; }

#endif
