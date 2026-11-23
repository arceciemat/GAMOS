#include "DicomReaderRTPlan.hh"
#include "DicomBeam.hh"
#include "DicomBeamDeviceBLD.hh"
#include "DicomBeamDevicePos.hh"
#include "DicomBeamRTControlPoint.hh"
#include "DicomBeamCompensator.hh"
#include "DicomBeamBlock.hh"
#include "DicomBeamWedge.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4ThreeVector.hh"

#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmrt/drtplan.h"
#include "dcmtk/dcmrt/seq/drtfgs.h" // DRTFractionGroupSequence
#include "dcmtk/dcmrt/seq/drtrbs8.h" // DRTReferencedBeamSequenceInRTFractionSchemeModule
#include "dcmtk/dcmrt/seq/drtbs.h"     // for BeamSequence
#include "dcmtk/dcmrt/seq/drtblds1.h"  // for BeamLimitingDeviceSequence
#include "dcmtk/dcmrt/seq/drtcps.h"    // for ControlPointSequence
#include "dcmtk/dcmrt/seq/drtbldps.h"  // for BeamLimitingDevicePositionSequence
#include "dcmtk/dcmrt/seq/drtcos.h" // for CompensatorSequence
#include "dcmtk/dcmrt/seq/drtbl2.h" // for BlockSequence
#include "dcmtk/dcmrt/seq/drtws.h"     // for WedgeSequence

//-----------------------------------------------------------------------------
DicomReaderRTPlan::DicomReaderRTPlan(DcmDataset* dset) : DicomVReader(dset, DRM_RTPlan)
{
  ReadData();
}

