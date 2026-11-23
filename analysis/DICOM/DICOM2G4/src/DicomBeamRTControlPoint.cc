#include "DicomBeamRTControlPoint.hh"
#include "DicomBeamDevicePos.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"

#include "dcmtk/dcmrt/seq/drtcps.h"    // for ControlPointSequence
#include "globals.hh"

//-----------------------------------------------------------------------------
DicomBeamRTControlPoint::DicomBeamRTControlPoint(DRTControlPointSequence::Item cpItem, DicomBeamRTControlPoint* point0 ) 
{
  DRTBeamLimitingDevicePositionSequence beamLDPS = cpItem.getBeamLimitingDevicePositionSequence();
  if( DicomVerb(debugVerb) ) G4cout << "  @ DicomBeamRTControlPoint: NUMBER OF BeamLimitingDevicePositionSequence " << beamLDPS.getNumberOfItems() << G4endl;
  beamLDPS.gotoFirstItem();
  for( size_t i3 = 0; i3 < beamLDPS.getNumberOfItems(); i3++ ) {
    DRTBeamLimitingDevicePositionSequence::Item bldpsItem = beamLDPS.getCurrentItem();
    DicomBeamDevicePos* dbd = new DicomBeamDevicePos(bldpsItem);
    AddDevice(dbd);
    
    beamLDPS.gotoNextItem();
  }
  //  DRTReferencedDoseReferenceSequenceInRTBeamsModule &getReferencedDoseReferenceSequence()
  //  DRTReferencedDoseSequence &getReferencedDoseSequence()
  //  DRTWedgePositionSequence &getWedgePositionSequence()

  if( point0 ) theParams = point0->GetParams();

  if( cpItem.getControlPointIndex(fint) == EC_Normal ) {
    SetParam("ControlPointIndex",fint);
  } else {
    DicomObjectException("DicomBeamRTControlPoint","ControlPointIndex");
  }
  if( cpItem.getNominalBeamEnergy(ffloat) == EC_Normal ) {
    SetParam("NominalBeamEnergy",ffloat);
  }
  
  if( cpItem.getDoseRateSet(ffloat) == EC_Normal ) {
    SetParam("DoseRateSet",ffloat);
  }
  
  if( cpItem.getGantryAngle(ffloat) == EC_Normal ) {
    if( cpItem.getGantryRotationDirection(fstr) == EC_Normal ) { 
      ffloat = CorrectByDirection(ffloat, fstr );
    } else {
      DicomObjectException("DicomBeamRTControlPoint","GantryRotationDirection");
    }
    SetParam("GantryAngle",ffloat);
  } else {
    SetParamFromPoint("GantryAngle",point0);
  }

  if( cpItem.getGantryPitchAngle(ffloat32) == EC_Normal ) {
    if( cpItem.getGantryPitchRotationDirection(fstr)== EC_Normal ) { 
      ffloat32 = CorrectByDirection(ffloat32, fstr );
    } else {
      DicomObjectException("DicomBeamRTControlPoint","GantryPitchRotationDirection");
    }
    SetParam("GantryPitchAngle",ffloat32);
  } else {
    SetParamFromPoint("GantryPitchAngle",point0);
  }
  
  if( cpItem.getPatientSupportAngle(ffloat) == EC_Normal ) {
    if( cpItem.getPatientSupportRotationDirection(fstr) == EC_Normal ) { 
      ffloat = CorrectByDirection(ffloat, fstr );
    } else {
      DicomObjectException("DicomBeamRTControlPoint","PatientSupportRotationDirection");
    }
    SetParam("PatientSupportAngle",ffloat);
  } else {
    SetParamFromPoint("PatientSupportAngle",point0);
  }
  
  if( cpItem.getTableTopPitchAngle(ffloat32) == EC_Normal ) {
    if( cpItem.getTableTopPitchRotationDirection(fstr) == EC_Normal ) {
      ffloat32 = CorrectByDirection(ffloat32, fstr );
    } else {
      DicomObjectException("DicomBeamRTControlPoint","TableTopPitchRotationDirection");
    }
    SetParam("TableTopPitchAngle",ffloat32);
  } else {
    SetParamFromPoint("TableTopPitchAngle",point0);
  }
  
  if( cpItem.getTableTopRollAngle(ffloat32) == EC_Normal ) {
    if( cpItem.getTableTopRollRotationDirection(fstr) == EC_Normal ) {
      ffloat32 = CorrectByDirection(ffloat32, fstr );
      SetParam("TableTopRollAngle",ffloat32);
    } else {
      DicomObjectException("DicomBeamRTControlPoint","TableTopRollRotationDirection");
    }
  } else {
    SetParamFromPoint("TableTopRollAngle",point0);
  } 

  if( cpItem.getTableTopEccentricAngle(ffloat) == EC_Normal ) {
    if( cpItem.getTableTopEccentricRotationDirection(fstr) == EC_Normal ) {
      ffloat = CorrectByDirection(ffloat, fstr );
    } else {
      DicomObjectException("DicomBeamRTControlPoint","TableTopEccentricRotationDirection");
    }
    SetParam("TableTopEccentricAngle",ffloat);
  } else {
    SetParamFromPoint("TableTopEccentricAngle",point0);
  }
  
  if( cpItem.getBeamLimitingDeviceAngle(ffloat) == EC_Normal ) {
    if( cpItem.getBeamLimitingDeviceRotationDirection(fstr)  == EC_Normal ) {
      ffloat = CorrectByDirection(ffloat, fstr );
    } else {
      DicomObjectException("DicomBeamRTControlPoint","BeamLimitingDeviceRotationDirection");
    }
    SetParam("LimitingDeviceAngle",ffloat);
  } else {
    SetParamFromPoint("LimitingDeviceAngle",point0);
  }
    
  if( cpItem.getCumulativeMetersetWeight(ffloat) == EC_Normal ) {
    SetParam("CumulativeMetersetWeight",ffloat);
  } else {
    SetParamFromPoint("CumulativeMetersetWeight",point0);
  }
  if( cpItem.getSourceToSurfaceDistance(ffloat) == EC_Normal ) {
    SetParam("SourceToSurfaceDistance",ffloat);
  } else {
    SetParamFromPoint("SourceToSurfaceDistance",point0);
  }
  
  if( cpItem.getTableTopEccentricAxisDistance(ffloat) == EC_Normal ) {
    SetParam("TableTopEccentricAxisDistance",ffloat);
  } else {
    SetParamFromPoint("TableTopEccentricAxisDistance",point0);
  }
  
  if( cpItem.getTableTopLongitudinalPosition(ffloat) == EC_Normal ) {
    SetParam("TableTopLongitudinalPosition",ffloat);
  } else {
    SetParamFromPoint("TableTopLongitudinalPosition",point0);
  }
  if( cpItem.getTableTopLateralPosition(ffloat) == EC_Normal ) {
    SetParam("TableTopLateralPosition",ffloat);
  } else {
    SetParamFromPoint("TableTopLateralPosition",point0);
  }
  if( cpItem.getTableTopVerticalPosition(ffloat) == EC_Normal ) {
    SetParam("TableTopVerticalPosition",ffloat);
  } else {
    SetParamFromPoint("TableTopVerticalPosition",point0);
  }
  
  if( cpItem.getIsocenterPosition(fvfloat) == EC_Normal && fvfloat.size() != 0 ) {
    SetParam("IsocenterPosition_X",fvfloat[0]);
    SetParam("IsocenterPosition_Y",fvfloat[1]);
    SetParam("IsocenterPosition_Z",fvfloat[2]);
  } else {
    if( point0 ) {
      SetParamFromPoint("IsocenterPosition_X",point0);
      SetParamFromPoint("IsocenterPosition_Y",point0);
      SetParamFromPoint("IsocenterPosition_Z",point0);
    }
  }
  
  
  if( cpItem.getSurfaceEntryPoint(fvfloat) == EC_Normal && fvfloat.size() != 0 ) {
    SetParam("SurfaceEntryPoint_X",fvfloat[0]);
    SetParam("SurfaceEntryPoint_Y",fvfloat[1]);
    SetParam("SurfaceEntryPoint_Z",fvfloat[2]);
  } else {
    if( point0 ) {
      SetParamFromPoint("SurfaceEntryPoint_X",point0);
      SetParamFromPoint("SurfaceEntryPoint_Y",point0);
      SetParamFromPoint("SurfaceEntryPoint_Z",point0);
    }
  }
  
}

//-----------------------------------------------------------------------------
void DicomBeamRTControlPoint::DumpToFile( std::ofstream& fout )
{
  //  DicomVObjectFromParams::Print( fout, "");
 Print( fout, "");
  
  for( size_t ii = 0; ii < theDevices.size(); ii++ ){
    theDevices[ii]->DumpToFile(fout);
  }
}

//-----------------------------------------------------------------------------
void DicomBeamRTControlPoint::SetParamFromPoint( std::string name,  DicomBeamRTControlPoint* point0 )
{
  if( DicomVerb(debugVerb) ) G4cout << " DicomBeamRTControlPoint::SetParam " << name << G4endl; 
  if( point0 && ParamExists(name) ) {
    theParams[name] = point0->GetParam(name);
  }

}
  
