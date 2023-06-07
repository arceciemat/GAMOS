#ifndef GAMOS_NO_ROOT

#include "DicomDrawerROOT.hh"
#include "DicomVImage.hh"
#include "DicomVLineSet.hh"
#include "DicomVLineList.hh"
#include "DicomVLine.hh"
#include "DicomPolygonSet.hh"
#include "DicomPolygonList.hh"
#include "DicomPolygon.hh"
#include "DicomParameterMgr.hh"
#include "DicomMgr.hh"
#include "DicomVerbosity.hh"
#include "DicomOperLog.hh"
#include "DicomOperRotateXY180.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "CLHEP/Random/RandFlat.h"

#include "TCanvas.h"
#include "TH2F.h"
#include "TGaxis.h"
#include "TLine.h"
#include "TStyle.h"
#include "TText.h" 
#include "TFile.h" 

#include <set>

//-----------------------------------------------------------------------------
DicomDrawerROOT::DicomDrawerROOT()
{
  theImageFormat = "jpg";
  
  theCanvasNPixelsX = G4int(DicomParameterMgr::GetInstance()->GetNumericValue("nPixelsX",1000));
  theCanvasNPixelsY = G4int(DicomParameterMgr::GetInstance()->GetNumericValue("nPixelsY",1000));
  theCanvasNPixelsZ = G4int(DicomParameterMgr::GetInstance()->GetNumericValue("nPixelsZ",1000));
  gStyle->SetOptStat(0);
  thePaletteMin = DicomParameterMgr::GetInstance()->GetNumericValue("paletteMin",-DBL_MAX);
  thePaletteMax = DicomParameterMgr::GetInstance()->GetNumericValue("paletteMax",DBL_MAX);

  bShowTitle = bool(DicomParameterMgr::GetInstance()->GetNumericValue("bFigureTitle",1));
/*  G4bool drawLog = G4bool(DicomParameterMgr::GetInstance()->GetNumericValue("drawLog",0));
  if( drawLog ) {
    theOperatorLog = new DicomOperLog();
  } else {
    theOperatorLog = 0;
    }*/
  theParamMgr = (DicomParameterMgr*)(DicomParameterMgr::GetInstance());
  if( DicomMgr::GetInstance()->GetRotateXY180() ) {
    bRotateXY180 = true;
  } else {
    bRotateXY180 = false;
  }

  theLineDrawingOptions = DicomMgr::GetInstance()->GetLineDrawingOptions();
  if( theLineDrawingOptions.size() == 0 ) {
    bLineDrawingOptions = false;
  } else {
    bLineDrawingOptions = true;
  }

  theOutputFile = 0;

}

//-----------------------------------------------------------------------------
void DicomDrawerROOT::SetOutputFile( G4String foutName )
{
  theOutputFile = new TFile(foutName.c_str(),"RECREATE");
}

