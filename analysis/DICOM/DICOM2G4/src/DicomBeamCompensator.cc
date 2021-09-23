#include "DicomBeamCompensator.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"
#include "dcmtk/dcmrt/seq/drtcos.h"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

// DOC at https://www.dabsoft.ch/dicom/3/C.8.8.14/
//-----------------------------------------------------------------------------
DicomBeamCompensator::DicomBeamCompensator(DRTCompensatorSequence::Item bcompItem)
{
  theCompensatorColumns = 0;
  theCompensatorRows = 0;
  
  if( DicomVerb(debugVerb) ) G4cout << " DicomBeamCompensator::DicomBeamCompensator " << G4endl;
  if( bcompItem.getCompensatorNumber(fint) == EC_Normal ) {
    theParams["CompensatorNumber"] = fint;
  } else {
    DicomObjectException("DicomBeamCompensator","CompensatorNumber");
  }
  if( DicomVerb(debugVerb) ) G4cout << " Number " << fint << G4endl;
  
  if( bcompItem.getCompensatorColumns(fint) == EC_Normal ) {
    theParams["CompensatorColumns"] = fint;
    theCompensatorColumns = fint;
  } else {
    DicomObjectException("DicomBeamCompensator","CompensatorColumns");
  }
  if( bcompItem.getCompensatorRows(fint) == EC_Normal ) {
    theParams["CompensatorRows"] = fint;
    theCompensatorRows = fint;
  } else {
    DicomObjectException("DicomBeamCompensator","CompensatorRows");
  }
  
  //  first value is the spacing between the center of adjacent rows, and the second value (column spacing) is the spacing between the center of adjacent columns.  
  if( bcompItem.getCompensatorPixelSpacing(fvfloat) == EC_Normal && fvfloat.size() != 0 ) {
    theParams["CompensatorPixelSpacing_1"] = fvfloat[0];
    theParams["CompensatorPixelSpacing_2"] = fvfloat[1];
  } else {
    DicomObjectException("DicomBeamCompensator","CompensatorPixelSpacing");
  }
  
  if( bcompItem.getCompensatorPosition(fvfloat) == EC_Normal && fvfloat.size() != 0 ) {
    theParams["CompensatorPosition_X"] = fvfloat[0];
    theParams["CompensatorPosition_Y"] = fvfloat[1];
  } else {
    DicomObjectException("DicomBeamCompensator","CompensatorPosition");
  }
  
  if( bcompItem.getCompensatorTransmissionData(fvfloat) == EC_Normal && fvfloat.size() != 0 ) {
    theParams["CompensatorTransmissionData_X"] = fvfloat[0];
    theParams["CompensatorTransmissionData_Y"] = fvfloat[1]; // HOW MANY? 
  } else {
    DicomObjectException("DicomBeamCompensator","CompensatorTransmissionData");
  }
  if( bcompItem.getCompensatorThicknessData(fvfloat) == EC_Normal && fvfloat.size() != 0 ) {
    theParams["CompensatorThicknessData_X"] = fvfloat[0];
    theParams["CompensatorThicknessData_Y"] = fvfloat[1]; // HOW MANY? 
  } else {
    DicomObjectException("DicomBeamCompensator","CompensatorThicknessData");
  }
  //  if( cond.good() ) theCompensatorThicknessData = fvfloat;
  
  if( bcompItem.getCompensatorTrayID(fstr) == EC_Normal ) {
    theParamStrs["CompensatorTrayID"] = fstr;
  } else {
    DicomObjectException("DicomBeamCompensator","CompensatorTrayID");
  }
  if( bcompItem.getCompensatorType(fstr) == EC_Normal ) {
    theParamStrs["CompensatorType"] = fstr;
  } else {
    DicomObjectException("DicomBeamCompensator","CompensatorType");
  }
  
  if( bcompItem.getMaterialID(fstr) == EC_Normal ) {
    theParamStrs["MaterialID"] = fstr;
  } else {
    DicomObjectException("DicomBeamCompensator","MaterialID");
  }
  
  if( bcompItem.getSourceToCompensatorDistance(fvfloat) == EC_Normal && fvfloat.size() != 0 ) {
    for( size_t ii = 0; ii < fvfloat.size(); ii++ ) {
      int iCol = ii%theCompensatorColumns;
      int iRow = ii/theCompensatorColumns;
      theParams["SourceToCompensatorDistance_" + GmGenUtils::itoa(iRow) + "_" + GmGenUtils::itoa(iCol)] = fvfloat[ii];
    }
  } else {
    DicomObjectException("DicomBeamCompensator","SourceToCompensatorDistance");
  }
  
  if( bcompItem.getSourceToCompensatorTrayDistance(ffloat) == EC_Normal ) {
    theParams["SourceToCompensatorTrayDistance"] = ffloat;
  } else {
    DicomObjectException("DicomBeamCompensator","SourceToCompensatorTrayDistance");
  }
  
  if( bcompItem.getCompensatorDescription(fstr) == EC_Normal ) {
    theParamStrs["CompensatorDescription"] = fstr;
  } else {
    DicomObjectException("DicomBeamCompensator","CompensatorDescription");
  }
  if( bcompItem.getCompensatorDivergence(fstr) == EC_Normal ) {
    theParamStrs["CompensatorDivergence"] = fstr;
  } else {
    DicomObjectException("DicomBeamCompensator","CompensatorDivergence");
  }
  if( bcompItem.getCompensatorID(fstr) == EC_Normal ) {
    theParamStrs["CompensatorID"] = fstr;
  } else {
    DicomObjectException("DicomBeamCompensator","CompensatorID");
  }
  if( bcompItem.getCompensatorMountingPosition(fstr) == EC_Normal ) {
    theParamStrs["CompensatorMountingPosition"] = fstr;
  } else {
    DicomObjectException("DicomBeamCompensator","CompensatorMountingPosition");
  }
  if( bcompItem.getAccessoryCode(fstr) == EC_Normal ) {
    theParamStrs["AccessoryCode"] = fstr;
  } else {
    DicomObjectException("DicomBeamCompensator","AccessoryCode");
  }
  
}

