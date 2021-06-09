#include "SPECTIOMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include <iomanip>
#include <iostream>

//----------------------------------------------------------------------
SPECTIOMgr* SPECTIOMgr::theInstance = 0;

//----------------------------------------------------------------------
SPECTIOMgr* SPECTIOMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new SPECTIOMgr;
  }

  return theInstance;

}

//-----------------------------------------------------------------------
SPECTIOMgr::SPECTIOMgr() 
{

  theOutputType = "SPECT";
  Init();

  G4String fnout = theOutputType+":DumpToCout";
  
  fnout = theOutputType+":DumpToCout";
  bDumpCout = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(fnout,G4bool(0)));
  
  fnout = theOutputType+":DumpCartesian";
  bCartesian = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(fnout,G4bool(0)));

}


//-----------------------------------------------------------------------
void SPECTIOMgr::WriteEvent( const G4ThreeVector& posVtx, const G4ThreeVector& pos1 , const G4ThreeVector& pos2, const G4double energy, G4int evtClass, const G4String& name )
{
  if( theFileOut == 0 ) {
    G4Exception("SPECTIOMgr::WriteEvent",
		"Error",
		FatalException,
		"File not opened, call OpenFileOut() first ");
  }

  SPECTOutput po;
  strcpy(po.name,name);
  //  po.name = "PET ";
  po.xVtx = posVtx.x();
  po.yVtx = posVtx.y();
  po.zVtx = posVtx.z();
  po.x1 = pos1.x();
  po.y1 = pos1.y();
  po.z1 = pos1.z();
  po.x2 = pos2.x();
  po.y2 = pos2.y();
  po.z2 = pos2.z();
  po.energy = energy;
  po.eventClass = evtClass;

  //-  theFile->write(reinterpret_cast<char *>(&po),sizeof(SPECTOutput));

  fwrite (&po, sizeof(struct SPECTOutput),1,theFileOut);

  if(bDumpCout) PrintEvent(" SPECTOutput: ", po, bCartesian);

}

//-----------------------------------------------------------------------
SPECTIOMgr::~SPECTIOMgr()
{
}

//-----------------------------------------------------------------------
void SPECTIOMgr::ReadFile()
{
  if( !theFileIn ) OpenFileIn();

  SPECTOutput po;
  G4bool bEof;
  for(;;) {
    po = ReadEvent( bEof );
    if(bDumpCout) PrintEvent(" SPECTOutput: ", po, bCartesian);
    if( bEof ) break;
  }

}

//-----------------------------------------------------------------------
SPECTOutput SPECTIOMgr::ReadEvent( G4bool& bEof )
{
  if( theFileIn == 0 ){
    G4Exception("SPECTIOMgr::ReadEvent",
		"Error",
		FatalException,
		"File not opened, call OpenFileIn() first ");
  }

  SPECTOutput po;
  fread (&po, sizeof(struct SPECTOutput),1,theFileIn);
  if ( feof( theFileIn ) ) {
    bEof = TRUE;
  } else {
    bEof = FALSE;
  }

  return po; 

}

//-----------------------------------------------------------------------
void SPECTIOMgr::PrintEvent(const G4String& text, const SPECTOutput& po, G4bool bCart )
{
  G4cout<< text << po.name;
  G4ThreeVector pVtx(po.xVtx, po.yVtx, po.zVtx );
  G4ThreeVector p1(po.x1, po.y1, po.z1 );
  G4ThreeVector p2(po.x2, po.y2, po.z2 );

  if( bCart ) {
    PrintCart(" Vtx",pVtx);
    PrintCart(" p1",p1);
    PrintCart(" p2",p2);
  } else {
    PrintCyl(" Vtx",pVtx);
    PrintCyl(" p1",p1);
    PrintCyl(" p2",p2);
  }
  G4cout << " " << po.energy;
  G4cout << " " << po.eventClass;
  G4cout << G4endl;
}