//-----------------------------------------------------------------------------
DicomPolygonSet* DicomDrawerROOT::VoxelsInUseClosest( DicomPolygonSet* polySet, DicomVImage* image )
{
  DicomPolygonSet* newPolySet = new DicomPolygonSet(polySet->GetName(),polySet->GetOrientation());
  std::vector<DicomVLineList*> lineLists = polySet->GetLineLists();
  std::vector<DicomVLineList*> newLineLists;
  // build lines in polylist, taking closest to voxel
  G4double imMinZ = image->GetMinZ();
  G4double imMaxZ = image->GetMaxZ();
  G4double imWidthZ = image->GetVoxelDimZ();
  for( size_t ipl = 0; ipl < lineLists.size(); ipl++ ) {
    DicomPolygonList* polyList = (DicomPolygonList*)(lineLists[ipl]);
    DicomPolygonList* newPolyList = new DicomPolygonList(polyList->GetName(),polyList->GetNumber(),polySet->GetOrientation());
    if( DicomVerb(debugVerb) ) {
      G4cout << " bVoxelsInUseClosest polylist " << polyList->GetName() << " N " << polyList->GetLines().size() << G4endl;
    }
    std::vector<DicomVLine*> lines = polyList->GetLines();
    if( lines.size() == 0 ) continue;
    
    mmddp posOrderedLines = polyList->BuildPosOrderedLines();
    pmmddpi polygonsInPlane;
    pmmddpi pitet;
    if( DicomVerb(debugVerb) ) {
      G4cout << "DicomPolygonList bVoxelsInUseClosest N " << posOrderedLines.size() << G4endl;
    }
    // line_range=line+-1/2*distance_between_lines
    G4double line_range = 0;
    mmddp::const_iterator iteDOWN = posOrderedLines.begin();
    mmddp::const_iterator iteUP = posOrderedLines.end(); iteUP--;
    if( DicomVerb(debugVerb) ) {
      G4cout << imMinZ << " posOrderedLines BOTTOM " << iteDOWN->first << " TOP " << iteUP->first << G4endl; 
    }
    G4bool bOnePolygonZ = false;
    pitet = posOrderedLines.equal_range((*iteDOWN).first);
    if( pitet.first->first == pitet.second->first ) bOnePolygonZ = true;
    // all polygons in one unique Z
    // lines are all below image: check if adding line_range is in
    if( iteUP->first < imMinZ ) {
      if( bOnePolygonZ ) { // all polygons in one unique Z
	return polySet;
      } else {
	pitet = posOrderedLines.equal_range((*iteUP).first);
	mmddpi ite1 = pitet.first; ite1--;
	line_range = (ite1->first-pitet.first->first)/2.;
	if( iteUP->first+line_range < imMinZ ) {
	  return polySet;
	} else {
	  polygonsInPlane = posOrderedLines.equal_range((*iteUP).first);
	  for( mmddpi item = polygonsInPlane.first; item != polygonsInPlane.second; item++ ) {
	    newPolyList->AddDisplacedLine( (*item).second, +line_range );
	  }
	}
      }
      // lines are all above image: check if subtracting line_range is in
    } else if( iteDOWN->first > imMaxZ ) {
      if( bOnePolygonZ ) { // all polygons in one unique Z
	return polySet;
      } else {
	pitet = posOrderedLines.equal_range((*iteDOWN).first);
	mmddpi ite1 = pitet.second; ite1++; 
	line_range = -(pitet.first->first-ite1->first)/2.;
	if( iteDOWN->first-line_range > imMaxZ ) {
	  return polySet;
	} else {	
	  polygonsInPlane = posOrderedLines.equal_range((*iteDOWN).first);
	  for( mmddpi item = polygonsInPlane.first; item != polygonsInPlane.second; item++ ) {
	    newPolyList->AddDisplacedLine( (*item).second, +line_range );
	  }
	}
      }
    } else {
      if( bOnePolygonZ ) { // all polygons in one unique Z
	polygonsInPlane = posOrderedLines.equal_range((*iteUP).first);
	for( mmddpi item = polygonsInPlane.first; item != polygonsInPlane.second; item++ ) {
	  newPolyList->AddDisplacedLine( (*item).second, +line_range );
	}
      } else {
	// If 2*line_range > voxelBinWidth: a voxel lower edge and upper edge will have the same lineList as upper
	// If 2*line_range < voxelBinWidth: two or more polygons are in the same voxel: take the one closest to the voxel center
	// loop to all voxels from the ones of the minimum polygon to the maximum one
	G4int iPolygonZMin = std::max(iteDOWN->second->GetPolygonZIndex( image ),0);
	G4int iPolygonZMax = std::min(iteUP->second->GetPolygonZIndex( image ),G4int(image->GetNoVoxelsZ())-1);
	if( DicomVerb(debugVerb) ) {
	  G4cout << " DicomPolygonList  iPolygonZMin/Max " << iPolygonZMin << " " << iPolygonZMax << G4endl;
	}
	mmddp::const_iterator mite;
	for( int ipz = iPolygonZMin; ipz <= iPolygonZMax; ipz++ ) {
	  line_range = 0.;
	  G4double poszUP = imMinZ+(ipz+1)*imWidthZ; // upper position of voxels in Z plane
	  iteUP = posOrderedLines.upper_bound(poszUP);
	  G4double poszDOWN = imMinZ+ipz*imWidthZ; // lower position of voxels in Z plane
	  iteDOWN = posOrderedLines.upper_bound(poszDOWN);
	  if( DicomVerb(debugVerb) ) {
	    G4cout << iteUP->first << " FROM poszUP " << poszUP << " = " << imMinZ << " + " << (ipz+1) << " * " << imWidthZ << G4endl;
	    G4cout << iteDOWN->first << " FROM poszDOWN " << poszDOWN << " = " << imMinZ << " + " << ipz << " * " << imWidthZ << G4endl;
	  }
	  std::set<G4double> positions; // how many different Z pos in this Z plane
	  for( mite = iteDOWN; mite != iteUP; mite++ )  {
	    positions.insert((*mite).first);
	    if( DicomVerb(debugVerb) ) {
	      G4cout << " INSERT POSITION " << (*mite).first << G4endl;
	    }
	  }
	  positions.insert((*mite).first); // iteUP
	  
	  G4int distance = positions.size()-1;
	  //	G4cout << "DicomPolygonList  distance " << distance << G4endl; //GDEB
	  if( distance == 0 ) { // if upper_bound of poszDOWN and poszUP is equal, both are above
	    // No polygon: use the one above or the one below (the one that is closest to the voxel boundaries) subtracting/adding line_range
	    pitet = posOrderedLines.equal_range((*iteUP).first);
	    mmddpi ite1 = pitet.first;  ite1--;
	    line_range = (pitet.first->first-ite1->first)/2.;
	    G4double dist2up = iteUP->first - poszUP;
	    iteDOWN = pitet.first; iteDOWN--; // polygon below poszDOWN
	    G4double dist2down = poszDOWN - iteDOWN->first;
	    line_range = ((*iteUP).first-(*iteDOWN).first)/2.;
	    if( DicomVerb(debugVerb) ) {
	      G4cout << " DISTANCE 0  UP: " << dist2up << " = " << iteUP->first << " - " << poszUP << " DOWN: " << dist2down << " = " << poszDOWN << " -  " << iteDOWN->first << " line_range " << line_range << G4endl;
	    }
	    if(dist2up <= dist2down) {
	      polygonsInPlane = posOrderedLines.equal_range((*iteUP).first);
	      for( mmddpi item = polygonsInPlane.first; item != polygonsInPlane.second; item++ ) {
		newPolyList->AddDisplacedLine( (*item).second, +line_range );
	      }
	      line_range *= -1;
	    } else {
	      polygonsInPlane = posOrderedLines.equal_range((*iteDOWN).first);
	      for( mmddpi item = polygonsInPlane.first; item != polygonsInPlane.second; item++ ) {
		newPolyList->AddDisplacedLine( (*item).second, +line_range );
	      }
	      //	      line_range *= +1;
	    }
	  } else if( distance == 1 ) {
	    if( DicomVerb(debugVerb) ) {
	      G4cout << " DISTANCE 1  USE DOWN : " << iteDOWN->first << G4endl;
	    }
	    // OK: loop to all Polygon's with  Z = the one below
	    polygonsInPlane = posOrderedLines.equal_range((*iteDOWN).first);
	    for( mmddpi item = polygonsInPlane.first; item != polygonsInPlane.second; item++ ) {
	      newPolyList->AddDisplacedLine( (*item).second, +line_range );
	    }
	    //pmite = posOrderedLines.equal_range((*iteDOWN).first);
	  } else {
	    // find the one closest to voxel center
	    G4double poszCENTRE = imMinZ+(ipz+0.5)*imWidthZ;
	    std::map<G4double, DicomPolygon*>::const_iterator iteoC;
	    std::map<G4double, DicomPolygon*>::const_iterator iteoClosest;
	    G4double distMin = DBL_MAX; 
	    for( iteoC = iteDOWN; iteoC != iteUP; iteoC++ ) {
	      G4double dist = fabs( (*iteoC).first - poszCENTRE );
	      if( DicomVerb(debugVerb) ) {
		G4cout << " DISTANCE >1  DIST " << dist << " = fabs " << (*iteoC).first << " - " << poszCENTRE << G4endl;
	      }
	      if( dist < distMin ) {
		dist = distMin;
		iteoClosest = iteoC;
	      }
	    }
	    polygonsInPlane = posOrderedLines.equal_range(iteoClosest->first);
	    for( mmddpi item = polygonsInPlane.first; item != polygonsInPlane.second; item++ ) {
	      newPolyList->AddDisplacedLine( (*item).second, +line_range );
	    }
	  }
	}
      }
    }
    newLineLists.push_back(newPolyList);
    if( DicomVerb(debugVerb) ) {
      G4cout << " bVoxelsInUseClosest newLineLists ADD " << newPolyList->GetName() << " N " << newLineLists.size() << G4endl;
    }
  } // end of old poly list loop
  newPolySet->SetLineLists(newLineLists);

  return newPolySet;

}

