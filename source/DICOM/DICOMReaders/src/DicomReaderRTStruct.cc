#include "DicomReaderRTStruct.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"
#include "DICOM/DICOMBase/include/DicomPolygon.hh"
#include "DICOM/DICOMBase/include/DicomPolygonList.hh"
#include "DICOM/DICOMBase/include/DicomPolygonSet.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmVerbosity.hh"
#include "GamosCore/GamosReadDICOM/include/GmReadPhantomStMgr.hh"

#include "G4ThreeVector.hh"

#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcpixel.h"
#include "dcmtk/dcmdata/dcpxitem.h"
#include "dcmtk/dcmdata/dcpixseq.h"
#include "dcmtk/dcmrt/drtstrct.h"
#include "dcmtk/dcmrt/seq/drtrfors.h"  // for ReferencedFrameOfReferenceSequence
#include "dcmtk/dcmrt/seq/drtssrs.h"   // for StructureSetROISequence
#include "dcmtk/dcmrt/seq/drtrcs.h"      // for ROIContourSequence
#include "dcmtk/dcmrt/seq/drtcs.h"      // for ContourSequence
#include "dcmtk/dcmrt/seq/drtcis.h"      // for ContourImageSequence
#include "dcmtk/config/osconfig.h"   // make sure OS specific configuration is included 

//-----------------------------------------------------------------------------
DicomReaderRTStruct::DicomReaderRTStruct(DcmDataset* dset) : DicomVReader(dset, DRM_RTStruct)
{
  ReadData();
}

//-----------------------------------------------------------------------------
DicomReaderRTStruct::DicomReaderRTStruct(G4String fName ): DicomVReader(DRM_RTStruct)
{
  theFileName = fName;
  if( DicomVerb(infoVerb) ) G4cout << " DicomReaderRTStruct " << fName << G4endl; 
  DcmFileFormat dfile;
  if( ! (dfile.loadFile(fName.c_str())).good() ) {
    G4Exception("DicomHandler::ReadFile",
		"",
		FatalErrorInArgument,


		("Error reading file " + fName).c_str());
  }
  DcmDataset* dset = dfile.getDataset();
  theDataset = dset;

  ReadData();
}

