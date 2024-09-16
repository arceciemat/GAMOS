#include "DicomVReaderImage.hh"
#include "DicomReaderMgr.hh"
#include "DicomUIDInfo.hh"
#include "DICOM/DICOMBase/include/DicomMgr.hh"
#include "DICOM/DICOMBase/include/DicomOperResize.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"
#include "DICOM/DICOMBase/include/DicomParameterMgr.hh"

#include "G4GeometryTolerance.hh"

#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcpixel.h"
#include "dcmtk/dcmdata/dcpxitem.h"
#include "dcmtk/dcmdata/dcpixseq.h"
#include "dcmtk/dcmrt/drtimage.h"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmVerbosity.hh"
using namespace std;

#include <set>

//-----------------------------------------------------------------------------
DicomVReaderImage::DicomVReaderImage(DRModality mod) : DicomVReader(mod)
{
  theDicomMgr = DicomMgr::GetInstance();
  theReaderMgr = DicomReaderMgr::GetInstance();
  theReaderMgr->AddImageReader( this, mod );
  if( DicomVerb(debugVerb) ) {
    G4cout << " DicomVReaderImage AddImageReader " << this->GetName() << " " << DicomVReader::GetModalityStr(mod) << G4endl;
  }
  theModality = mod;
  theCompression = 1;
  theUIDInfo = 0;

}

//-----------------------------------------------------------------------------
DicomVReaderImage::DicomVReaderImage(DcmDataset* dset, DRModality mod) : DicomVReader(dset, mod)
{
  theDicomMgr = DicomMgr::GetInstance();
  theReaderMgr = DicomReaderMgr::GetInstance();
  theReaderMgr->AddImageReader( this, mod );
  if( DicomVerb(debugVerb) ) {
    G4cout << this << " DicomVReaderImage AddImageReader " << this->GetName() << " " << DicomVReader::GetModalityStr(mod) << G4endl;
  }
  theDataset = dset;
  theModality = mod;
  theCompression = 1;
  theUIDInfo = 0;

}

//-----------------------------------------------------------------------------
DicomVReaderImage::~DicomVReaderImage()
{
  theReaderMgr->DeleteImageReader(this);
}

//-----------------------------------------------------------------------------
void DicomVReaderImage::InitialiseTo0()
{
  theNoVoxelsX = 0;
  theMinX = 0.;
  theMaxX = 0.;
  theVoxelDimX = 0.;
  theNoVoxelsY = 0;
  theMinY = 0.;
  theMaxY = 0.;
  theVoxelDimY = 0.;
  theNoVoxelsZ = 0;
  theMinZ = 0.;
  theMaxZ = 0.;
  theVoxelDimZ = 0.;
  theOrientationRows = G4ThreeVector(1.,0.,0.);
  theOrientationColumns = G4ThreeVector(0.,1.,0.);
}

