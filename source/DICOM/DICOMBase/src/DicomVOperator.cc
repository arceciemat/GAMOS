#include "DicomVOperator.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "DicomMgr.hh"

//-----------------------------------------------------------------------------
DicomVOperator::DicomVOperator( G4int iOrder, G4String name)
  : theName(name), theOrder(iOrder)
{
  //G4cout << " DicomVOperator " << theName << " = " << name << " iord " << iOrder << G4endl; //GDEB
  theDicomMgr = DicomMgr::GetInstance();
  theDicomMgr->AddOperator( this, iOrder );

  bAutomaticOperate = true;
}