//-----------------------------------------------------------------------------
void DicomDrawerROOT::DrawXY( DicomVImage* image, std::vector<DicomVLineSet*> lineSets, G4String extraFileName )
{
  if( theRotateXY180Images.find(image) == theRotateXY180Images.end() ) {
    if( bRotateXY180 ) {
      std::vector<DicomVOperator*> operators = DicomMgr::GetInstance()->GetOperators();
      DicomOperRotateXY180* rotateOper = 0;
      for( size_t ii = 0; ii < operators.size(); ii++ ) {
	if( dynamic_cast<DicomOperRotateXY180*>(operators[ii]) != 0 ) {
	  rotateOper = dynamic_cast<DicomOperRotateXY180*>(operators[ii]);
	  break;
	}
      }
      rotateOper->Operate(image);
      theRotateXY180Images.insert(image);
    }
  }
  
  TCanvas* canvas = new TCanvas; 
  canvas->SetRightMargin(0.2);
  size_t imNVoxZ = image->GetNoVoxelsZ();
  size_t imNVoxX = image->GetNoVoxelsX();
  size_t imNVoxY = image->GetNoVoxelsY();
  //  size_t imNVoxXY = imNVoxX*imNVoxY;
  //  size_t imNVoxZ = image->GetNoVoxelsZ();
  G4double imMinX = image->GetMinX();
  G4double imMinY = image->GetMinY();
  G4double imMaxX = image->GetMaxX();
  G4double imMaxY = image->GetMaxY();
  G4double imMinZ = image->GetMinZ();
  G4double imWidthZ = image->GetVoxelDimZ();
  canvas->SetCanvasSize(theCanvasNPixelsX,theCanvasNPixelsY*(imMaxY-imMinY)/(imMaxX-imMinX));

  if( DicomVerb(debugVerb) ) {
    std::cout << " DrawXY IMAGE NAME " << image->GetName() << " " << DicomVImage::GetModalityStr( image->GetModality() )<< std::endl; //GDEB
  }
  std::vector<G4double>* imgDatap = image->GetData();
  G4double* imgData = &(imgDatap->at(0));

  //  std::cout << " imagData 0 " << imNVoxXY*imNVoxZ << " " << imNVoxX << " " << imNVoxY << " " << imNVoxXY << " " << imNVoxZ << std::endl; //GDEB
#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(testVerb) ) {
    std::cout << "DicomDrawerROOT::DrawXY imgData  Min " << imMinX << " " << imMinY << " " << imMinZ << std::endl; 
    std::cout << "DicomDrawerROOT::DrawXY imgData  Max " << imMaxX << " " << imMaxY << " " << image->GetMaxZ() << std::endl;
    std::cout << "DicomDrawerROOT::DrawXY imgData  N " << imNVoxX << " " << imNVoxY << " " <<imNVoxZ << std::endl;
  }