//-----------------------------------------------------------------------------
void DicomVReaderImage::ReadHeaderAndPixels(G4bool bReadPixelData)
{
  thePatientPosition = G4String(Read1DataStr(theDataset, DCM_PatientPosition).c_str());
  if( DicomVerb(infoVerb) ) G4cout << " DicomVReaderImage::ReadHeaderAndPixels thePatientPosition " <<  thePatientPosition << G4endl; 
  //  if( thePatientPosition != "" ) theCodeMeaning = G4String(Read1DataStr(theDataset, DCM_CodeMeaning).c_str());

  std::vector<double> dImagePositionPatient = Read1Data(theDataset, DCM_ImagePositionPatient,3); // center of first voxel read
  std::vector<double> dImageOrientationPatient = Read1Data(theDataset, DCM_ImageOrientationPatient,6);
  Float64 data;
  if( dImagePositionPatient.size() == 0 ) { 
    DcmStack resultStack;
    DcmObject *dObj;
    if (theDataset->search(DCM_DetectorInformationSequence, resultStack, ESM_fromHere, OFFalse).good()) {
      dObj = resultStack.top();
      //    dObj->print(G4cout);
      DcmSequenceOfItems* dObjSeq = dynamic_cast<DcmSequenceOfItems*>(dObj);
      int ii = 0;
      DcmItem *item;
      //--- Loop to item's in this sequence (only one should be found) 
      for( ;; ) {
	/*	if( ii >= 1 ) {
	  G4Exception("DicomReaderNM::ReadHeaderAndPixels",
		      "",
		      JustWarning,
		      ("Only one item in DCM_DetectorInformationSequence, finding item number " + GmGenUtils::itoa(ii)).c_str());
		      }*/
	item = dObjSeq->getItem(ii++);
	if( !item ) break;
	//      item->print(G4cout);
	unsigned long pos; 
	for( pos = 0; pos < 3; pos++ ) {
	  if( item->findAndGetFloat64(DCM_ImagePositionPatient,data,pos).good() ) {
	    dImagePositionPatient.push_back(data);
	    //	    G4cout << " GET findAndGetFloat64Array Position " << pos << " = " << data << G4endl; //GDEB
	  }
	}
	for( pos = 0; pos < 6; pos++ ) {
	  if( item->findAndGetFloat64(DCM_ImageOrientationPatient,data,pos).good() ) {
	    dImageOrientationPatient.push_back(data);
	    //	    G4cout << " GET findAndGetFloat64Array Orientation " << pos << " = " << data << G4endl; //GDEB
	  }
	}
      }
    }
  }

  if( dImagePositionPatient.size() == 0 ) {
    dImagePositionPatient = Read1Data(theDataset, DCM_RTImagePosition,2); // center of first voxel read
    theLocation = 0.;
  }

  if( dImagePositionPatient.size() == 2 ) { //RTImage
    theOrientationRows = G4ThreeVector(1,0,0);
    theOrientationColumns = G4ThreeVector(0,1,0);
  } else {
    if( dImageOrientationPatient.size() == 0 ) {
      G4Exception("DicomVReaderImage::ReadHeaderAndPixels",
		  "",
		  JustWarning,
		  "No ImageOrientationPatient TAG in DICOM file");
      theOrientationRows = G4ThreeVector(1,0,0);
      theOrientationColumns = G4ThreeVector(0,1,0);
    } else {
      theOrientationRows = G4ThreeVector(dImageOrientationPatient[0],dImageOrientationPatient[1],dImageOrientationPatient[2]);
      theOrientationColumns = G4ThreeVector(dImageOrientationPatient[3],dImageOrientationPatient[4],dImageOrientationPatient[5]);
    }    
  }

  std::vector< std::vector<G4double> > dImagePositionPatientMulti;
  if( dImagePositionPatient.size() == 0 ) {
    // Check if it has many slices (UID_EnhancedCTImageStorage)
    // Find PerFrameFunctionalGroupsSequence
    DcmSequenceOfItems *perFrameSeq = NULL;
    OFCondition status = theDataset->findAndGetSequence(DCM_PerFrameFunctionalGroupsSequence, perFrameSeq);
    if (status.good() && perFrameSeq) {
      // Loop through PerFrameFunctionalGroupsSequence items
      unsigned long numPerFrameItems = perFrameSeq->card();
      //      G4cout << " PERFRAME numPerFrameItems " <<numPerFrameItems << G4endl; //GDEB
      for (unsigned long ii1 = 0; ii1 < numPerFrameItems; ++ii1) {
        DcmItem *perFrameItem = perFrameSeq->getItem(ii1);
	//G4cout << ii1 << "  perFrameItem " << perFrameItem << G4endl; //GDEB
        if (perFrameItem) {
          // Find PlanePositionSequence within the PerFrame item
          DcmSequenceOfItems *planePositionSeq = NULL;
          status = perFrameItem->findAndGetSequence(DCM_PlanePositionSequence, planePositionSeq);
	  //G4cout << perFrameItem << " POSITI " << status.good() << " " << planePositionSeq << G4endl; //GDE
	  if (status.good() && planePositionSeq) {
	    // Loop through items in the sequence
	    unsigned long numItems = planePositionSeq->card();
  	    for (unsigned long ii2 = 0; ii2 < numItems; ii2++) {
	      DcmItem* item = planePositionSeq->getItem(ii2);
	      if (item != NULL) {
		unsigned long pos;
		dImagePositionPatient.clear();
		for( pos = 0; pos < 3; pos++ ) {
		  if( item->findAndGetFloat64(DCM_ImagePositionPatient,data,pos).good() ) {
		    dImagePositionPatient.push_back(data);
		  }
		}
		dImagePositionPatientMulti.push_back( dImagePositionPatient );
		//G4cout << dImagePositionPatientMulti.size() << " ADDING dImagePositionPatient to MULTI " << dImagePositionPatient[0]<<","<<dImagePositionPatient[1]<<"," <<dImagePositionPatient[2]<< G4endl; //GDEB
		
	      }
	    }
	    if( dImagePositionPatientMulti.size() == 0 ) {
	      G4Exception("DicomVImageReader::ReadData",
			  "",
			  FatalException,
			  "No ImagePositionPatient");
	    }
	  }
	  //	  G4cout << " NOT ADDING " << theDataset->findAndGetSequence(DCM_PlanePositionSequence, planePositionSequence).good() << " " << planePositionSequence << G4endl; //GDEB
	}
      }
    }
  }
  
  if( thePatientPosition == "HFS" || thePatientPosition == "FFS" ) {
    if( theOrientationRows != G4ThreeVector(1,0,0)
	|| theOrientationColumns != G4ThreeVector(0,1,0) ) {
      G4cerr << " OrientationRows " << theOrientationRows << " OrientationColumns " << theOrientationColumns << G4endl;
      G4Exception("DicomVReaderImage::ReadHeaderAndPixels",
		  "DFCT0002",
		  JustWarning,
		  "OrientationRows must be (1,0,0) and OrientationColumns (0,1,0), please contact GAMOS authors");
    }
  } else if( thePatientPosition == "HFP" || thePatientPosition == "FFP" ) {
    if( theOrientationRows != G4ThreeVector(-1,0,0)
	|| theOrientationColumns != G4ThreeVector(0,-1,0) ) {
      G4cerr << " OrientationRows " << theOrientationRows << " OrientationColumns " << theOrientationColumns << G4endl;
      G4Exception("DicomVReaderImage::ReadHeaderAndPixels",
		  "DFCT0002",
		  JustWarning,
		  "OrientationRows must be (-1,0,0) and OrientationColumns (0,-1,0), please contact GAMOS authors");
      theOrientationRows = G4ThreeVector(-1,0,0);
      theOrientationColumns = G4ThreeVector(0,-1,0);
    }
  }

  OFString dModality = Read1DataStr(theDataset, DCM_Modality);

  std::vector<double> gridFrameOffsets;
  Uint16 numberOfFrames = 1;
  // OFCondition status = theDataset->findAndGetUint16(DCM_NumberOfFrames, numberOfFrames);
  std::vector<double> numberOfFramesV = Read1Data(theDataset, DCM_NumberOfFrames,1);
  //G4cout << " DCM_NUMBEROFFRAMES " << numberOfFrames << " " << numberOfFramesV.size() << G4endl; //GDEB
  if( numberOfFramesV.size() == 0 ) {
    numberOfFramesV = Read1Data(theDataset, DCM_NumberOfSlices,1);
    //    G4cout << " DCM_NUMBEROFSLICES NV " << numberOfFramesV.size() << G4endl; //GDEB
  }
  if( numberOfFramesV.size() != 0 ) numberOfFrames = numberOfFramesV[0];

  if( dImagePositionPatientMulti.size() != 0 && numberOfFrames == 1 )  {
    DcmSequenceOfItems *perFrameSeq = NULL;
    OFCondition status = theDataset->findAndGetSequence(DCM_PerFrameFunctionalGroupsSequence, perFrameSeq);
    unsigned long numPerFrameItems = perFrameSeq->card();
    numberOfFrames = numPerFrameItems;
    G4Exception("DicomVImageReader::ReadData",
		"",
		JustWarning,
		"No DCM_NumberOfFrames tag found, setting it to number of PerFrameFunctionalGroupsSequence");
  }
	
  //  G4cout << " NUMBEROFFRAMES " << numberOfFrames << " " << numberOfFramesV.size() << G4endl; //GDEB
  /*
  Sint16 datai;
  theDataset->findAndGetSint16(DCM_NumberOfFrames,datai,0).good();
  Uint16 dataiu;
  theDataset->findAndGetUint16(DCM_NumberOfFrames,dataiu,0).good();
    G4cout << " NUMBEROFFRAMES " << numberOfFrames << " = " << datai << " = " << dataiu << " " << numberOfFramesV.size() << G4endl; //GDEB
  */

  std::vector<double> dSliceThickness = Read1Data(theDataset, DCM_SliceThickness, 1);
  std::vector<double> dPixelSpacing = Read1Data(theDataset, DCM_PixelSpacing, 2);
  if( DicomVerb(debugVerb) ) G4cout << " dPixelSpacing " << dPixelSpacing[0] << " " << dPixelSpacing[1] << G4endl;
  
  if( dSliceThickness.size() == 0 ) {    
    // set dSliceThickness from GridFrameOffsetVector (for RTDose)
    if( gridFrameOffsets.size() < 2 ) {
       G4Exception("DicomVReaderImage::ReadHeaderAndPixels",
		   "",
		   JustWarning, //FatalException,
		   "Error reading slice number, and GridFrameOffsetVector has also no information. Forcing thickness to be 1 (temporarily, as this info is not saved in testCT.g4dcm)");
       //       "Error reading slice number, and GridFrameOffsetVector has also no information, file cannot be treated");
    }
    if( numberOfFrames > 1 ) {
      if( dImagePositionPatientMulti.size() == 0 ) {
	dSliceThickness.push_back(gridFrameOffsets[1] - gridFrameOffsets[0]);
	for( size_t ii = 2; ii < gridFrameOffsets.size(); ii++ ){
	  if( dSliceThickness[0] != gridFrameOffsets[ii]- gridFrameOffsets[ii-1] ) {
	    G4Exception("DicomVReaderImage::ReadHeaderAndPixels",
			"",
			FatalException,
			("Error reading slice number " + GmGenUtils::itoa(ii)
			 + " Files with different slice thickness cannot be used in the same job : "
			 + GmGenUtils::ftoa( dSliceThickness[0] ) + " != "
			 + GmGenUtils::ftoa( gridFrameOffsets[ii]- gridFrameOffsets[ii-1])).c_str());
	  }
	}
      } else {
	// Navigate to SharedFunctionalGroupsSequence
	DcmStack resultStack;
	if (theDataset->search(DCM_SharedFunctionalGroupsSequence, resultStack, ESM_fromHere, true) != EC_Normal || resultStack.empty())  {
	  G4Exception("ReadPixelData",
		    "",
		      FatalException,
		      "Error finding SharedFunctionalGroupsSequence" );
	}
	DcmSequenceOfItems* sharedFGSequenceItem = dynamic_cast<DcmSequenceOfItems*>(resultStack.top());
	//G4cout << sharedFGSequenceItem << " STACK TOP " << resultStack.top() << G4endl; //GDEB
	if (!sharedFGSequenceItem) {
	  G4Exception("ReadPixelData",
		    "",
		      FatalException,
		      "Error accessing SharedFunctionalGroupsSequence item");
	}
	// Navigate to PixelMeasuresSequence within SharedFunctionalGroupsSequence
	if (sharedFGSequenceItem->search(DCM_PixelMeasuresSequence, resultStack, ESM_fromHere, true) != EC_Normal || resultStack.empty()) {
	  G4Exception("ReadPixelData",
		    "",
		      FatalException,
		      "Error finding PixelMeasuresSequence within SharedFunctionalGroupsSequence");
	}
	//	G4cout << " PMI stack " << resultStack.top() << G4endl; //GDEB
	DcmSequenceOfItems* seqpixelMeasuresItem = dynamic_cast<DcmSequenceOfItems*>(resultStack.top());
	G4cout << "seqpixelMeasuresItem " << seqpixelMeasuresItem << G4endl; //
	if (!seqpixelMeasuresItem) {
	  G4Exception("ReadPixelData",
		      "",
		      FatalException,
		      "Error accessing PixelMeasuresSequence item");
	}
        DcmItem *pixelMeasuresItem = seqpixelMeasuresItem->getItem(0); // it should only have one
	G4cout << "pixelMeasuresItem " << pixelMeasuresItem << G4endl; //
	// Access SliceThickness within PixelMeasuresSequence
	Float64 sliceT = 0.0;
	if( pixelMeasuresItem->findAndGetFloat64(DCM_SliceThickness, sliceT, 0).good() )  {
	  dSliceThickness.push_back(sliceT);
	  G4cout << " dSliceThickness from PixelMeasuresSequence " << dSliceThickness[0] << G4endl; 
	} else  {	
	  G4Exception("ReadPixelData",
		    "",
		      FatalException,
		      "Error retrieving Pixel Spacing");
	}
	if( dPixelSpacing.size() == 0 ) {
	  // Access PixelSpacing within PixelMeasuresSequence
	  Float64 pixelSpacingRow = 0.0;
	  Float64 pixelSpacingCol = 0.0;
	  if (pixelMeasuresItem->findAndGetFloat64(DCM_PixelSpacing, pixelSpacingRow, 0).good() &&
	      pixelMeasuresItem->findAndGetFloat64(DCM_PixelSpacing, pixelSpacingCol, 1).good()) {
	    std::cout << "Pixel Spacing (Row): " << pixelSpacingRow << std::endl;
	    std::cout << "Pixel Spacing (Column): " << pixelSpacingCol << std::endl;
	    dPixelSpacing.push_back(pixelSpacingRow);
	    dPixelSpacing.push_back(pixelSpacingCol);
	  } else  {
	    G4Exception("ReadPixelData",
			"",
			JustWarning,
			"Error retrieving Pixel Spacing");
	  }
	}
	
      }
    
    } 

  }
  
  bZDirectionInvert = false;
  if( theModality == DRM_NM ) {
    if( theDataset->findAndGetFloat64(DCM_SpacingBetweenSlices,data,0).good() ) {
      if( fabs(data+dSliceThickness[0]) < 1.e-4 ) { // DicomVImage::DP_PRECISION
	G4Exception("ReadPixelData",
		    "",
		    JustWarning,
		    ("SpacingBetweenSlices changes the sign of SliceThickness: " + GmGenUtils::ftoa(data)).c_str());
	bZDirectionInvert = true;
      } else if( fabs(data-dSliceThickness[0]) > 1.e-4 ) { // DicomVImage::DP_PRECISION
	G4Exception("ReadPixelData",
		    "",
		    FatalException,
		    ("SpacingBetweenSlices has a different value than SliceThickness: " + GmGenUtils::ftoa(data) + " != " +  GmGenUtils::ftoa(dSliceThickness[0])).c_str());
      }
    }
  }
  
  std::vector<double> dRows = Read1Data(theDataset, DCM_Rows, 1);
  std::vector<double> dColumns = Read1Data(theDataset, DCM_Columns, 1);
  theNoVoxelsY = dRows[0];
  theNoVoxelsX = dColumns[0];
  theNoVoxelsZ = numberOfFrames;

  // Image Position (0020,0032) specifies the x, y, and z coordinates of the upper left hand corner of the image; it is the center of the first voxel transmitted
  // it is the center of the first voxel transmitted.
  G4bool bPosVoxelCentre = theDicomMgr->GetImagePositionPatientIsPosVoxelCentre();

  if( bPosVoxelCentre ) {
    theMinX = dImagePositionPatient[0]-dPixelSpacing[0]*0.5; // center of pixel
    theMaxX = dImagePositionPatient[0]+(dColumns[0]-0.5)*dPixelSpacing[0];
    
    theMinY = dImagePositionPatient[1]-dPixelSpacing[1]*0.5;
    theMaxY = dImagePositionPatient[1]+(dRows[0]-0.5)*dPixelSpacing[1];

    if( dSliceThickness.size() == 0 ) {
      dSliceThickness.push_back(1.);
    }
    if( dImagePositionPatientMulti.size() == 0 ) {
      theMinZ = dImagePositionPatient[2]-dSliceThickness[0]*0.5;
      //G4cout << " theMinZ= " << theMinZ << " " << dImagePositionPatient[2] << " - " <<dSliceThickness[0] << "*0.5" <<  G4endl; //GDEB
      theMaxZ = dImagePositionPatient[2]+dSliceThickness[0]*(numberOfFrames-0.5);
      //G4cout << " theMaxZ= " << theMaxZ << " " << dImagePositionPatient[2] << " + " <<dSliceThickness[0] << "*" << numberOfFrames << "-0.5" <<  G4endl; //GDEB
    } else {
      // get minimum and maximum Z 
      theMinZ = DBL_MAX;
      theMaxZ = -DBL_MAX;
      //G4cout << " theMinZ " << theMinZ << " " << dImagePositionPatientMulti.size() << G4endl; //GDEB
      for( size_t ii = 0; ii < dImagePositionPatientMulti.size(); ii++ ) {
	theMinZ = std::min(theMinZ,dImagePositionPatientMulti[ii][2]);
	theMaxZ = std::max(theMaxZ,dImagePositionPatientMulti[ii][2]);
	// 	G4cout << ii << " theMinZ " << theMinZ << " " << dImagePositionPatientMulti[ii][0] << " " << dImagePositionPatientMulti[ii][1] << " " << dImagePositionPatientMulti[ii][2] << G4endl; //GDEB
      }
      theMinZ -= dSliceThickness[0]*0.5;
      theMaxZ -= dSliceThickness[0]*0.5;
    }
  } else {
  // image position are coordinates of the upper left hand corner of the image;
  // it is the corner of the first voxel transmitted. !!!
    theMinX = dImagePositionPatient[0]-dPixelSpacing[0]*0.; // corner of pixel
    theMaxX = dImagePositionPatient[0]+(dColumns[0]-0.)*dPixelSpacing[0];
    
    theMinY = dImagePositionPatient[1]-dPixelSpacing[1]*0.;
    theMaxY = dImagePositionPatient[1]+(dRows[0]-0.)*dPixelSpacing[1];

    if( dImagePositionPatientMulti.size() == 0 ) {
      theMinZ = dImagePositionPatient[2]-dSliceThickness[0]*0.;
      //    G4cout << " theMinZ= " << theMinZ << " " << dImagePositionPatient[2] << G4endl; //GDEB
      theMaxZ = dImagePositionPatient[2]+dSliceThickness[0]*(numberOfFrames);
    } else {
      // get minimum and maximum Z 
      theMinZ = DBL_MAX;
      theMaxZ = -DBL_MAX;
      for( size_t ii = 0; ii << dImagePositionPatientMulti.size(); ii++ ) {
	theMinZ = std::min(theMinZ,dImagePositionPatientMulti[ii][2]);
	theMaxZ = std::max(theMaxZ,dImagePositionPatientMulti[ii][2]);
      }
    }
  }
    
  //G4cout << bPosVoxelCentre << " MINX " << theMinX << " " << theMaxX << G4endl; //GDEB
  //  G4cout << " MINY " << theMinY << " " << theMaxY << G4endl; //GDEB
  //  G4cout << " MINZ " << theMinZ << " " << theMaxZ << G4endl; //GDEB
  
  theVoxelDimX = dPixelSpacing[0];
  theVoxelDimY = dPixelSpacing[1];
  theVoxelDimZ = dSliceThickness[0];
  
  if( DicomVerb(debugVerb) ) G4cout << " DicomVReaderImage::ReadHeaderAndPixels:  theNoVoxels " << theNoVoxelsX << " " << theNoVoxelsY << " " << theNoVoxelsZ << G4endl;
  if( DicomVerb(debugVerb) ) G4cout << " DicomVReaderImage::ReadHeaderAndPixels:  theMin " << theMinX << " " << theMinY << " " << theMinZ << G4endl;
  if( DicomVerb(debugVerb) ) G4cout << " DicomVReaderImage::ReadHeaderAndPixels:  theMax " << theMaxX << " " << theMaxY << " " << theMaxZ << G4endl;
  if( DicomVerb(debugVerb) ) G4cout << " DicomVReaderImage::ReadHeaderAndPixels:  theVoxelDim " << theVoxelDimX << " " << theVoxelDimY << " " << theVoxelDimZ << G4endl;
  if( DicomVerb(debugVerb) ) G4cout << this <<  " DicomVReaderImage::ReadHeaderAndPixels:  MinMaxZ " << theMinZ << " " << theMaxZ << G4endl;
  //  G4cout << " SmallestImagePixelValue = " << Read1Data(theDataset, DCM_SmallestImagePixelValue, 1)[0];
  //    G4cout << " LargestImagePixelValue = " << Read1Data(theDataset, DCM_LargestImagePixelValue, 1)[0];
  
  //  std::vector<double> d = Read1Data(theDataset, DCM_, 1);
  
  theBitAllocated = Read1Data(theDataset, DCM_BitsAllocated, 1)[0];
  if( DicomVerb(-infoVerb) ) G4cout << " BIT ALLOCATED " << theBitAllocated << G4endl; 

  std::vector<double> vDoseGridScaling = Read1Data(theDataset, DCM_DoseGridScaling, 1);
  if( vDoseGridScaling.size() == 1 ) {
    theDoseGridScaling = vDoseGridScaling[0];
  } else {
    theDoseGridScaling = 1.;
  }

  std::vector<double> vRescaleSlope = Read1Data(theDataset, DCM_RescaleSlope, 1);
  if( vRescaleSlope.size() == 1 ) {
    theRescaleSlope = vRescaleSlope[0];
  } else {
    theRescaleSlope = 1;
  }
  std::vector<double> vRescaleIntercept = Read1Data(theDataset, DCM_RescaleIntercept, 1);
  if( vRescaleIntercept.size() == 1 ) {
    theRescaleIntercept = vRescaleIntercept[0];
  } else {
    theRescaleIntercept = 0;
  }

  // UID's FOR sqdoseToRTDose
  theUIDInfo = new DicomUIDInfo(theDataset);
  
  if( bReadPixelData ) ReadPixelData();

}


