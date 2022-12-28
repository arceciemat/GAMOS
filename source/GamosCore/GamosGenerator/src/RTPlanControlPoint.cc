#include "RTPlanControlPoint.hh"
#include "RTPlanBeam.hh"
#include "RTPlanSubControlPoint.hh"
#include "GmGenerVerbosity.hh"

#include "GamosCore/GamosGenerator/include/GmGenerVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "G4tgrUtils.hh"

//-----------------------------------------------------------------------
RTPlanControlPoint::RTPlanControlPoint( const G4String& fileName, RTPlanBeam* beam)
{
  theBeam = beam;
  theFileName = fileName;
  //  G4cout << " RTPlanControlPoint::RTPlanControlPoint " << theBeam << G4endl; //GDEB

  GmFileIn fin = GmFileIn::GetInstance(fileName,1);
  std::vector<G4String> wl;
  for(;;) {
    if( !fin.GetWordsInLine(wl) ) break;
    
    //------------------------------- parameter number
    if( wl[0]== ":P" )  {
      CheckIfNewParameter( wl, 0 );
      theParams[ wl[1] ] = G4tgrUtils::GetDouble( wl[2] );
      //      G4cout << this << " CPReadParams " << wl[1] << " = " << wl[2] << G4endl; //GDEB
   //------------------------------- parameter string
    } else if( wl[0] == ":PS" ) {
      CheckIfNewParameter( wl, 0 );
      theParamStrs[ wl[1] ] = wl[2];
    }
  }

  //G4int CPNo = GetParam("ControlPointIndex",1);
  //  theAccumulativeMeterset = GetParam("AccumulativeMeterset",1);
  theGantryAngle = GetParam("GantryAngle",1)*CLHEP::deg;
  theGantryPitchAngle = GetParam("GantryPitchAngle",0)*CLHEP::deg;
  theLimitingDeviceAngle = GetParam("LimitingDeviceAngle",0)*CLHEP::deg;
  //  theIsocenterPosition = G4ThreeVector( GetParam("IsocenterPosition_X",1), GetParam("IsocenterPosition_Y",1), GetParam("IsocenterPosition_Z",1) );
  theNominalBeamEnergy = GetParam("NominalBeamEnergy",1);
  theSourceAxisDistanceX = theBeam->GetParam("SourceAxisDistance",1);
  theSourceAxisDistanceY = theBeam->GetParam("SourceAxisDistance",1);
  theLimitingDeviceAngle = theBeam->GetParam("LimitingDeviceAngle",0.);
  theIndex = G4int(GetParam("ControlPointIndex",1));
  G4double metersetWeight = GetParam("MetersetWeight",-1);
  if( metersetWeight == -1 ) {
    G4Exception("RTPlanControlPoint::RTPlanControlPoint",
		"",
		FatalException,
		("Control Point "+GmGenUtils::itoa(theIndex)+" has no parameter 'MetersetWeight.   PLEASE CONTACT GAMOS AUTHORS").c_str());
  }
  G4double beamMS = 0.;  
  if( theBeam->ParamExists("BeamMeterset") ) {
    beamMS = theBeam->GetParam("BeamMeterset",0);
  } else {
    beamMS = theBeam->GetParam("FinalCumulativeMetersetWeight",1);
  }
  RTPlanSubControlPoint* subCP = new RTPlanSubControlPoint(beamMS*metersetWeight,this);
  theSubCPs.push_back(subCP);
  //-  theMetersets.push_back(GetParam("MetersetWeight",1)*beamMS);
  theNumberOfPaintings = 1; //only one meterset for RTPlan, several for RTPlan
  theLimitingDeviceAngle = GetParam("LimitingDeviceAngle",0.);

}

//ROTATIONS FOLLOW IEC 61217    ftp://dicom.nema.org/MEDICAL/dicom/2014c/output/chtml/part03/figures/PS3.3_C.8.8.25-2a.svg     ftp://dicom.nema.org/MEDICAL/dicom/2014c/output/chtml/part03/figures/PS3.3_C.8.8.25-2b.svg
//-----------------------------------------------------------------------
RTBeamStateData RTPlanControlPoint::GetBeamStateData(RTBeamStateData bsData)
{

  if( GenerVerb(debugVerb) ) G4cout << theBeam->GetIndex() << " : " << GetIndex() << " RTPlanControlPoint::GetBeamStateData " << G4endl;
  
  // By default (GantryAngle Y) accelerator head local coordinates X,Y are X,Y in global coordinates : IEC 61217 
  bsData.PositionX = 0.;
  bsData.PositionY = 0.;
  bsData.PositionZ = theSourceAxisDistanceX;
  bsData.Energy = theNominalBeamEnergy;
  bsData.RotAngleY = theGantryAngle;
  bsData.RotAngleX = theGantryPitchAngle;
  bsData.RotAngleZ = theLimitingDeviceAngle;
  bsData.SpotSize = -DBL_MAX; // change it in PlanSource
  bsData.SourceAxisDistanceX = theSourceAxisDistanceX;
  bsData.SourceAxisDistanceY = theSourceAxisDistanceX;
  bsData.ControlPoint = this;
  bsData.Beam = theBeam;
  bsData.IsocenterPosition = G4ThreeVector(GetParam("IsocenterPosition_X"),GetParam("IsocenterPosition_Y"),GetParam("IsocenterPosition_Z"));
  
  return bsData;
}

