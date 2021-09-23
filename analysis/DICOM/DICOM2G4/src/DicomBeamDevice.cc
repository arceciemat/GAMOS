#include "DicomBeamDevice.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"
#include "dcmtk/dcmrt/seq/drtrbs8.h" // DRTReferencedBeamSequenceInRTFractionSchemeModule
#include "globals.hh"

//-----------------------------------------------------------------------------
DicomBeamDevice::DicomBeamDevice(DRTBeamLimitingDeviceSequenceInRTBeamsModule::Item bldsItem)
{
  OFString fstr;
  Sint32 fint;
  Float64 ffloat;
  OFVector<Float64> fvfloat;

  bldsItem.getRTBeamLimitingDeviceType(fstr);
  if( DicomVerb(debugVerb) ) G4cout << "   " << " RTBeamLimitingDeviceType " << fstr << G4endl;
  SetType(fstr);
  bldsItem.getSourceToBeamLimitingDeviceDistance(ffloat);
  if( DicomVerb(debugVerb) ) G4cout << "   " << " SourceToBeamLimitingDeviceDistance " << ffloat << G4endl;
  SetSourceToBeamLimitingDeviceDistance( ffloat ); 
  bldsItem.getNumberOfLeafJawPairs(fint);
  SetNumberOfLeafJawPairs(fint);
  if( DicomVerb(debugVerb) ) G4cout << "   " << " NumberOfLeafJawPairs " << fint << G4endl;
  bldsItem.getLeafPositionBoundaries(fvfloat);
  if( fint != 1 ) fint++;
  for( int ii = 0; ii < fint; ii++ ) {
     if( DicomVerb(debugVerb) ) G4cout << "   " << ii << " LeafPositionBoundaries " << fvfloat[ii] << G4endl;
    AddPositionBoundary(fvfloat[ii]);
  }
}

//-----------------------------------------------------------------------------
DicomBeamDevice::DicomBeamDevice(DRTBeamLimitingDevicePositionSequence::Item bldpsItem)
{
  OFString fstr;
  Float64 ffloat;
  
  bldpsItem.getRTBeamLimitingDeviceType(fstr);
   if( DicomVerb(debugVerb) ) G4cout << "    " << " BeamLimitingDeviceType " << fstr << G4endl;
  SetType(fstr);
  for(size_t ii = 0;; ii++ ){
    if( bldpsItem.getLeafJawPositions(ffloat,ii) == EC_Normal ){
       if( DicomVerb(debugVerb) ) G4cout << "    " << ii << " LeafPositionBoundaries " << ffloat << G4endl;
      AddPositionBoundary(ffloat);
    } else {
      break;
    }
  }
}

//-----------------------------------------------------------------------------
void DicomBeamDevice::Print( std::ostream&  )
{

}
  
