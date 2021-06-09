#ifndef ROOT5

#ifndef DetComptAlgorithmVariableFactory_H
#define DetComptAlgorithmVariableFactory_H

#include "DetVComptAlgorithmVariable.hh"
#include "PluginManager/PluginFactory.h"

/// Plug-in factory for objects of type DetVComptAlgorithmVariable
class DetComptAlgorithmVariableFactory : public seal::PluginFactory<DetVComptAlgorithmVariable*()>
{
public:
    static DetComptAlgorithmVariableFactory *get (void);

private:
    DetComptAlgorithmVariableFactory (void);
    static DetComptAlgorithmVariableFactory s_instance;
};


#define DEFINE_GAMOS_COMPT_ALGORITHM_VARIABLE(type) \
  DEFINE_SEAL_PLUGIN(DetComptAlgorithmVariableFactory, type,#type)

#endif // DetComptAlgorithmVariableFactory_H

#endif

