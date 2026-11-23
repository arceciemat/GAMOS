#ifndef ROOT5
#include "Det1stHitAlgorithmFactory.hh"

Det1stHitAlgorithmFactory Det1stHitAlgorithmFactory::s_instance;


Det1stHitAlgorithmFactory::Det1stHitAlgorithmFactory (void)
    : seal::PluginFactory<DetV1stHitAlgorithm*()> ("Det1stHitAlgorithmFactory")
{}

Det1stHitAlgorithmFactory *
Det1stHitAlgorithmFactory::get (void)
{ return &s_instance; }
#endif
