#include "RTExtraInfoProviderZLast.hh"
#include "RTExtraInfoMgr.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserRunActionList.hh"
#include "G4RunManager.hh"

//----------------------------------------------------------------
RTVExtraInfoProvider::RTVExtraInfoProvider()
{ 
  //-----  Extra info providers should be executed before RTPhaseSpaceUA
  GmUserRunActionList* runActionList = (GmUserRunActionList*)(G4RunManager::GetRunManager()->GetUserRunAction());
  G4int nPHSP = runActionList->CountActions("RTPhaseSpaceUA");

  if( nPHSP != 0 ) {
    G4Exception("RTVExtraInfoProvider::RTVExtraInfoProvider ",
		"Creating an extra info after when RTPhaseSpaceUA, it should be done before",
		FatalException,
		"change order of '/gamos/userAction' commands");
  }

}


#include <fstream>
//---------------------------------------------------------------------
void RTVExtraInfoProvider::BeginOfRunAction( const G4Run* )
{
  InitVariables();
  std::ofstream fout;
  if( theOrder == 0 ) {
    fout.open(theFileName);
  } else {
    fout.open(theFileName,std::ios_base::app);
  }
  DumpIndices(fout);
  DumpIndices(G4cout);
  fout.close();

}