//-----------------------------------------------------------------------------
void DicomVReaderImage::ReadPixelData()
{
  G4bool bPixelRepresentationSigned = false;
  std::vector<double> pixelRep = Read1Data(theDataset, DCM_PixelRepresentation, 1);
  // G4cout << " pixelRep. " << pixelRep.size() << G4endl; //GDEB
  if( pixelRep.size() == 1 ) bPixelRepresentationSigned = G4bool(pixelRep[0]);
  if( DicomVerb(debugVerb) ) G4cout << " pixelRep. " << pixelRep[0] << " = " << bPixelRepresentationSigned << G4endl; 
  
  theVoxelData = new std::vector<G4double>;
  //  READING THE PIXELS :
  OFCondition result = EC_Normal;
  //---- CHECK IF DATA IS COMPRESSED
  DcmElement* element = NULL;
  DcmPixelData *dpix = NULL;
  result = theDataset->findAndGetElement(DCM_PixelData, element);
  if (result.bad() || element == NULL) {
    G4Exception("ReadPixelData",
		"findAndGetElement(DCM_PixelData, ",
		FatalException,
		("Element PixelData not found: " + G4String(result.text())).c_str());
  }
  dpix = OFstatic_cast(DcmPixelData*, element);
  // If we have compressed data, we must utilize DcmPixelSequence
  //   in order to access it in raw format, e. g. for decompressing it
  //   with an external library.
  DcmPixelSequence *dseq = NULL;
  E_TransferSyntax xferSyntax = EXS_Unknown;
  const DcmRepresentationParameter *rep = NULL;
  // Find the key that is needed to access the right representation of the data within DCMTK
  dpix->getOriginalRepresentationKey(xferSyntax, rep);
  // Access original data representation and get result within pixel sequence
  result = dpix->getEncapsulatedRepresentation(xferSyntax, rep, dseq);
  //  std::cout << dseq << " result getEncapsulatedRepresentation " << std::endl; //GDEB
  size_t theNoVoxelsXY = theNoVoxelsX*theNoVoxelsY;
  size_t theNoVoxels = theNoVoxelsXY*theNoVoxelsZ;
  //  std::cout << " theNoVoxelsZ; " << theNoVoxelsZ << theNoVoxelsXY << "*"<< theNoVoxelsZ << G4endl; //GDEB
  if ( result == EC_Normal ) // COMPRESSED DATA
    {
    G4Exception("DicomVReaderImage::ReadPixelData()",
      "DFCT004",
      FatalException,
      "Compressed pixel data is not supported");
    /*  dcpixseq:
        if (!itemList->empty())
        {
	DcmObject *dO;
	itemList->seek(ELP_first);
	do {
	dO = itemList->get();
	dO->print(out, flags, level + 1, pixelFileName, pixelCounter);
	} while (itemList->seek(ELP_next));
        }
	
	dcpixel:
	typedef OFListIterator(DcmRepresentationEntry *) DcmRepresentationListIterator;
	
	DcmRepresentationListIterator current;
	(*current)->pixSeq->p
    */
    
    if( DicomVerb(infoVerb) ) G4cout << " DicomVReaderImage::ReadPixelData:  result == EC_Normal Reading compressed data " << std::endl;
    DcmPixelItem* pixitem = NULL;
    // Access first frame (skipping offset table)
    for( int ii = 1; ii < 2; ii++ ) {
      OFCondition cond =  dseq->getItem(pixitem, ii);
      if( !cond.good()) break;
      if( DicomVerb(debugVerb) ) G4cout << ii << " PIX LENGTH " << pixitem->getLength() << G4endl;
    }
    if (pixitem == NULL) {
     	G4Exception("ReadPixelData",
		    "dseq->getItem()",
		    FatalException,
		    "No DcmPixelItem in DcmPixelSequence");
    }
    Uint8* pixData = NULL;
    // Get the length of this pixel item (i.e. fragment, i.e. most of the time, the lenght of the frame)
    Uint32 pixLength = pixitem->getLength();
    if (pixLength == 0) {
 	G4Exception("ReadPixelData",
		    "pixitem->getLength()",
		    FatalException,
		    "PixelData empty");
    }

    if( DicomVerb(debugVerb) ) G4cout << " DicomVReaderImage::ReadPixelData:  number of pixels " << pixLength << G4endl;
    // Finally, get the compressed data for this pixel item
    result = pixitem->getUint8Array(pixData);
    /*    //       for( int ic = 0; ic < theNoVoxelsX; ic++ ) {
    const unsigned long count = pixitem->getLengthField();
    for (unsigned long i = 0; i < count; i++) {
      G4cout << i << " " << std::setw(2) << " pix= " << OFstatic_cast(int, *(pixData++)) << G4endl;
      //??      G4cout << i << " " << std::setw(2) << " pix= " <<  OFstatic_cast(Uint8, *(pixData++)) << G4endl;
    }
	 //	 G4cout << ic+1*theNoVoxelsX << " pixdata= " << pixData[ic+1*theNoVoxelsX] << G4endl; //GDEB
	 // }
       exit(1);
  */
	 
    /*      //    pixitem->print(G4cout);
    // Do something useful with pixData...
    for( int ir = 0; ir < theNoVoxelsY; ir++ ) {
      for( int ic = 0; ic < theNoVoxelsX; ic++ ) {
	//	  G4cout << ic+ir*theNoVoxelsX << " pixdata " << ir << " " << ic << " = " << pixData[ic+ir*theNoVoxelsX] << G4endl; //GDEB
	fVoxelData->push_back(pixData[ic+ir*theNoVoxelsX]*theRescaleSlope + theRescaleIntercept);	
      }
      } */
  } else { // UNCOMPRESSED DATA
    // Access first frame (skipping offset table)
    Uint32 pixLength = dpix->getLength();
    //    G4cout << "N VOX " << theNoVoxels << " !=? " << pixLength << G4endl; //GDEB
    if(theBitAllocated == 8) { // Case 8 bits :
      theNoVoxelsZ = pixLength/theNoVoxelsXY;
      theNoVoxels = theNoVoxelsXY*theNoVoxelsZ;
      theVoxelData = new std::vector<double>(theNoVoxelsX*theNoVoxelsY*theNoVoxelsZ);
      if( theNoVoxels != pixLength ) {
	G4Exception("ReadPixelData",
		    "getUint16Array pixData, ",
		    JustWarning,
		    ("PixelData length="+GmGenUtils::itoa(pixLength)+" is not equal to NoVoxels= "+GmGenUtils::itoa(theNoVoxels)).c_str());
	std::cerr << " NoVoxelZ changed to " << theNoVoxelsZ << std::endl;
      }
      
      Uint8* pixData = NULL;
      if(! (element->getUint8Array(pixData)).good() ) {
	G4Exception("ReadPixelData",
		    "getUint8Array pixData, ",
		    FatalException,
		    ("Uint8 PixelData not found: " + G4String(result.text())).c_str());
      }
      double val;
      for( size_t iz = 0; iz < theNoVoxelsZ; iz++ ) {
        for( size_t ir = 0; ir < theNoVoxelsY; ir++ ) {
   	  for( size_t ic = 0; ic < theNoVoxelsX; ic++ ) {
	    Uint8 val1U = pixData[ic+ir*theNoVoxelsX+iz*theNoVoxelsXY];
	    if( bPixelRepresentationSigned ) {
	      if( val1U < 128 ) {
		val = val1U;
	      } else {
		val = val1U - 256;
		// val = 256-val1U;
	      }
	    } else {
	      val = val1U;
	    }
	    val = val*theRescaleSlope*theDoseGridScaling + theRescaleIntercept;
	    //	    theVoxelData->push_back(val);
	    size_t newCopyNo;
	    if( !bZDirectionInvert ) {
	      newCopyNo = ic+ir*theNoVoxelsX+iz*theNoVoxelsXY;
	    } else {
	      newCopyNo = ic+ir*theNoVoxelsX+(theNoVoxelsZ-1-iz)*theNoVoxelsXY;
	    }
	    theVoxelData->at(newCopyNo) = val;
	    if( DicomVerb(testVerb) ) {
	      if( val1U != 0 )  G4cout << GetName() << " DicomVReaderImage::Pixel " << ic << " : " << ir << " : " << iz << " copyNo " << newCopyNo << " = " << val << " = " << pixData[ic+ir*theNoVoxelsX+iz*theNoVoxelsXY] << "="  << " * " <<theRescaleSlope << " * " << theDoseGridScaling << " + " << theRescaleIntercept << G4endl;           
	    }	  
          }
        }
      }
    } else if(theBitAllocated == 16) { // Case 16 bits :
      theNoVoxelsZ = pixLength/theNoVoxelsXY/2;
      theNoVoxels = theNoVoxelsXY*theNoVoxelsZ;
      theVoxelData = new std::vector<double>(theNoVoxelsX*theNoVoxelsY*theNoVoxelsZ);
      if( theNoVoxels != pixLength/2 ) {
	G4Exception("ReadPixelData",
		    "getUint16Array pixData, ",
		    JustWarning,
		    ("PixelData length="+GmGenUtils::itoa(pixLength)+" is not equal to NoVoxels*2= "+GmGenUtils::itoa(theNoVoxels)).c_str());
	std::cerr << " NoVoxelZ changed to " << theNoVoxelsZ << " " << theNoVoxelsX << " " << theNoVoxelsY << std::endl;
      }
      
      Uint16* pixData = NULL;
      result = dpix->getUint16Array(pixData);
      //      Sint16* pixData = NULL;
      //      result = dpix->getSint16Array(pixData);
      if( result != EC_Normal ) {
         //      if(! (element->getUint16Array(pixData)).good() ) {      
        G4Exception("ReadPixelData",
		    "getUint16Array pixData, ",
		    FatalException,
		    ("Uint16 PixelData not found: " + G4String(result.text())).c_str());
      }
      double val;
      for( size_t iz = 0; iz < theNoVoxelsZ; iz++ ) {
        for( size_t ir = 0; ir < theNoVoxelsY; ir++ ) {
   	  for( size_t ic = 0; ic < theNoVoxelsX; ic++ ) {
	    Uint16 val1U = pixData[ic+ir*theNoVoxelsX+iz*theNoVoxelsXY];
	    if( bPixelRepresentationSigned ) {
	      if( val1U < 32768 ) {
		val = val1U;
	      } else {
		//		val = 65535 - val1U;
		val = val1U - 65535;
	      }
	    } else {
	      val = val1U;
	    }
	    // G4cout << ic+ir*theNoVoxelsX+iz*theNoVoxelsXY << " pixdata " << iz<<":"<<ir<<":"<<ic << " = " << pixData[ic+ir*theNoVoxelsX] << " ==> " << val << G4endl; //GDEB
	    val = val*theRescaleSlope*theDoseGridScaling + theRescaleIntercept;
	    //G4cout << " FINAL val " << val << " " << theRescaleSlope << "*" << theDoseGridScaling <<"+" << theRescaleIntercept << G4endl; //GDEB
	    size_t newCopyNo;
	    if( thePatientPosition == "HFS" || thePatientPosition == "" ) {
	      newCopyNo = ic+ir*theNoVoxelsX+iz*theNoVoxelsXY;
	    } else if( thePatientPosition == "FFS" ) {
	      newCopyNo = (theNoVoxelsX-1-ic)+ir*theNoVoxelsX+(theNoVoxelsZ-1-iz)*theNoVoxelsXY;
	    } else if( thePatientPosition == "HFP" ) {
	      newCopyNo = (theNoVoxelsX-1-ic)+(theNoVoxelsY-1-ir)*theNoVoxelsX+iz*theNoVoxelsXY;
	    } else if( thePatientPosition == "FFP" ) {
	      newCopyNo = ic+(theNoVoxelsY-1-ir)*theNoVoxelsX+(theNoVoxelsZ-1-iz)*theNoVoxelsXY;
	    } else { 
	      G4Exception("DicomVReaderImage::ReadPixelData",
			  "",
			  FatalException,
			  ("Only DICOM PatientPosition's implemented HFS, FFS, HFP and FFP, while it is"+G4String(thePatientPosition)+"\n Please contact GAMOS authors").c_str());
	    }
	    theVoxelData->at(newCopyNo) = val;
	    if( DicomVerb(testVerb) ) {
	      if( val != 0 )  G4cout << GetName() << " DicomVReaderImage::Pixel " << ic << " : " << ir << " : " << iz << " copyNo " << newCopyNo << " = " << val << "  " << pixData[ic+ir*theNoVoxelsX+iz*theNoVoxelsXY] << " * " <<theRescaleSlope << " * " << theDoseGridScaling << " + " << theRescaleIntercept << G4endl; 
	    }	  
          }
	}
      }
    } else if(theBitAllocated == 32) { // Case 32 bits :
      theNoVoxelsZ = pixLength/theNoVoxelsXY/4;
      theNoVoxels = theNoVoxelsXY*theNoVoxelsZ;
      theVoxelData = new std::vector<double>(theNoVoxelsX*theNoVoxelsY*theNoVoxelsZ);
      if( theNoVoxels != pixLength/4 ) {
	G4Exception("ReadPixelData",
		    "getUint16Array pixData, ",
		    JustWarning,
		    ("PixelData length="+GmGenUtils::itoa(pixLength)+" is not equal to NoVoxels*4= "+GmGenUtils::itoa(theNoVoxels)).c_str());
	std::cerr << " NoVoxelZ changed to " << theNoVoxelsZ << std::endl;
      }
      
      Uint32* pixData = NULL;
      result = dpix->getUint32Array(pixData);
      if( result != EC_Normal ) {
         //      if(! (element->getUint32Array(pixData)).good() ) {      
        G4Exception("ReadPixelData",
 	"getUint32Array pixData, ",
	FatalException,
		    ("Uint32 PixelData not found: " + G4String(result.text())).c_str());
      }
      double val;
      for( size_t iz = 0; iz < theNoVoxelsZ; iz++ ) {
        for( size_t ir = 0; ir < theNoVoxelsY; ir++ ) {
   	  for( size_t ic = 0; ic < theNoVoxelsX; ic++ ) {
    //	    G4cout << ic+ir*theNoVoxelsX << " pixdata " << ir << " " << ic << " = " << pixData[ic+ir*theNoVoxelsX] << G4endl; //GDEB
	    Uint32 val1U = pixData[ic+ir*theNoVoxelsX+iz*theNoVoxelsXY];
	    if( bPixelRepresentationSigned ) {
	      if( val1U < 2147483648 ) {
		val = val1U;
	      } else {
		val = val1U - 4294967296;
		//		val = 4294967296-val1U;
	      }
	    } else {
	      val = val1U;
	    }
	    val = val*theRescaleSlope*theDoseGridScaling + theRescaleIntercept;
	    //	    theVoxelData->push_back(val);
	    size_t newCopyNo;
	    if( thePatientPosition == "HFS" || thePatientPosition == "" ) {
	      newCopyNo = ic+ir*theNoVoxelsX+iz*theNoVoxelsXY;
	    } else if( thePatientPosition == "FFS" ) {
	      newCopyNo = (theNoVoxelsX-1-ic)+ir*theNoVoxelsX+(theNoVoxelsZ-1-iz)*theNoVoxelsXY;
	    } else if( thePatientPosition == "HFP" ) {
	      newCopyNo = (theNoVoxelsX-1-ic)+(theNoVoxelsY-1-ir)*theNoVoxelsX+iz*theNoVoxelsXY;
	    } else if( thePatientPosition == "FFP" ) {
	      newCopyNo = ic+(theNoVoxelsY-1-ir)*theNoVoxelsX+(theNoVoxelsZ-1-iz)*theNoVoxelsXY;
	    } else { 
	      G4Exception("DicomVReaderImage::ReadPixelData",
			  "",
			  FatalException,
			  ("Only DICOM PatientPosition's implemented HFS, FFS, HFP and FFP, while it is"+G4String(thePatientPosition)+"\n Please contact GAMOS authors").c_str());
	    }
	    theVoxelData->at(newCopyNo) = val;
	    if( DicomVerb(testVerb) ) {
	      //	      if( val1U != 0 )  G4cout << GetName() << " DicomVReaderImage::Pixel " << ic << " : " << ir << " : " << iz << " copyNo " << newCopyNo << " = " << val << " = " << pixData[ic+ir*theNoVoxelsX+iz*theNoVoxelsXY] << "="  << " * " <<theRescaleSlope << " * " << theDoseGridScaling << " + " << theRescaleIntercept << G4endl; //GDEB           
	    }	  
	  }
	}
      }
    }
  }

  G4cout << GetName() << "  DicomVReaderImage::ReadPixelData READ  " << theVoxelData->size() << G4endl;

}

