#include "GmGeomTextDumperUA.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "G4Run.hh"
#include "G4tgbGeometryDumper.hh"

#include "GmGeometryUtils.hh"

GmGeomTextDumperUA::GmGeomTextDumperUA()
{
  theFileName = GmParameterMgr::GetInstance()->GetStringValue("GmGeomTextDumperUA:OutputName","text.out");
  
}

void GmGeomTextDumperUA::BeginOfRunAction( const G4Run* )
{
  //  GmGeometryUtils::GetInstance()->DumpHierarchyTreePVLV();
  G4tgbGeometryDumper::GetInstance()->DumpGeometry(theFileName);

}
