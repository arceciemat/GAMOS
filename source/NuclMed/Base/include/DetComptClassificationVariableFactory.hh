#ifndef ROOT5

#ifndef DetComptClassificationVariableFactory_H
#define DetComptClassificationVariableFactory_H

#include "DetVComptClassificationVariable.hh"
#include "PluginManager/PluginFactory.h"

/// Plug-in factory for objects of type DetVComptClassificationVariable
class DetComptClassificationVariableFactory : public seal::PluginFactory<DetVComptClassificationVariable*()>
{
public:
    static DetComptClassificationVariableFactory *get (void);

private:
    DetComptClassificationVariableFactory (void);
    static DetComptClassificationVariableFactory s_instance;
};


#define DEFINE_GAMOS_COMPT_CLASSIF_VARIABLE(type) \
  DEFINE_SEAL_PLUGIN(DetComptClassificationVariableFactory, type,#type)

#endif // DetComptClassificationVariableFactory_H

#endif