#endif
  //  std::string hisName = DicomVImage::GetModalityStr( image->GetModality() ).substr(4,999)+"_XY";
  size_t copyNo = 0;
  for( size_t iz = 0; iz < imNVoxZ; iz++ ) {
    G4String zstr = GmGenUtils::ftoa(imMinZ+(iz+0.5)*imWidthZ);
    theImageFormat = DicomParameterMgr::GetInstance()->GetStringValue("imageFormat","jpg");
    std::string hisName = image->GetName()+"_XY"+extraFileName+"_"+zstr;
    TH2F* his = new TH2F(hisName.c_str(), hisName.c_str(), imNVoxX, imMinX, imMaxX, imNVoxY, imMinY, imMaxY );
    gStyle->SetOptTitle(bShowTitle);
    for( size_t iy = 1; iy <= imNVoxY; iy++ ) {
      for( size_t ix = 1; ix <= imNVoxX; ix++ ) {
	//	if( *imgData > -60 )
	//	G4cout << "XYDATA " << (ix-1)+(iy-1)*imNVoxX+iz*imNVoxX*imNVoxY << ": " << *imgData << " " << ix-1 << " " << iy-1 << " " << iz << " : " << imMinX+(ix-0.5)*image->GetVoxelDimX() << "  " << imMinY+(iy-0.5)*image->GetVoxelDimY() << "  " << imMinZ+(iz+0.5)*image->GetVoxelDimZ() << G4endl; //GDEB
	G4double data = *imgData;
	//	if( iz == 50 ) G4cout << "XYDATA " << (ix-1)+(iy-1)*imNVoxX+iz*imNVoxX*imNVoxY << " data= " << *imgData << " " << ix-1 << " " << iy-1 << " " << iz << " : " << imMinX+(ix-0.5)*image->GetVoxelDimX() << "  " << imMinY+(iy-0.5)*image->GetVoxelDimY() << "  " << imMinZ+(iz+0.5)*image->GetVoxelDimZ() << G4endl; //GDEB
	if( bAverageSurroundingBig ) {
	  data = GetValueSurrounding(ix-1,iy-1,iz, data, image, imgDatap);
	}
	if( data > thePaletteMin ) {
	  his->SetBinContent(ix,iy,data);
	} else {
	  his->SetBinContent(ix,iy,thePaletteMin);
	}
	//	G4cout << *imgData << " HISTOXY " << ix-1 << " " << iy-1 << " " << his->GetXaxis()->GetBinCenter(ix) << " " <<his->GetYaxis()->GetBinCenter(iy) << " " << his->GetBinContent(ix,iy) << " " <<  GmGenUtils::ftoa(imMinZ+(iz+0.5)*imWidthZ) << G4endl; //GDEB
#ifndef GAMOS_NO_VERBOSE
	if( DicomVerb(testVerb) ) {
	  if( data != 0 ) G4cout << " DrawerROOT XY " << ix-1 << " " << iy-1 << " " << iz << " : " << (ix-1)+(iy-1)*imNVoxX+iz*imNVoxX*imNVoxY << " = " << data << G4endl;
	  //	  if( *imgData != 0 ) G4cout << (ix-1)+(iy-1)*imNVoxX + iz*imNVoxX*imNVoxY << " DrawerROOT XY " << ix-1 << " " << iy-1 << " " << iz << " = " << *imgData << " =? " << imgDatap->at( (ix-1)+(iy-1)*imNVoxX + iz*imNVoxX*imNVoxY) << G4endl;
	  //	  if( *imgData != 0 ) G4cout << " DrawerROOT XY " << imMinX+(ix-1)*image->GetVoxelDimX() << " = " <<  his->GetXaxis()->GetXmin()+(ix-1)*his->GetXaxis()->GetBinWidth((ix-1)) << " " << his->GetXaxis()->GetBinCenter((ix-1)) << " " <<  his->GetXaxis()->GetXmin() << " " <<  his->GetXaxis()->GetXmax()<< " " << his->GetXaxis()->GetBinWidth((ix-1))<< " " << *imgData << " " << his->GetXaxis()->GetBinCenter(0)<< " " << his->GetXaxis()->GetBinCenter(1) << G4endl;
	}
#endif
	imgData++;
	copyNo++;
      }
      //  std::cout << iy << " imagData Y " << imgData-imgData0 << std::endl; //GDEB
    }
    //    std::cout << iz << " DRAWER imagData Z " << std::endl; //GDEB
    canvas->Draw();
    gStyle->SetOptStat(0);
    /*  Int_t myPalette[5];
  myPalette[0] = 1;
  myPalette[1] = 4;
  myPalette[2] = 6;
  myPalette[3] = 8;
  myPalette[4] = 12;
   gStyle->SetPalette(5,myPalette);
    */
    
    /*   const Int_t Number = 3;
   Double_t Red[Number]    = { 1.00, 0.00, 0.00};
   Double_t Green[Number]  = { 0.00, 1.00, 0.00};
   Double_t Blue[Number]   = { 1.00, 0.00, 1.00};
   Double_t Length[11] = { 0.00, 0.1,0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.90, 1.00 };
   Int_t nb=50;
   TColor::CreateGradientColorTable(Number,Length,Green,Blue,Red,nb);
   his->SetContour(nb);
   his->SetLineWidth(1);
   his->SetLineColor(kBlack);
   //   f2->Draw("surf1z");
   */
    //    G4cout << " thePaletteMin " << thePaletteMin << G4endl; //GDEB
    if( thePaletteMin != -DBL_MAX ) {
      his->SetMinimum(thePaletteMin);
      //    G4cout << " SETthePaletteMin " << thePaletteMin << G4endl; //GDEB
    }
    if( thePaletteMax != DBL_MAX ) his->SetMaximum(thePaletteMax);
    /*  UInt_t Number = 3;
  //  double Red[Number] = {1,0.,0};
  //  double Red[Number] = {1,0.,0};
  double Red[3] = { 1.00, 0.00, 0.00};
  Double_t Green[3]  = { 1.00, 0.20, 0.00};
  Double_t Blue[3]   = { 1.00, 0.40, .00};
  Double_t Length[3] = { 0.00, 0.50, 1.00 };
  Int_t nb=50;
  //  TColor::CreateGradientColorTable(Number,Length,Red,Green,Blue,nb);
  */
    
    his->Draw("colz");
    if( theOutputFile ) {
      theOutputFile->cd();
      his->Write();
      G4cout << " WRITE HISTO " << hisName << G4endl; //GDEB
    }
    
    if( bRotateXY180 ) {
      his->GetXaxis()->SetLabelOffset(9999);
      his->GetXaxis()->SetTickLength(0);
      TGaxis* newXaxis = new TGaxis(gPad->GetUxmax(), gPad->GetUymin(), gPad->GetUxmin(), gPad->GetUymin(),
				    his->GetXaxis()->GetXmin(), his->GetXaxis()->GetXmax(),
				    510,"-");
      newXaxis->SetLabelOffset(0.01);
      newXaxis->SetLabelSize(0.03);
      newXaxis->Draw();
      //      G4cout << " XAXIS " << gPad->GetUxmax() << " " << gPad->GetUymin() << " " << gPad->GetUxmin() << " " << gPad->GetUymin() << " " << his->GetXaxis()->GetXmin() << " " << his->GetXaxis()->GetXmax() << G4endl; //GDEB
      
      his->GetYaxis()->SetLabelOffset(9999);
      his->GetYaxis()->SetTickLength(0);
      double xmin = gPad->GetUxmin();
     //      double xmin = gPad->GetUxmin()-(gPad->GetUxmax()-gPad->GetUxmin)/20.;
      TGaxis* newYaxis = new TGaxis(xmin, gPad->GetUymax(), xmin, gPad->GetUymin(),
				    his->GetYaxis()->GetXmin(), his->GetYaxis()->GetXmax(),
				    510,"+");
      newYaxis->SetLabelOffset(0.01);
      newYaxis->SetLabelSize(0.03);
      newYaxis->SetTextFont(20);
      newYaxis->Draw();
      //      G4cout << " YAXIS " << gPad->GetUymax() << " " << gPad->GetUxmin() << " " << gPad->GetUymin() << " " << gPad->GetUxmin() << " " << his->GetYaxis()->GetXmin() << " " << his->GetYaxis()->GetXmax() << G4endl; //GDEB
    } 

    G4bool bVoxelsInUseClosest = DicomParameterMgr::GetInstance()->GetNumericValue("closestPolygon",1);
    //--- Draw lines
    for( size_t ils = 0; ils < lineSets.size(); ils++ ) {
      DicomVLineSet* lineSet = lineSets[ils];
      if( dynamic_cast<DicomPolygonSet*>(lineSet) != 0 ) {
	if( bVoxelsInUseClosest ) {
	  DicomPolygonSet* newPolySet = VoxelsInUseClosest((DicomPolygonSet*)lineSet, image);
	  lineSet = (DicomPolygonSet*)newPolySet;
	} else {
	  lineSet = lineSets[ils];
	}
      }
      if( bRotateXY180 && !lineSet->HasBeenRotatedXY180() ) {
	G4ThreeVector imgCentre = image->GetCentre();
	lineSet->RotateXY180( 180.*CLHEP::deg, imgCentre );
	lineSet->SetHasBeenRotatedXY180( true );
      } 
      //      G4cout << " DicomDrawerROOT::DrawXY DrawLinesXY " << imMinZ+iz*imWidthZ << G4endl; //GDEB
      DrawLinesXY( lineSet, imMinZ+iz*imWidthZ, imMinZ+(iz+1)*imWidthZ);
    }
    
    theImageFormat = DicomParameterMgr::GetInstance()->GetStringValue("imageFormat","jpg");
    std::string jpgName = "his"+hisName+"."+theImageFormat;
    canvas->Print(jpgName.c_str());

    delete his;
  }

}

