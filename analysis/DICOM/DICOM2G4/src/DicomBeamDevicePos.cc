#include "DicomBeamDevicePos.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"
#include "dcmtk/dcmrt/seq/drtrbs8.h" // DRTReferencedBeamSequenceInRTFractionSchemeModule
#include "globals.hh"

//-----------------------------------------------------------------------------
DicomBeamDevicePos::DicomBeamDevicePos(DRTBeamLimitingDevicePositionSequence::Item bldpsItem)
{
  OFString fstr;
  Float64 ffloat;
  
  bldpsItem.getRTBeamLimitingDeviceType(fstr);
  if( DicomVerb(debugVerb) ) G4cout << "    " << " BeamLimitingDeviceType " << fstr << G4endl;
  SetType(fstr);
  size_t ii = 0;
  for(;; ii++ ){
    if( bldpsItem.getLeafJawPositions(ffloat,ii) == EC_Normal ){
      if( DicomVerb(debugVerb) ) G4cout << "    " << ii << " LeafPositionBoundaries " << ffloat << G4endl;
      AddPositionBoundary(ffloat);
    } else {
      break;
    }
  }
  SetNumberOfLeafJawPairs(ii);

}

//-----------------------------------------------------------------------------
void DicomBeamDevicePos::DumpToFile( std::ofstream& fout )
{
  G4int nLeafs = theNumberOfLeafJawPairs;
  for( G4int jj = 0; jj < nLeafs; jj++ ){
    fout << ":P " << theType << "_" + std::to_string(jj+1) + "_POS " << thePositionBoundaries[jj] << G4endl;
  }
  
}

//-----------------------------------------------------------------------------
void DicomBeamDevicePos::Print( std::ostream&  )
{

}
  
