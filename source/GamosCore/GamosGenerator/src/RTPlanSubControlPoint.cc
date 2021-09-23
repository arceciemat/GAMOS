#include "RTPlanSubControlPoint.hh"
#include "RTPlanControlPoint.hh"
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
RTPlanSubControlPoint::RTPlanSubControlPoint( G4double ms, RTPlanControlPoint* CP)
{  
  theControlPoint = CP;
  //  G4cout << this << " RTPlanSubControlPoint::RTPlanSubControlPoint " << CP << G4endl; //GDEB
  theMeterset = ms;
}

//-----------------------------------------------------------------------
RTBeamStateData RTPlanSubControlPoint::GetBeamStateData()
{
  RTBeamStateData bsdata;
  return theControlPoint->GetBeamStateData(bsdata);
}

