#include "PETIOMgrCrystalID.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosSD/include/GmRecHit.hh"
#include "GamosCore/GamosSD/include/GmHitsEventMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"
#include "G4TouchableHistory.hh"
#include <iomanip>
#include <iostream>
#include <bitset>

//----------------------------------------------------------------------
PETIOMgrCrystalID* PETIOMgrCrystalID::theInstance = 0;

//----------------------------------------------------------------------
PETIOMgrCrystalID* PETIOMgrCrystalID::GetInstance()
{
  if( !theInstance ) {
    theInstance = new PETIOMgrCrystalID;
  }

  return theInstance;

}

//-----------------------------------------------------------------------
PETIOMgrCrystalID::PETIOMgrCrystalID() 
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

  theNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  theTouchable = new G4TouchableHistory;
  theShifts.push_back(1);
  theShifts.push_back(64*theShifts[0]); // crystals
  theShifts.push_back(1*theShifts[1]); // matrix
  theShifts.push_back(4*theShifts[2]); // detblock
  theShifts.push_back(4*theShifts[3]); // detModule
  //theShifts.push_back(2); // detBlockBlock
  
  theHitsEventMgr = GmHitsEventMgr::GetInstance();
}


//-----------------------------------------------------------------------
void PETIOMgrCrystalID::WriteEvent( const G4ThreeVector& posVtx, G4double EVtx, G4double tVtx, const DetRecHitSet& rh1, const DetRecHitSet& rh2, const G4String& name)
{

  if( theFileOut == 0 ) {
    G4Exception("PETIOMgrCrystalID::WriteEvent",
		"Wrong argument",
		FatalErrorInArgument,
		"File not opened, call OpenFileOut() first ");
  }

  PETOutput po;
  strcpy(po.name,name);
  po.name[0] = 'P';
  po.name[1] = 'E';
  po.name[2] = 'T';
  po.name[3] = ' ';
  G4ThreeVector pos1 = rh1.GetPosition();
  theNavigator->LocateGlobalPointAndUpdateTouchable( pos1, theTouchable, false );
  unsigned int ID1 = GetTouchableID( theTouchable );
  //  po.name = "PET ";
  po.name[4] = binary_char(ID1,0);
  po.name[5] = binary_char(ID1,1);
  G4ThreeVector pos2 = rh2.GetPosition();
  theNavigator->LocateGlobalPointAndUpdateTouchable( pos2, theTouchable, false );
  unsigned int ID2 = GetTouchableID( theTouchable );
  //  po.name = "PET ";
  po.name[6] = binary_char(ID2,0);
  po.name[7] = binary_char(ID2,1);
  //  G4cout << "PETIOMgrCrystalID::WriteEvent name " << po.name << G4endl; //GDEB

  po.xVtx = posVtx.x();
  po.yVtx = posVtx.y();
  po.zVtx = posVtx.z();
  po.EVtx = EVtx;
  po.tVtx = tVtx-theHitsEventMgr->GetInitialEventTime();
  po.x1 = pos1.x();
  po.y1 = pos1.y();
  po.z1 = pos1.z();
  po.E1 = rh1.GetEnergy();
  po.t1 = rh1.GetTime()-rh2.GetTime();
  po.x2 = pos2.x();
  po.y2 = pos2.y();
  po.z2 = pos2.z();
  po.E2 = rh2.GetEnergy();
  po.t2 = 0.;

  //  G4cout << theHitsEventMgr->GetInitialEventTime() <<" PETIOMgrCrystalID TIMES " << po.tVtx << " " << po.t1 << " " << po.t2 << " " << rh1.GetTime() << " " <<rh2.GetTime() << G4endl; //GDEB

  fwrite (&po, sizeof(struct PETOutput),1,theFileOut);

  if(bDumpCout) PrintEvent(" PETOutput: ", po, bCartesian);

}

//-----------------------------------------------------------------------
char PETIOMgrCrystalID::binary_char(unsigned int num, int iset )
{
  unsigned int n28 = std::pow(2,8);
  /*  if( num >= n28*n28 ) { 
    std::cerr << " PETIOMgrCrystalID::binary_char( " <<  num << " " << iset << " " << n28 << std::endl; 
    G4Exception("PETIOMgrCrystalID::binary_char",
		"",
		JustWarning,
		//		FatalException,
		("It cannot treat numbers bigger than "+GmGenUtils::itoa(n28*n28)+" number is="+GmGenUtils::itoa(num)).c_str());
		}*/
  unsigned numS = int(num/std::pow(n28,iset))%n28;
  std::bitset<8> bins;
  for( int ib = 0; ib < 8; ib++ ) {
    int val = (numS/std::pow(2,ib));
    //  bool res = (num/std::pow(2,ii))%2;
    bool res = val%2;
    bins[ib] = res;
    //     G4cout << " NUM BIN " << val << ":" << ib << " = " << res << G4endl;
  }
  char charbin = bins.to_ulong();
  //  G4cout << n28 << " CHAR BIN " << num << ":" << iset << "-> " << numS << " = " << charbin << G4endl;
  return charbin;
  
}

//-----------------------------------------------------------------------
unsigned int PETIOMgrCrystalID::GetTouchableID( G4TouchableHistory* touch )
{
  G4int hDepth = touch->GetHistoryDepth();
  unsigned int ID = 0;
  for( G4int ii = 0; ii < hDepth; ii++ ) {
    ID += theShifts[ii]*(touch->GetReplicaNumber(ii));
    //      G4cout << " PETIOMgrCrystalID::GetTouchableID ii=" << ii << " : " << theShifts[ii] << "*" << (touch->GetReplicaNumber(ii)) << " ID= " << ID << " " << touch->GetVolume(ii)->GetName() << G4endl; //GDEB
  }

  return ID;
}
  
//-----------------------------------------------------------------------
PETIOMgrCrystalID::~PETIOMgrCrystalID()
{
}

//-----------------------------------------------------------------------
void PETIOMgrCrystalID::ReadFile()
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
PETOutput PETIOMgrCrystalID::ReadEvent( G4bool& bEof )
{
  if( theFileIn == 0 ){
    G4Exception("PETIOMgrCrystalID::ReadEvent",
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
void PETIOMgrCrystalID::PrintEvent(const G4String& text, const PETOutput& po, G4bool bCart )
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
