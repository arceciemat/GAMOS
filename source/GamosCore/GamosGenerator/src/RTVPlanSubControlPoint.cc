#include "RTVPlanSubControlPoint.hh"
#include "RTVPlanBeam.hh"
#include "GmGenerVerbosity.hh"

#include "GamosCore/GamosGenerator/include/GmGenerVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"


//-----------------------------------------------------------------------
RTVPlanSubControlPoint::RTVPlanSubControlPoint()
{  
}

//-----------------------------------------------------------------------
RTVPlanBeam* RTVPlanSubControlPoint::GetBeam() const
{
  //  G4cout << this << " RTVPlanSubControlPoint::GetBeam() " << theControlPoint << G4endl; //GDEB
  return theControlPoint->GetBeam();
}
