#include "DicomBeamDeviceBLD.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"
#include "dcmtk/dcmrt/seq/drtrbs8.h" // DRTReferencedBeamSequenceInRTFractionSchemeModule
#include "globals.hh"
#include "dcmtk/dcmrt/seq/drtblds1.h" // for BeamLimitingDeviceSequenceInRTBeamsModule

//-----------------------------------------------------------------------------
DicomBeamDeviceBLD::DicomBeamDeviceBLD(DRTBeamLimitingDeviceSequenceInRTBeamsModule::Item bldsItem)
{
  OFString fstr;
  Sint32 fint;
  Float64 ffloat;
  OFVector<Float64> fvfloat;

  bldsItem.getRTBeamLimitingDeviceType(fstr);
   if( DicomVerb(debugVerb) ) G4cout << "   " << " RTBeamLimitingDeviceType " << fstr << G4endl;
  SetType("BLD_"+fstr);
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
DicomBeamDeviceBLD::DicomBeamDeviceBLD(DRTIonBeamLimitingDeviceSequence::Item bldsItem)
{
  OFString fstr;
  Sint32 fint;
  //  Float64 ffloat;
  Float32 ffloat32;
  OFVector<Float64> fvfloat;

  bldsItem.getRTBeamLimitingDeviceType(fstr);
  if( DicomVerb(debugVerb) ) G4cout << "   " << " RTBeamLimitingDeviceType " << fstr << G4endl;
  SetType(fstr);
  bldsItem.getIsocenterToBeamLimitingDeviceDistance(ffloat32);
  if( DicomVerb(debugVerb) ) G4cout << "   " << " IsocenterToBeamLimitingDeviceDistance " << ffloat32 << G4endl;
  //  SetSourceToBeamLimitingDeviceDistance( ffloat ); 
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
void DicomBeamDeviceBLD::DumpToFile( std::ofstream& fout )
{

  fout << ":P " << theType << "_Z " << theSourceToBeamLimitingDeviceDistance << G4endl;
  fout << ":P " << theType << "_NumberOfLeafJawPairs " << theNumberOfLeafJawPairs << G4endl;

  /*  if( theType == "MLCX" || theType == "MLCY" ) {
    G4int nLeafs = theNumberOfLeafJawPairs;
    for( G4int jj = 0; jj < nLeafs; jj++ ){
      fout << ":P " << theType << "_" + std::to_string(jj+1) + "_CROSS " << thePositionBoundaries[jj] << G4endl;
    }
    }*/
  
}

//-----------------------------------------------------------------------------
void DicomBeamDeviceBLD::Print( std::ostream&  )
{

}
  
