#ifndef ROOT5

#ifndef Det1stHitAlgorithmFactory_H
#define Det1stHitAlgorithmFactory_H

#include "DetV1stHitAlgorithm.hh"
#include "PluginManager/PluginFactory.h"

/// Plug-in factory for objects of type DetV1stHitAlgorithm
class Det1stHitAlgorithmFactory : public seal::PluginFactory<DetV1stHitAlgorithm*()>
{
public:
    static Det1stHitAlgorithmFactory *get (void);

private:
    Det1stHitAlgorithmFactory (void);
    static Det1stHitAlgorithmFactory s_instance;
};


#define DEFINE_GAMOS_1STHIT_ALGORITHM_FACTORY(type) \
  DEFINE_SEAL_PLUGIN(Det1stHitAlgorithmFactory, type,#type)

#endif // Det1stHitAlgorithmFactory_H

#endif