//-----------------------------------------------------------------------------
void DicomDrawerROOT::DrawXZ( DicomVImage* image, std::vector<DicomVLineSet*> lineSets, G4String extraFileName )
{
  if( theRotateXY180Images.find(image) == theRotateXY180Images.end() ) {
    if( bRotateXY180 ) {
      std::vector<DicomVOperator*> operators = DicomMgr::GetInstance()->GetOperators();
      DicomOperRotateXY180* rotateOper = 0;
      for( size_t ii = 0; ii < operators.size(); ii++ ) {
	if( dynamic_cast<DicomOperRotateXY180*>(operators[ii]) != 0 ) {
	  rotateOper = dynamic_cast<DicomOperRotateXY180*>(operators[ii]);
	  break;
	}
      }
      rotateOper->Operate(image);
      theRotateXY180Images.insert(image);
    }
  }

  TCanvas* canvas = new TCanvas; 
  canvas->SetRightMargin(0.2);
  size_t imNVoxX = image->GetNoVoxelsX();
  size_t imNVoxY = image->GetNoVoxelsY();
  size_t imNVoxZ = image->GetNoVoxelsZ();
  size_t imNVoxXY = imNVoxX*imNVoxY;
  //  size_t imNVoxZ = image->GetNoVoxelsZ();
  G4double imMinX = image->GetMinX();
  G4double imMinY = image->GetMinY();
  G4double imMinZ = image->GetMinZ();
  G4double imMaxX = image->GetMaxX();
  G4double imMaxY = image->GetMaxY();
  G4double imMaxZ = image->GetMaxZ();
  G4double imWidthY = image->GetVoxelDimY();
  canvas->SetCanvasSize(theCanvasNPixelsX,theCanvasNPixelsZ*(imMaxZ-imMinZ)/(imMaxX-imMinX));
  
  if( DicomVerb(debugVerb) ) {
    std::cout << " DrawXZ DICOM IMAGE NAME " << image->GetName() << " " << DicomVImage::GetModalityStr( image->GetModality() )<< std::endl;
  }
  std::vector<G4double>* imgDatap = image->GetData();
  G4double* imgData = &(imgDatap->at(0));
  G4double* imgData0 = &(imgDatap->at(0)); //GDEB
  //  std::cout << " imagData 0 " << imNVoxXY*imNVoxZ << " " << imNVoxX << " " << imNVoxY << " " << imNVoxXY << " " << imNVoxZ << std::endl; //GDEB
  //  std::string hisName = DicomVImage::GetModalityStr( image->GetModality() ).substr(4,999)+"_XZ";
  //  std::string hisName = image->GetName()+"_XZ";

  for( size_t iy = 0; iy < imNVoxY; iy++ ) {
    G4String ystr;
    if( bRotateXY180 ) {
      ystr = GmGenUtils::ftoa(imMaxY-(iy+0.5)*imWidthY);
    } else {
      ystr = GmGenUtils::ftoa(imMinY+(iy+0.5)*imWidthY);
    }
    std::string hisName = image->GetName()+"_XZ"+extraFileName+"_"+ystr;
    imgData = imgData0 + iy*imNVoxX;
    TH2F* his = new TH2F(hisName.c_str(), hisName.c_str(), imNVoxX, imMinX, imMaxX, imNVoxZ, imMinZ, imMaxZ);
    gStyle->SetOptTitle(bShowTitle);
    for( size_t iz = 1; iz <= imNVoxZ; iz++ ) {
      for( size_t ix = 1; ix <= imNVoxX; ix++ ) {
	//	size_t copyNo = (ix-1) + iy*imNVoxX + (iz-1)*imNVoxXY;
	//	if( copyNo != imgData-imgData0 ) G4cout << " !!ERROR  " << ix-1 << " " << iy << " " << iz-1 << " " << copyNo << " imagData " << imgData-imgData0 << std::endl; //GDEB
	G4double data = *imgData;
	if( bAverageSurroundingBig ) {
	  data = GetValueSurrounding(ix-1,iy,iz-1, data, image, imgDatap);
	}
	if( data > thePaletteMin ) {
	  his->SetBinContent(ix,iz,data);
	} else {
	  his->SetBinContent(ix,iy,thePaletteMin);
	}
#ifndef GAMOS_NO_VERBOSE
	if( DicomVerb(testVerb) ) {
	//	if( data != 0 )	G4cout << "XZDATA " << (ix-1)+(iy)*imNVoxX+(iz-1)*imNVoxX*imNVoxY << ": " << data << " " << ix-1 << " " << iy << " " << iz-1 << " : " << imMinX+(ix-1.5)*image->GetVoxelDimX() << "  " << imMinY+(iy-0.5)*image->GetVoxelDimY() << "  " << imMinZ+(iz+1.5)*image->GetVoxelDimZ() << G4endl; //GDEB
	  if( data != 0 ) G4cout << " DrawerROOT XZ " << ix-1 << " " << iy << " " << iz-1 << " = " << data << G4endl;
	}
#endif
	imgData ++;
      }
      imgData += imNVoxXY - imNVoxX;
    }
    //  std::cout << iz << " imagData Z " << imgData-imgData0 << std::endl; //GDEB

    canvas->Draw();
    gStyle->SetOptStat(0);
    if( thePaletteMin != -DBL_MAX ) his->SetMinimum(thePaletteMin);
    if( thePaletteMax != DBL_MAX ) his->SetMaximum(thePaletteMax);
    his->Draw("colz");
    if( bRotateXY180 ) {
      his->GetXaxis()->SetLabelOffset(9999);
      his->GetXaxis()->SetTickLength(0);
      TGaxis* newXaxis = new TGaxis(gPad->GetUxmax(), gPad->GetUymin(), gPad->GetUxmin(), gPad->GetUymin(),
				    his->GetXaxis()->GetXmin(), his->GetXaxis()->GetXmax(),
				    510,"-");
      newXaxis->SetLabelOffset(0.01);
      newXaxis->SetLabelSize(0.03);
      newXaxis->Draw();

      his->GetYaxis()->SetLabelOffset(9999);
      his->GetYaxis()->SetTickLength(0);
      double xmin = gPad->GetUxmin();
     //      double xmin = gPad->GetUxmin()-(gPad->GetUxmax()-gPad->GetUxmin)/20.;
      TGaxis* newYaxis = new TGaxis(xmin, gPad->GetUymin(), xmin, gPad->GetUymax(),
				    his->GetYaxis()->GetXmin(), his->GetYaxis()->GetXmax(),
				    510,"-");
      newYaxis->SetLabelOffset(0.01);      
      newYaxis->SetLabelSize(0.03);
      newYaxis->SetTextFont(20);
      newYaxis->Draw();
      //      G4cout << " XAXIS " << gPad->GetUxmax() << " " << gPad->GetUymin() << " " << gPad->GetUxmin() << " " << gPad->GetUymin() << " " << his->GetXaxis()->GetXmin() << " " << his->GetXaxis()->GetXmax() << G4endl; //GDEB
    } 

    //--- Draw lines
    for( size_t ils = 0; ils < lineSets.size(); ils++ ) {
      if( bRotateXY180 && !lineSets[ils]->HasBeenRotatedXY180() ) {
	G4ThreeVector imgCentre = image->GetCentre();
	lineSets[ils]->RotateXY180( 180.*CLHEP::deg, imgCentre );
	lineSets[ils]->SetHasBeenRotatedXY180( true );
      }
      DrawLinesXZ( lineSets[ils], imMinY+iy*imWidthY, imMinY+(iy+1)*imWidthY);
    }
    
    std::string jpgName = "his"+hisName+"."+theImageFormat;
    canvas->Print(jpgName.c_str());
    if( theOutputFile ) {
      theOutputFile->cd();
      his->Write();
    }
    delete his;
  }

}