//-----------------------------------------------------------------------------
void DicomVReaderImage::operator+=( const DicomVReaderImage& rhs )
{
  *this = *this + rhs;

}

//-----------------------------------------------------------------------------
DicomVReaderImage DicomVReaderImage::operator+( const DicomVReaderImage& rhs )
{
  //----- If Reader is empty, fill it with rhs
  if( GetNoVoxels() == 0 ) {
    SetParamsX( rhs.GetNoVoxelsX(), rhs.GetMinX(), rhs.GetMaxX()); // set also theVoxelDimX
    SetParamsY( rhs.GetNoVoxelsY(), rhs.GetMinY(), rhs.GetMaxY()); // set also theVoxelDimY
    SetParamsZ( rhs.GetNoVoxelsZ(), rhs.GetMinZ(), rhs.GetMaxZ()); // set also theVoxelDimZ

    theOrientationRows = rhs.GetOrientationRows();
    theOrientationColumns = rhs.GetOrientationColumns();
    theVoxelData = new std::vector<G4double>;
    theCompression = rhs.GetCompression();
    
  } else {
    CheckIsEquivalent( &rhs, true );

    /*    //---- Check Z dimension does not collide with previous ones
    if( rhs.GetMaxZ() < theMaxZ && rhs.GetMaxZ() > theMinZ ) {
      G4double changeZ = rhs.GetMaxZ() - theMinZ;
      G4cerr << " DicomVReaderImage::operator+ TOO BIG MaxZ " << rhs.GetMaxZ() << " > " << theMinZ << G4endl
	     << " Previous minZ will be increased " << changeZ << G4endl
	     << " Previous maxZ will be decreased " << changeZ << G4endl;
      theMinZ = rhs.GetMaxZ();
      theMaxZ -= changeZ;
    }      
    if( rhs.GetMinZ() < theMaxZ && rhs.GetMinZ() > theMinZ ) {
      G4double changeZ = theMaxZ - rhs.GetMinZ();
      G4cerr << " TOO SMALL MinZ " << rhs.GetMinZ() << " < " << theMaxZ << G4endl
	     << " Previous minZ will be increased " << changeZ << G4endl
	     << " Previous maxZ will be decreased " << changeZ << G4endl;
      theMaxZ = rhs.GetMinZ();
      theMinZ += (changeZ);
    }
    */
    //----- Add data from second slice header
    if( DicomVerb(debugVerb) ) {
      G4cout << " SetMinZ " << theMinZ << " " << rhs.GetMinZ() << G4endl; 
      G4cout << " SetMaxZ " << theMaxZ << " " << rhs.GetMaxZ() << G4endl; 
    }
    SetMinZ( min( theMinZ, rhs.GetMinZ() ) );
    SetMaxZ( max( theMaxZ, rhs.GetMaxZ() ) );
    //    G4cout << " DicomVReaderImage operator+ nZprev " << GetNoVoxelsZ() << " " << theNoVoxelsZ << " + " << rhs.GetNoVoxelsZ() << G4endl; //GDEB
    SetNoVoxelsZ( theNoVoxelsZ + rhs.GetNoVoxelsZ() );
    //    G4cout << " DicomVReaderImage operator+ nZ " << GetNoVoxelsZ() << " " << theNoVoxelsZ << " + " << rhs.GetNoVoxelsZ() << G4endl; //GDEB
  }
  
  //----- Add data from second slice data
  std::vector<G4double>* vData2 = rhs.GetVoxelData();
  size_t nData2 = vData2->size();
  for( size_t ii = 0; ii < nData2; ii++ ) {
    theVoxelData->push_back(vData2->at(ii));
  }

  return *this;

}

