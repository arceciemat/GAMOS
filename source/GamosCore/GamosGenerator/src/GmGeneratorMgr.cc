#include "GmGeneratorMgr.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4UImanager.hh"
#include "G4RadioactiveDecay.hh"
#include "G4ProcessManager.hh"
#include "G4Ions.hh"
#include "G4IonTable.hh"
#include "G4NistManager.hh"

GmGeneratorMgr*  GmGeneratorMgr::theInstance;


//----------------------------------------------------------------------
GmGeneratorMgr* GmGeneratorMgr::GetInstance()
{
  if( !theInstance ) theInstance = new GmGeneratorMgr;

  return theInstance;

}

//----------------------------------------------------------------------
GmGeneratorMgr::GmGeneratorMgr()
{

}

//--------------------------------------------------------------------
GmGeneratorMgr::~GmGeneratorMgr() 
{
}

