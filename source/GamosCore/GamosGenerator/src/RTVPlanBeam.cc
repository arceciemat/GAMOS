#include "RTVPlanBeam.hh"
#include "RTVPlanControlPoint.hh"
#include "RTVPlanSource.hh"
#include "RTPlanMgr.hh"
#include "G4PrimaryVertex.hh"

#include "GamosCore/GamosGenerator/include/GmGenerVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include <fstream>
 
//-----------------------------------------------------------------------
RTVPlanControlPoint* RTVPlanBeam::GetControlPoint( G4int ii )
{
  if( ii < 0 || ii >= G4int(theControlPoints.size()) ) {
    G4Exception("RTVPlanSource::GetControlPoint",
		"",
		FatalErrorInArgument,
		G4String("Asking for control point " + GmGenUtils::itoa(ii) + " while there are only " + GmGenUtils::itoa(theControlPoints.size())).c_str());
  }

  return theControlPoints[ii];

}
