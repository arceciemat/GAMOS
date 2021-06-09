#ifndef ROOT5
#include "DetComptAlgorithmVariableFactory.hh"

DetComptAlgorithmVariableFactory DetComptAlgorithmVariableFactory::s_instance;


DetComptAlgorithmVariableFactory::DetComptAlgorithmVariableFactory (void)
    : seal::PluginFactory<DetVComptAlgorithmVariable*()> ("DetComptAlgorithmVariableFactory")
{}

DetComptAlgorithmVariableFactory *
DetComptAlgorithmVariableFactory::get (void)
{ return &s_instance; }
#endif
