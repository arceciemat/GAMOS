#include "DicomReaderRTIonPlan.hh"
#include "DicomBeam.hh"
#include "DicomBeamDeviceBLD.hh"
#include "DicomBeamDevicePos.hh"
#include "DicomBeamRTIonControlPoint.hh"
#include "DicomBeamCompensator.hh"
#include "DicomBeamBlock.hh"
#include "DicomBeamWedge.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4ThreeVector.hh"

#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmrt/drtionpl.h"
#include "dcmtk/dcmrt/seq/drtfgs.h" // DRTFractionGroupSequence
#include "dcmtk/dcmrt/seq/drtrbs8.h" // DRTReferencedBeamSequenceInRTFractionSchemeModule
#include "dcmtk/dcmrt/seq/drtibs.h"     // for IonBeamSequence
#include "dcmtk/dcmrt/seq/drtiblds.h"  // for BeamLimitingDeviceSequence
#include "dcmtk/dcmrt/seq/drtcps.h"    // for ControlPointSequence
#include "dcmtk/dcmrt/seq/drtbldps.h"  // for BeamLimitingDevicePositionSequence
#include "dcmtk/dcmrt/seq/drtcos.h" // for CompensatorSequence
#include "dcmtk/dcmrt/seq/drtbl2.h" // for BlockSequence
#include "dcmtk/dcmrt/seq/drtws.h"     // for WedgeSequence
#include "dcmtk/dcmrt/seq/drtrshs.h"
#include "dcmtk/dcmrt/seq/drtrlsds.h"
#include "dcmtk/dcmrt/seq/drtrms.h"

//-----------------------------------------------------------------------------
DicomReaderRTIonPlan::DicomReaderRTIonPlan(DcmDataset* dset) : DicomVReader(dset, DRM_RTIonPlan)
{
  ReadData();
}

