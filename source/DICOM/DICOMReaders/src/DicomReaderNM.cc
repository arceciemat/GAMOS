#include "DicomReaderNM.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"
#include "DICOM/DICOMBase/include/DicomVImage.hh"
#include "DICOM/DICOMBase/include/DicomMgr.hh"
#include "DICOM/DICOMBase/include/DicomVOperator.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include <fstream>

//-----------------------------------------------------------------------------
DicomReaderNM::DicomReaderNM(DcmDataset* dset) : DicomVReaderImage(dset, DRM_NM)
{
  ReadHeaderAndPixels();
}

//-----------------------------------------------------------------------------
DicomReaderNM::DicomReaderNM(G4String fName) : DicomVReaderImage(DRM_NM)
{
  theFileName = fName;
  DcmFileFormat dfile;
  if( ! (dfile.loadFile(fName.c_str())).good() ) {
    G4Exception("DicomHandler::ReadFile",
                "",
                FatalErrorInArgument,
                ("Error reading file " + fName).c_str());
  }
  DcmDataset* dset = dfile.getDataset();
  theDataset = dset;

  ReadHeaderAndPixels();
}

//-----------------------------------------------------------------------------
DicomReaderNM::DicomReaderNM() : DicomVReaderImage(DRM_NM)
{
  InitialiseTo0();
}

//-----------------------------------------------------------------------------
void DicomReaderNM::DICOMCreateImages()
{
  theDicomImage = CreateImage("NM", DIM_NM, true, true );
  ResizeImage();
  // operate the others 
  DicomVOperator* operResize = DicomMgr::GetInstance()->FindOperator("DicomOperResize");
  if( operResize ) {
    operResize->SetAutomaticOperate(false); // do not operate resize again
  } 
 
}
