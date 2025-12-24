
//----- DATA USERS
#include "GmStepDataHistosUA.hh"
#include "GmTrackDataHistosUA.hh"
#include "GmSecondaryTrackDataHistosUA.hh"
#include "GmEventDataHistosUA.hh"
#include "GmRunDataHistosUA.hh"
#include "GmStackDataHistosUA.hh"
#include "GmStepDataTextFileUA.hh"
#include "GmTrackDataTextFileUA.hh"
#include "GmSecondaryTrackDataTextFileUA.hh"
#include "GmEventDataTextFileUA.hh"
#include "GmRunDataTextFileUA.hh"
#include "GmStackDataTextFileUA.hh"
#include "GmStepDataBinFileUA.hh"
#include "GmTrackDataBinFileUA.hh"
#include "GmSecondaryTrackDataBinFileUA.hh"
#include "GmEventDataBinFileUA.hh"
#include "GmRunDataBinFileUA.hh"
#include "GmStackDataBinFileUA.hh"
#include "GmStepDataCoutUA.hh"
#include "GmTrackDataCoutUA.hh"
#include "GmSecondaryTrackDataCoutUA.hh"
#include "GmEventDataCoutUA.hh"
#include "GmRunDataCoutUA.hh"
#include "GmStackDataCoutUA.hh"

#include "GmNumericDataFilter.hh"
#include "GmStringDataFilter.hh"

#include "GmClassifierByNumericData.hh"
#include "GmClassifierByStringData.hh"
#include "GmClassifierByDistribution.hh"


#ifdef ROOT5
#include "Reflex/PluginService.h"

PLUGINSVC_FACTORY(GmStepDataHistosUA,GmUserAction*())
PLUGINSVC_FACTORY(GmTrackDataHistosUA,GmUserAction*())
PLUGINSVC_FACTORY(GmSecondaryTrackDataHistosUA,GmUserAction*())
PLUGINSVC_FACTORY(GmEventDataHistosUA,GmUserAction*())
PLUGINSVC_FACTORY(GmRunDataHistosUA,GmUserAction*())
PLUGINSVC_FACTORY(GmStackDataHistosUA,GmUserAction*())
PLUGINSVC_FACTORY(GmStepDataTextFileUA,GmUserAction*())
PLUGINSVC_FACTORY(GmTrackDataTextFileUA,GmUserAction*())
PLUGINSVC_FACTORY(GmSecondaryTrackDataTextFileUA,GmUserAction*())
PLUGINSVC_FACTORY(GmEventDataTextFileUA,GmUserAction*())
PLUGINSVC_FACTORY(GmRunDataTextFileUA,GmUserAction*())
PLUGINSVC_FACTORY(GmStackDataTextFileUA,GmUserAction*())
PLUGINSVC_FACTORY(GmStepDataBinFileUA,GmUserAction*())
PLUGINSVC_FACTORY(GmTrackDataBinFileUA,GmUserAction*())
PLUGINSVC_FACTORY(GmSecondaryTrackDataBinFileUA,GmUserAction*())
PLUGINSVC_FACTORY(GmEventDataBinFileUA,GmUserAction*())
PLUGINSVC_FACTORY(GmRunDataBinFileUA,GmUserAction*())
PLUGINSVC_FACTORY(GmStackDataBinFileUA,GmUserAction*())
PLUGINSVC_FACTORY(GmStepDataCoutUA,GmUserAction*())
PLUGINSVC_FACTORY(GmTrackDataCoutUA,GmUserAction*())
 
PLUGINSVC_FACTORY(GmSecondaryTrackDataCoutUA,GmUserAction*())
PLUGINSVC_FACTORY(GmEventDataCoutUA,GmUserAction*())
PLUGINSVC_FACTORY(GmRunDataCoutUA,GmUserAction*())
PLUGINSVC_FACTORY(GmStackDataCoutUA,GmUserAction*())

#ifndef GAMOS_NO_ROOT
#include "GmDataTTreeUA.hh"
PLUGINSVC_FACTORY(GmDataTTreeUA,GmUserAction*())
#endif

PLUGINSVC_FACTORY(GmNumericDataFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmStringDataFilter,GmVFilter*(G4String))

PLUGINSVC_FACTORY(GmClassifierByNumericData,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierByStringData,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierByDistribution,GmVClassifier*(G4String))

#else

#include "SEAL_Foundation/PluginManager/PluginManager/ModuleDef.h"
DEFINE_SEAL_MODULE ();

#include "GamosCore/GamosUserActionMgr/include/GmUserActionFactory.hh"
//----- DATA USERS
DEFINE_GAMOS_USER_ACTION(GmStepDataHistosUA);
DEFINE_GAMOS_USER_ACTION(GmTrackDataHistosUA);
DEFINE_GAMOS_USER_ACTION(GmSecondaryTrackDataHistosUA);
DEFINE_GAMOS_USER_ACTION(GmEventDataHistosUA);
DEFINE_GAMOS_USER_ACTION(GmRunDataHistosUA);
DEFINE_GAMOS_USER_ACTION(GmStackDataHistosUA);
DEFINE_GAMOS_USER_ACTION(GmStepDataTextFileUA);
DEFINE_GAMOS_USER_ACTION(GmTrackDataTextFileUA);
DEFINE_GAMOS_USER_ACTION(GmSecondaryTrackDataTextFileUA);
DEFINE_GAMOS_USER_ACTION(GmEventDataTextFileUA);
DEFINE_GAMOS_USER_ACTION(GmRunDataTextFileUA);
DEFINE_GAMOS_USER_ACTION(GmStackDataTextFileUA);
DEFINE_GAMOS_USER_ACTION(GmStepDataBinFileUA);
DEFINE_GAMOS_USER_ACTION(GmTrackDataBinFileUA);
DEFINE_GAMOS_USER_ACTION(GmSecondaryTrackDataBinFileUA);
DEFINE_GAMOS_USER_ACTION(GmEventDataBinFileUA);
DEFINE_GAMOS_USER_ACTION(GmRunDataBinFileUA);
DEFINE_GAMOS_USER_ACTION(GmStackDataBinFileUA);
DEFINE_GAMOS_USER_ACTION(GmStepDataCoutUA);
DEFINE_GAMOS_USER_ACTION(GmTrackDataCoutUA);
DEFINE_GAMOS_USER_ACTION(GmSecondaryTrackDataCoutUA);
DEFINE_GAMOS_USER_ACTION(GmEventDataCoutUA);
DEFINE_GAMOS_USER_ACTION(GmRunDataCoutUA);
DEFINE_GAMOS_USER_ACTION(GmStackDataCoutUA);

#ifndef GAMOS_NO_ROOT
#include "GmDataTTreeUA.hh"
DEFINE_GAMOS_USER_ACTION(GmDataTTreeUA);
#endif

#include "GamosCore/GamosBase/Base/include/GmFilterFactory.hh"

DEFINE_GAMOS_FILTER(GmNumericDataFilter);
DEFINE_GAMOS_FILTER(GmStringDataFilter);

#include "GamosCore/GamosBase/Base/include/GmClassifierFactory.hh"

DEFINE_GAMOS_CLASSIFIER(GmClassifierByNumericData);
DEFINE_GAMOS_CLASSIFIER(GmClassifierByStringData);
DEFINE_GAMOS_CLASSIFIER(GmClassifierByDistribution);
#endif
