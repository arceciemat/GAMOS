
#include "GmDumpGeometryUA.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//----------------------------------------------------------------
GmDumpGeometryUA::GmDumpGeometryUA()
{
}

//----------------------------------------------------------------
void GmDumpGeometryUA::BeginOfRunAction( const G4Run* )
{
  G4int verbosity = GmParameterMgr::GetInstance()->GetNumericValue("GmDumpGeometryUA:Verbosity",33);

  GmGeometryUtils* geomUtils = GmGeometryUtils::GetInstance();
  if( verbosity >= 1 ) {
    geomUtils->DumpSummary();
  }
  if( verbosity >= 2 ) {
    geomUtils->DumpMaterialList(G4cout, verbosity);
    geomUtils->DumpG4LVList(G4cout);
    //    geomUtils->DumpTouchableList(G4cout, verbosity);
  }
  if( verbosity >= 3 ) {
    geomUtils->DumpG4LVTree();
    geomUtils->DumpG4PVLVTree();
  }
  
}

