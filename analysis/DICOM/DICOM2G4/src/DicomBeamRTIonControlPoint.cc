#include "DicomBeamRTIonControlPoint.hh"
#include "DicomBeamDevicePos.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"

#include "dcmtk/dcmrt/seq/drtcps.h"    // for ControlPointSequence
#include "globals.hh"

//-----------------------------------------------------------------------------
DicomBeamRTIonControlPoint::DicomBeamRTIonControlPoint(DRTIonControlPointSequence::Item cpItem, DicomBeamRTIonControlPoint* point0 )
{
  
  DRTBeamLimitingDevicePositionSequence beamLDPS = cpItem.getBeamLimitingDevicePositionSequence();
  if( DicomVerb(debugVerb) ) G4cout << "  @ NUMBER OF BeamLimitingDevicePositionSequence " << beamLDPS.getNumberOfItems() << G4endl;
  beamLDPS.gotoFirstItem();
  for( size_t i3 = 0; i3 < beamLDPS.getNumberOfItems(); i3++ ) {
    DRTBeamLimitingDevicePositionSequence::Item bldpsItem = beamLDPS.getCurrentItem();
    DicomBeamDevicePos* dbd = new DicomBeamDevicePos(bldpsItem);
    AddDevice(dbd);
    beamLDPS.gotoNextItem();
  }
  
  /*          DRTIonWedgePositionSequence &getIonWedgePositionSequence()
	      const DRTIonWedgePositionSequence &getIonWedgePositionSequence() const
	      DRTLateralSpreadingDeviceSettingsSequenceInRTIonBeamsModule &getLateralSpreadingDeviceSettingsSequence()
	      const DRTLateralSpreadingDeviceSettingsSequenceInRTIonBeamsModule &getLateralSpreadingDeviceSettingsSequence() const
	      DRTRangeModulatorSettingsSequenceInRTIonBeamsModule &getRangeModulatorSettingsSequence()
	      const DRTRangeModulatorSettingsSequenceInRTIonBeamsModule &getRangeModulatorSettingsSequence() const
	      DRTRangeShifterSettingsSequenceInRTIonBeamsModule &getRangeShifterSettingsSequence()
	      const DRTRangeShifterSettingsSequenceInRTIonBeamsModule &getRangeShifterSettingsSequence() const
	      DRTReferencedDoseReferenceSequenceInRTIonBeamsModule &getReferencedDoseReferenceSequence()
	      const DRTReferencedDoseReferenceSequenceInRTIonBeamsModule &getReferencedDoseReferenceSequence() const
  */
  
  if( point0 ) theParams = point0->GetParams();
  
  if( cpItem.getControlPointIndex(fint) == EC_Normal ) {
    theParams["ControlPointIndex"] = fint;
  } else {
    DicomObjectException("DicomBeamRTIonControlPoint","ControlPointIndex");
  }
  if( DicomVerb(debugVerb) ) G4cout << this << " READING ControlPoint " << fint << G4endl;
  //  cpItem.getNominalBeamEnergyUnit(fstr);
  //  theParams["NominalBeamEnergyUnit"] = fstr;
  if( cpItem.getNominalBeamEnergy(ffloat) == EC_Normal ) {
    theParams["NominalBeamEnergy"] = ffloat;
  } else {
    DicomObjectException("DicomBeamRTIonControlPoint","NominalBeamEnergy");
  }
  if( cpItem.getCumulativeMetersetWeight(ffloat) == EC_Normal ) {
    theParams["CumulativeMetersetWeight"] = ffloat;
  } else {
    DicomObjectException("DicomBeamRTIonControlPoint","CumulativeMetersetWeight");
  }
  
  if( cpItem.getNumberOfScanSpotPositions(fint) == EC_Normal ) {
    theParams["NumberOfScanSpotPositions"] = fint;
  } else {
    DicomObjectException("DicomBeamRTIonControlPoint","NumberOfScanSpotPositions");
  }
  //    DcmFloatingPointSingle   ScanSpotPositionMap(DCM_ScanSpotPositionMap);
  //    cpItem.getScanSpotPositionMap(ffloat32);
  for( int ii = 0; ii < fint*2; ii++ ) {
    if( cpItem.getScanSpotPositionMap(ffloat32,ii) == EC_Normal ) {
      theSpotPos.push_back(ffloat32);
    } else {
      DicomObjectException("DicomBeamRTIonControlPoint","ScanSpotPositionMap");
    }
    //    G4cout << " SCAN SPOT " << ii << " : " << ffloat32 << " = " << theSpotPos[ii] << G4endl; 
  }
  
  for( int ii = 0; ii < fint; ii++ ) { 
    if( cpItem.getScanSpotMetersetWeights(ffloat32,ii) == EC_Normal ) {
      theSpotWei.push_back(ffloat32);
    } else {
      DicomObjectException("DicomBeamRTIonControlPoint","ScanSpotMetersetWeights");
    }
  }
  
  if( cpItem.getScanningSpotSize(ffloat32) == EC_Normal ) {
    theParams["ScanningSpotSize"] = ffloat32;
  } else {
    DicomObjectException("DicomBeamRTIonControlPoint","ScanningSpotSize");
  }
  if( cpItem.getNumberOfPaintings(fint) == EC_Normal ) {
    theParams["NumberOfPaintings"] = fint;
  } else {
    DicomObjectException("DicomBeamRTIonControlPoint","NumberOfPaintings");
  }
  
  if( cpItem.getGantryAngle(ffloat) == EC_Normal ) {
    if( cpItem.getGantryRotationDirection(fstr) == EC_Normal ) {
    } else {
      DicomObjectException("DicomBeamRTControlPoint","GantryRotationDirection");
    }
    ffloat = CorrectByDirection(ffloat, fstr );
    theParams["GantryAngle"] = ffloat;
  } else {
    SetParam("GantryAngle",point0);
  }
  
  if( cpItem.getGantryPitchAngle(ffloat32) == EC_Normal ) {
    if( cpItem.getGantryPitchRotationDirection(fstr) == EC_Normal ) {
    } else {
      DicomObjectException("DicomBeamRTControlPoint","GantryPitchRotationDirection");
    }
    ffloat32 = CorrectByDirection(ffloat32, fstr );
    theParams["GantryPitchAngle"] = ffloat32;
  } else {
    SetParam("GantryPitchAngle",point0);
  }
  
  if( cpItem.getPatientSupportAngle(ffloat) == EC_Normal ) {
    if( cpItem.getPatientSupportRotationDirection(fstr) == EC_Normal ) {
    ffloat = CorrectByDirection(ffloat, fstr );
  } else {
    DicomObjectException("DicomBeamRTControlPoint","PatientSupportRotationDirection");
    }
    theParams["PatientSupportAngle"] = ffloat;
  } else {
    SetParam("PatientSupportAngle",point0);
  }
  
  if( cpItem.getTableTopPitchAngle(ffloat32) == EC_Normal ) {
    if( cpItem.getTableTopPitchRotationDirection(fstr) == EC_Normal ) {
      ffloat32 = CorrectByDirection(ffloat32, fstr );
    } else {
      DicomObjectException("DicomBeamRTControlPoint","TableTopPitchRotationDirection");
    }
    theParams["TableTopPitchAngle"] = ffloat32;
  } else {
    SetParam("TableTopPitchAngle",point0);
  }
  
  if( cpItem.getTableTopRollAngle(ffloat32) == EC_Normal ) {
    if( cpItem.getTableTopRollRotationDirection(fstr) == EC_Normal ) {
      ffloat32 = CorrectByDirection(ffloat32, fstr );
    } else {
      DicomObjectException("DicomBeamRTControlPoint","TableTopRollRotationDirection");
    }
    theParams["TableTopRollAngle"] = ffloat32;
  } else {
    SetParam("TableTopRollAngle",point0);
  }
  
  if( cpItem.getBeamLimitingDeviceAngle(ffloat) == EC_Normal ) {
    if( cpItem.getBeamLimitingDeviceRotationDirection(fstr) == EC_Normal ) {
      ffloat = CorrectByDirection(ffloat, fstr );
    } else {
      DicomObjectException("DicomBeamRTControlPoint","BeamLimitingDeviceRotationDirection");
    }
    theParams["LimitingDeviceAngle"] = ffloat;
  } else {
    SetParam("LimitingDeviceAngle",point0);
  }
  
  if( cpItem.getIsocenterPosition(fvfloat) == EC_Normal && fvfloat.size() != 0 ) {
    theParams["IsocenterPosition_X"] = fvfloat[0];
    theParams["IsocenterPosition_Y"] = fvfloat[1];
    theParams["IsocenterPosition_Z"] = fvfloat[2];
    //    G4cout <<  theParams["ControlPointIndex"]  << " 1getIsocenterPosition " << point0 << " = " <<   theParams["IsocenterPosition_X"] << std::endl; //GDEB
  } else {
    if( point0 ) {
      SetParam("IsocenterPosition_X",point0);
      SetParam("IsocenterPosition_Y",point0);
      SetParam("IsocenterPosition_Z",point0);
    }
    //    G4cout <<  theParams["ControlPointIndex"]  << " 2getIsocenterPosition " << point0 << " = " <<   theParams["IsocenterPosition_X"] << std::endl; //GDEB
    std::ofstream fou("test.txt");
    point0->DumpToFile( fou );
    fou.close();
  }
  
  if( cpItem.getSurfaceEntryPoint(fvfloat) == EC_Normal && fvfloat.size() != 0 ) {
    theParams["SurfaceEntryPoint_X"] = fvfloat[0];
    theParams["SurfaceEntryPoint_Y"] = fvfloat[1];
    theParams["SurfaceEntryPoint_X"] = fvfloat[2];
  } else {
    if( point0 ) {
      SetParam("SurfaceEntryPoint_X",point0);
      SetParam("SurfaceEntryPoint_Y",point0);
      SetParam("SurfaceEntryPoint_Z",point0);
    }
  }
  
  if( cpItem.getKVP(ffloat) == EC_Normal ) {
    theParams["KVP"] = ffloat;
  } else {
    SetParam("KVP",point0);
  }
  
  if( cpItem.getSurfaceEntryPoint(ffloat) == EC_Normal ) {
    theParams["SurfaceEntryPoint"] = ffloat;
  } else {
    SetParam("SurfaceEntryPoint",point0);
  }
  
  if( cpItem.getTableTopLateralPosition(ffloat) == EC_Normal ) {
    theParams["TableTopLateralPosition"] = ffloat;
  } else {
    SetParam("TableTopLateralPosition",point0);
  }
  
  if( cpItem.getTableTopLongitudinalPosition(ffloat) == EC_Normal ) {
    theParams["TableTopLongitudinalPosition"] = ffloat;
  } else {
    SetParam("TableTopLongitudinalPosition",point0);
  }
  
  if( cpItem.getTableTopVerticalPosition(ffloat) == EC_Normal ) {
    theParams["TableTopVerticalPosition"] = ffloat;
  } else {
    SetParam("TableTopVerticalPosition",point0);
  }
  
  if( cpItem.getHeadFixationAngle(ffloat32) == EC_Normal ) {
    theParams["HeadFixationAngle"] = ffloat32;
  } else {
    SetParam("HeadFixationAngle",point0);
  }
  
  if( cpItem.getMetersetRate(ffloat32) == EC_Normal ) {
    theParams["MetersetRate"] = ffloat32;
  } else {
    SetParam("MetersetRate",point0);
  }
  
  if( cpItem.getSnoutPosition(ffloat32) == EC_Normal ) {
    theParams["SnoutPosition"] = ffloat32;
  } else {
    SetParam("SnoutPosition",point0);
  }
  
}

