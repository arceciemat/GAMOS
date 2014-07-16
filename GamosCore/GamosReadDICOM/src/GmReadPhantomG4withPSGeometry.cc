#include "globals.hh"
#include "GmReadPhantomG4withPSGeometry.hh"

#include <fstream>
#include "GmReadPhantomSVMgr.hh"

//---------------------------------------------------------------------------
GmReadPhantomG4withPSGeometry::GmReadPhantomG4withPSGeometry()
{
}

//---------------------------------------------------------------------------
GmReadPhantomG4withPSGeometry::~GmReadPhantomG4withPSGeometry()
{
}

//---------------------------------------------------------------------------
void GmReadPhantomG4withPSGeometry::ReadPV( std::ifstream& fin )
{
  GmReadPhantomSVMgr::GetInstance()->ReadPVG4(fin, nVoxelX, nVoxelY, nVoxelZ); 

}
