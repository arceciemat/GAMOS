#include "RTIonPlanSubControlPoint.hh"
#include "RTIonPlanControlPoint.hh"
#include "GmGenerVerbosity.hh"

#include "GamosCore/GamosGenerator/include/GmGenerVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "G4tgrUtils.hh"
#ifndef GAMOS_NO_ROOT
#include "TH2F.h"
#include "TCanvas.h"
#include "TEllipse.h"
#include "TColor.h"
#endif


//-----------------------------------------------------------------------
RTIonPlanSubControlPoint::RTIonPlanSubControlPoint( const std::vector<G4String> wl, RTIonPlanControlPoint* CP)
{  
  theControlPoint = CP;
  
  thePosX = GmGenUtils::GetValue(wl[0]);
  thePosY = GmGenUtils::GetValue(wl[1]);
  theMeterset = GmGenUtils::GetValue(wl[2]);

  if( wl.size() != 3 ) {
	G4Exception("RTIonPlanSubControlPoint",
		    "",
		    FatalException,
		    (G4String("Number of Words != 3 ") + GmGenUtils::itoa(wl.size())).c_str());
  }
}

//-----------------------------------------------------------------------
RTBeamStateData RTIonPlanSubControlPoint::GetBeamStateData()
{
  RTBeamStateData bsdata;
  bsdata.PositionX = thePosX;
  bsdata.PositionY = thePosY;
  
  return theControlPoint->GetBeamStateData(bsdata);
}
