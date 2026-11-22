#include "RTFilterByEILong.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "RTVerbosity.hh"
#include "RTExtraInfoMgr.hh"

//---------------------------------------------------------------
RTFilterByEILong::RTFilterByEILong(G4String name)
  :GmVFilter(name)
{
  theExtraInfoMgr = RTExtraInfoMgr::GetInstance();
}


//---------------------------------------------------------------
RTFilterByEILong::~RTFilterByEILong()
{ 
}


//---------------------------------------------------------------
G4bool RTFilterByEILong::AcceptStep(const G4Step* aStep)
{
  return AcceptTrack( aStep->GetTrack() );
}

//---------------------------------------------------------------
G4bool RTFilterByEILong::AcceptTrack(const G4Track*)
{
  G4int nWords = theExtraInfoMgr->GetNumberOfLongs();
  if( nWords == 0 ) return 0;

  G4bool bFound;
  std::set<G4int>::const_iterator ite;
  for( ite = theIndices.begin(); ite != theIndices.end(); ite++ ){
    bFound = false;
    G4int iw = (*ite)/32;
    if( theExtraInfoMgr->GetExtraInfoLong(iw) & 1<< *ite ) {
      bFound = true;
    }
#ifndef GAMOS_NO_VERBOSE
    if( RTVerb(debugVerb) ) G4cout << " RTFilterByEILong::AcceptStep " << *ite << " = " << bFound << " in " << theExtraInfoMgr->GetExtraInfoLong(iw) << G4endl;
#endif
    if( !bFound ) return FALSE;
  }

  return TRUE;
}


//---------------------------------------------------------------
void RTFilterByEILong::show()
{
    G4cout << "----RTFilterByEILong ------"<<G4endl;
}


//---------------------------------------------------------------
void RTFilterByEILong::SetParameters( std::vector<G4String>& params)
{
  if( params.size() < 1  ){
    G4String parastr;
    for( unsigned int ii = 0; ii < params.size(); ii++ ){
      parastr += params[ii] + " ";
    }
    G4Exception("RTFilterByEILong::SetParameters",
		"There should be at least one parameter: INDEX_1",
		FatalErrorInArgument,
		"There is no parameter");
  }
  
  for( unsigned int ii = 0; ii < params.size(); ii++ ){
    theIndices.insert( atoi(params[ii]) );
  }
}
