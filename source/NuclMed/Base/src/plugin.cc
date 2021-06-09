#include "DetHistosGammaDist.hh"
#include "DetComptonStudyHistosUA.hh"

#include "DetCutsStudyFilter.hh"

#include "Det1stHitByEnergy.hh"
#include "Det1stHitByXPos.hh"
#include "Det1stHitByXYPos.hh"
#include "Det1stHitByXYZPos.hh"
#include "Det1stHitByComptonCone.hh"
#include "Det1stHitByDistanceToOther.hh"

#include "DetCAlgoEnergy.hh" 
#include "DetCAlgoXPos.hh" 
#include "DetCAlgoYPos.hh" 
#include "DetCAlgoZPos.hh" 
#include "DetCAlgoXYPos.hh" 
#include "DetCAlgoXZPos.hh" 
#include "DetCAlgoYZPos.hh" 
#include "DetCAlgoXYZPos.hh" 

#include "DetCClassifNRecHit.hh" 
#include "DetCClassifEnergyMin.hh" 
#include "DetCClassifEnergyMax.hh" 
#include "DetCClassifEnergyAverage.hh" 
#include "DetCClassifXPosMin.hh" 
#include "DetCClassifYPosMin.hh" 
#include "DetCClassifZPosMin.hh" 
#include "DetCClassifXYPosMin.hh" 
#include "DetCClassifXZPosMin.hh" 
#include "DetCClassifYZPosMin.hh" 
#include "DetCClassifXYZPosMin.hh" 

#ifdef ROOT5
#include "Reflex/PluginService.h"

PLUGINSVC_FACTORY(DetHistosGammaDist,GmUserAction*())
PLUGINSVC_FACTORY(DetComptonStudyHistosUA,GmUserAction*())

