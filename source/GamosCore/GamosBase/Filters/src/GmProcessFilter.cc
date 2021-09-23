#include "GmProcessFilter.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"

//--------------------------------------------------------------------
GmProcessFilter::GmProcessFilter(G4String name)
  :GmVFilter(name)
{
  theProcesses.clear();
}

//--------------------------------------------------------------------
GmProcessFilter::~GmProcessFilter()
{ 
  theProcesses.clear();
}

//--------------------------------------------------------------------
G4bool GmProcessFilter::AcceptStep(const G4Step* aStep)
{
  G4VProcess* proc = const_cast<G4VProcess*>(aStep->GetPostStepPoint()->GetProcessDefinedStep());
  if( theProcesses.find(proc) != theProcesses.end() ) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << GetName() << " GmProcessFilter::AcceptStep 1 " << G4endl;
#endif
    return TRUE;
  }

#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << GetName() << " GmProcessFilter::AcceptStep 0 " << proc->GetProcessName() << G4endl;
#endif
  return FALSE;
}

//--------------------------------------------------------------------
void GmProcessFilter::show()
{
  G4cout << "----G4ProcessNameFilter volume list------"<<G4endl;
  std::set<G4VProcess*>::const_iterator iter;
  for ( iter = theProcesses.begin(); iter != theProcesses.end(); iter++ ){
    G4cout << (*iter)->GetProcessName() << G4endl;
  }
  G4cout << "-------------------------------------------"<<G4endl;
}

//--------------------------------------------------------------------
void GmProcessFilter::SetParameters( std::vector<G4String>& params)
{
  if( params.size() < 1  ){
    G4Exception("GmProcessFilter::SetParameters",
		"There should be at least one parameter: PROCESS_NAME",
		FatalErrorInArgument,
		"There is no parameter");
  }
  
  for( unsigned int ii = 0; ii < params.size(); ii++ ){
    std::vector<G4VProcess*> procs = GmGetParticleMgr::GetInstance()->GetG4ProcessList( params[ii] );
    for(unsigned int jj = 0; jj < procs.size(); jj++ ){
      theProcesses.insert( procs[jj] );
    }
  }

}