//-----------------------------------------------------------------------------
void DicomDrawerROOT::DrawYZ( DicomVImage* image, std::vector<DicomVLineSet*> lineSets, G4String extraFileName )
{
  if( theRotateXY180Images.find(image) == theRotateXY180Images.end() ) {
    if( bRotateXY180 ) {
      std::vector<DicomVOperator*> operators = DicomMgr::GetInstance()->GetOperators();
      DicomOperRotateXY180* rotateOper = 0;
      for( size_t ii = 0; ii < operators.size(); ii++ ) {
	if( dynamic_cast<DicomOperRotateXY180*>(operators[ii]) != 0 ) {
	  rotateOper = dynamic_cast<DicomOperRotateXY180*>(operators[ii]);
	  break;
	}
      }
      rotateOper->Operate(image);
      theRotateXY180Images.insert(image);
    }
  }

  TCanvas* canvas = new TCanvas; 
  canvas->SetRightMargin(0.2);
  
  size_t imNVoxX = image->GetNoVoxelsX();
  size_t imNVoxY = image->GetNoVoxelsY();
  size_t imNVoxZ = image->GetNoVoxelsZ();
  //  size_t imNVoxXY = imNVoxX*imNVoxY;
  //  size_t imNVoxZ = image->GetNoVoxelsZ();
  G4double imMinX = image->GetMinX();
  G4double imMinY = image->GetMinY();
  G4double imMinZ = image->GetMinZ();
  G4double imMaxX = image->GetMaxX();
  G4double imMaxY = image->GetMaxY();
  G4double imMaxZ = image->GetMaxZ();
  G4double imWidthX = image->GetVoxelDimX();
  canvas->SetCanvasSize(theCanvasNPixelsY,theCanvasNPixelsZ*(imMaxZ-imMinZ)/(imMaxY-imMinY));
  
  if( DicomVerb(debugVerb) ) {
    std::cout << "DrawYZ DICOM IMAGE NAME " << image->GetName() << " " << DicomVImage::GetModalityStr( image->GetModality() )<< std::endl;
  }
  std::vector<G4double>* imgDatap = image->GetData();
  G4double* imgData = &(imgDatap->at(0));
  G4double* imgData0 = &(imgDatap->at(0)); //GDEB
  //  std::cout << " imagData 0 " << imNVoxXY*imNVoxZ << " " << imNVoxX << " " << imNVoxY << " " << imNVoxXY << " " << imNVoxZ << std::endl; //GDEB
  //  std::string hisName = DicomVImage::GetModalityStr( image->GetModality() ).substr(4,999)+"_YZ";
  for( size_t ix = 0; ix < imNVoxX; ix++ ) {
    imgData = imgData0 + ix;
    G4String xstr; 
    if( bRotateXY180 ) {
      xstr = GmGenUtils::ftoa(imMaxX-(ix+0.5)*imWidthX);
    } else {
      xstr = GmGenUtils::ftoa(imMinX+(ix+0.5)*imWidthX);
    }      
    std::string hisName = image->GetName()+"_YZ"+extraFileName+"_"+xstr;
    TH2F* his = new TH2F(hisName.c_str(), hisName.c_str(), imNVoxY, imMinY, imMaxY, imNVoxZ, imMinZ, imMaxZ );
    gStyle->SetOptTitle(bShowTitle);

    for( size_t iz = 1; iz <= imNVoxZ; iz++ ) {
      for( size_t iy = 1; iy <= imNVoxY; iy++ ) {
	G4double data = *imgData;
	if( bAverageSurroundingBig ) {
	  data = GetValueSurrounding(ix,iy-1,iz-1, data, image, imgDatap);
	}
	if( data > thePaletteMin ) {
	  his->SetBinContent(iy,iz,data);
	} else {
	  his->SetBinContent(ix,iy,thePaletteMin);
	}
	//	size_t copyNo = ix + (iy-1)*imNVoxX + (iz-1)*imNVoxXY;
	//	if( copyNo != imgData-imgData0 ) G4cout << " !!ERROR  " << ix << " " << iy-1 << " " << iz-1 << " " << copyNo << " imagData " << imgData-imgData0 << std::endl; //GDEB
#ifndef GAMOS_NO_VERBOSE
	if( DicomVerb(testVerb) ) {
	  if( data != 0 ) G4cout << " DrawerROOT YZ " << ix << " " << iy-1 << " " << iz-1 << " = " << data << G4endl;
	}
#endif
	imgData += imNVoxX;
      }
      //      imgData += imNVoxXY - ;
      //  std::cout << iy << " imagData Y " << imgData-imgData0 << std::endl; //GDEB
    }
    //  std::cout << iz << " imagData Z " << imgData-imgData0 << std::endl; //GDEB

    canvas->Draw();
    gStyle->SetOptStat(0);
    if( thePaletteMin != -DBL_MAX ) his->SetMinimum(thePaletteMin);
    if( thePaletteMax != DBL_MAX ) his->SetMaximum(thePaletteMax);
    his->Draw("colz");

    if( bRotateXY180 ) {
      his->GetXaxis()->SetLabelOffset(9999);
      his->GetXaxis()->SetTickLength(0);
      double ymin = gPad->GetUymin()*1.;
      TGaxis* newXaxis = new TGaxis(gPad->GetUxmax(), ymin, gPad->GetUxmin(), ymin,
				    his->GetXaxis()->GetXmin(), his->GetXaxis()->GetXmax(),
				    510,"-");
      //      G4cout << "XZ XAXIS " << gPad->GetUxmax() << " " << gPad->GetUymin() << " " << gPad->GetUxmax() << " " << gPad->GetUymin() << " " << his->GetXaxis()->GetXmin() << " " << his->GetXaxis()->GetXmax() << G4endl; //GDEB
      newXaxis->SetLabelOffset(0.01);
      newXaxis->SetLabelSize(0.03);
      newXaxis->Draw();
 
      his->GetYaxis()->SetLabelOffset(9999);
      his->GetYaxis()->SetTickLength(0);
      double xmin = gPad->GetUxmin();
     //      double xmin = gPad->GetUxmin()-(gPad->GetUxmax()-gPad->GetUxmin)/20.;
      TGaxis* newYaxis = new TGaxis(xmin, gPad->GetUymin(), xmin, gPad->GetUymax(),
				    his->GetYaxis()->GetXmin(), his->GetYaxis()->GetXmax(),
				    510,"-");
      newYaxis->SetLabelOffset(+0.01);      
      newYaxis->SetLabelSize(0.03);
      newYaxis->SetTextFont(20);
      newYaxis->Draw();

      //      G4cout << " YAXIS " << gPad->GetUymax() << " " << gPad->GetUxmin() << " " << gPad->GetUymin() << " " << gPad->GetUxmin() << " " << his->GetYaxis()->GetXmin() << " " << his->GetYaxis()->GetXmax() << G4endl; //GDEB
    } 

    //--- Draw lines
    for( size_t ils = 0; ils < lineSets.size(); ils++ ) {
      if( bRotateXY180 && !lineSets[ils]->HasBeenRotatedXY180() ) {
	G4ThreeVector imgCentre = image->GetCentre();
	lineSets[ils]->RotateXY180( 180.*CLHEP::deg, imgCentre );
	lineSets[ils]->SetHasBeenRotatedXY180( true );
      }
      DrawLinesYZ( lineSets[ils], imMinX+ix*imWidthX, imMinX+(ix+1)*imWidthX);
    }
    
    std::string jpgName = "his"+hisName+"."+theImageFormat;
    canvas->Print(jpgName.c_str());
    if( theOutputFile ) {
      theOutputFile->cd();
      his->Write();
    }
    delete his;
  }

}

