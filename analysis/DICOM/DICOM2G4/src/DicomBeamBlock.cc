#include "DicomBeamBlock.hh"
#include "dcmtk/dcmrt/seq/drtcos.h"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

// DOC at https://www.dabsoft.ch/dicom/3/C.8.8.14/
//-----------------------------------------------------------------------------
DicomBeamBlock::DicomBeamBlock(DRTBlockSequenceInRTBeamsModule::Item bblItem)
{
  if( bblItem.getBlockName(fstr) == EC_Normal ) {
    theParamStrs["BlockName"] = fstr;
  } else {
    DicomObjectException("DicomBeamBlock","BlockName");
  }
  if( bblItem.getBlockType(fstr) == EC_Normal ) {
    theParamStrs["BlockType"] = fstr;
  } else {
    DicomObjectException("DicomBeamBlock","BlockType");
  }
  if( bblItem.getBlockNumber(fint) == EC_Normal ) {
    theParams["BlockNumber"] = fint;
  } else {
    DicomObjectException("DicomBeamBlock","BlockNumber");
  }
  if( bblItem.getBlockTrayID(fstr) == EC_Normal ) {
    theParamStrs["BlockTrayID"] = fstr;
  } else {
    DicomObjectException("DicomBeamBlock","BlockTrayID");
  }
  if( bblItem.getMaterialID(fstr) == EC_Normal ) {
    theParamStrs["MaterialID"] = fstr;
  } else {
    DicomObjectException("DicomBeamBlock","MaterialID");
  }
  if( bblItem.getBlockNumberOfPoints(fint) == EC_Normal ) {
    theParams["BlockNumberOfPoints"] = fint;
  } else {
    DicomObjectException("DicomBeamBlock","BlockNumberOfPoints");
  }
  if( bblItem.getBlockData(theBlockData) == EC_Normal ) {
  } else {
    DicomObjectException("DicomBeamBlock","BlockData");
  }
  
  if( bblItem.getSourceToBlockTrayDistance(ffloat) == EC_Normal ) {
    theParams["SourceToBlockTrayDistance"] = ffloat;
  } else {
    DicomObjectException("DicomBeamBlock","SourceToBlockTrayDistance");
  }
  
  if( bblItem.getBlockMountingPosition(fstr) == EC_Normal ) {
    theParamStrs["BlockMountingPosition"] = fstr;
  } else {
    DicomObjectException("DicomBeamBlock","BlockMountingPosition");
  }
  
  if( bblItem.getBlockDivergence(fstr) == EC_Normal ) {
    theParamStrs["BlockDivergence"] = fstr;
  } else {
    DicomObjectException("DicomBeamBlock","BlockDivergence");
  }
  if( bblItem.getBlockThickness(ffloat) == EC_Normal ) {
    theParams["BlockThickness"] = ffloat;
  } else {
    DicomObjectException("DicomBeamBlock","BlockThickness");
  }
  if( bblItem.getBlockTransmission(ffloat) == EC_Normal ) {
    theParams["BlockTransmission"] = ffloat;
  } else {
    DicomObjectException("DicomBeamBlock","BlockTransmission");
  }

}

//-----------------------------------------------------------------------------
DicomBeamBlock::DicomBeamBlock(DRTIonBlockSequence::Item bblItem)
{
  if( DicomVerb(debugVerb) ) G4cout << " DicomBeamBlock::DicomBeamBlock " << G4endl;
  
  if( bblItem.getBlockName(fstr) == EC_Normal ) {
    theParamStrs["BlockName"] = fstr;
  } else {
    DicomObjectException("DicomBeamBlock","BlockName");
  }
  if( bblItem.getBlockType(fstr) == EC_Normal ) {
    theParamStrs["BlockType"] = fstr;
  } else {
    DicomObjectException("DicomBeamBlock","BlockType");
  }
  if( bblItem.getBlockNumber(fint) == EC_Normal ) {
    theParams["BlockNumber"] = fint;
  } else {
    DicomObjectException("DicomBeamBlock","BlockNumber");
  }
  if( bblItem.getBlockTrayID(fstr) == EC_Normal ) {
    theParamStrs["BlockTrayID"] = fstr;
  } else {
    DicomObjectException("DicomBeamBlock","BlockTrayID");
  }
  if( bblItem.getMaterialID(fstr) == EC_Normal ) {
    theParamStrs["MaterialID"] = fstr;
  } else {
    DicomObjectException("DicomBeamBlock","MaterialID");
  }
  if( bblItem.getBlockNumberOfPoints(fint) == EC_Normal ) {
    theParams["BlockNumberOfPoints"] = fint;
  } else {
    DicomObjectException("DicomBeamBlock","BlockNumberOfPoints");
  }
  if( bblItem.getBlockData(theBlockData) == EC_Normal ) {
  } else {
    DicomObjectException("DicomBeamBlock","BlockData");
  }
  
  if( bblItem.getIsocenterToBlockTrayDistance(ffloat32) == EC_Normal ) {
    theParams["IsocenterToBlockTrayDistance"] = ffloat32;
  } else {
    DicomObjectException("DicomBeamBlock","IsocenterToBlockTrayDistance");
  }
  
  if( bblItem.getBlockMountingPosition(fstr) == EC_Normal ) {
    theParamStrs["BlockMountingPosition"] = fstr;
  } else {
    DicomObjectException("DicomBeamBlock","BlockMountingPosition");
  }
  
  if( bblItem.getBlockDivergence(fstr) == EC_Normal ) {
    theParamStrs["BlockDivergence"] = fstr;
  } else {
    DicomObjectException("DicomBeamBlock","BlockDivergence");
  }
  if( bblItem.getBlockThickness(ffloat) == EC_Normal ) {
    theParams["BlockThickness"] = ffloat;
  } else {
    DicomObjectException("DicomBeamBlock","BlockThickness");
  }
}
 
//-----------------------------------------------------------------------------
void DicomBeamBlock::DumpToFile( std::ofstream& fout )
{
  int numBlock = GetParam("BlockNumber");
  std::string name  = "BLOCK_" + GmGenUtils::itoa(numBlock) + "_";
  DicomVObjectWithParams::Print( fout, name );
  int nData = GetParam("BlockNumberOfPoints");
  for( int ii = 0; ii < nData; ii++ ) {
    fout << ":P " << name << "DATA_" << GmGenUtils::itoa(ii) << " " << theBlockData[ii] << G4endl;    
  }
}
  
