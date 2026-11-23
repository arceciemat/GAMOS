#include "GmTimeStudyEventUA.hh"
#include "GmTimeStudyMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include <math.h>

//------------------------------------------------------------------
GmTimeStudyEventUA::GmTimeStudyEventUA()
{
  GmTimeStudyMgr::GetInstance()->AddTimeStudy( this );
}

//------------------------------------------------------------------
GmTimeStudyEventUA::~GmTimeStudyEventUA()
{
}

//------------------------------------------------------------------
void GmTimeStudyEventUA::BeginOfEventAction( const G4Event* )
{
  theName = "GmTimeStudyEventUA";
  if( theClassifier ) {
    theName += "_" + theClassifier->GetName();
  }

  GmTimeStudyMgr::GetInstance()->InitManagers();

}

//------------------------------------------------------------------
void GmTimeStudyEventUA::EndOfEventAction( const G4Event* )
{
  Report();

  mut::const_iterator ite;
  for(ite = fTimers.begin(); ite != fTimers.end(); ite++) {
    delete (*ite).second;
  }
  fTimers.clear();
}

