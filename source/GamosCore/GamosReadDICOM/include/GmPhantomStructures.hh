#ifndef GmVolumeFilters_hh
#define GmVolumeFilters_hh
#include "GmTouchablePhantomStructure.hh"
#include "GamosCore/GamosBase/Filters/include/GmVVolumeConditionFilter.hh"
#include "GamosCore/GamosBase/Filters/include/GmGeomConditionEnter.hh"
#include "GamosCore/GamosBase/Filters/include/GmGeomConditionExit.hh"
#include "GamosCore/GamosBase/Filters/include/GmGeomConditionStart.hh"
#include "GamosCore/GamosBase/Filters/include/GmGeomConditionEnd.hh"
#include "GamosCore/GamosBase/Filters/include/GmGeomConditionIn.hh"
#include "GamosCore/GamosBase/Filters/include/GmGeomConditionTraverse.hh"

class GmEnterPhantomStructureFilter : public GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchablePhantomStructure>
{
public:
  GmEnterPhantomStructureFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchablePhantomStructure>(name) {}
};

class GmExitPhantomStructureFilter : public GmVVolumeConditionFilter<GmGeomConditionExit,GmTouchablePhantomStructure>
{
public:
  GmExitPhantomStructureFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionExit,GmTouchablePhantomStructure>(name) {}
};

class GmStartPhantomStructureFilter : public GmVVolumeConditionFilter<GmGeomConditionStart,GmTouchablePhantomStructure>
{
public:
  GmStartPhantomStructureFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionStart,GmTouchablePhantomStructure>(name) {}
};

class GmEndPhantomStructureFilter : public GmVVolumeConditionFilter<GmGeomConditionEnd,GmTouchablePhantomStructure>
{
public:
  GmEndPhantomStructureFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnd,GmTouchablePhantomStructure>(name) {}
};

class GmTraversePhantomStructureFilter : public GmVVolumeConditionFilter<GmGeomConditionTraverse,GmTouchablePhantomStructure>
{
public:
  GmTraversePhantomStructureFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionTraverse,GmTouchablePhantomStructure>(name) {}
};
class GmInPhantomStructureFilter : public GmVVolumeConditionFilter<GmGeomConditionIn,GmTouchablePhantomStructure>
{
public:
  GmInPhantomStructureFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionIn,GmTouchablePhantomStructure>(name) {}
};

#endif
