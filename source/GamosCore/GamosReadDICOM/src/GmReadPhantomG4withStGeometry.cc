#include "globals.hh"
#include "GmReadPhantomG4withStGeometry.hh"

#include <fstream>
#include "GmReadPhantomStMgr.hh"

//---------------------------------------------------------------------------
GmReadPhantomG4withStGeometry::GmReadPhantomG4withStGeometry()
{
}

//---------------------------------------------------------------------------
GmReadPhantomG4withStGeometry::~GmReadPhantomG4withStGeometry()
{
}

//---------------------------------------------------------------------------
void GmReadPhantomG4withStGeometry::ReadPV( GmFileIn& fin )
{
  GmReadPhantomStMgr::GetInstance()->ReadStructG4(fin, nVoxelX, nVoxelY, nVoxelZ); 

}