//-----------------------------------------------------------------------------
void DicomVReaderImage::AddData( const DicomVReaderImage* rhs )
{
  std::vector<G4double>* vData2 = rhs->GetVoxelData();
  size_t nData2 = vData2->size();

  //----- If Reader is empty, fill it with rhs
  if( GetNoVoxels() == 0 ) {
    SetParamsX( rhs->GetNoVoxelsX(), rhs->GetMinX(), rhs->GetMaxX()); // set also theVoxelDimX
    SetParamsY( rhs->GetNoVoxelsY(), rhs->GetMinY(), rhs->GetMaxY()); // set also theVoxelDimY
    SetParamsZ( rhs->GetNoVoxelsZ(), rhs->GetMinZ(), rhs->GetMaxZ()); // set also theVoxelDimZ

    theOrientationRows = rhs->GetOrientationRows();
    theOrientationColumns = rhs->GetOrientationColumns();
    theVoxelData = new std::vector<G4double>;
    theCompression = rhs->GetCompression();

    //----- Add data from second slice data
    for( size_t ii = 0; ii < nData2; ii++ ) {
      theVoxelData->push_back(vData2->at(ii));
    }

  } else {
    CheckIsEquivalent( rhs, false );
 
    //----- Add data from second slice data
    for( size_t ii = 0; ii < nData2; ii++ ) {
      theVoxelData->at(ii)=theVoxelData->at(ii)+vData2->at(ii);
    }
  }

}

