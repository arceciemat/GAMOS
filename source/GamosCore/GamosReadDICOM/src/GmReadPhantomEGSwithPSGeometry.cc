#include "globals.hh"
#include "GmReadPhantomEGSwithPSGeometry.hh"

#include <fstream>
#include "GmReadPhantomSVMgr.hh"

//---------------------------------------------------------------------------
GmReadPhantomEGSwithPSGeometry::GmReadPhantomEGSwithPSGeometry()
{
}

//---------------------------------------------------------------------------
GmReadPhantomEGSwithPSGeometry::~GmReadPhantomEGSwithPSGeometry()
{
}

//---------------------------------------------------------------------------
void GmReadPhantomEGSwithPSGeometry::ReadPV( std::ifstream& fin )
{
  GmReadPhantomSVMgr::GetInstance()->ReadPVEGS(fin, nVoxelX, nVoxelY, nVoxelZ); 

}
