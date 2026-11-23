#include "DicomBeamWedge.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

// DOC at https://www.dabsoft.ch/dicom/3/C.8.8.14/
//-----------------------------------------------------------------------------
DicomBeamWedge::DicomBeamWedge(DRTWedgeSequence::Item bwItem)
{
  if( DicomVerb(debugVerb) ) G4cout << " DicomBeamWedge::DicomBeamWedge " << G4endl;

  if( bwItem.getWedgeID(fstr) == EC_Normal ) {
    theParamStrs["WedgeID"] = fstr;
  } else {
    DicomObjectException("DicomBeamWedge","WedgeID");
  }
  if( bwItem.getWedgeNumber(fint) == EC_Normal ) {
    theParams["WedgeNumber"] = fint;
  } else {
    DicomObjectException("DicomBeamWedge","WedgeNumber");
  }
  if( DicomVerb(debugVerb) ) G4cout << " Number " << fint << G4endl;
  if( bwItem.getWedgeType(fstr) == EC_Normal ) {
    theParamStrs["WedgeType"] = fstr;
  } else {
    DicomObjectException("DicomBeamWedge","WedgeType");
  }
  if( bwItem.getSourceToWedgeTrayDistance(ffloat) == EC_Normal ) {
    theParams["SourceToWedgeTrayDistance"] = ffloat;
  } else {
    DicomObjectException("DicomBeamWedge","SourceToWedgeTrayDistance");
  }
  if( bwItem.getWedgeAngle(fint) == EC_Normal ) {
    fint = CorrectByDirection(fint, fstr );
    theParams["WedgeAngle"] = fint;
  } else {
    DicomObjectException("DicomBeamWedge","WedgeAngle");
  }
  if( bwItem.getWedgeFactor(ffloat) == EC_Normal ) {
    theParams["WedgeFactor"] = ffloat;
  } else {
    DicomObjectException("DicomBeamWedge","WedgeFactor");
  }
}

DicomBeamWedge::DicomBeamWedge(DRTIonWedgeSequence::Item bwItem)
{
  if( bwItem.getWedgeID(fstr) == EC_Normal ) {
    theParamStrs["WedgeID"] = fstr;
  } else {
    DicomObjectException("DicomBeamWedge","WedgeID");
  }
  if( bwItem.getWedgeNumber(fint) == EC_Normal ) {
    theParams["WedgeNumber"] = fint;
  } else {
    DicomObjectException("DicomBeamWedge","WedgeNumber");
  }
  if( DicomVerb(debugVerb) ) G4cout << " Number " << fint << G4endl;
  if( bwItem.getWedgeType(fstr) == EC_Normal ) {
    theParamStrs["WedgeType"] = fstr;
  } else {
    DicomObjectException("DicomBeamWedge","WedgeType");
  }
  if( bwItem.getIsocenterToWedgeTrayDistance(ffloat32) == EC_Normal ) {
    theParams["IsocenterToWedgeTrayDistance"] = ffloat32;
  } else {
    DicomObjectException("DicomBeamWedge","IsocenterToWedgeTrayDistance");
  }
  if( bwItem.getWedgeAngle(fint) == EC_Normal ) {
    bwItem.getWedgeOrientation(fstr);
    fint = CorrectByDirection(fint, fstr );
    theParams["WedgeAngle"] = fint;
  } else {
    DicomObjectException("DicomBeamWedge","WedgeAngle");
  }
}

//-----------------------------------------------------------------------------
void DicomBeamWedge::DumpToFile( std::ofstream& fout )
{
  int numBlock = GetParam("WedgeNumber");
  std::string name  = "WEDGE_" + GmGenUtils::itoa(numBlock) + "_";
  DicomVObjectWithParams::Print( fout, name );
  
}
  