//-----------------------------------------------------------------------------
void DicomBeamRTIonControlPoint::DumpToFile( std::ofstream& fout )
{
  DicomVObjectWithParams::Print( fout, "");
  
  for( size_t ii = 0; ii < theDevices.size(); ii++ ){
    theDevices[ii]->DumpToFile(fout);
  }

  std::map<G4String,Float64>::const_iterator ite;
  ite = theParams.find("NumberOfScanSpotPositions");
  int nPos = int((*ite).second);
  fout << "ScanSpotPositions" << G4endl; 
  for( int ii = 0; ii < nPos; ii++ ) {
    //  G4cout << ii  << " ScanSpotPositions " << theSpotPos[ii*2+0] << " " << theSpotPos[ii*2+1] << " " << theSpotWei[ii] << G4endl;
    //  fout << ii  << " ScanSpotPositions "  << theSpotPos[ii*2+0] << " " << theSpotPos[ii*2+1] << " " << theSpotWei[ii] << G4endl;
    fout << theSpotPos[ii*2+0] << " " << theSpotPos[ii*2+1] << " " << theSpotWei[ii] << G4endl;
  }
}


//-----------------------------------------------------------------------------
void DicomBeamRTIonControlPoint::SetParam( std::string name,  DicomBeamRTIonControlPoint* point0 )
{
  if( point0 && ParamExists(name) ) {
    theParams[name] = point0->GetParam(name);
  }

}
  