//-----------------------------------------------------------------------------
void DicomReaderRTStruct::ReadData()
{
  DRTStructureSetIOD rtstruct;
  OFCondition result = rtstruct.read(*theDataset);
  //  DCMRT_INFO("Read RT Structure Set: " << status.text());
  if (!result.good()) {
    G4Exception("DicomReaderRTStruct::ReadData",
		"DFS001",
		FatalException,
		result.text());
  }


  //@@@@@@@@@@@@ DRTReferencedFrameOfReferenceSequence
  DRTReferencedFrameOfReferenceSequence refSeq = rtstruct.getReferencedFrameOfReferenceSequence();
  if( refSeq.isEmpty() ) {
    G4Exception("DicomReaderRTStruct::ReadData",
		"DFS002",
		JustWarning,
		"DRTReferencedFrameOfReferenceSequence is empty");
  }

  if( DicomVerb(debugVerb) ) G4cout << "@@@@@ NUMBER OF ReferenceSequences " << refSeq.getNumberOfItems() << G4endl;
  refSeq.gotoFirstItem();
  for( size_t i1 = 0; i1 < refSeq.getNumberOfItems(); i1++ ) {
    DRTReferencedFrameOfReferenceSequence::Item &item = refSeq.getCurrentItem();
    OFString uid;
    item.getFrameOfReferenceUID(uid);
    if( DicomVerb(debugVerb) ) G4cout << " FrameOfReferenceUID " << uid << G4endl;
    DRTRTReferencedStudySequence &reference_study_sequence_ref = item.getRTReferencedStudySequence();   
    if( DicomVerb(debugVerb) ) G4cout << "@@@@ NUMBER OF ReferenceStudySequences " << reference_study_sequence_ref.getNumberOfItems() << G4endl;
    reference_study_sequence_ref.gotoFirstItem();
    for( size_t i2 = 0; i2 < reference_study_sequence_ref.getNumberOfItems(); i2++ ) {
      DRTRTReferencedStudySequence::Item &rss_item = reference_study_sequence_ref.getCurrentItem();
      DRTRTReferencedSeriesSequence &series_seq_ref = rss_item.getRTReferencedSeriesSequence();
      if( DicomVerb(debugVerb) ) G4cout << "@@@ NUMBER OF SeriesSequences " << series_seq_ref.getNumberOfItems() << G4endl;
      series_seq_ref.gotoFirstItem();
      for( size_t i3 = 0; i3 < series_seq_ref.getNumberOfItems(); i3++ ) {
	DRTRTReferencedSeriesSequence::Item &ref_series_seq_item = series_seq_ref.getCurrentItem();
	DRTContourImageSequence &image_sequence_seq_ref = ref_series_seq_item.getContourImageSequence();
	if( DicomVerb(debugVerb) ) G4cout << "@@ NUMBER OF ContourImageSequences " << image_sequence_seq_ref.getNumberOfItems() << G4endl;
	image_sequence_seq_ref.gotoFirstItem();
	for( size_t i4 = 0; i4 < image_sequence_seq_ref.getNumberOfItems(); i4++ ) {
	  DRTContourImageSequence::Item &image_contour_item = image_sequence_seq_ref.getCurrentItem();
	  OFString refSOPInstUID;
	  image_contour_item.getReferencedSOPInstanceUID(refSOPInstUID);
	  std::cout <<"ReferencedSOPInstanceUID= " << refSOPInstUID << std::endl;
	  image_sequence_seq_ref.gotoNextItem().good();
	} // end if image_sequence_seq_ref
	series_seq_ref.gotoNextItem();
      } // end if series_seq_ref good
      reference_study_sequence_ref.gotoNextItem();
    } // end if reference_study_sequence_ref good
    refSeq.gotoNextItem();
  } // end if refSeq.first item
  
  //@@@@@@@@@@@@   DRTROISequence 
  DRTStructureSetROISequence ROISeq = rtstruct.getStructureSetROISequence();
  if( DicomVerb(debugVerb) ) G4cout << "@@@@@ NUMBER OF ROISequences " << ROISeq.getNumberOfItems() << G4endl;
  for( size_t i1 = 0; i1 < ROISeq.getNumberOfItems(); i1++ ) {
    DRTStructureSetROISequence::Item &item = ROISeq.getCurrentItem();
    OFString ROIName, ROINumber, ROIGenerationAlgorithm;
    item.getROINumber(ROINumber);
    item.getROIName(ROIName);
    item.getROIGenerationAlgorithm(ROIGenerationAlgorithm);
    if( ROINumber != "" ) {
      G4int roiID = GmGenUtils::GetInt(ROINumber.c_str());
      DicomROI* roi = new DicomROI(roiID, ROIName.c_str());
      theROIs[roiID] = roi;
      if( DicomVerb(debugVerb) ) G4cout << "New ROI: " << roiID << " = " << roi->GetNumber() << " " << roi->GetName() << " " << ROIGenerationAlgorithm << G4endl;
    }

    ROISeq.gotoNextItem().good();
  } // end if ROISeq.first item


  //@@@@@@@@@@@@   DRTROIContourSequence
  DRTROIContourSequence ROIContourSeq = rtstruct.getROIContourSequence();
  if( DicomVerb(debugVerb) ) G4cout << "@@@@@ NUMBER OF ROIContourSequences " << ROIContourSeq.getNumberOfItems() << G4endl;
  /*  if( ROISeq.getNumberOfItems() != ROIContourSeq.getNumberOfItems() ) {
    G4ExceptionSeverity sever = FatalException;
    if( ROISeq.getNumberOfItems() == 0 ) sever = JustWarning;
    G4Exception("DicomReaderRTStruct",
		"DCS0001",
		sever,
		("Different number of ROIs and ROI Contours " + std::to_string(ROISeq.getNumberOfItems()) + " <> " + std::to_string(ROIContourSeq.getNumberOfItems())).c_str());
		} */

  ROIContourSeq.gotoFirstItem();
  for( size_t i1 = 0; i1 < ROIContourSeq.getNumberOfItems(); i1++ ) {
    DRTROIContourSequence::Item &item = ROIContourSeq.getCurrentItem();
    OFString displayColor;
    item.getROIDisplayColor(displayColor);
    OFString refROINumberStr;
    item.getReferencedROINumber(refROINumberStr);   
    if( DicomVerb(debugVerb) ) G4cout << " ROINumber " << refROINumberStr << " ROIDisplayColor " << displayColor << G4endl;
    DicomROI* roi = 0;
    if( refROINumberStr != "" ) {
      G4int refROINumber = GmGenUtils::GetInt(refROINumberStr.c_str());
      std::map<G4int,DicomROI*>::const_iterator iter = theROIs.find(refROINumber); 
      if( iter == theROIs.end() ) {
	G4Exception("DicomReaderRTStruct::DicomReaderRTStruct",
		    "",
		    JustWarning,
		    ("Contour sequence has a ReferencedROINumber tag that does not correspond to any ROI in DRTStructureSetROISequence, a new ROI will be created with name Struct"+refROINumberStr).c_str());
	roi = new DicomROI(refROINumber,(G4String("Struct")+refROINumberStr).c_str());
	theROIs[refROINumber] = roi;
	if( DicomVerb(debugVerb) ) G4cout << "New ROI: " << roi->GetNumber() << " " << roi->GetName() << G4endl;
      } else {
	roi = (*iter).second;
      }

    } else {
      G4Exception("DicomReaderRTStruct::DicomReaderRTStruct",
		  "",
		  JustWarning,
		  "Contour sequence does not have any ReferencedROINumber tag, it will be assigned to the last ROI");
      roi = (*(theROIs.rbegin())).second;
    }
    //    std::cout << refROINumberStr << " ROI ASSIGNED " << roi->GetNumber() << " " << roi->GetName() << std::endl; //GDEB

    //--- Loop to contours lists (sequences)
    DRTContourSequence contour_seq = item.getContourSequence();
    //    if( Dicom2G4FileMgr::verbose >= -testVerb ) G4cout << "@@@@ NUMBER OF ContourSequences " << contour_seq.getNumberOfItems() << G4endl;
    contour_seq.gotoFirstItem();
    for( size_t i2 = 0; i2 < contour_seq.getNumberOfItems(); i2++ ) {
      //      if (contour_seq.gotoFirstItem().good()) {
      //	do {
      DRTContourSequence::Item &cs_item = contour_seq.getCurrentItem();
      
      DicomROIContour* roiC = new DicomROIContour();
      
      DRTContourImageSequence &contour_image_seq = cs_item.getContourImageSequence();
      
      //      if( Dicom2G4FileMgr::verbose >= -testVerb ) G4cout << "@@@ NUMBER OF ContourImageSequences " << contour_image_seq.getNumberOfItems() << G4endl;
      contour_image_seq.gotoFirstItem();
      //--- Loop to contours image sequences (CT images associated to this contour, usually only 1)
      for( size_t i3 = 0; i3 < contour_image_seq.getNumberOfItems(); i3++ ) {
	DRTContourImageSequence::Item cis_item = contour_image_seq.getCurrentItem();
	OFString refSOPCUID;
	cis_item.getReferencedSOPClassUID(refSOPCUID);
	OFString refSOPIUID;
	cis_item.getReferencedSOPInstanceUID(refSOPIUID);
	//	if( DicomVerb(debugVerb) ) G4cout << " ContourImageSequence C= " << refSOPCUID << " I= " << refSOPIUID << G4endl;
	if( refSOPIUID != "") roiC->AddImageIUID(refSOPIUID.c_str());
	contour_image_seq.gotoNextItem();
      } // end if contour_image_seq
      
	//@@@
      OFString geomType;
      cs_item.getContourGeometricType(geomType);
      Sint32 nPoints;
      cs_item.getNumberOfContourPoints(nPoints);
      //      if( DicomVerb(debugVerb) ) G4cout << "@@@ GeomType " << geomType << " NPOINTS " << nPoints << G4endl;
      roiC->SetGeomType(geomType);
      OFVector<Float64> data;
      cs_item.getContourData(data);
      std::vector<G4ThreeVector> dataV;
      for( Sint32 ii = 0; ii < nPoints*3; ii++ ) {
	//	if( ii%3 == 0 ) if( DicomVerb(debugVerb) ) G4cout << "(";
	//	if( DicomVerb(debugVerb) ) G4cout << data[ii];
	//	if( ii%3 == 0 || ii%3 == 1 ) G4cout << ",";
	//	if( ii%3 == 2 ) G4cout << ")" << G4endl;
	if( ii%3 == 2 ) dataV.push_back( G4ThreeVector( data[ii-2], data[ii-1], data[ii] ) );
      }
      roiC->SetData(dataV);
      //      if( roiC->GetDirections().size() > 1 ) G4cout << roiC << " " << " CHECK1 RTSTR ADD2nd DIRECTION IN POLY " <<roiC->GetDirections()[1]  << " " << roiC->GetPoints()[2] << " - " <<  roiC->GetPoints()[1]  << " " << i2 << " <? " <<contour_seq.getNumberOfItems() <<G4endl; //GDEB
      roi->AddContour(roiC);
      //      G4cout << " refROINumber " << refROINumberStr << " geomType " << geomType <<  G4endl;

      //      if( roiC->GetDirections().size() > 1 ) G4cout << roiC->GetDirections().size() << " " << roiC << " " << " CHECK1b RTSTR ADD2nd DIRECTION IN POLY " <<roiC->GetDirections()[1]  << " " << roiC->GetPoints()[2] << " - " <<  roiC->GetPoints()[1]  << " " << i2 << " <? " <<contour_seq.getNumberOfItems() <<G4endl; //GDEB
      //      G4cout << roi->GetNumber() << " " << roi->GetName() << " ROI Added Contour " << roi->GetContours().size() << G4endl; //GDEB

      contour_seq.gotoNextItem();
      //	} while (contour_seq.gotoNextItem().good());
      //      } // end if contour_seq
    }
    /*    std::vector<DicomROIContour*> contours = roi->GetContours();
    for( size_t kk = 0; kk < contours.size(); kk++ ){
      // check contour corresponds to this CT slice
      DicomROIContour* roiC = contours[kk];
      if( roiC->GetGeomType() == "CLOSED_PLANAR" ){
	//	if( roiC->GetDirections().size() > 1 ) G4cout << roiC->GetDirections().size() << " " << roiC << " " << " CHECK2 RTSTR ADD2nd DIRECTION IN POLY " <<roiC->GetDirections()[1]  << " " << roiC->GetPoints()[2] << " - " <<  roiC->GetPoints()[1]  << G4endl; //GDEB
      }
      } */
    
    ROIContourSeq.gotoNextItem();
  } // end if ROIContourSeq.first item

  
  //  double theBitsAllocated = Read1Data(theDataset, DCM_BitsAllocated, 1)[0];
  //  G4cout << " BITS ALLOCATED " << theBitsAllocated << G4endl;
  
/*SamplesPerPixel
  BitsAllocated
  PixelRepresentation
  dsrdoc
  dsrdoctr
    
  // The x, y, and z coordinates of the upper left hand corner (center of the first voxel transmitted) of the image, in mm.
  std::vector<double> dImagePositionPatient = Read1Data(theDataset, DCM_ImagePositionPatient,3);
  fLocation = dImagePositionPatient[2];
  std::vector<double> dSliceThickness = Read1Data(theDataset, DCM_SliceThickness, 1);
  std::vector<double> dPixelSpacing = Read1Data(theDataset, DCM_PixelSpacing, 2);

  std::vector<double> dRows = Read1Data(theDataset, DCM_Rows, 1);
  std::vector<double> dColumns = Read1Data(theDataset, DCM_Columns, 1);
  */

  //@@@@ Print ROIs
  std::map<G4int,DicomROI*>::const_iterator iter; 
  if( DicomVerb(debugVerb) ) {
    G4cout << " @@@@@@@@@@@ ROIs " << G4endl;
    for( iter = theROIs.begin(); iter != theROIs.end(); iter++ ) {
      DicomROI* roi = (*iter).second;
      roi->Print(G4cout);
    }
  }


  //  for( size_t jj = 0; jj < theROIs.size(); jj++ ){    
    //    std::cout <<this << " " << jj << " " << theROIs[jj] << " LOOP ROIS "<< theROIs[jj]->GetNumber() << " " << theROIs[jj]->GetName() << " CONT " << theROIs[jj]->GetContours().size() << G4endl; //GDEB
  //  }

  /*  for( iter = theROIs.begin(); iter != theROIs.end(); iter++ ) {
    DicomROI* roi = (*iter).second;
    std::vector<DicomROIContour*> contours = roi->GetContours();
    for( size_t kk = 0; kk < contours.size(); kk++ ){
      // check contour corresponds to this CT slice
      DicomROIContour* roic = contours[kk];
      if( roic->GetGeomType() == "CLOSED_PLANAR" ){
	//	if( roic->GetDirections().size() > 1 ) G4cout << this << " " << " CHECK RTSTR ADD2nd DIRECTION IN POLY " <<roic->GetDirections()[1]  << " " << roic->GetPoints()[2] << " - " <<  roic->GetPoints()[1]  << G4endl; //GDEB
      }
    }
    }*/

  //-------------
  BuildPolygonSet();
  //  G4cout << "  DicomReaderRTStruct after BuildPolygonSet" << G4endl; //GDEB
}