//-----------------------------------------------------------------------------
void DicomVReaderImage::CheckIsEquivalent(const DicomVReaderImage *rhs, G4bool bContiguous )
{
  //----- Check that both slices has the same dimensions
  if( theNoVoxelsX != rhs->GetNoVoxelsX() ) {
    G4cerr << "DicomVReaderImage error adding two slice headers:  !!! Different number of voxels X or: "
	   << "  X= " << theNoVoxelsX << " =? " << rhs->GetNoVoxelsX()
	   << G4endl;
    G4Exception("DicomVReaderImage::DicomVReaderImage",
		"",FatalErrorInArgument,"");
  }
  if( theNoVoxelsY != rhs->GetNoVoxelsY() ) {
    G4cerr << "DicomVReaderImage error adding two slice headers:  !!! Different number of voxels Y: "
	   << "  Y=  " << theNoVoxelsY << " =? " << rhs->GetNoVoxelsY()
	   << G4endl;
    G4Exception("DicomVReaderImage::DicomVReaderImage",
		"",FatalErrorInArgument,"");
  }
  if( !bContiguous ) {
    if( theNoVoxelsZ != rhs->GetNoVoxelsZ() ) {
      G4cerr << "DicomVReaderImage error adding two slice headers:  !!! Different number of voxels Z: "
	     << "  Z=  " << theNoVoxelsZ << " =? " << rhs->GetNoVoxelsZ()
	     << G4endl;
      G4Exception("DicomVReaderImage::DicomVReaderImage",
		  "",FatalErrorInArgument,"");
    }
  }

  //----- Check that both slices has the same extensions
  if( theMinX != rhs->GetMinX() || theMaxX != rhs->GetMaxX() ) {
    G4cerr << "DicomVReaderImage error adding two slice headers:\
        !!! Different extensions in X: "
	   << "  Xmin= " << theMinX << " =? " << rhs->GetMinX()
	   << "  Xmax= " << theMaxX << " =? " << rhs->GetMaxX()
	   << G4endl;
    G4Exception("DicomVReaderImage::operator+","",
		FatalErrorInArgument,"");
  }
  if( theMinY != rhs->GetMinY() || theMaxY != rhs->GetMaxY() ) {
    G4cerr << "DicomVReaderImage error adding two slice headers:\
        !!! Different extensions in Y: "
	   << "  Ymin= " << theMinY << " =? " << rhs->GetMinY()
	   << "  Ymax= " << theMaxY << " =? " << rhs->GetMaxY()
	   << G4endl;
    G4Exception("DicomVReaderImage::operator+","",
		FatalErrorInArgument,"");
  }
  if( !bContiguous ) {
    if( theMinZ != rhs->GetMinZ() || theMaxZ != rhs->GetMaxZ() ) {
      G4cerr << "DicomVReaderImage error adding two slice headers:\
        !!! Different extensions in Z: "
	     << "  Zmin= " << theMinZ << " =? " << rhs->GetMinZ()
	     << "  Zmax= " << theMaxZ << " =? " << rhs->GetMaxZ()
	     << G4endl;
      G4Exception("DicomVReaderImage::operator+","",
		  FatalErrorInArgument,"");
    }
  }
  
  //----- Check that both slices has the same orientations
  if( theOrientationRows != rhs->GetOrientationRows() ||
      theOrientationColumns != rhs->GetOrientationColumns() ) {
    G4cerr << "DicomVReaderImage error adding two slice headers: !!!\
        Slices have different orientations "
	   << "  Orientation Rows = " << theOrientationRows << " & " << rhs->GetOrientationRows() 
	   << "  Orientation Columns " << theOrientationColumns << " & " << rhs->GetOrientationColumns()
	   << G4endl;
    G4Exception("DicomVReaderImage::operator+","",
		FatalErrorInArgument,"");
  }
  
  //----- Check that the slices are contiguous in Z
  if( bContiguous ) {
    if( std::fabs( theMinZ - rhs->GetMaxZ() ) > 1.e-5 &&
	std::fabs( theMaxZ - rhs->GetMinZ() ) > 1.e-5 ){
      G4cerr << "DicomVReaderImage error adding two slice headers: !!!\
        Slices are not contiguous in Z "
	     << "  Zmin= " << theMinZ << " & " << rhs->GetMinZ()
	     << "  Zmax= " << theMaxZ << " & " << rhs->GetMaxZ()
	     << G4endl;
      G4Exception("DicomVReaderImage::operator+","",
		  JustWarning,"");
    }
  }

  //----- Check that the slices have the same Compression
  if( theCompression != rhs->GetCompression()) {
    G4cerr << "DicomVReaderImage error adding two slice headers: !!!\
        Slices do not have the same compression "
	   << theCompression << " & " << rhs->GetCompression()
	   << G4endl;
    G4Exception("DicomVReaderImage::operator+","",
		FatalException,"");
  }
  
}
	

