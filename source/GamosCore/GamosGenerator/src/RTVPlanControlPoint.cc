#include "RTVPlanControlPoint.hh"
#include "GmGenerVerbosity.hh"
#include "CLHEP/Units/SystemOfUnits.h"

//-----------------------------------------------------------------------
RTVPlanControlPoint::RTVPlanControlPoint()
{
  theScanningSpotSize = 0.; // initialise it
}

//-----------------------------------------------------------------------
G4bool RTVPlanControlPoint::operator==( const RTVPlanControlPoint& right ) const 
{
  if( GetParam("DoseRateSet") == right.GetParam("DoseRateSet") &&
      GetParam("GantryAngle") == right.GetParam("GantryAngle") &&
      GetParam("IsocenterPosition_X") == right.GetParam("IsocenterPosition_X") &&
      GetParam("IsocenterPosition_Y") == right.GetParam("IsocenterPosition_Y") &&
      GetParam("IsocenterPosition_Z") == right.GetParam("IsocenterPosition_Z") &&
      GetParam("LimitingDeviceAngle") == right.GetParam("LimitingDeviceAngle") &&
      GetParam("NominalBeamEnergy") == right.GetParam("NominalBeamEnergy") &&
      GetParam("PatientSupportAngle") == right.GetParam("PatientSupportAngle") &&
      GetParam("SourceToSurfaceDistance") == right.GetParam("SourceToSurfaceDistance") &&
      GetParam("TableTopEccentricAngle") == right.GetParam("TableTopEccentricAngle") ) {
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(testVerb) ) G4cout << " RTVPlanControlPoint::operator== RETURNS TRUE " << G4endl;
#endif
    
    return true;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(testVerb) ) {
      G4cout << " RTVPlanControlPoint::operator== RETURNS FALSE " << G4endl; 
      G4cout << " FIRST CONTROL POINT: " << *this << G4endl; 
      G4cout << " SECOND CONTROL POINT: " << right << G4endl;
    }
#endif
    return false;
  }
}

//-----------------------------------------------------------------------
std::ostream& operator<<(std::ostream& out, const RTVPlanControlPoint& cp)
{
  std::map<G4String,G4String>::const_iterator ites;
  std::map<G4String,G4String> paramStrs = cp.GetParamStrs();
  out << "@@ RTVPlanControlPoint " << cp.GetIndex() << G4endl;
  out << " NominalBeamEnergy= " << cp.GetNominalBeamEnergy()
      << " GantryAngle= " << cp.GetGantryAngle()/CLHEP::deg
      << " GantryPitchAngle= " << cp.GetGantryPitchAngle()/CLHEP::deg
      << " ScanningSpotSize= " << cp.GetScanningSpotSize()
      << " NumberOfPaintings= " << cp.GetNumberOfPaintings()
      << " SourceAxisDistanceX= " << cp.GetSourceAxisDistanceX()
      << " SourceAxisDistanceY= " << cp.GetSourceAxisDistanceY()
      << G4endl;   
  //  G4int iCurrentBeamState;
  G4String theFileName;
  out << "@@ N PARAMS str " << paramStrs.size() << G4endl;
  for(ites = paramStrs.begin(); ites != paramStrs.end(); ites++ ) {
    out << (*ites).first << " " << (*ites).second << G4endl;
  }

  std::map<G4String,G4double>::const_iterator ited;
  std::map<G4String,G4double> params = cp.GetParams();
  out << "@@ N PARAMS " << params.size() << G4endl;
  for(ited = params.begin(); ited != params.end(); ited++ ) {
    out << "@ " << (*ited).first << " " << (*ited).second << G4endl;
  }
  out << " ENDED " << G4endl;

  return out;
}

//-----------------------------------------------------------------------
void RTVPlanControlPoint::SetMetersets( std::vector<RTVPlanSubControlPoint*> subCPs)
{
  for( size_t ii = 0; ii < subCPs.size(); ii++ ) {
    theSubCPs[ii]->SetMeterset(subCPs[ii]->GetMeterset());
  }

}
