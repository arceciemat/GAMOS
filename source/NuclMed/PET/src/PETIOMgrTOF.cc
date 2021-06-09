#include "PETIOMgrTOF.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosSD/include/GmRecHit.hh"
#include <iomanip>
#include <iostream>

//----------------------------------------------------------------------
PETIOMgrTOF* PETIOMgrTOF::theInstance = 0;

//----------------------------------------------------------------------
PETIOMgrTOF* PETIOMgrTOF::GetInstance()
{
  if( !theInstance ) {
    theInstance = new PETIOMgrTOF;
  }

  return theInstance;

}

//-----------------------------------------------------------------------
PETIOMgrTOF::PETIOMgrTOF() 
{
  theFileOut = 0;
  theOutputType = "PET";
  Init();

  G4String fnout = theOutputType+":DumpToCout";
  
  fnout = theOutputType+":DumpToCout";
  bDumpCout = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(fnout,G4bool(0)));
  
  fnout = theOutputType+":DumpCartesian";
  bCartesian = G4bool(GmParameterMgr::GetInstance()->GetNumericValue(fnout,G4bool(0)));

  /*  //- std::ifstream* theFileIn = new std::ifstream(theFileName,std::ios::binary|std::ios::app);
  PetOutput po;
  FILE *theFileIn = fopen("pet.out","rb");
  for(;;) {
    fread (&po, sizeof(struct PetOutput),1,theFileIn);
// theFileIn->read(reinterpret_cast<char *>(&po),sizeof(PetOutput)); 
    G4cout<< " PETOutput:Read " << po.name;
    G4cout<<" "<< po.x1 << " " << po.y1 << " " << po.z1;
    G4cout<<" "<< po.x2 << " " << po.y2 << " " << po.z2 <<G4endl;
    
    if ( feof( theFileIn ) ) break;
    // return po;
    
  }
  */
}


//-----------------------------------------------------------------------
void PETIOMgrTOF::WriteEvent( const G4ThreeVector& posVtx, G4double EVtx, G4double tVtx, const DetRecHitSet& rh1, const DetRecHitSet& rh2, const G4String& name)
{

  if( theFileOut == 0 ) {
    G4Exception("PETIOMgrTOF::WriteEvent",
		"Wrong argument",
		FatalErrorInArgument,
		"File not opened, call OpenFileOut() first ");
  }

  PETOutput po;
  strcpy(po.name,name);
  //  po.name = "PET ";
  po.xVtx = posVtx.x();
  po.yVtx = posVtx.y();
  po.zVtx = posVtx.z();
  po.EVtx = EVtx;
  po.tVtx = tVtx;
  G4ThreeVector pos1 = rh1.GetPosition();
  po.x1 = pos1.x();
  po.y1 = pos1.y();
  po.z1 = pos1.z();
  po.E1 = rh1.GetEnergy();
  po.t1 = rh1.GetTime();
  G4ThreeVector pos2 = rh2.GetPosition();
  po.x2 = pos2.x();
  po.y2 = pos2.y();
  po.z2 = pos2.z();
  po.E2 = rh2.GetEnergy();
  po.t2 = rh2.GetTime();

  //-  theFile->write(reinterpret_cast<char *>(&po),sizeof(PetOutput));

  fwrite (&po, sizeof(struct PETOutput),1,theFileOut);

  if(bDumpCout) PrintEvent(" PETOutput: ", po, bCartesian);

}

//-----------------------------------------------------------------------
PETIOMgrTOF::~PETIOMgrTOF()
{
}

//-----------------------------------------------------------------------
void PETIOMgrTOF::ReadFile()
{
  if( !theFileIn ) OpenFileIn();

  PETOutput po;
  G4bool bEof;
  for(;;) {
    po = ReadEvent( bEof );
    // theFileIn->read(reinterpret_cast<char *>(&po),sizeof(PetOutput)); 
    if(bDumpCout) PrintEvent(" PETOutput: ", po, bCartesian);
    if( bEof ) break;
  }

}

//-----------------------------------------------------------------------
PETOutput PETIOMgrTOF::ReadEvent( G4bool& bEof )
{
  if( theFileIn == 0 ){
    G4Exception("PETIOMgrTOF::ReadEvent",
		"Wrong argument",
		FatalErrorInArgument,
		"File not opened, call OpenFileIn() first ");
  }

  PETOutput po;
  fread (&po, sizeof(struct PETOutput),1,theFileIn);
  if ( feof( theFileIn ) ) {
    bEof = TRUE;
  } else {
    bEof = FALSE;
  }

  return po; 

}

//-----------------------------------------------------------------------
void PETIOMgrTOF::PrintEvent(const G4String& text, const PETOutput& po, G4bool bCart )
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
  G4cout << G4endl;
}