//-----------------------------------------------------------------------------
void DicomReaderRTIonPlan::ReadData()
{
  DRTIonPlanIOD rtplan;
  /*DRTApplicatorSequenceInRTIonBeamsModule
  DRTGeneralAccessorySequence
  DRTIonBeamLimitingDeviceSequence
  DRTLateralSpreadingDeviceSequence
  DRTRangeModulatorSequence
  RangeShifterSequence
  ReferencedBolusSequence
  ReferencedDoseSequence
  DRTReferencedReferenceImageSequenceInRTIonBeamsModule
  DRTSnoutSequence */
  OFCondition result = rtplan.read(*theDataset);
  if (!result.good()) {
    G4Exception("DicomReaderRTIonPlan::ReadData",
		"DFS001",
		FatalException,
		result.text());
  }
  DRTFractionGroupSequence frgSeq = rtplan.getFractionGroupSequence();
  if( frgSeq.isEmpty() ) {
    G4Exception("DicomReaderRTIonPlan::ReadData",
		"DFS002",
		JustWarning,
		"DRTFractionGroupSequence is empty");
  }
  if( DicomVerb(debugVerb) ) G4cout << "@@@@@ NUMBER OF FractionSequences " << frgSeq.getNumberOfItems() << G4endl;
  frgSeq.gotoFirstItem();
  for( size_t i1 = 0; i1 < frgSeq.getNumberOfItems(); i1++ ) {
    DRTFractionGroupSequence::Item &rfgItem = frgSeq.getCurrentItem();

    if( rfgItem.getFractionGroupNumber(fint) == EC_Normal ) {
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " FractionGroupNumber " << fint << G4endl;
      SetParam("FractionGroup_"+GmGenUtils::itoa(fint)+"_Number",fint);
    } else {
      DicomObjectException("DicomReaderRTIonPlan","FractionBeamNumber");
    } 
    std::string fracNo = GmGenUtils::itoa(fint);
    
    if( rfgItem.getBeamDoseMeaning(fstr) == EC_Normal ) {
      SetParamStr("FractionGroup_"+fracNo+"_BeamDoseMeaning",fstr);
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " BeamDoseMeaning " << fstr << G4endl;
    }
    if( rfgItem.getFractionGroupDescription(fstr) == EC_Normal ) {
      SetParamStr("FractionGroup_"+fracNo+"_FractionGroupDescription",fstr);
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " FractionGroupDescription " << fstr << G4endl;
    }
    if( rfgItem.getFractionPattern(fstr) == EC_Normal ) {
      SetParamStr("FractionGroup_"+fracNo+"_FractionPattern",fstr);
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " FractionPattern " << fstr << G4endl;
    }
    if( rfgItem.getNumberOfBeams(fint) == EC_Normal ) {
      SetParam("FractionGroup_"+fracNo+"_NumberOfBeams",fint);
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " NumberOfBeams " << fint << G4endl;
    } else {
      DicomObjectException("DicomReaderRTIonPlan","NumberOfBeams");
    }  
    theNumberOfBeams = fint;
    if( rfgItem.getNumberOfBrachyApplicationSetups(fint) == EC_Normal ) {
      SetParam("FractionGroup_"+fracNo+"_NumberOfBrachyApplicationSetups",fint);
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " NumberOfBrachyApplicationSetups " << fint << G4endl;
      //      CheckData0(" NumberOfBrachyApplicationSetups ", fint);
    }
    if( rfgItem.getNumberOfFractionPatternDigitsPerDay(fint) == EC_Normal ) {
      SetParam("FractionGroup_"+fracNo+"_NumberOfFractionPatternDigitsPerDay",fint);
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " NumberOfFractionPatternDigitsPerDay " << fint << G4endl;
    }
    if( rfgItem.getNumberOfFractionsPlanned(fint) == EC_Normal ) {
      SetParam("FractionGroup_"+fracNo+"_NumberOfFractionsPlanned",fint);
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " NumberOfFractionsPlanned " << fint << G4endl;
    }
    if( rfgItem.getRepeatFractionCycleLength(fint) == EC_Normal ) {
      SetParam("FractionGroup_"+fracNo+"_RepeatFractionCycleLength",fint);
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " RepeatFractionCycleLength " << fint << G4endl;
    }

    //-----------------     DRTReferencedBeamSequenceInRTFractionSchemeModule
    DRTReferencedBeamSequenceInRTFractionSchemeModule refBeamSeq = rfgItem.getReferencedBeamSequence();
//       DRTReferencedDoseReferenceSequenceInRTFractionSchemeModule &getReferencedDoseReferenceSequence()
    if( DicomVerb(debugVerb) ) G4cout << " @@@ NUMBER OF ReferencedBeamSequences " << refBeamSeq.getNumberOfItems() << G4endl;
    //--- Each refBeamSeq corresponds to a DicomBeam
    refBeamSeq.gotoFirstItem();
    for( size_t i2 = 0; i2 < refBeamSeq.getNumberOfItems(); i2++ ) {
      DicomBeam* db = new DicomBeam(1);
      theBeams.push_back(db);
      DRTReferencedBeamSequenceInRTFractionSchemeModule::Item &rbsItem = refBeamSeq.getCurrentItem();

      if( rbsItem.getReferencedBeamNumber(fint) == EC_Normal ) {
	if( DicomVerb(debugVerb) ) G4cout << "  " << i2 << " ReferencedBeamNumber " << fint << G4endl;
	db->SetNumber(fint);
	db->SetParam("BeamNumber",fint);
      } else {
	DicomObjectException("DicomReaderRTIonPlan","ReferencedBeamNumber");
      }
      std::string beamNo = GmGenUtils::itoa(fint);

      if( rbsItem.getBeamDeliveryDurationLimit(ffloat) == EC_Normal ) {
	db->SetParam("BeamDeliveryDurationLimit",ffloat);
	if( DicomVerb(debugVerb) ) G4cout << "  " << i2 << " BeamDeliveryDurationLimit " << ffloat << G4endl;
      }
      if( rbsItem.getBeamDose(ffloat) == EC_Normal ) {
	db->SetParam("BeamDose",ffloat);
	if( DicomVerb(debugVerb) ) G4cout << "  " << i2 << " BeamDose " << ffloat << G4endl; // dose at dose point
      }
      if( rbsItem.getBeamDoseSpecificationPoint(fvfloat) == EC_Normal && fvfloat.size() != 0 ) {
	db->SetParam("BeamDoseSpecificationPoint_X",fvfloat[0]);
	db->SetParam("BeamDoseSpecificationPoint_Y",fvfloat[1]);
	db->SetParam("BeamDoseSpecificationPoint_Z",fvfloat[2]);
	if( DicomVerb(debugVerb) ) G4cout << "  " << i2 << " BeamDoseSpecificationPoint (" << fvfloat[0] << "," << fvfloat[1] << "," << fvfloat[2] << ")" << G4endl;
      }
      if( rbsItem.getBeamMeterset(ffloat) == EC_Normal ) {
	db->SetParam("BeamMeterset",ffloat);
	if( DicomVerb(debugVerb) ) G4cout << "  " << i2 << " BeamMeterset " << ffloat << G4endl;
      }
      refBeamSeq.gotoNextItem();
    }

    frgSeq.gotoNextItem();
  }

  //t DRTPatientSetupSequence patientSet = rtplan.getPatientSetupSequence()
  //----------------- DRTIonBeamSequence
  DRTIonBeamSequence beamSeq = rtplan.getIonBeamSequence();
  if( beamSeq.isEmpty() ) {
    G4Exception("DicomReaderRTIonPlan::ReadData",
		"DFS002",
		JustWarning,
		"DRTIonBeamSequence is empty");
  }
  if( DicomVerb(debugVerb) ) G4cout << "@@@@@ NUMBER OF IonBeamSequences " << beamSeq.getNumberOfItems() << G4endl;
  beamSeq.gotoFirstItem();
  for( size_t i1 = 0; i1 < beamSeq.getNumberOfItems(); i1++ ) {
    DicomBeam* db = 0;
    DRTIonBeamSequence::Item &beamItem = beamSeq.getCurrentItem();

    if( beamItem.getBeamNumber(fint) == EC_Normal ) {
      if( theBeams[i1]->GetParam("BeamNumber") == fint ) {
	db = theBeams[i1];
      } else {
	db = new DicomBeam(0);
	theBeams.push_back(db);
	G4cerr << i1 << " !! " << theBeams[i1]->GetParam("BeamNumber") << " != " << fint << G4endl; 
	G4Exception("DicomReaderRTIonPlan::ReadData",
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
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " Manufacturer " << fstr << G4endl;
    }
    if( beamItem.getManufacturerModelName(fstr) == EC_Normal ) {
      db->SetParamStr("ManufacturerModelName",fstr);
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " ManufacturerModelName " << fstr << G4endl;
    }
    if( beamItem.getTreatmentMachineName(fstr) == EC_Normal ) {
      db->SetParamStr("TreatmentMachineName",fstr);
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " TreatmentMachineName " << fstr << G4endl;
    }
    if( beamItem.getTreatmentDeliveryType(fstr) == EC_Normal ) {
      db->SetParamStr("TreatmentDeliveryType",fstr);
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " TreatmentDeliveryType " << fstr << G4endl;
    }
    if( beamItem.getPrimaryDosimeterUnit(fstr) == EC_Normal ) {
      db->SetParamStr("PrimaryDosimeterUnit",fstr);
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " PrimaryDosimeterUnit " << fstr << G4endl;
    }
    if( beamItem.getVirtualSourceAxisDistances(ffloat32,0) == EC_Normal ) { 
      db->SetParam("VirtualSourceAxisDistanceX",ffloat32);
      //t    if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " VirtualSourceAxisDistances " << fvfloat[0] << " " << fvfloat[1] << G4endl;
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " VirtualSourceAxisDistancesX " << ffloat32 << G4endl;
      //-    db->SetSourceAxisDistance(ffloat);
    }
    if( beamItem.getVirtualSourceAxisDistances(ffloat32,1) == EC_Normal ) { 
      db->SetParam("VirtualSourceAxisDistanceY",ffloat32);
      //t    if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " VirtualSourceAxisDistances " << fvfloat[0] << " " << fvfloat[1] << G4endl;
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " VirtualSourceAxisDistancesY " << ffloat32 << G4endl;
      //-    db->SetSourceAxisDistance(ffloat);
    }    
    DRTIonBeamLimitingDeviceSequence beamLDS = beamItem.getIonBeamLimitingDeviceSequence();
    if( DicomVerb(debugVerb) ) G4cout << " @@@ NUMBER OF BeamLimitingDeviceSequence " << beamLDS.getNumberOfItems() << G4endl;
    beamLDS.gotoFirstItem();
    for( size_t i2 = 0; i2 < beamLDS.getNumberOfItems(); i2++ ) {
      DRTIonBeamLimitingDeviceSequence::Item bldsItem = beamLDS.getCurrentItem();
      DicomBeamDeviceBLD* dbd = new DicomBeamDeviceBLD(bldsItem);
      db->AddDevice(dbd);
      
      beamLDS.gotoNextItem();
    }
    
    if( beamItem.getBeamName(fstr) == EC_Normal ) {
      db->SetParamStr("BeamName",fstr);
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " BeamName " << fstr << G4endl;
    }
    if( beamItem.getBeamDescription(fstr) == EC_Normal ) {
      db->SetParamStr("BeamDescription",fstr);
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " BeamDescription " << fstr << G4endl;
    }
    if( beamItem.getBeamType(fstr) == EC_Normal ) {
      db->SetParamStr("BeamType",fstr);
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " BeamType " << fstr << G4endl;
    }
    if( beamItem.getRadiationType(fstr) == EC_Normal ) {
      db->SetParamStr("RadiationType",fstr);
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " RadiationType " << fstr << G4endl;
    }     
    if( beamItem.getTreatmentDeliveryType(fstr) == EC_Normal ) {
      db->SetParamStr("TreatmentDeliveryType",fstr);
    }
    if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " TreatmentDeliveryType " << fstr << G4endl;
    if( beamItem.getNumberOfWedges(fint) == EC_Normal ) {
      db->SetParam("NumberOfWedges",fint);
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " NumberOfWedges " << fint << G4endl;
    }
    DRTIonWedgeSequence beamWedge = beamItem.getIonWedgeSequence();
    beamWedge.gotoFirstItem();
    for( size_t i2 = 0; i2 < beamWedge.getNumberOfItems(); i2++ ) {
      DRTIonWedgeSequence::Item bwedItem = beamWedge.getCurrentItem();
      DicomBeamWedge* dbwed = new DicomBeamWedge( bwedItem );
      db->AddWedge( dbwed );
      beamWedge.gotoNextItem();
    }
    
    if( beamItem.getNumberOfCompensators(fint) == EC_Normal ) {
      db->SetParam("NumberOfCompensators",fint);
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " NumberOfIonRangeCompensators " << fint << G4endl;
    }
    DRTIonRangeCompensatorSequence beamCompens = beamItem.getIonRangeCompensatorSequence();
    beamCompens.gotoFirstItem();
    for( size_t i2 = 0; i2 < beamCompens.getNumberOfItems(); i2++ ) {
      DRTIonRangeCompensatorSequence::Item bcompItem = beamCompens.getCurrentItem();
      DicomBeamCompensator* dbcomp = new DicomBeamCompensator( bcompItem );
      db->AddCompensator( dbcomp );
      beamCompens.gotoNextItem();
    }
    
    if( beamItem.getNumberOfBoli(fint) == EC_Normal ) {
      db->SetParam("NumberOfBoli",fint);
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " NumberOfBoli " << fint << G4endl; //Bolus has no relevant info (see drtrbos1.h)
    }
    
    if( beamItem.getNumberOfBlocks(fint) == EC_Normal ) {
      db->SetParam("NumberOfBlocks",fint);
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " NumberOfBlocks " << fint << G4endl;
    }
    DRTIonBlockSequence beamBlock = beamItem.getIonBlockSequence();
    beamBlock.gotoFirstItem();
    for( size_t i2 = 0; i2 < beamBlock.getNumberOfItems(); i2++ ) {
      DRTIonBlockSequence::Item bblItem = beamBlock.getCurrentItem();
      DicomBeamBlock* dbbl = new DicomBeamBlock( bblItem );
      db->AddBlock( dbbl );
      beamBlock.gotoNextItem();
    }
    
    if( beamItem.getFinalCumulativeMetersetWeight(ffloat) == EC_Normal ) {
      db->SetParam("FinalCumulativeMetersetWeight",ffloat);
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " FinalCumulativeMetersetWeight " << ffloat << G4endl;
    }
    if( beamItem.getScanMode(fstr) == EC_Normal ) {
      db->SetParamStr("ScanMode",fstr);
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " ScanMode " << fstr << G4endl;
    }
    if( beamItem.getNumberOfCompensators(fint) == EC_Normal ) {
      db->SetParam("NumberOfCompensators",fint);
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " NumberOfIonRangeCompensators " << fint << G4endl;
    }
    DRTSnoutSequence snoutSeq = beamItem.getSnoutSequence();
    /*t    snoutSeq.gotoFirstItem();
    for( size_t i2 = 0; i2 < snoutSeq.getNumberOfItems(); i2++ ) {
      DRTSnoutSequence::Item snoutItem = snoutSeq.getCurrentItem();
      DicomBeamSnout* dbsnout = new DicomBeamSnout( snoutItem );
      db->AddSnout( dbsnout );
      
      snoutItem.getSnoutID(fstr) == EC_Normal ) {
      db->SetParamStr("SnoutID",fstr);
      if( DicomVerb(debugVerb) ) G4cout << "  " << i2 << " Snout ID " << fstr << G4endl;
      snoutItem.getAccessoryCode(fstr) == EC_Normal ) {
      db->SetParamStr("AccessoryCode",fstr);
      if( DicomVerb(debugVerb) ) G4cout << "  " << i2 << " Snout AccessoryCode " << fstr << G4endl;
      } */
    
    if( beamItem.getNumberOfRangeShifters(fint) == EC_Normal ) {
      db->SetParam("NumberOfRangeShifters",fint);
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " NumberOfIonRangeShifters " << fint << G4endl;
    }
    if(fint > 1 ) {
      G4Exception("DicomReaderRTIonPlan::ReadData",
		  "",
		  FatalException,
		  ("NumberOfRangeShifters cannot be > 1. Please contact GAMOS authors"+GmGenUtils::itoa(fint)).c_str());
    }    
    // Retrieve Range Shifter ID 
    // Access the Range Shifter Sequence
    DRTRangeShifterSequence &rangeShifterSeq = beamItem.getRangeShifterSequence();
    // Get the number of items in the Range Shifter Sequence
    const unsigned long numItemsrhs = rangeShifterSeq.getNumberOfItems();
    if(int(numItemsrhs) != fint ) {
      G4Exception("DicomReaderRTIonPlan::ReadData",
		  "",
		  FatalException,
		  ("beamItem.getNumberOfRangeShifters() != rangeShifterSeq.getNumberOfItems(). Please contact GAMOS authors"+GmGenUtils::itoa(fint)+" , "+GmGenUtils::itoa(numItemsrhs)).c_str());
    }
    DRTRangeShifterSequence::Item itemrsh = rangeShifterSeq.getItem(0);
    OFString rangeShifterID;
    OFCondition status = itemrsh.getRangeShifterID(rangeShifterID);
    if( status.good() ) {
      G4cout << "Range Shifter ID: " << rangeShifterID << G4endl;
      db->SetParam("RangeShifterID",GmGenUtils::GetInt(rangeShifterID.c_str()));
    } else {
      G4cerr << "Error: Range Shifter ID not found in DICOM file." << G4endl;
    }
    
    //--- LateralSpreadingDevices
    if( beamItem.getNumberOfLateralSpreadingDevices(fint) == EC_Normal ) {
      db->SetParam("NumberOfLateralSpreadingDevices",fint);
    }
    DRTLateralSpreadingDeviceSequence beamLSD = beamItem.getLateralSpreadingDeviceSequence();
    const unsigned long numItemslsd = beamLSD.getNumberOfItems();
    if(int(numItemslsd) != fint ) {
      G4Exception("DicomReaderRTIonPlan::ReadData",
		  "",
		  FatalException,
		  ("beamItem.getNumberOfLateralSpreadingDevices() != beamLSD.getNumberOfItems(). Please contact GAMOS authors"+GmGenUtils::itoa(fint)+" , "+GmGenUtils::itoa(numItemslsd)).c_str());
    }
    //    for( size_t i2 = 0; i2 < beamLSD.getNumberOfItems(); i2++ ) {
    DRTLateralSpreadingDeviceSequence::Item itemlsd = beamLSD.getItem(0);
    OFString LatSpreDevType;
    status = itemlsd.getLateralSpreadingDeviceType(LatSpreDevType);
    if( status.good() ) {
      G4cout << "LateralSpreadingDeviceType: " << LatSpreDevType << G4endl;
      db->SetParamStr("LateralSpreadingDeviceType",LatSpreDevType);
    } else {
      G4cerr << "Error: LateralSpreadingDeviceType not found in DICOM file." << G4endl;
    } 

    //--- Range modulator 
    if( beamItem.getNumberOfRangeModulators(fint) == EC_Normal ) {
      db->SetParam("NumberOfRangeModulators",fint);
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " NumberOfRangeModulators " << fint << G4endl;
    }
    if(fint > 1 ) {
      G4Exception("DicomReaderRTIonPlan::ReadData",
		  "",
		  FatalException,
		  ("NumberOfRangeModulators cannot be > 1. Please contact GAMOS authors"+GmGenUtils::itoa(fint)).c_str());
    }
	
    DRTRangeModulatorSequence beamRangeModSeq = beamItem.getRangeModulatorSequence();
    // Get the number of items in the Range Shifter Sequence
    const unsigned long numItemrm = beamRangeModSeq.getNumberOfItems();
    if(int(numItemrm) != fint ) {
      G4Exception("DicomReaderRTIonPlan::ReadData",
		  "",
		  FatalException,
		  ("beamItem.getNumberOfRangeModulators() != beamRangeModSeq.getNumberOfItems(). Please contact GAMOS authors"+GmGenUtils::itoa(fint)+" , "+GmGenUtils::itoa(numItemrm)).c_str());
    }
    DRTRangeModulatorSequence::Item itemrm = beamRangeModSeq.getItem(0);
    OFString RangModType;
    status = itemrm.getRangeModulatorType(RangModType);
    if( status.good() ) {
      G4cout << "RangeModulatorType: " << RangModType << G4endl;
      db->SetParamStr("RangeModulatorType",RangModType);
    } else {
      G4cerr << "Error: RangeModulatorType not found in DICOM file." << G4endl;
    } 
    //    DicomBeamRangeModulator* dbcomp = new DicomBeamRangeModulator( bcompItem );
    //      db->AddRangeModulator( dbcomp );
    //      beamRangeMod.gotoNextItem();

    
    if( beamItem.getDeviceSerialNumber(fstr) == EC_Normal ) {
      db->SetParamStr("DeviceSerialNumber",fstr);
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " DeviceSerialNumber " << fstr << G4endl;
    }
    if( beamItem.getInstitutionAddress(fstr) == EC_Normal ) {
      db->SetParamStr("InstitutionAddress",fstr);
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " InstitutionAddress " << fstr << G4endl;
    }
    if( beamItem.getInstitutionName(fstr) == EC_Normal ) {
      db->SetParamStr("InstitutionName",fstr);
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " InstitutionName " << fstr << G4endl;
    }
    if( beamItem.getInstitutionalDepartmentName(fstr) == EC_Normal ) {
      db->SetParamStr("InstitutionalDepartmentName",fstr);
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " InstitutionalDepartmentName " << fstr << G4endl;
    }
    if( beamItem.getReferencedPatientSetupNumber(fint) == EC_Normal ) {
      db->SetParam("ReferencedPatientSetupNumber",fint);
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " ReferencedPatientSetupNumber " << fint << G4endl;
    }
    if( beamItem.getReferencedToleranceTableNumber(fint) == EC_Normal ) {
      db->SetParam("ReferencedToleranceTableNumber",fint);
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " ReferencedToleranceTableNumber " << fint << G4endl;
    }
    if( beamItem.getPatientSupportType(fstr) == EC_Normal ) {
      db->SetParamStr("PatientSupportType",fstr);
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " PatientSupportType " << fstr << G4endl;
    }
    if( beamItem.getPatientSupportID(fstr) == EC_Normal ) {
      db->SetParamStr("PatientSupportID",fstr);
      if( DicomVerb(debugVerb) ) G4cout << " " << i1 << " PatientSupportID " << fstr << G4endl;
    }    
    if( beamItem.getNumberOfControlPoints(fint) == EC_Normal ) {
      db->SetParam("NumberOfControlPoints",fint);
    }
    
    DRTIonControlPointSequence controlPSeq = beamItem.getIonControlPointSequence();
    if( DicomVerb(debugVerb) ) G4cout << " @@@ NUMBER OF ControlPointSequences " << controlPSeq.getNumberOfItems() << " = " << fint << G4endl;
    controlPSeq.gotoFirstItem();
    DicomBeamRTIonControlPoint* dbcp0 = 0; // Only first ControlPoint has some info if it does not change 
    for( size_t i2 = 0; i2 < controlPSeq.getNumberOfItems(); i2++ ) {
      DRTIonControlPointSequence::Item &cpItem = controlPSeq.getCurrentItem();
      if( db->GetNControlPoints() != 0 ) dbcp0 = dynamic_cast<DicomBeamRTIonControlPoint*>(db->GetControlPoint(0));
      DicomBeamRTIonControlPoint* dbcp = new DicomBeamRTIonControlPoint( cpItem, dbcp0 );
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
void DicomReaderRTIonPlan::CheckData0(OFString title, Sint32 val )
{
  if( val != 0 ){
    G4Exception("DicomReaderRTIonPlan::CheckData",
		"DFP003",
		FatalException,
		(title + " exists, and code is not ready ").c_str());
  }
}

//-----------------------------------------------------------------------------
void DicomReaderRTIonPlan::SetControlPointMetersets()
{
  for( size_t ii = 0; ii < theBeams.size(); ii++ ){
    theBeams[ii]->SetControlPointMetersets();
  }
}

//-----------------------------------------------------------------------------
void DicomReaderRTIonPlan::DumpToFile()
{
  std::ofstream fout("RTIonPlan");
  std::string name = "";
  DicomVObjectWithParams::Print( fout, name );
 
  for( size_t ii = 0; ii < theBeams.size(); ii++ ){
    theBeams[ii]->DumpToFileF("RTIonPlanBeam_"+GmGenUtils::ftoa(theBeams[ii]->GetParam("BeamNumber",1)));
  }
}
  
