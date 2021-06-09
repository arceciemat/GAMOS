#include "CCIOMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include <iomanip>
#include <iostream>

//----------------------------------------------------------------------
CCIOMgr* CCIOMgr::theInstance = 0;

//----------------------------------------------------------------------
CCIOMgr* CCIOMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new CCIOMgr;
  }

  return theInstance;

}

//-----------------------------------------------------------------------
CCIOMgr::CCIOMgr() 
{
  theOutputType = "CompCam";

  bBinFile = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(theOutputType+":BinFile",1));

  if( bBinFile ) {
   theCCIOMgr = new GmIObinMgr;
  } else {
   theCCIOMgr = new GmIOtextMgr;
  }

  Init();

  G4String fnout = theOutputType+":DumpToCout";
  bDumpCout = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(fnout,G4bool(0)));
  
  fnout = theOutputType+":DumpCartesian";
  bCartesian = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(fnout,G4bool(0)));
}

//-----------------------------------------------------------------------
void CCIOMgr::WriteEvent( const G4ThreeVector& pos1, const G4double& energy1, const G4ThreeVector& pos2,const G4double& energy2, const G4String& name )
{
  if( theFileOut == 0 ) {
    G4Exception("CCIOMgr::WriteEvent",
		"Error",
		FatalException,
		"File not opened, call OpenFileOut() first ");
  }

  CCOutput po;
  strcpy(po.name,name);
  po.x1 = pos1.x();
  po.y1 = pos1.y();
  po.z1 = pos1.z();
  po.e1 = energy1;
  po.x2 = pos2.x();
  po.y2 = pos2.y();
  po.z2 = pos2.z();
  po.e2 = energy2;
 
  fwrite (&po, sizeof(struct CCOutput),1,theFileOut);

  if(bDumpCout) PrintEvent(" CCOutput: ", po, bCartesian);

}

//-----------------------------------------------------------------------
CCIOMgr::~CCIOMgr()
{
}

//-----------------------------------------------------------------------
void CCIOMgr::ReadFile()
{
  if( !theFileIn ) OpenFileIn();

  CCOutput po;
  G4bool bEof;
  for(;;) {
    po = ReadEvent( bEof );
    if(bDumpCout) PrintEvent(" CCOutput: ", po, bCartesian);
    if( bEof ) break;
  }

}

//-----------------------------------------------------------------------
CCOutput CCIOMgr::ReadEvent( G4bool& bEof )
{
  if( theFileIn == 0 ){
    G4Exception("CCIOMgr::ReadEvent",
		"Error",
		FatalException,
		"File not opened, call OpenFileIn() first ");
  }

  CCOutput po;
  fread (&po, sizeof(struct CCOutput),1,theFileIn);
  if ( feof( theFileIn ) ) {
    bEof = TRUE;
  } else {
    bEof = FALSE;
  }

  return po; 

}

//-----------------------------------------------------------------------
void CCIOMgr::PrintEvent(const G4String& text, const CCOutput& po, G4bool bCart )
{
  G4cout<< text << po.name;
  G4ThreeVector p1(po.x1, po.y1, po.z1 );
  G4ThreeVector p2(po.x2, po.y2, po.z2 );

  if( bCart ) {
    PrintCart(" p1",p1);
    G4cout<<" en1 "<<po.e1;
    PrintCart(" p2",p2);
    G4cout<<" en2 "<<po.e2;

  } else {
    PrintCyl(" p1",p1);
    G4cout<<" en1 "<<po.e1;
    PrintCyl(" p2",p2);
    G4cout<<" en2 "<<po.e2;
  }
  G4cout << G4endl;
}
