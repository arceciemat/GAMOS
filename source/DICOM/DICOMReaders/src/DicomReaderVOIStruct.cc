#include "DicomReaderVOIStruct.hh"
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
/*DicomReaderVOIStruct::DicomReaderVOIStruct(DcmDataset* dset) : DicomVReader(dset, DRM_RTStruct)
{
  ReadData();
  }*/

//-----------------------------------------------------------------------------
DicomReaderVOIStruct::DicomReaderVOIStruct(G4String fName )
{
  theModality = DRM_RTStruct;
  theFileName = fName;
  //-filename = GmGenUtils::FileInPath( filename );
  theFileIn = GmFileIn::GetInstance(fName);
  theFileIn.SetUseCommentHashtag(false);
  
  if( DicomVerb(-infoVerb) ) G4cout << " DicomReaderVOIStruct " << fName << G4endl; 
  /*  DcmFileFormat dfile;
      if( ! (dfile.loadFile(fName.c_str())).good() ) {
    G4Exception("DicomReaderVOIStruct::ReadFile",
    "",
    FatalErrorInArgument,
    
    
    ("Error reading file " + fName).c_str());
    }
    DcmDataset* dset = dfile.getDataset();
    theDataset = dset;*/
  theDataset = 0;

  ReadData();
}

//-----------------------------------------------------------------------------
void DicomReaderVOIStruct::ReadData()
{
  //  DicomVerb.SetFilterLevel(testVerb); //GDEB  
  
  G4bool bReadingData = false;
  OFString ROIName, ROINumber, ROIGenerationAlgorithm;
  int nROIs;
  G4double minX, maxX, minY , maxY, minZ, maxZ, widthX, widthY, widthZ;
  G4int nVoxX, nVoxY, nVoxZ;
  G4cout << "  DicomReaderVOIStruct::ReadData START " << theFileName << G4endl; //GDEB 
  JumpLine(3); //  # number_of_columns_x
  nVoxX = GmGenUtils::GetInt(wl[0]);
  JumpLine(1); //  # number_of_columns_x
  nVoxY = GmGenUtils::GetInt(wl[0]);
  JumpLine(1); //  # number_of_columns_z
  nVoxZ = GmGenUtils::GetInt(wl[0]);
  if( DicomVerb(debugVerb) ) G4cout << " NVOXXYZ " << nVoxX << " " << nVoxY << " " << nVoxZ << G4endl;
  JumpLine(3); //  # x_pixel_size
  widthX = GmGenUtils::GetValue(wl[0]);
  JumpLine(1); //  # y_pixel_size
  widthY = GmGenUtils::GetValue(wl[0]);
  JumpLine(1); //  # z_pixel_size
  widthZ = GmGenUtils::GetValue(wl[0]);
  if( DicomVerb(debugVerb) ) G4cout << " WIDTHXYZ " << widthX << " " << widthY << " " << widthZ << G4endl;
  JumpLine(2); //  # # x_origin_location
  minX = GmGenUtils::GetValue(wl[0])-widthX/2.;
  maxX = minX + nVoxX*widthX;
  JumpLine(1); //  # # y_origin_location
  minY = GmGenUtils::GetValue(wl[0])-widthY/2.;
  maxY = minY + nVoxY*widthY;
  JumpLine(1); //  # # z_origin_location
  minZ = GmGenUtils::GetValue(wl[0])-widthZ/2.;
  maxZ = minZ + nVoxZ*widthZ;
  if( DicomVerb(debugVerb) ) G4cout << " MINXYZ " << minX << " " << minY << " " << minZ << G4endl;
  if( DicomVerb(debugVerb) ) G4cout << " MAXXYZ " << maxX << " " << maxY << " " << maxZ << G4endl;
  for( ;; ){
    if(! theFileIn.GetWordsInLine( wl ) ) break;
    //    #NUMBER OF VOI TIME LISTS:
    if( wl.size() == 5 && wl[0] == "#NUMBER" && wl[1] == "OF" && wl[2] == "VOI" && wl[3] == "TIME"  && wl[4] == "LISTS:" ) {
      // START READING FIXED STRUCTURE
      JumpLine(1); // 2       tif(! theFileIn.GetWordsInLine( wl ) ) break;
      nROIs = GmGenUtils::GetInt(wl[0]);  // VOI is DICOM 2 ROI
      if( DicomVerb(debugVerb) ) G4cout << " VOI nROIs " << nROIs << G4endl;
      JumpLine(1); // #VOI TIME LIST DEFINITION:
      for( int ir = 0; ir < nROIs; ir++ ) {
	JumpLine(1);  // #VOI TIME LIST NUMBER 1 DEFINITION:
	JumpLine(1);  // # 1 2 # voi_time_list_index voi_direction <UID>6602407263300</UID>
	CheckLine(5,"voi_direction" );
	if( wl[1] == "2" ) {
	  bReadingData = true;
	  //	ROINumber = wl[0];
	} else {
	  G4Exception("DicomReaderVOIStruct::ReadData",
		      "",
		      JustWarning,
		      ("Only VOI with ROIs in Z direction (=2) can be read, this is direction="+wl[1]).c_str());
	  bReadingData = false;
	}
	JumpLine(1);  // R01_01_GB11ProtPBS_2023_10_27_ # voi_name
	CheckLine(3,"voi_name");
	ROIName = wl[0];
	JumpLine(6); // 1 # voi_index(1 ..)
	if ( wl[2].find("voi_index(") == std::string::npos) {
	  G4Exception("DicomReaderVOIStruct::ReadData",
		      "",
		      JustWarning,
		      ("Error Reading ROI number "+GmGenUtils::itoa(ir)+" It seems to be empty. Aborting data reading").c_str());
	  return;
	} else {
	  G4cout << " OK  Reading ROI number "+GmGenUtils::itoa(ir) << G4endl; //GDEB
	}
	CheckLine(3,"voi_index",false);
	ROINumber = wl[0];
	G4int roiID = GmGenUtils::GetInt(ROINumber.c_str());
	DicomROI* roi = new DicomROI(roiID, ROIName.c_str());
	JumpLine(4); // 35 # number_of_rois
	CheckLine(3,"number_of_rois");
	int nROICs = GmGenUtils::GetInt(wl[0]);  // ROI is DICOM 2 ROIContour
	if( DicomVerb(debugVerb) ) G4cout << " VOI nROICs " << nROICs << G4endl;
	JumpLine(2); // 272 1 # #ROIS DEFINITION:
	for( int irc = 0; irc < nROICs; irc++ ) {
	  JumpLine(2); // 272 1 # roi_slice_index number_of_contours (skip #ROI ON SLICE 273 DEFINITION:)
	  CheckLine(5,"number_of_contours");
	  int nROICsubs = GmGenUtils::GetInt(wl[1]); // Two contours with same Z are considered as 1 ROIs, in DICOM 2 ROIContours
	  if( DicomVerb(debugVerb) ) G4cout << " VOI nROICsubs " << nROICsubs << G4endl;
	  for( int ircs = 0; ircs < nROICsubs; ircs++ ) {
	    DicomROIContour* roiC = new DicomROIContour();
	    roiC->SetGeomType("CLOSED_PLANAR");
	    JumpLine(2); // 21 137.0 -137.0 false 0 false " Contour 1 " # number_of_vertices [sector_start_angle sector_stop_angle] is_line_contour operation is_bspline name VERTICES_DEFINITION_x_y_z_IN_MM:
	    int nPoints = GmGenUtils::GetInt(wl[0]) - 1;
	    if( DicomVerb(debugVerb) ) G4cout << " VOI nPoints " << nPoints << G4endl;
	    std::vector<G4ThreeVector> dataV;
	    for( int ipt = 0; ipt < nPoints; ipt++ ) {
	      if( DicomVerb(testVerb) )  G4cout << ir << " " << irc << " " << ircs << " TO READ PT " << ipt << G4endl;
	      JumpLine(1); // 37.80435205078125 -29.016818359374998 -37.664
	      dataV.push_back( G4ThreeVector( maxX-GmGenUtils::GetValue(wl[0]),maxY+GmGenUtils::GetValue(wl[1]),GmGenUtils::GetValue(wl[2])));
	    }
	    roiC->SetData(dataV);
	    roi->AddContour(roiC);
	  }
	  if( bReadingData ) {
	    theROIs[roiID] = roi;
	  }
	}
      }
    }
  }      

  
#ifndef GAMOS_NO_VERBOSE
  //@@@@ Print ROIs
  std::map<G4int,DicomROI*>::const_iterator iter;
  if( DicomVerb(debugVerb) ) {
    G4cout << " @@@@@@@@@@@ ROIs " << G4endl;
    for( iter = theROIs.begin(); iter != theROIs.end(); iter++ ) {
      DicomROI* roi = (*iter).second;
      roi->Print(G4cout);
    }
  }
#endif
    
  //  DicomVerb.SetFilterLevel(warningVerb); //GDEB
  
  //-------------
  BuildPolygonSet();
  //  G4cout << "  DicomReaderVOIStruct after BuildPolygonSet" << G4endl; //GDEB

  
}