//-----------------------------------------------------------------------------
void DicomReaderRTStruct::BuildPolygonSet()
{
  thePolygonSet = new DicomPolygonSet("RTStruct_"+theFileName,DPOrientXY);
  
  //  for( size_t jj = 0; jj < theROIs.size(); jj++ ){    
  //    std::cout << this << " " << jj << " " << theROIs[jj] << " 2LOOP ROIS "<< theROIs[jj]->GetNumber() << " " << theROIs[jj]->GetName() << " CONT " << theROIs[jj]->GetContours().size() << G4endl; //GDEB
  //  }
    
  std::map<G4int,DicomROI*>::const_iterator iter; 
  for( iter = theROIs.begin(); iter != theROIs.end(); iter++ ) {
    DicomROI* roi = (*iter).second;
    G4int roiID = roi->GetNumber(); 
    DicomPolygonList* polyList = new DicomPolygonList(roi->GetName().c_str(), roiID, DPOrientXY);
    thePolygonSet->AddLineList(static_cast<DicomVLineList*>(polyList));
    if( DicomVerb(debugVerb) ) G4cout << thePolygonSet->GetName() << " DicomReaderRTStruct::BuildPolygonSet AddLineList " << roi->GetName() << " " << roiID << " NLists " << thePolygonSet->GetLineLists().size() << G4endl;

    //    roi->GetContours();
    //    std::cout << jj << " " << roi << " BuildStructureIDs checking ROI "<< roiID << " " << roi->GetName() << " CONT " << roi->GetContours().size() << G4endl; //GDEB

    std::vector<DicomROIContour*> contours = roi->GetContours();
    for( size_t kk = 0; kk < contours.size(); kk++ ){
      // check contour corresponds to this CT slice
      DicomROIContour* roic = contours[kk];
      // Check Contour type      
      if( roic->GetGeomType() == "CLOSED_PLANAR" ){
  	// Get min and max X and Y, and corresponding slice indexes
	std::vector<G4ThreeVector> points = roic->GetPoints();	  
	if( points.size() < 3 ) {
	  G4Exception("DicomReaderRTStruct::BuildPolygonSet",
		      "",
		      JustWarning,
		      ("Number of points less than 3 for contour " + roi->GetName() + " : "+GmGenUtils::itoa(kk)+" No  polygon will be built").c_str());
	  continue;
	}
	if( DicomVerb(debugVerb) ) G4cout << " " << kk << " NPOINTS " << points.size() << G4endl;
	std::vector<G4ThreeVector> dirs = roic->GetDirections();
	DicomPolygon* polyg = new DicomPolygon(points,dirs,roi->GetName().c_str(), DPOrientXY);
	polyList->AddLine(polyg);
	if( DicomVerb(debugVerb) ) G4cout << "DicomReaderRTStruct::BuildPolygonSet AddPolygon FROM " << roi->GetName() << " " << dirs[1] << " " << points[2] << " " << points[1] << " Npoly " << polyList->GetLines().size() << G4endl;
      }
    }
  }
  //  G4cout << " END BulidPolygon " << G4endl; //GDEB
}


//-----------------------------------------------------------------------------
void DicomReaderRTStruct::FindVoxelsInXY( DicomVImageStr* imageStruct, DicomPolygonSet* polygonSet )
{
  //  G4cout << " DicomReaderRTStruct::FindVoxelsInXY( " << G4endl; //GDEB
  theStructImage = imageStruct;
  polygonSet->FindVoxelsInXY( imageStruct );
}