//-----------------------------------------------------------------------------
DicomBeamCompensator::DicomBeamCompensator(DRTIonRangeCompensatorSequence::Item bcompItem)
{
  theCompensatorColumns = 0;
  theCompensatorRows = 0;
   if( DicomVerb(debugVerb) ) G4cout << " DicomBeamCompensator::DicomBeamCompensator " << G4endl;
  if( bcompItem.getCompensatorNumber(fint)  == EC_Normal ) {
      theParams["CompensatorNumber"] = fint;
      } else {
      DicomObjectException("DicomBeamCompensator","CompensatorNumber");
    }
   if( DicomVerb(debugVerb) ) G4cout << " Number " << fint << G4endl;
  
  if( bcompItem.getCompensatorColumns(fint) == EC_Normal ) {
    theParams["CompensatorColumns"] = fint;
    theCompensatorColumns = fint;
  } else {
    DicomObjectException("DicomBeamCompensator","CompensatorColumns");
  }
  if( bcompItem.getCompensatorRows(fint) == EC_Normal ) {
    theParams["CompensatorRows"] = fint;
    theCompensatorRows = fint;
  } else {
    DicomObjectException("DicomBeamCompensator","");
  }
  
  //  first value is the spacing between the center of adjacent rows, and the second value (column spacing) is the spacing between the center of adjacent columns.  
  if( bcompItem.getCompensatorPixelSpacing(fvfloat) == EC_Normal && fvfloat.size() != 0 ) {
    theParams["CompensatorPixelSpacing_1"] = fvfloat[0];
    theParams["CompensatorPixelSpacing_2"] = fvfloat[1];
  } else {
    DicomObjectException("DicomBeamCompensator","CompensatorPixelSpacing");
  }
  
  if( bcompItem.getCompensatorPosition(fvfloat) == EC_Normal && fvfloat.size() != 0 ) {
    theParams["CompensatorPosition_X"] = fvfloat[0];
    theParams["CompensatorPosition_Y"] = fvfloat[1];
  } else {
    DicomObjectException("DicomBeamCompensator","CompensatorPosition");
  }
  
  if( bcompItem.getCompensatorThicknessData(fvfloat) == EC_Normal && fvfloat.size() != 0 ) {
    theParams["CompensatorPosition_X"] = fvfloat[0];
    theParams["CompensatorPosition_Y"] = fvfloat[1];
  } else {
    DicomObjectException("DicomBeamCompensator","CompensatorPosition");
  }
  
  if( bcompItem.getMaterialID(fstr) == EC_Normal ) {
    theParamStrs["MaterialID"] = fstr;
  } else {
    DicomObjectException("DicomBeamCompensator","MaterialID");
  }
  
  if( bcompItem.getCompensatorDescription(fstr) == EC_Normal ) {
    theParamStrs["CompensatorDescription"] = fstr;
  } else {
    DicomObjectException("DicomBeamCompensator","CompensatorDescription");
  }
  if( bcompItem.getCompensatorDivergence(fstr) == EC_Normal ) {
    theParamStrs["CompensatorDivergence"] = fstr;
  } else {
    DicomObjectException("DicomBeamCompensator","CompensatorDivergence");
  }
  if( bcompItem.getCompensatorID(fstr) == EC_Normal ) {
    theParamStrs["CompensatorID"] = fstr;
  } else {
    DicomObjectException("DicomBeamCompensator","CompensatorID");
  }
  if( bcompItem.getCompensatorMountingPosition(fstr) == EC_Normal ) {
    theParamStrs["CompensatorMountingPosition"] = fstr;
  } else {
    DicomObjectException("DicomBeamCompensator","CompensatorMountingPosition");
  }
  if( bcompItem.getAccessoryCode(fstr) == EC_Normal ) {
    theParamStrs["AccessoryCode"] = fstr;
  } else {
    DicomObjectException("DicomBeamCompensator","AccessoryCode");
  }

}

//-----------------------------------------------------------------------------
void DicomBeamCompensator::DumpToFile( std::ofstream& fout )
{
  int num = GetParam("CompensatorNumber");
  std::string name = "COMPENSATOR_" + GmGenUtils::itoa(num) + "_";
  DicomVObjectWithParams::Print( fout, name );
  /*  for( size_t ii = 0; ii < theCompensatorTransmissionData.size(); ii++ ) {
    int iCol = ii%theCompensatorColumns;
    int iRow = ii/theCompensatorColumns;
    fout << name << "Transmission_" + GmGenUtils::itoa(iRow) + "_" + GmGenUtils::itoa(iCol) << " " << theCompensatorTransmissionData[ii] << G4endl;
  }

  for( size_t ii = 0; ii < theCompensatorThicknessData.size(); ii++ ) {
    int iCol = ii%theCompensatorColumns;
    int iRow = ii/theCompensatorColumns;
    fout << name << "Thickness_" + GmGenUtils::itoa(iRow) + "_" + GmGenUtils::itoa(iCol) << " " << theCompensatorThicknessData[ii] << G4endl;
  }
  */
}
  