//-----------------------------------------------------------------------------
DicomVImage* DicomVReaderImage::CreateImage(G4String name, DIModality modality, G4bool bImageFromData, G4bool bAutoOper )
{
  if( theNoVoxelsX%theCompression != 0 || theNoVoxelsY%theCompression != 0 ) {
    G4Exception("DicomVReaderImage::CreateImage",
		"",
		FatalException,
		("Compression factor = " + std::to_string(theCompression) + " has to be a divisor of Number of voxels X = " + std::to_string(theNoVoxelsX) + " and Y " + std::to_string(theNoVoxelsY)).c_str());
  }
  DicomVImage* image = new DicomVImage(name, modality); 
  
  image->SetAutomaticOperate(bAutoOper); 
  image->SetParamsX( theNoVoxelsX/theCompression, theMinX, theMaxX );
  image->SetParamsY( theNoVoxelsY/theCompression, theMinY, theMaxY );
  image->SetParamsZ( theNoVoxelsZ, theMinZ, theMaxZ );

  if( bImageFromData ) {
    theDicomImage = image;
    theDicomImage->SetData( theVoxelData );
    if( DicomVerb(debugVerb) ) G4cout<< this << " " << theDicomImage << " DicomVReaderImage SetData " << theVoxelData << " " << theVoxelData->size() << G4endl;
  } else {
    std::vector<G4double>* imageData = new std::vector<G4double>(theNoVoxelsX/theCompression*theNoVoxelsY/theCompression*theNoVoxelsZ);
    image->SetData(imageData);
  }
  image->SetPrecision( image->GetPrecision()*min(min(GetVoxelDimX(),GetVoxelDimY()),GetVoxelDimZ()) );
  //  G4cout << " DicomVReaderImage::CreateImage PRECISION " << image->GetPrecision() << " " << GetVoxelDimX() << " " << GetVoxelDimY() << " " << GetVoxelDimZ() << G4endl; //GDEB
  if( DicomVerb(debugVerb) ) G4cout << " DicomVReaderImage::CreateImage " << GetName() << " " << GetNoVoxelsX() << " " << GetNoVoxelsY() << " " << GetNoVoxelsZ() << G4endl; 
  return image;
}