//-----------------------------------------------------------------------------
void DicomReaderRTPlan::ReadData()
{
  DRTPlanIOD rtplan;
  /*  DRTApplicatorSequenceInRTBeamsModule 
    DRTGeneralAccessorySequence
      DRTPlannedVerificationImageSequence
      PrimaryFluenceModeSequence
      DRTReferencedBolusSequenceInRTBeamsModule
      DRTReferencedDoseSequence
      DRTReferencedReferenceImageSequenceInRTBeamsModule
*/
      OFCondition result = rtplan.read(*theDataset);
  if (!result.good()) {
    G4Exception("DicomReaderRTPlan::ReadData",
		"DFS001",
		FatalException,
		result.text());
  }

  DRTFractionGroupSequence frgSeq = rtplan.getFractionGroupSequence();
  if( frgSeq.isEmpty() ) {
    G4Exception("DicomReaderRTPlan::ReadData",
		"DFS002",
		JustWarning,
		"DRTFractionGroupSequence is empty");
  }
  if(DicomVerb(debugVerb) ) G4cout << "@@@@@ NUMBER OF FractionSequences " << frgSeq.getNumberOfItems() << G4endl;
  frgSeq.gotoFirstItem();
  for( size_t i1 = 0; i1 < frgSeq.getNumberOfItems(); i1++ ) {
    DRTFractionGroupSequence::Item &rfgItem = frgSeq.getCurrentItem();
    if( rfgItem.getFractionGroupNumber(fint) == EC_Normal ) {
      if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " FractionGroupNumber " << fint << G4endl;
      //      SetParam("FractionGroup_"+GmGenUtils::itoa(fint)+"_Number",fint);
      SetParam("FractionGroup_"+GmGenUtils::itoa(fint)+"_Number",fint);
    } else {
      DicomObjectException("DicomReaderRTPlan","FractionBeamNumber");
    } 
    std::string fracNo = GmGenUtils::itoa(fint);

    if( rfgItem.getBeamDoseMeaning(fstr) == EC_Normal ) {
      if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " BeamDoseMeaning " << fstr << G4endl;
      SetParamStr("FractionGroup_"+fracNo+"_BeamDoseMeaning",fstr);
    }

    if( rfgItem.getFractionGroupDescription(fstr) == EC_Normal ) {
      if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " FractionGroupDescription " << fstr << G4endl;
      SetParamStr("FractionGroup_"+fracNo+"_Description",fstr);
    } 

    if( rfgItem.getFractionPattern(fstr) == EC_Normal ) {
      SetParamStr("FractionPattern",fstr);
      if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " FractionPattern " << fstr << G4endl;
    }

    if( rfgItem.getNumberOfBeams(fint) == EC_Normal ) {
      SetParam("FractionGroup_"+fracNo+"_NumberOfBeams",fint);
      if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " NumberOfBeams " << fint << G4endl;
    } else {
      DicomObjectException("DicomReaderRTIonPlan","NumberOfBeams");
    }  
    theNumberOfBeams = fint;

    if( rfgItem.getNumberOfBrachyApplicationSetups(fint) == EC_Normal ) {
      SetParam("FractionGroup_"+fracNo+"_NumberOfBrachyApplicationSetups",fint);
      if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " NumberOfBrachyApplicationSetups " << fint << G4endl;
      CheckData0("NumberOfBrachyApplicationSetups ", fint);
    }
    if( rfgItem.getNumberOfFractionPatternDigitsPerDay(fint) == EC_Normal ) {
      SetParam("FractionGroup_"+fracNo+"_NumberOfFractionPatternDigitsPerDay",fint);
      if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " NumberOfFractionPatternDigitsPerDay " << fint << G4endl;
    }
    if( rfgItem.getNumberOfFractionsPlanned(fint) == EC_Normal ) {
      SetParam("FractionGroup_"+fracNo+"_NumberOfFractionsPlanned",fint);
      if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " NumberOfFractionsPlanned " << fint << G4endl;
    } else {
      DicomObjectException("DicomReaderRTIonPlan","NumberOfFractionsPlanned");
    }
    if( rfgItem.getRepeatFractionCycleLength(fint) == EC_Normal ) {
      SetParam("FractionGroup_"+fracNo+"_RepeatFractionCycleLength",fint);
      if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " RepeatFractionCycleLength " << fint << G4endl;
    }

    DRTReferencedBeamSequenceInRTFractionSchemeModule refBeamSeq = rfgItem.getReferencedBeamSequence();
//       DRTReferencedDoseReferenceSequenceInRTFractionSchemeModule &getReferencedDoseReferenceSequence()
    if(DicomVerb(debugVerb) ) G4cout << " @@@ NUMBER OF ReferencedBeamSequences " << refBeamSeq.getNumberOfItems() << G4endl;

    refBeamSeq.gotoFirstItem();
    for( size_t i2 = 0; i2 < refBeamSeq.getNumberOfItems(); i2++ ) {
      DicomBeam* db = new DicomBeam(1);
      theBeams.push_back(db);
      DRTReferencedBeamSequenceInRTFractionSchemeModule::Item &rbsItem = refBeamSeq.getCurrentItem();
      if( rbsItem.getReferencedBeamNumber(fint) == EC_Normal ) {
	if(DicomVerb(debugVerb) ) G4cout << "  " << i2 << " ReferencedBeamNumber " << fint << G4endl;
	db->SetNumber(fint);
	db->SetParam("BeamNumber",fint);
      } else {
	DicomObjectException("DicomReaderRTIonPlan","ReferencedBeamNumber");
      }
      std::string beamNo = GmGenUtils::itoa(fint);

      if( rbsItem.getBeamMeterset(ffloat) == EC_Normal ) {
	db->SetParam("BeamMeterset",ffloat);
	if(DicomVerb(debugVerb) ) G4cout << "  " << i2 << " BeamMeterset " << ffloat << G4endl;
         } else {
	DicomObjectException("DicomReaderRTIonPlan","BeamMeterset");
      }

      if( rbsItem.getBeamDeliveryDurationLimit(ffloat) == EC_Normal ) {
	db->SetParam("BeamDeliveryDurationLimit",ffloat);
	if(DicomVerb(debugVerb) ) G4cout << "  " << i2 << " BeamDeliveryDurationLimit " << ffloat << G4endl;
      }
      if( rbsItem.getBeamDose(ffloat) == EC_Normal ) {
	db->SetParam("BeamDose",ffloat);
	if(DicomVerb(debugVerb) ) G4cout << "  " << i2 << " BeamDose " << ffloat << G4endl; // dose at dose point
      }
      if( rbsItem.getBeamDoseSpecificationPoint(fvfloat) == EC_Normal && fvfloat.size() != 0 ) {
	db->SetParam("BeamDoseSpecificationPoint_X",fvfloat[0]);
	db->SetParam("BeamDoseSpecificationPoint_Y",fvfloat[1]);
	db->SetParam("BeamDoseSpecificationPoint_Z",fvfloat[2]);
	if(DicomVerb(debugVerb) ) G4cout << "  " << i2 << " BeamDoseSpecificationPoint (" << fvfloat[0] << "," << fvfloat[1] << "," << fvfloat[2] << ")" << G4endl;
      }
    
      refBeamSeq.gotoNextItem();
    }

    frgSeq.gotoNextItem();
  }


  DRTBeamSequence beamSeq = rtplan.getBeamSequence();
  if( beamSeq.isEmpty() ) {
    G4Exception("DicomReaderRTPlan::ReadData",
		"DFS002",
		JustWarning,
		"DRTBeamSequence is empty");
  }
  if(DicomVerb(debugVerb) ) G4cout << "@@@@@ NUMBER OF BeamSequences " << beamSeq.getNumberOfItems() << G4endl;
  beamSeq.gotoFirstItem();
  for( size_t i1 = 0; i1 < beamSeq.getNumberOfItems(); i1++ ) {
    DicomBeam* db = 0;
    //    DicomBeam* db = new DicomBeam(0);
    //    theBeams.push_back(db);
    // merge with Referenced beam
    DRTBeamSequence::Item &beamItem = beamSeq.getCurrentItem();

    if( beamItem.getBeamNumber(fint) == EC_Normal ) {
      if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " BeamNumber " << fint << G4endl;
      if( theBeams[i1]->GetParam("BeamNumber") == fint ) {
	db = theBeams[i1];
      } else {
	db = new DicomBeam(0);
	theBeams.push_back(db);
	G4cerr << i1 << " !! " << theBeams[i1]->GetParam("BeamNumber") << " != " << fint << G4endl; 
	G4Exception("DicomReaderRTPlan::ReadData",
		    "",
		    JustWarning,
		    ("No beam Referenced found for beam " + GmGenUtils::itoa(fint)).c_str());
      }
      db->SetParam("BeamNumber",fint);
      db->SetNumber(fint);
    } else {
      DicomObjectException("DicomReaderRTIonPlan","BeamNumber");
    }
    std::string beamNo = GmGenUtils::itoa(fint);

    if( beamItem.getManufacturer(fstr) == EC_Normal ) {
      db->SetParamStr("Manufacturer",fstr);
      if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " Manufacturer " << fstr << G4endl;
    }
    if( beamItem.getManufacturerModelName(fstr) == EC_Normal ) {
      db->SetParamStr("ManufacturerModelName",fstr);
      if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " ManufacturerModelName " << fstr << G4endl;
    }
    if( beamItem.getTreatmentMachineName(fstr) == EC_Normal ) {
      db->SetParamStr("TreatmentMachineName",fstr);
      if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " TreatmentMachineName " << fstr << G4endl;
    }
    if( beamItem.getTreatmentDeliveryType(fstr) == EC_Normal ) {
      db->SetParamStr("TreatmentDeliveryType",fstr);
      if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " TreatmentDeliveryType " << fstr << G4endl;
    }
    if( beamItem.getPrimaryDosimeterUnit(fstr) == EC_Normal ) {
      db->SetParamStr("PrimaryDosimeterUnit",fstr);
      if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " PrimaryDosimeterUnit " << fstr << G4endl;
    }
    if( beamItem.getSourceAxisDistance(ffloat) == EC_Normal ) {
      db->SetParam("SourceAxisDistance",ffloat);
      if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " SourceAxisDistance " << ffloat << G4endl;
      //-    db->SetSourceAxisDistance(ffloat);
    } else {
      DicomObjectException("DicomReaderRTIonPlan","SourceAxisDistance");
    } 
    
    DRTBeamLimitingDeviceSequenceInRTBeamsModule beamLDS = beamItem.getBeamLimitingDeviceSequence();
    if(DicomVerb(debugVerb) ) G4cout << " @@@ NUMBER OF BeamLimitingDeviceSequence " << beamLDS.getNumberOfItems() << G4endl;
    beamLDS.gotoFirstItem();
    for( size_t i2 = 0; i2 < beamLDS.getNumberOfItems(); i2++ ) {
      DRTBeamLimitingDeviceSequenceInRTBeamsModule::Item bldsItem = beamLDS.getCurrentItem();
      DicomBeamDeviceBLD* dbd = new DicomBeamDeviceBLD(bldsItem);
      db->AddDevice(dbd);
      
      beamLDS.gotoNextItem();
    }
    
    if( beamItem.getBeamName(fstr) == EC_Normal ) {
      db->SetParamStr("BeamName",fstr);
      if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " BeamName " << fstr << G4endl;
    }
    if( beamItem.getBeamDescription(fstr) == EC_Normal ) {
      db->SetParamStr("BeamDescription",fstr);
      if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " BeamDescription " << fstr << G4endl;
    }
    if( beamItem.getBeamType(fstr) == EC_Normal ) {
      db->SetParamStr("BeamType",fstr);
      if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " BeamType " << fstr << G4endl;
    }
    if( beamItem.getRadiationType(fstr) == EC_Normal ) {
      db->SetParamStr("RadiationType",fstr);
      if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " RadiationType " << fstr << G4endl;
    }     
    if( beamItem.getTreatmentDeliveryType(fstr) == EC_Normal ) {
      db->SetParamStr("TreatmentDeliveryType",fstr);
    }
    if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " TreatmentDeliveryType " << fstr << G4endl;
    beamItem.getNumberOfWedges(fint);
    if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " NumberOfWedges " << fint << G4endl;
    DRTWedgeSequence beamWedge = beamItem.getWedgeSequence();
    beamWedge.gotoFirstItem();
    for( size_t i2 = 0; i2 < beamWedge.getNumberOfItems(); i2++ ) {
      DRTWedgeSequence::Item bwedItem = beamWedge.getCurrentItem();
      DicomBeamWedge* dbwed = new DicomBeamWedge( bwedItem );
      db->AddWedge( dbwed );
      beamWedge.gotoNextItem();
    }
    
    if( beamItem.getNumberOfBoli(fint) == EC_Normal ) {
      db->SetParam("NumberOfBoli",fint);
      if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " NumberOfBoli " << fint << G4endl; 
    }
      
    if( beamItem.getNumberOfBlocks(fint) == EC_Normal ) {
      db->SetParam("NumberOfBlocks",fint);
      if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " NumberOfBlocks " << fint << G4endl;
      DRTBlockSequenceInRTBeamsModule beamBlock = beamItem.getBlockSequence();
      beamBlock.gotoFirstItem();
      for( size_t i2 = 0; i2 < beamBlock.getNumberOfItems(); i2++ ) {
	DRTBlockSequenceInRTBeamsModule::Item bblItem = beamBlock.getCurrentItem();
	DicomBeamBlock* dbbl = new DicomBeamBlock( bblItem );
	db->AddBlock( dbbl );
	beamBlock.gotoNextItem();
      }
    }
    
    if( beamItem.getNumberOfCompensators(fint) == EC_Normal ) {
      db->SetParam("NumberOfCompensators",fint);
      if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " NumberOfIonRangeCompensators " << fint << G4endl;
      if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " NumberOfCompensators " << fint << G4endl;
      DRTCompensatorSequence beamCompens = beamItem.getCompensatorSequence();
      beamCompens.gotoFirstItem();
      for( size_t i2 = 0; i2 < beamCompens.getNumberOfItems(); i2++ ) {
	DRTCompensatorSequence::Item bcompItem = beamCompens.getCurrentItem();
	DicomBeamCompensator* dbcomp = new DicomBeamCompensator( bcompItem );
	db->AddCompensator( dbcomp );
	beamCompens.gotoNextItem();
      }
    }

    if( beamItem.getFinalCumulativeMetersetWeight(ffloat) == EC_Normal ) {
      db->SetParam("FinalCumulativeMetersetWeight",ffloat);
      if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " FinalCumulativeMetersetWeight " << ffloat << G4endl;
    }

    if( beamItem.getDeviceSerialNumber(fstr) == EC_Normal ) {
      db->SetParamStr("DeviceSerialNumber",fstr);
      if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " DeviceSerialNumber " << fstr << G4endl;
    }
    if( beamItem.getInstitutionAddress(fstr) == EC_Normal ) {
      db->SetParamStr("InstitutionAddress",fstr);
      if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " InstitutionAddress " << fstr << G4endl;
    }
    if( beamItem.getInstitutionName(fstr) == EC_Normal ) {
      db->SetParamStr("InstitutionName",fstr);
      if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " InstitutionName " << fstr << G4endl;
    }
    if( beamItem.getInstitutionalDepartmentName(fstr) == EC_Normal ) {
      db->SetParamStr("InstitutionalDepartmentName",fstr);
      if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " InstitutionalDepartmentName " << fstr << G4endl;
    }
    if( beamItem.getReferencedPatientSetupNumber(fint) == EC_Normal ) {
      db->SetParam("ReferencedPatientSetupNumber",fint);
      if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " ReferencedPatientSetupNumber " << fint << G4endl;
    }
    if( beamItem.getReferencedToleranceTableNumber(fint) == EC_Normal ) {
      db->SetParam("ReferencedToleranceTableNumber",fint);
      if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " ReferencedToleranceTableNumber " << fint << G4endl;
    }

    if( beamItem.getHighDoseTechniqueType(fstr) == EC_Normal ) {
      db->SetParam("HighDoseTechniqueType",fint);
      if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " HighDoseTechniqueType " << fstr << G4endl;
    }
    if( beamItem.getTotalBlockTrayFactor(ffloat) == EC_Normal ) {
      db->SetParam("TotalBlockTrayFactor",fint);
      if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " TotalBlockTrayFactor " << ffloat << G4endl;
    }
    if( beamItem.getTotalCompensatorTrayFactor(ffloat) == EC_Normal ) {
      db->SetParam("TotalCompensatorTrayFactor",fint);
      if(DicomVerb(debugVerb) ) G4cout << " " << i1 << " TotalCompensatorTrayFactor " << ffloat << G4endl;
    }

    if( beamItem.getNumberOfControlPoints(fint) == EC_Normal ) {
      db->SetParam("NumberOfControlPoints",fint);
    }
    
   DRTControlPointSequence controlPSeq = beamItem.getControlPointSequence();
    if(DicomVerb(debugVerb) ) G4cout << " @@@ NUMBER OF ControlPointSequences " << controlPSeq.getNumberOfItems() << " = " << fint << G4endl;

    controlPSeq.gotoFirstItem();
        DicomBeamRTControlPoint* dbcp0 = 0; // Only first ControlPoint has some info if it does not change 
    for( size_t i2 = 0; i2 < controlPSeq.getNumberOfItems(); i2++ ) {
      DRTControlPointSequence::Item &cpItem = controlPSeq.getCurrentItem();
      if( db->GetNControlPoints() != 0 ) dbcp0 = dynamic_cast<DicomBeamRTControlPoint*>(db->GetControlPoint(0));
      // pass the previous control point: it happens that some parameters like IsocenterPosition_X,  TableTopEccentricAngle, are only defined for first ControlPoint
      DicomBeamRTControlPoint* dbcp = new DicomBeamRTControlPoint( cpItem, dbcp0 ); 
      db->AddControlPoint( dbcp );
      controlPSeq.gotoNextItem();
  
    }
    
    beamSeq.gotoNextItem();
  }

  //(300a,0180) SQ (Sequence with explicit length #=1)      #  30, 1 PatientSetupSequence
  //(300c,0060) SQ (Sequence with explicit length #=1)      # 112, 1 ReferencedStructureSetSequence

  //  (300e,0002) CS [UNAPPROVED]                             #  10, 1 ApprovalStatus
  //(7fe0,0010) OW (no value available)                     #   0, 1 PixelData

}

//-----------------------------------------------------------------------------
void DicomReaderRTPlan::CheckData0(OFString title, Sint32 val )
{
  if( val != 0 ){
    G4Exception("DicomReaderRTPlan::CheckData",
		"DFP003",
		FatalException,
		(title + " exists, and code is not ready ").c_str());
  }
}

//-----------------------------------------------------------------------------
void DicomReaderRTPlan::SetControlPointMetersets()
{
  for( size_t ii = 0; ii < theBeams.size(); ii++ ){
    theBeams[ii]->SetControlPointMetersets();
  }
}

//-----------------------------------------------------------------------------
void DicomReaderRTPlan::DumpToFile()
{
  std::ofstream fout("RTPlan");
  std::string name = "";
  DicomVObjectWithParams::Print( fout, name );
 
  for( size_t ii = 0; ii < theBeams.size(); ii++ ){
    theBeams[ii]->DumpToFileF("RTPlanBeam_"+GmGenUtils::ftoa(theBeams[ii]->GetParam("BeamNumber",1)));
  }
}
  