//-----------------------------------------------------------------------------
G4bool DicomReaderVOIStruct::JumpLine( size_t nLines )
{
  for( size_t ii = 0; ii < nLines; ii++ ) {
    if( ! theFileIn.GetWordsInLine( wl ) ) {
      return false;
    }
  }
  
  return true;
  
  //break;
  
}

//-----------------------------------------------------------------------------
void DicomReaderVOIStruct::CheckLine(size_t iword, G4String text, G4bool bFullText )
{ 
#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(testVerb) ) {
    PrintLine( G4cerr );
  }
#endif
  iword = iword - 1;
  
  if( wl.size() <= iword ){
    PrintLine( G4cerr );
    G4Exception("DicomReaderVOIStruct::CheckLine",
		"",
		FatalErrorInArgument,  
		("Asking for word number "+GmGenUtils::itoa(iword)+", while there are only "+GmGenUtils::itoa(wl.size())).c_str());
  }
  if(bFullText) {
    if( wl[iword] != text ) {
      PrintLine( G4cerr );
      G4Exception("DicomReaderVOIStruct::CheckLine",
		  "",
		  FatalErrorInArgument,
		  ("Different word found= "+wl[iword]+" requested= "+text).c_str());
    }
  } else {
    if ( wl[iword].find(text) == std::string::npos) {
      PrintLine( G4cerr );
      G4Exception("DicomReaderVOIStruct::CheckLine",
		  "",
		  FatalErrorInArgument,
		  ("Different word found= "+wl[iword]+" should contain= "+text).c_str());
    }
  }
}


//-----------------------------------------------------------------------------
void DicomReaderVOIStruct::PrintLine( std::ostream& out )
{
  G4String line = "VOILINE: ";
  for( size_t ii = 0; ii < wl.size(); ii++ ) {
    line += wl[ii] + " ";
  }
  out << line << G4endl;
}