//-----------------------------------------------------------------------------
void DicomDrawerROOT::DrawLinesXY( DicomVLineSet* lineSet, G4double minZ, G4double maxZ )
{
  DrawLines( lineSet, minZ, maxZ, 0, 1, DPOrientXY);
}

//-----------------------------------------------------------------------------
void DicomDrawerROOT::DrawLinesXZ( DicomVLineSet* lineSet, G4double minY, G4double maxY )
{
  DrawLines( lineSet, minY, maxY, 0, 2, DPOrientXZ);
}

//-----------------------------------------------------------------------------
void DicomDrawerROOT::DrawLinesYZ( DicomVLineSet* lineSet, G4double minX, G4double maxX )
{
  DrawLines( lineSet, minX, maxX, 1, 2, DPOrientYZ);
}

#include "TMarker.h"
//-----------------------------------------------------------------------------
void DicomDrawerROOT::DrawLines( DicomVLineSet* lineSet, G4double minPos, G4double maxPos, size_t ix, size_t iy, DPOrientation orient)
{
  //  G4cout << " DicomDrawerROOT::DrawLines lineSet " << lineSet << " " << DicomVLine::GetOrientationName(lineSet->GetOrientation()) << " =? " << DicomVLine::GetOrientationName(orient) << G4endl; //GDEB

  if( lineSet->GetOrientation() != orient ) return;
  std::vector<DicomVLineList*> lineLists = lineSet->GetLineLists();
  for( size_t ill = 0; ill < lineLists.size(); ill++ ) {
    if( DicomVerb(testVerb) ) G4cout << " DicomDrawerROOT::DrawLines " << lineLists[ill]->GetName() << G4endl;
    std::vector<DicomVLine*> lines = lineLists[ill]->GetLines();
    //    G4cout << " N LINES " <<  lines.size()<< G4endl;  //GDEB
    for( size_t il = 0; il < lines.size(); il++ ) {
      G4double planePos = lines[il]->GetPlanePosition();
      if( DicomVerb(testVerb) ) G4cout << " DicomDrawerROOT::DrawLines ?? LINE " << lineLists[ill]->GetName() << " ZPOS " << minPos <<  " <? " << planePos << " <? " << maxPos << G4endl;
      if( planePos >= minPos && planePos <= maxPos ) {
	if( DicomVerb(testVerb) ) G4cout << " DicomDrawerROOT::DrawLines LINE OK " << lineLists[ill]->GetName() << " ZPOS " << planePos << G4endl; 
	std::vector<G4ThreeVector> points = lines[il]->GetPoints();
	G4int ns4 = points.size()/4;
	G4int ntxt = G4int(CLHEP::RandFlat::shoot()*ns4); // point where to draw the text
	for( size_t ip = 1; ip < points.size(); ip++ ) {
	  //  TLine* line = new TLine(points[ip-1].x(),points[ip-1].y(),points[ip].x(),points[ip].y());
	  /*	  TMarker * mark = new TMarker(points[ip-1](ix),points[ip-1](iy),20); //GDEB
	  mark->SetMarkerSize(0.25);
	  mark->Draw();
	  //   continue; //GDEB */
	  TLine* line = new TLine(points[ip-1](ix),points[ip-1](iy),points[ip](ix),points[ip](iy));
	  G4bool bDrawText = false;
	  //	  if( ns4 != 0 ) { 
	  bDrawText = (ns4 == 0 && ip == 1);
	  if( ns4 != 0 ) bDrawText = bDrawText || (G4int(ip%ns4) == ntxt);
	  //	  }
	  DrawLineColorAndStyle( line, lines[il]->GetName(), bDrawText );
	  if( DicomVerb(testVerb) ) G4cout << " DicomDrawerROOT " << il << " " << ip << " DRAW LINE " << points[ip-1] << " " << points[ip] << G4endl;
	}
      }
    }
  }

}