PLUGINSVC_FACTORY(DetCutsStudyFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(Det1stHitByEnergy,DetV1stHitAlgorithm*())
PLUGINSVC_FACTORY(Det1stHitByXPos,DetV1stHitAlgorithm*())
PLUGINSVC_FACTORY(Det1stHitByXYPos,DetV1stHitAlgorithm*())
PLUGINSVC_FACTORY(Det1stHitByXYZPos,DetV1stHitAlgorithm*())
PLUGINSVC_FACTORY(Det1stHitByComptonCone,DetV1stHitAlgorithm*())
PLUGINSVC_FACTORY(Det1stHitByDistanceToOther,DetV1stHitAlgorithm*())

PLUGINSVC_FACTORY(DetCAlgoEnergy,DetVComptAlgorithmVariable*())
PLUGINSVC_FACTORY(DetCAlgoXPos,DetVComptAlgorithmVariable*())
PLUGINSVC_FACTORY(DetCAlgoYPos,DetVComptAlgorithmVariable*())
PLUGINSVC_FACTORY(DetCAlgoZPos,DetVComptAlgorithmVariable*())
PLUGINSVC_FACTORY(DetCAlgoXYPos,DetVComptAlgorithmVariable*())
PLUGINSVC_FACTORY(DetCAlgoXZPos,DetVComptAlgorithmVariable*())
PLUGINSVC_FACTORY(DetCAlgoYZPos,DetVComptAlgorithmVariable*())
PLUGINSVC_FACTORY(DetCAlgoXYZPos,DetVComptAlgorithmVariable*())

PLUGINSVC_FACTORY(DetCClassifNRecHit,DetVComptClassificationVariable*())
PLUGINSVC_FACTORY(DetCClassifEnergyMin,DetVComptClassificationVariable*())
PLUGINSVC_FACTORY(DetCClassifEnergyMax,DetVComptClassificationVariable*())
PLUGINSVC_FACTORY(DetCClassifEnergyAverage,DetVComptClassificationVariable*())
PLUGINSVC_FACTORY(DetCClassifXPosMin,DetVComptClassificationVariable*())
PLUGINSVC_FACTORY(DetCClassifYPosMin,DetVComptClassificationVariable*())
PLUGINSVC_FACTORY(DetCClassifZPosMin,DetVComptClassificationVariable*())
PLUGINSVC_FACTORY(DetCClassifXYPosMin,DetVComptClassificationVariable*())
PLUGINSVC_FACTORY(DetCClassifXZPosMin,DetVComptClassificationVariable*())
PLUGINSVC_FACTORY(DetCClassifYZPosMin,DetVComptClassificationVariable*())
PLUGINSVC_FACTORY(DetCClassifXYZPosMin,DetVComptClassificationVariable*())

#include "DetVerbosity.hh"
PLUGINSVC_FACTORY(DetVerbosity,GmVVerbosity*())

#else

#include "DetComptAlgorithmVariableFactory.hh"
#include "DetComptClassificationVariableFactory.hh"
#include "Det1stHitAlgorithmFactory.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserActionFactory.hh"
#include "GamosCore/GamosBase/Base/include/GmFilterFactory.hh"
#include "GamosCore/GamosBase/Base/include/GmVerbosityFactory.hh"

DEFINE_SEAL_MODULE();

DEFINE_GAMOS_USER_ACTION(DetHistosGammaDist);
DEFINE_GAMOS_USER_ACTION(DetComptonStudyHistosUA);

DEFINE_GAMOS_FILTER(DetCutsStudyFilter);
DEFINE_GAMOS_1STHIT_ALGORITHM_FACTORY(Det1stHitByEnergy);
DEFINE_GAMOS_1STHIT_ALGORITHM_FACTORY(Det1stHitByXPos);
DEFINE_GAMOS_1STHIT_ALGORITHM_FACTORY(Det1stHitByXYPos);
DEFINE_GAMOS_1STHIT_ALGORITHM_FACTORY(Det1stHitByXYZPos);
DEFINE_GAMOS_1STHIT_ALGORITHM_FACTORY(Det1stHitByComptonCone);
DEFINE_GAMOS_1STHIT_ALGORITHM_FACTORY(Det1stHitByDistanceToOther);

DEFINE_GAMOS_COMPT_ALGORITHM_VARIABLE(DetCAlgoEnergy);
DEFINE_GAMOS_COMPT_ALGORITHM_VARIABLE(DetCAlgoXPos);
DEFINE_GAMOS_COMPT_ALGORITHM_VARIABLE(DetCAlgoYPos);
DEFINE_GAMOS_COMPT_ALGORITHM_VARIABLE(DetCAlgoZPos);
DEFINE_GAMOS_COMPT_ALGORITHM_VARIABLE(DetCAlgoXYPos);
DEFINE_GAMOS_COMPT_ALGORITHM_VARIABLE(DetCAlgoXZPos);
DEFINE_GAMOS_COMPT_ALGORITHM_VARIABLE(DetCAlgoYZPos);
DEFINE_GAMOS_COMPT_ALGORITHM_VARIABLE(DetCAlgoXYZPos);

DEFINE_GAMOS_COMPT_CLASSIF_VARIABLE(DetCClassifNRecHit);
DEFINE_GAMOS_COMPT_CLASSIF_VARIABLE(DetCClassifEnergyMin);
DEFINE_GAMOS_COMPT_CLASSIF_VARIABLE(DetCClassifEnergyMax);
DEFINE_GAMOS_COMPT_CLASSIF_VARIABLE(DetCClassifEnergyAverage);
DEFINE_GAMOS_COMPT_CLASSIF_VARIABLE(DetCClassifXPosMin);
DEFINE_GAMOS_COMPT_CLASSIF_VARIABLE(DetCClassifYPosMin);
DEFINE_GAMOS_COMPT_CLASSIF_VARIABLE(DetCClassifZPosMin);
DEFINE_GAMOS_COMPT_CLASSIF_VARIABLE(DetCClassifXYPosMin);
DEFINE_GAMOS_COMPT_CLASSIF_VARIABLE(DetCClassifXZPosMin);
DEFINE_GAMOS_COMPT_CLASSIF_VARIABLE(DetCClassifYZPosMin);
DEFINE_GAMOS_COMPT_CLASSIF_VARIABLE(DetCClassifXYZPosMin);

#include "DetVerbosity.hh"
DEFINE_GAMOS_VERBOSITY(DetVerbosity);

#endif