//-----------------------------------------------------------------------------
void DicomVReaderImage::Print(std::ostream& out )
{
  out << "@@ CT Slice " << theLocation << G4endl;

  out << "@ NoVoxels " << theNoVoxelsX << " " << theNoVoxelsY << " " << theNoVoxelsZ << G4endl;
  out << "@ DIM X: " << theMinX << " " << theMaxX
      << " Y: " << theMinY << " " << theMaxY
      << " Z: " << theMinZ << " " << theMaxZ << G4endl;
}

 
//-----------------------------------------------------------------------------
void DicomVReaderImage::ResizeImage() // for DicomReaderNM, DicomReaderRTDose
{
  GmParameterMgr* paramMgr = GmParameterMgr::GetInstance();
  
  DicomVOperator* operResize = theDicomMgr->FindOperator("DicomOperResize");
  if( operResize && theCompression != 1 ) {
    if( paramMgr->IsParameterInScript("nVoxX") || paramMgr->IsParameterInScript("nVoxY") ) {
      G4Exception(" DicomVReaderImage::ResizeImage",
		  "",
		  FatalException,
		  "Cannot change image size with DICOM2G4 parameters and at the same time change the :COMPRESSION in metadata file");
    }
  }

  if( !operResize ) {
    if( theCompression != 1 ) {
      operResize = new DicomOperResize();
    } else {
      return;
    }
  }

  //--- compression will be done by DicomOperResize
  if( theCompression != 1 ) {
    theDicomImage->SetNoVoxelsX(theDicomImage->GetNoVoxelsX()*theCompression);
    theDicomImage->SetNoVoxelsY(theDicomImage->GetNoVoxelsY()*theCompression);
    paramMgr->AddParam("nVoxX "+GmGenUtils::itoa(theDicomImage->GetNoVoxelsX()/theCompression),PTdouble); 
    paramMgr->AddParam("nVoxY "+GmGenUtils::itoa(theDicomImage->GetNoVoxelsY()/theCompression),PTdouble);
    //    G4cout << " nVoxX " << GmGenUtils::itoa(theDicomImage->GetNoVoxelsX()/theCompression) << " " << theDicomImage->GetNoVoxelsX() << " / " << theCompression << "   === " << paramMgr->GetNumericValue("nVoxX",-99) << G4endl; //GDEB
  }
  
#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(infoVerb) ) G4cout << " DicomOperResize FOUND " << operResize << std::endl; 
#endif
  if( operResize ) {
    operResize->Operate(theDicomImage);
  }
}
