#ifndef ROOT5
#include "DetComptClassificationVariableFactory.hh"

DetComptClassificationVariableFactory DetComptClassificationVariableFactory::s_instance;


DetComptClassificationVariableFactory::DetComptClassificationVariableFactory (void)
    : seal::PluginFactory<DetVComptClassificationVariable*()> ("DetComptClassificationVariableFactory")
{}

DetComptClassificationVariableFactory *
DetComptClassificationVariableFactory::get (void)
{ return &s_instance; }
#endif