//-----------------------------------------------------------------------------
void DicomDrawerROOT::DrawLineColorAndStyle( TLine* line, G4String name, G4bool bDrawText )
{
  if( DicomVerb(debugVerb) ) G4cout << " DicomDrawerROOT::DrawLineColorAndStyle(  LINE " << name << " bDrawText " << bDrawText << G4endl; //GDEB
  // set line colors
  // if( !bLineDrawingOptions = false ) return; // take default options
  DicomLineDrawingOptions* ldo; 
 
  msldo::const_iterator ite;
  for( ite = theLineDrawingOptions.begin(); ite != theLineDrawingOptions.end(); ite++ ) {
    if( GmGenUtils::AreWordsEquivalent( ite->first, name ) ) {
      break;
    }
  }
  if( ite != theLineDrawingOptions.end() ) {
    ldo = (*ite).second;
    //  } else if( !bLineDrawingOptions ) {
    // ldo = new DicomLineDrawingOptions;// default options
    // G4cout << " LINE NOT FOUND " << name << " NEW " << ldo->GetOption() << " " << ldo->GetTColor() << " " << ldo->GetWidth() << G4endl; //GDEB
  } else {
    ldo = new DicomLineDrawingOptions;// default options
// return;
  }

  //  G4cout << " DRAW LINE " << name << " : " << ldo->GetOption() << " " << ldo->GetStyle() << " " << ldo->GetTColor() << " " << ldo->GetWidth() << " bText " << bDrawText << G4endl; //GDEB

  if( ldo->GetOption() >= 1 ) {
    line->SetLineColor(ldo->GetTColor());
    line->SetLineStyle(ldo->GetStyle());
    line->SetLineWidth(ldo->GetWidth());
    line->Draw();
  }
  //  G4cout << " DRAW LINE " << name <<" "<< line->GetX1() << ", " << line->GetY1() << G4endl; //GDEB

  if( bDrawText && ldo->GetOption() == 2 ) {
    TText* text = new TText( line->GetX1(), line->GetY1(), name.c_str());
    double angle = atan2(line->GetY2()-line->GetY1(), line->GetX2()-line->GetX1() )*180./CLHEP::pi;
    text->SetTextAngle( angle );
    text->SetTextColor( ldo->GetTColor() );
    text->SetTextSize( ldo->GetTextSize() );
    //    G4cout << " DRAW TEXT " << name << " AT " <<  line->GetX1() << " " <<  line->GetY1() << G4endl; //GDEB
    text->Draw();
  }

  //  delete line;
 
}

//-----------------------------------------------------------------------------
G4double DicomDrawerROOT::GetValueSurrounding(size_t ix, size_t iy, size_t iz, G4double value, DicomVImage* image, std::vector<G4double>* imgDatap ) 
{
  G4double valueAve = 0.;
  G4int nVals = 0;
  if( ix > 0 ) {
    nVals++;
    valueAve += imgDatap->at(ix-1+iy*image->GetNoVoxelsX()+iz*image->GetNoVoxelsXY());
  }
  if( iy > 0 ) {
    nVals++;
    valueAve += imgDatap->at(ix+(iy-1)*image->GetNoVoxelsX()+iz*image->GetNoVoxelsXY());
  }
  if( iz > 0 ) {
    nVals++;
    valueAve += imgDatap->at(ix+iy*image->GetNoVoxelsX()+(iz-1)*image->GetNoVoxelsXY());
  }
  if( ix < image->GetNoVoxelsX()-1 ) {
    nVals++;
    valueAve += imgDatap->at(ix+1+iy*image->GetNoVoxelsX()+iz*image->GetNoVoxelsXY());
  }
  if( iy < image->GetNoVoxelsY()-1 ) {
    nVals++;
    valueAve += imgDatap->at(ix+(iy+1)*image->GetNoVoxelsX()+iz*image->GetNoVoxelsXY());
  }
  if( iz < image->GetNoVoxelsZ()-1 ) {
    nVals++;
    valueAve += imgDatap->at(ix+iy*image->GetNoVoxelsX()+(iz+1)*image->GetNoVoxelsXY());
  }

  if( value > valueAve*theOverSurrounding ) {
    value = valueAve*theOverSurrounding;
  }
  return value;
}

//-----------------------------------------------------------------------------
void DicomDrawerROOT::DrawWithLinesXY( DicomVImage*, DicomVLineList* ){}

//-----------------------------------------------------------------------------
void DicomDrawerROOT::DrawWithLinesXZ( DicomVImage*, DicomVLineList* ){}

//-----------------------------------------------------------------------------
void DicomDrawerROOT::DrawWithLinesYZ( DicomVImage*, DicomVLineList* ){}

//-----------------------------------------------------------------------------
void DicomDrawerROOT::DrawWithLinesXY( DicomVImage*, DicomVLine* ){}

//-----------------------------------------------------------------------------
void DicomDrawerROOT::DrawWithLinesXZ( DicomVImage*, DicomVLine* ){}

//-----------------------------------------------------------------------------
void DicomDrawerROOT::DrawWithLinesYZ( DicomVImage*, DicomVLine* ){}

#endif
