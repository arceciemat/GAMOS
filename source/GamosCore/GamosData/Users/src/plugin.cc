#include "Reflex/PluginService.h"

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

#include "GmNumericDataFilter.hh"
#include "GmStringDataFilter.hh"

PLUGINSVC_FACTORY(GmNumericDataFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmStringDataFilter,GmVFilter*(G4String))

#include "GmClassifierByNumericData.hh"
#include "GmClassifierByStringData.hh"
#include "GmClassifierByDistribution.hh"

PLUGINSVC_FACTORY(GmClassifierByNumericData,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierByStringData,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierByDistribution,GmVClassifier*(G4String))
