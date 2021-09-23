#include "globals.hh"
#include "GmReadPhantomEGSwithStGeometry.hh"

#include <fstream>
#include "GmReadPhantomStMgr.hh"

//---------------------------------------------------------------------------
GmReadPhantomEGSwithStGeometry::GmReadPhantomEGSwithStGeometry()
{
}

//---------------------------------------------------------------------------
GmReadPhantomEGSwithStGeometry::~GmReadPhantomEGSwithStGeometry()
{
}

//---------------------------------------------------------------------------
void GmReadPhantomEGSwithStGeometry::ReadPV( GmFileIn& fin )
{
  GmReadPhantomStMgr::GetInstance()->ReadStructEGS(fin, nVoxelX, nVoxelY, nVoxelZ); 

}
