#ifdef GAMOS_ROOT

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
#include "DicomOperReflectY.hh"
#include "DicomOperRotateXY180.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "CLHEP/Random/RandFlat.h"

#include "TCanvas.h"
#include "TH1F.h"
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

  bFigureTitle = bool(DicomParameterMgr::GetInstance()->GetNumericValue("bFigureTitle",bFigureTitle));
/*  G4bool drawLog = G4bool(DicomParameterMgr::GetInstance()->GetNumericValue("drawLog",0));
  if( drawLog ) {
    theOperatorLog = new DicomOperLog();
  } else {
    theOperatorLog = 0;
    }*/
  theParamMgr = (DicomParameterMgr*)(DicomParameterMgr::GetInstance());
  if( DicomMgr::GetInstance()->GetReflectY() ) {
    bReflectY = true;
  } else {
    bReflectY = false;
  }
  if( DicomMgr::GetInstance()->GetRotateXY180() ) {
    bRotateXY180 = true;
  } else {
    bRotateXY180 = false;
  }

  theOutputFile = 0;

  bDrawHisto1 = false;
}

//-----------------------------------------------------------------------------
void DicomDrawerROOT::SetOutputFile( G4String foutName )
{
  theOutputFile = new TFile(foutName.c_str(),"RECREATE");
}

//-----------------------------------------------------------------------------
void DicomDrawerROOT::CloseOutputFile() 
{
  if( theOutputFile ) theOutputFile->Close();  
}

//-----------------------------------------------------------------------------
void DicomDrawerROOT::DrawXY( DicomVImage* image, std::vector<DicomVLineSet*> lineSets, G4String extraFileName, G4bool bNonZero )
{
  if( theReflectYImages.find(image) == theReflectYImages.end() ) {
    if( bReflectY ) {
      std::vector<DicomVOperator*> operators = DicomMgr::GetInstance()->GetOperators();
      DicomOperReflectY* rotateOper = 0;
      for( size_t ii = 0; ii < operators.size(); ii++ ) {
	if( dynamic_cast<DicomOperReflectY*>(operators[ii]) != 0 ) {
	  rotateOper = dynamic_cast<DicomOperReflectY*>(operators[ii]);
	  break;
	}
      }
      //G4cout << image << " SIZE DicomDrawerROOT::DrawXY " << image->GetName() << " " << image->GetData()->size() << G4endl; //GDEB
      rotateOper->Operate(image);
      theReflectYImages.insert(image);
    }
  }
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
  gPad->SetLeftMargin(0.2);
  gPad->SetBottomMargin(0.2);
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
    G4cout << " DrawXY IMAGE NAME " << image->GetName() << " " << DicomVImage::GetModalityStr( image->GetModality() )<< std::endl;
  }
  std::vector<G4double>* imgDatap = image->GetData();
  G4double* imgData = &(imgDatap->at(0));

  //  G4cout << " imagData 0 " << imNVoxXY*imNVoxZ << " " << imNVoxX << " " << imNVoxY << " " << imNVoxXY << " " << imNVoxZ << std::endl; //GDEB
#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(testVerb) ) {
    G4cout << "DicomDrawerROOT::DrawXY imgData  Min " << imMinX << " " << imMinY << " " << imMinZ << std::endl; 
    G4cout << "DicomDrawerROOT::DrawXY imgData  Max " << imMaxX << " " << imMaxY << " " << image->GetMaxZ() << std::endl;
    G4cout << "DicomDrawerROOT::DrawXY imgData  N " << imNVoxX << " " << imNVoxY << " " <<imNVoxZ << std::endl;
  }
#endif
  
  //  std::string hisName = DicomVImage::GetModalityStr( image->GetModality() ).substr(4,999)+"_XY";
  std::string hisName1All = GmGenUtils::ReplaceChar(image->GetName()+"_XY"+extraFileName,'/',':');
  if(hisName1All.find("_2D") != std::string::npos ) {
    hisName1All.replace(hisName1All.find("_2D"), 3, "_1D.ALL");
  }
  TH1F* his1All = 0;
  if ( bDrawHisto1 ) his1All = new TH1F(hisName1All.c_str(), hisName1All.c_str(), 100, 0., thePaletteMax );
  size_t copyNo = 0;
  theImageFormat = DicomParameterMgr::GetInstance()->GetStringValue("imageFormat","jpg");
  for( size_t iz = 0; iz < imNVoxZ; iz++ ) {
    G4String zstr = GmGenUtils::ftoa(imMinZ+(iz+0.5)*imWidthZ);
    std::string hisName2 = GmGenUtils::ReplaceChar(image->GetName()+"_XY"+extraFileName+"_"+zstr,'/',':');
    //    G4cout << " hisName2 " << hisName2 <<" = "<< image->GetName()<<"+_XY+"<<extraFileName<<"_+"<<zstr << G4endl; //GDEB
    G4String hisName1 = hisName2;
    if( hisName1.find("_2D") != std::string::npos ) {
      hisName1.replace(hisName1.find("_2D"), 3, "_1D");
    }
    TH1F* his1 = 0; 
    if ( bDrawHisto1 ) his1 = new TH1F(hisName1.c_str(), hisName1.c_str(), 100, 0., thePaletteMax );
    TH2F* his2 = new TH2F(hisName2.c_str(), hisName2.c_str(), imNVoxX, imMinX, imMaxX, imNVoxY, imMinY, imMaxY );
    //    G4cout << " CREATE his2 " << his2->GetName() << " " << hisName2 << G4endl; //GDEB
    gStyle->SetOptTitle(bFigureTitle);
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
	  his2->SetBinContent(ix,iy,data);
	} else {
	  his2->SetBinContent(ix,iy,thePaletteMin);
	}
	if ( bDrawHisto1 ) {
	  if( !bNonZero || data != 0. ) his1->Fill(data);
	  if (data != 0. ) his1All->Fill(data);
	}
	//	G4cout << *imgData << " HISTOXY " << ix-1 << " " << iy-1 << " " << his2->GetXaxis()->GetBinCenter(ix) << " " <<his2->GetYaxis()->GetBinCenter(iy) << " " << his2->GetBinContent(ix,iy) << " " <<  GmGenUtils::ftoa(imMinZ+(iz+0.5)*imWidthZ) << G4endl; //GDEB
	/*#ifndef GAMOS_NO_VERBOSE
	if( DicomVerb(testVerb) ) {
	  if( data != 0 ) G4cout << " DrawerROOT XY " << ix-1 << " " << iy-1 << " " << iz << " : " << (ix-1)+(iy-1)*imNVoxX+iz*imNVoxX*imNVoxY << " = " << data << G4endl;
	  //	  if( *imgData != 0 ) G4cout << (ix-1)+(iy-1)*imNVoxX + iz*imNVoxX*imNVoxY << " DrawerROOT XY " << ix-1 << " " << iy-1 << " " << iz << " = " << *imgData << " =? " << imgDatap->at( (ix-1)+(iy-1)*imNVoxX + iz*imNVoxX*imNVoxY) << G4endl;
	  //	  if( *imgData != 0 ) G4cout << " DrawerROOT XY " << imMinX+(ix-1)*image->GetVoxelDimX() << " = " <<  his2->GetXaxis()->GetXmin()+(ix-1)*his2->GetXaxis()->GetBinWidth((ix-1)) << " " << his2->GetXaxis()->GetBinCenter((ix-1)) << " " <<  his2->GetXaxis()->GetXmin() << " " <<  his2->GetXaxis()->GetXmax()<< " " << his2->GetXaxis()->GetBinWidth((ix-1))<< " " << *imgData << " " << his2->GetXaxis()->GetBinCenter(0)<< " " << his2->GetXaxis()->GetBinCenter(1) << G4endl;
	}
	#endif*/
	imgData++;
	copyNo++;
      }
      //  G4cout << iy << " imagData Y " << imgData-imgData0 << std::endl; //GDEB
    }
    //    G4cout << iz << " DRAWER imagData Z " << std::endl; //GDEB
    canvas->Draw();
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
   his2->SetContour(nb);
   his2->SetLineWidth(1);
   his2->SetLineColor(kBlack);
   //   f2->Draw("surf1z");
   */
    //    G4cout << " thePaletteMin " << thePaletteMin << G4endl; //GDEB
    if( thePaletteMin != -DBL_MAX ) {
      his2->SetMinimum(thePaletteMin);
      //    G4cout << " SETthePaletteMin " << thePaletteMin << G4endl; //GDEB
    }
    if( thePaletteMax != DBL_MAX ) his2->SetMaximum(thePaletteMax);
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
    
    gStyle->SetOptStat(0);
    his2->Draw("colz");
    gPad->Update();
    if( theOutputFile ) {
      theOutputFile->cd();
      his2->Write();
    }
    
    if( bReflectY ) {
      his2->GetXaxis()->SetLabelOffset(9999);
      his2->GetYaxis()->SetLabelOffset(9999);
      //      his2->GetYaxis()->SetTickLength(0);
      double xmin = gPad->GetUxmin();
     //      double xmin = gPad->GetUxmin()-(gPad->GetUxmax()-gPad->GetUxmin)/20.;
      TGaxis* newYaxis = new TGaxis(xmin, gPad->GetUymax(), xmin, gPad->GetUymin(),
				    his2->GetYaxis()->GetXmin(), his2->GetYaxis()->GetXmax(),
				    510,"+");
      newYaxis->SetLabelOffset(0.01);
      newYaxis->SetLabelSize(0.03);
      newYaxis->SetTextFont(20);
      newYaxis->Draw();
      //      G4cout << " YAXIS " << gPad->GetUymax() << " " << gPad->GetUxmin() << " " << gPad->GetUymin() << " " << gPad->GetUxmin() << " " << his2->GetYaxis()->GetXmin() << " " << his2->GetYaxis()->GetXmax() << G4endl; //GDEB
    }
  
    if( bRotateXY180 ) {
      his2->GetXaxis()->SetLabelOffset(9999);
      //      his2->GetXaxis()->SetTickLength(0);
      TGaxis* newXaxis = new TGaxis(gPad->GetUxmax(), gPad->GetUymin(), gPad->GetUxmin(), gPad->GetUymin(),
				    his2->GetXaxis()->GetXmin(), his2->GetXaxis()->GetXmax(),
				    510,"-");
      newXaxis->SetLabelOffset(0.01);
      newXaxis->SetLabelSize(0.03);
      newXaxis->Draw();
      //      G4cout << " XAXIS " << gPad->GetUxmax() << " " << gPad->GetUymin() << " " << gPad->GetUxmin() << " " << gPad->GetUymin() << " " << his2->GetXaxis()->GetXmin() << " " << his2->GetXaxis()->GetXmax() << G4endl; //GDEB
      
      his2->GetYaxis()->SetLabelOffset(9999);
      //      his2->GetYaxis()->SetTickLength(0);
      double xmin = gPad->GetUxmin();
     //      double xmin = gPad->GetUxmin()-(gPad->GetUxmax()-gPad->GetUxmin)/20.;
      TGaxis* newYaxis = new TGaxis(xmin, gPad->GetUymax(), xmin, gPad->GetUymin(),
				    his2->GetYaxis()->GetXmin(), his2->GetYaxis()->GetXmax(),
				    510,"+");
      newYaxis->SetLabelOffset(0.01);
      newYaxis->SetLabelSize(0.03);
      newYaxis->SetTextFont(20);
      newYaxis->Draw();
      //      G4cout << " YAXIS " << gPad->GetUymax() << " " << gPad->GetUxmin() << " " << gPad->GetUymin() << " " << gPad->GetUxmin() << " " << his2->GetYaxis()->GetXmin() << " " << his2->GetYaxis()->GetXmax() << G4endl; //GDEB
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
      if( bReflectY && !lineSet->HasBeenReflectedY() ) {
	G4ThreeVector imgCentre = image->GetCentre();
	lineSet->ReflectY( imgCentre );
	lineSet->SetHasBeenReflectedY( true );
      } 
      if( bRotateXY180 && !lineSet->HasBeenRotatedXY180() ) {
	G4ThreeVector imgCentre = image->GetCentre();
	lineSet->RotateXY180( 180.*CLHEP::deg, imgCentre );
	lineSet->SetHasBeenRotatedXY180( true );
      } 
    /* GDEB 
      G4cout << " DCMDrawDICOM::DrawImage( DRAW DrawPOLYGON SET " << lineSets[ils]->GetName() << " Nlists " <<  lineSets[ils]->GetLineLists().size() << G4endl; //GDEB
      for( size_t ii = 0; ii <  lineSet->GetLineLists().size(); ii++ ) {
	DicomVLineList* lineList =  lineSet->GetLineLists()[ii];
	G4cout << ii << " DrawPOLYGON LINELIST " << lineList->GetName() << " Nlines "<< lineList->GetLines().size() << G4endl; 

	for( size_t jj = 0; jj < lineList->GetLines().size(); jj++ ) {
	  DicomVLine* line = lineList->GetLines()[jj];
	  G4cout << jj << " DrawPOLYGON LINE " << line->GetName() << " Nlines "<< line->GetPoints().size() << G4endl;
	  for( size_t ipt = 0; ipt < line->GetPoints().size(); ipt++ ) {
	    G4cout << line << " " << ipt << " DrawPOLYGON POINT " << line->GetPoints()[ipt] << G4endl;
	    }
	}
	}*/

      
      if( DicomVerb(debugVerb) ) {
	G4cout << " DicomDrawerROOT::DrawXY DrawLinesXY " << lineSet->GetName() << " " << imMinZ+iz*imWidthZ << G4endl;
      }
      DrawLinesXY( lineSet, imMinZ+iz*imWidthZ, imMinZ+(iz+1)*imWidthZ);
    }
    
    std::string figName = "his"+G4String(his2->GetName())+"."+theImageFormat;
    canvas->Print(figName.c_str());
    delete his2;
    
    if ( bDrawHisto1 ) {
      if( theOutputFile ) {
	theOutputFile->cd();
	his1->Write();
      }
      gStyle->SetOptStat(111111);
      his1->Draw("");
      figName = "his"+G4String(his1->GetName())+"."+theImageFormat;
      canvas->Print(figName.c_str());
      delete his1;
    }

  }

  if ( bDrawHisto1 ) {
    if( theOutputFile ) {
      theOutputFile->cd();
      his1All->Write();
    }
    //    G4cout << " WRITE HISTO his1All " << his1All->GetName() << G4endl; //GDEB
    his1All->Draw("");
    std::string figName = "his"+G4String(his1All->GetName())+"."+theImageFormat;
    canvas->Print(figName.c_str());
    delete his1All;
  }
  
}

//-----------------------------------------------------------------------------
void DicomDrawerROOT::DrawXZ( DicomVImage* image, std::vector<DicomVLineSet*> lineSets, G4String extraFileName, G4bool bNonZero )
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
    G4cout << " DrawXZ DICOM IMAGE NAME " << image->GetName() << " " << DicomVImage::GetModalityStr( image->GetModality() )<< std::endl;
  }
  std::vector<G4double>* imgDatap = image->GetData();
  G4double* imgData = &(imgDatap->at(0));
  G4double* imgData0 = &(imgDatap->at(0)); 
  //  G4cout << " imagData 0 " << imNVoxXY*imNVoxZ << " " << imNVoxX << " " << imNVoxY << " " << imNVoxXY << " " << imNVoxZ << std::endl; //GDEB
  //  std::string hisName2 = DicomVImage::GetModalityStr( image->GetModality() ).substr(4,999)+"_XZ";
  //  std::string hisName2 = image->GetName()+"_XZ";
  std::string hisName1All = GmGenUtils::ReplaceChar(image->GetName()+"_XZ"+extraFileName,'/',':');
  if(hisName1All.find("_2D") != std::string::npos ) {
    hisName1All.replace(hisName1All.find("_2D"), 3, "_1D.ALL");
  }
  TH1F* his1All = 0;
  if ( bDrawHisto1 ) his1All = new TH1F(hisName1All.c_str(), hisName1All.c_str(), 100, 0., thePaletteMax );
  
  theImageFormat = DicomParameterMgr::GetInstance()->GetStringValue("imageFormat","jpg");
  for( size_t iy = 0; iy < imNVoxY; iy++ ) {
    G4String ystr;

    ystr = GmGenUtils::ftoa(imMinY+(iy+0.5)*imWidthY);
    if( bRotateXY180 ) {
      ystr = GmGenUtils::ftoa(imMaxY-(iy+0.5)*imWidthY);
    } else {
      ystr = GmGenUtils::ftoa(imMinY+(iy+0.5)*imWidthY);
    }
    std::string hisName2 = GmGenUtils::ReplaceChar(image->GetName()+"_XZ"+extraFileName+"_"+ystr,'/',':');
    TH2F* his2 = new TH2F(hisName2.c_str(), hisName2.c_str(), imNVoxX, imMinX, imMaxX, imNVoxZ, imMinZ, imMaxZ );
    G4String hisName1 = hisName2;
    if( hisName1.find("_2D") != std::string::npos ) {
      hisName1.replace(hisName1.find("_2D"), 3, "_1D");
    }
    TH1F* his1 = 0; 
    if ( bDrawHisto1 ) his1 = new TH1F(hisName1.c_str(), hisName1.c_str(), 100, 0., thePaletteMax );
    imgData = imgData0 + iy*imNVoxX;
    gStyle->SetOptTitle(bFigureTitle);
    
    for( size_t iz = 1; iz <= imNVoxZ; iz++ ) {
      for( size_t ix = 1; ix <= imNVoxX; ix++ ) {
	//	size_t copyNo = (ix-1) + iy*imNVoxX + (iz-1)*imNVoxXY;
	//	if( copyNo != imgData-imgData0 ) G4cout << " !!ERROR  " << ix-1 << " " << iy << " " << iz-1 << " " << copyNo << " imagData " << imgData-imgData0 << std::endl; //GDEB
	G4double data = *imgData;
	if( bAverageSurroundingBig ) {
	  data = GetValueSurrounding(ix-1,iy,iz-1, data, image, imgDatap);
	}
	if( data > thePaletteMin ) {
	  his2->SetBinContent(ix,iz,data);
	} else {
	  his2->SetBinContent(ix,iy,thePaletteMin);
	}
	if ( bDrawHisto1 ) {
	  if( !bNonZero || data != 0. ) his1->Fill(data);
	  if (data != 0. ) his1All->Fill(data);
	}
	/*#ifndef GAMOS_NO_VERBOSE
	if( DicomVerb(testVerb) ) {
	//	if( data != 0 )	G4cout << "XZDATA " << (ix-1)+(iy)*imNVoxX+(iz-1)*imNVoxX*imNVoxY << ": " << data << " " << ix-1 << " " << iy << " " << iz-1 << " : " << imMinX+(ix-1.5)*image->GetVoxelDimX() << "  " << imMinY+(iy-0.5)*image->GetVoxelDimY() << "  " << imMinZ+(iz+1.5)*image->GetVoxelDimZ() << G4endl; //GDEB
	  if( data != 0 ) G4cout << " DrawerROOT XZ " << ix-1 << " " << iy << " " << iz-1 << " = " << data << G4endl;
	}
	#endif */
	imgData ++;
      }
      imgData += imNVoxXY - imNVoxX;
    }
    //  G4cout << iz << " imagData Z " << imgData-imgData0 << std::endl; //GDEB

    canvas->Draw();
    if( thePaletteMin != -DBL_MAX ) his2->SetMinimum(thePaletteMin);
    if( thePaletteMax != DBL_MAX ) his2->SetMaximum(thePaletteMax);
    gStyle->SetOptStat(0);
    his2->Draw("colz");
    gPad->Update();
    if( bRotateXY180 ) {
      his2->GetXaxis()->SetLabelOffset(9999);
      //      his2->GetXaxis()->SetTickLength(0);
      TGaxis* newXaxis = new TGaxis(gPad->GetUxmax(), gPad->GetUymin(), gPad->GetUxmin(), gPad->GetUymin(),
				    his2->GetXaxis()->GetXmin(), his2->GetXaxis()->GetXmax(),
				    510,"-");
      newXaxis->SetLabelOffset(0.01);
      newXaxis->SetLabelSize(0.03);
      newXaxis->Draw();

      his2->GetYaxis()->SetLabelOffset(9999);
      //      his2->GetYaxis()->SetTickLength(0);
      double xmin = gPad->GetUxmin();
     //      double xmin = gPad->GetUxmin()-(gPad->GetUxmax()-gPad->GetUxmin)/20.;
      TGaxis* newYaxis = new TGaxis(xmin, gPad->GetUymin(), xmin, gPad->GetUymax(),
				    his2->GetYaxis()->GetXmin(), his2->GetYaxis()->GetXmax(),
				    510,"-");
      newYaxis->SetLabelOffset(0.01);      
      newYaxis->SetLabelSize(0.03);
      newYaxis->SetTextFont(20);
      newYaxis->Draw();
      //      G4cout << " XAXIS " << gPad->GetUxmax() << " " << gPad->GetUymin() << " " << gPad->GetUxmin() << " " << gPad->GetUymin() << " " << his2->GetXaxis()->GetXmin() << " " << his2->GetXaxis()->GetXmax() << G4endl; //GDEB
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
    
    std::string figName = "his"+hisName2+"."+theImageFormat;
    canvas->Print(figName.c_str()); 
    if( theOutputFile ) {
      theOutputFile->cd();
      his2->Write();
      //      G4cout << " WRITE HISTO his2XZ " << his2->GetName() << G4endl; //GDEB
    }
    delete his2;

    if ( bDrawHisto1 ) {
      if( theOutputFile ) {
	theOutputFile->cd();
	his1->Write();
      }
      gStyle->SetOptStat(111111);
      his1->Draw("");
      figName = "his"+G4String(his1->GetName())+"."+theImageFormat;
      canvas->Print(figName.c_str());
      delete his1;
    }

  }

  if ( bDrawHisto1 ) {
    if( theOutputFile ) {
      theOutputFile->cd();
      his1All->Write();
    }
    //    G4cout << " WRITE HISTO his1All " << his1All->GetName() << G4endl; //GDEB
    his1All->Draw("");
    std::string figName = "his"+G4String(his1All->GetName())+"."+theImageFormat;
    canvas->Print(figName.c_str());
    delete his1All;
  }  
}

//-----------------------------------------------------------------------------
void DicomDrawerROOT::DrawYZ( DicomVImage* image, std::vector<DicomVLineSet*> lineSets, G4String extraFileName, G4bool bNonZero )
{
  if( theReflectYImages.find(image) == theReflectYImages.end() ) {
    if( bReflectY ) {
      std::vector<DicomVOperator*> operators = DicomMgr::GetInstance()->GetOperators();
      DicomOperReflectY* rotateOper = 0;
      for( size_t ii = 0; ii < operators.size(); ii++ ) {
	if( dynamic_cast<DicomOperReflectY*>(operators[ii]) != 0 ) {
	  rotateOper = dynamic_cast<DicomOperReflectY*>(operators[ii]);
	  break;
	}
      }
      rotateOper->Operate(image);
      theReflectYImages.insert(image);
    }
  }
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
    G4cout << "DrawYZ DICOM IMAGE NAME " << image->GetName() << " " << DicomVImage::GetModalityStr( image->GetModality() )<< std::endl;
  }
  std::vector<G4double>* imgDatap = image->GetData();
  G4double* imgData = &(imgDatap->at(0));
  G4double* imgData0 = &(imgDatap->at(0));
  //  G4cout << " imagData 0 " << imNVoxXY*imNVoxZ << " " << imNVoxX << " " << imNVoxY << " " << imNVoxXY << " " << imNVoxZ << std::endl; //GDEB
  //  std::string hisName2 = DicomVImage::GetModalityStr( image->GetModality() ).substr(4,999)+"_YZ";
  std::string hisName1All = GmGenUtils::ReplaceChar(image->GetName()+"_YZ"+extraFileName,'/',':');
  if(hisName1All.find("_2D") != std::string::npos ) {
    hisName1All.replace(hisName1All.find("_2D"), 3, "_1D.ALL");
  }
  TH1F* his1All = 0;
  if ( bDrawHisto1 ) his1All = new TH1F(hisName1All.c_str(), hisName1All.c_str(), 100, 0., thePaletteMax );

  theImageFormat = DicomParameterMgr::GetInstance()->GetStringValue("imageFormat","jpg");
  for( size_t ix = 0; ix < imNVoxX; ix++ ) {
    imgData = imgData0 + ix;
    G4String xstr; 
    if( bReflectY ) {
      xstr = GmGenUtils::ftoa(imMaxX-(ix+0.5)*imWidthX);
    } else {
      xstr = GmGenUtils::ftoa(imMinX+(ix+0.5)*imWidthX);
    }      
    if( bRotateXY180 ) {
      xstr = GmGenUtils::ftoa(imMaxX-(ix+0.5)*imWidthX);
    } else {
      xstr = GmGenUtils::ftoa(imMinX+(ix+0.5)*imWidthX);
    }      
    std::string hisName2 = GmGenUtils::ReplaceChar(image->GetName()+"_YZ"+extraFileName+"_"+xstr,'/',':');
    TH2F* his2 = new TH2F(hisName2.c_str(), hisName2.c_str(), imNVoxY, imMinY, imMaxY, imNVoxZ, imMinZ, imMaxZ );
    G4String hisName1 = hisName2;
    if( hisName1.find("_2D") != std::string::npos ) {
      hisName1.replace(hisName1.find("_2D"), 3, "_1D");
    }
    TH1F* his1 = 0; 
    if ( bDrawHisto1 ) his1 = new TH1F(hisName1.c_str(), hisName1.c_str(), 100, 0., thePaletteMax );
    
    gStyle->SetOptTitle(bFigureTitle);

    for( size_t iz = 1; iz <= imNVoxZ; iz++ ) {
      for( size_t iy = 1; iy <= imNVoxY; iy++ ) {
	G4double data = *imgData;
	if( bAverageSurroundingBig ) {
	  data = GetValueSurrounding(ix,iy-1,iz-1, data, image, imgDatap);
	}
	if( data > thePaletteMin ) {
	  his2->SetBinContent(iy,iz,data);
	} else {
	  his2->SetBinContent(ix,iy,thePaletteMin);
	}
	if ( bDrawHisto1 ) {
	  if( !bNonZero || data != 0. ) his1->Fill(data);
	  if (data != 0. ) his1All->Fill(data);
	}	//	size_t copyNo = ix + (iy-1)*imNVoxX + (iz-1)*imNVoxXY;
	//	if( copyNo != imgData-imgData0 ) G4cout << " !!ERROR  " << ix << " " << iy-1 << " " << iz-1 << " " << copyNo << " imagData " << imgData-imgData0 << std::endl; //GDEB
	/*#ifndef GAMOS_NO_VERBOSE
	if( DicomVerb(testVerb) ) {
	  if( data != 0 ) G4cout << " DrawerROOT YZ " << ix << " " << iy-1 << " " << iz-1 << " = " << data << G4endl;
	}
	#endif*/
	imgData += imNVoxX;
      }
      //      imgData += imNVoxXY - ;
      //  G4cout << iy << " imagData Y " << imgData-imgData0 << std::endl; //GDEB
    }
    //  G4cout << iz << " imagData Z " << imgData-imgData0 << std::endl; //GDEB

    canvas->Draw();
    if( thePaletteMin != -DBL_MAX ) his2->SetMinimum(thePaletteMin);
    if( thePaletteMax != DBL_MAX ) his2->SetMaximum(thePaletteMax);
    gStyle->SetOptStat(0);
    his2->Draw("colz");
    gPad->Update();

    if( bReflectY ) {
      his2->GetXaxis()->SetLabelOffset(9999);
      //      his2->GetXaxis()->SetTickLength(0);
      double ymin = gPad->GetUymin()*1.;
      TGaxis* newXaxis = new TGaxis(gPad->GetUxmax(), ymin, gPad->GetUxmin(), ymin,
				    his2->GetXaxis()->GetXmin(), his2->GetXaxis()->GetXmax(),
				    510,"-");
      //      G4cout << "XZ XAXIS " << gPad->GetUxmax() << " " << gPad->GetUymin() << " " << gPad->GetUxmax() << " " << gPad->GetUymin() << " " << his2->GetXaxis()->GetXmin() << " " << his2->GetXaxis()->GetXmax() << G4endl; //GDEB
      newXaxis->SetLabelOffset(0.01);
      newXaxis->SetLabelSize(0.03);
      newXaxis->Draw();
      his2->GetYaxis()->SetLabelOffset(9999);
      //      his2->GetYaxis()->SetTickLength(0);
      double xmin = gPad->GetUxmin();
     //      double xmin = gPad->GetUxmin()-(gPad->GetUxmax()-gPad->GetUxmin)/20.;
      TGaxis* newYaxis = new TGaxis(xmin, gPad->GetUymin(), xmin, gPad->GetUymax(),
				    his2->GetYaxis()->GetXmin(), his2->GetYaxis()->GetXmax(),
				    510,"-");
      newYaxis->SetLabelOffset(+0.01);      
      newYaxis->SetLabelSize(0.03);
      newYaxis->SetTextFont(20);
      newYaxis->Draw();

      //      G4cout << " YAXIS " << gPad->GetUymax() << " " << gPad->GetUxmin() << " " << gPad->GetUymin() << " " << gPad->GetUxmin() << " " << his2->GetYaxis()->GetXmin() << " " << his2->GetYaxis()->GetXmax() << G4endl; //GDEB
    } 
    if( bRotateXY180 ) {
      his2->GetXaxis()->SetLabelOffset(9999);
      //      his2->GetXaxis()->SetTickLength(0);
      double ymin = gPad->GetUymin()*1.;
      TGaxis* newXaxis = new TGaxis(gPad->GetUxmax(), ymin, gPad->GetUxmin(), ymin,
				    his2->GetXaxis()->GetXmin(), his2->GetXaxis()->GetXmax(),
				    510,"-");
      //      G4cout << "XZ XAXIS " << gPad->GetUxmax() << " " << gPad->GetUymin() << " " << gPad->GetUxmax() << " " << gPad->GetUymin() << " " << his2->GetXaxis()->GetXmin() << " " << his2->GetXaxis()->GetXmax() << G4endl; //GDEB
      newXaxis->SetLabelOffset(0.01);
      newXaxis->SetLabelSize(0.03);
      newXaxis->Draw();
      his2->GetYaxis()->SetLabelOffset(9999);
      //      his2->GetYaxis()->SetTickLength(0);
      double xmin = gPad->GetUxmin();
     //      double xmin = gPad->GetUxmin()-(gPad->GetUxmax()-gPad->GetUxmin)/20.;
      TGaxis* newYaxis = new TGaxis(xmin, gPad->GetUymin(), xmin, gPad->GetUymax(),
				    his2->GetYaxis()->GetXmin(), his2->GetYaxis()->GetXmax(),
				    510,"-");
      newYaxis->SetLabelOffset(+0.01);      
      newYaxis->SetLabelSize(0.03);
      newYaxis->SetTextFont(20);
      newYaxis->Draw();

      //      G4cout << " YAXIS " << gPad->GetUymax() << " " << gPad->GetUxmin() << " " << gPad->GetUymin() << " " << gPad->GetUxmin() << " " << his2->GetYaxis()->GetXmin() << " " << his2->GetYaxis()->GetXmax() << G4endl; //GDEB
    } 

    //--- Draw lines
    for( size_t ils = 0; ils < lineSets.size(); ils++ ) {
      if( bReflectY && !lineSets[ils]->HasBeenRotatedXY180() ) {
	G4ThreeVector imgCentre = image->GetCentre();
	lineSets[ils]->ReflectY( imgCentre );
	lineSets[ils]->SetHasBeenRotatedXY180( true );
      }
      if( bRotateXY180 && !lineSets[ils]->HasBeenRotatedXY180() ) {
	G4ThreeVector imgCentre = image->GetCentre();
	lineSets[ils]->RotateXY180( 180.*CLHEP::deg, imgCentre );
	lineSets[ils]->SetHasBeenRotatedXY180( true );
      }
      DrawLinesYZ( lineSets[ils], imMinX+ix*imWidthX, imMinX+(ix+1)*imWidthX);
    }
    
    std::string figName = "his"+hisName2+"."+theImageFormat;
    canvas->Print(figName.c_str());
    if( theOutputFile ) {
      theOutputFile->cd();
      his2->Write();
      //      G4cout << " WRITE HISTO his2YZ " << his2->GetName() << G4endl; //GDEB
    }
    delete his2;
    if ( bDrawHisto1 ) {
      if( theOutputFile ) {
	theOutputFile->cd();
	his1->Write();
      }
      gStyle->SetOptStat(111111);
      his1->Draw("");
      figName = "his"+G4String(his1->GetName())+"."+theImageFormat;
      canvas->Print(figName.c_str());
      delete his1;
    }

  }
  
  if ( bDrawHisto1 ) {
    if( theOutputFile ) {
      theOutputFile->cd();
      his1All->Write();
    }
    //    G4cout << " WRITE HISTO his1All " << his1All->GetName() << G4endl; //GDEB
    his1All->Draw("");
    std::string figName = "his"+G4String(his1All->GetName())+"."+theImageFormat;
    canvas->Print(figName.c_str());
    delete his1All;
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
  theLineDrawingOptions = DicomMgr::GetInstance()->GetLineDrawingOptions();
  //  G4cout << "DICOMDrawerROOT theLineDrawingOptions " << theLineDrawingOptions.size()<< G4endl; //GDEB 
  if( theLineDrawingOptions.size() == 0 ) {
    bLineDrawingOptions = false;
  } else {
    bLineDrawingOptions = true;
  }


  if( DicomVerb(debugVerb) ) {
    G4cout << " DicomDrawerROOT::DrawLines lineSet " << lineSet->GetName() << " " << DicomVLine::GetOrientationName(lineSet->GetOrientation()) << " =? " << DicomVLine::GetOrientationName(orient) << G4endl; 
  }  

  if( lineSet->GetOrientation() != orient ) return;
  std::vector<DicomVLineList*> lineLists = lineSet->GetLineLists();
  for( size_t ill = 0; ill < lineLists.size(); ill++ ) {
    std::vector<DicomVLine*> lines = lineLists[ill]->GetLines();
    if( DicomVerb(testVerb) ) G4cout << " DicomDrawerROOT::DrawLines " << lineLists[ill]->GetName() << " N LINES " <<  lines.size()<< G4endl;  
    for( size_t il = 0; il < lines.size(); il++ ) {
      G4double planePos = lines[il]->GetPlanePosition();
      if( DicomVerb(testVerb) ) G4cout << il << " DicomDrawerROOT::DrawLines ?? LINE " << lineLists[ill]->GetName() << " ZPOS " << minPos <<  " <? " << planePos << " <? " << maxPos << G4endl;
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
  if( DicomVerb(debugVerb) ) G4cout << " DicomDrawerROOT::DrawLineColorAndStyle(  LINE " << name << " bDrawText " << bDrawText << G4endl;
  // set line colors
  // if( !bLineDrawingOptions = false ) return; // take default options
  DicomLineDrawingOptions* ldo; 
 
  vpsldo::const_iterator ite;
  for( ite = theLineDrawingOptions.begin(); ite != theLineDrawingOptions.end(); ite++ ) {
    if( DicomVerb(testVerb) ) G4cout << " CHECK theLineDrawingOptions " <<  ite->first << " == " << name << G4endl; 
  
    if( GmGenUtils::AreWordsEquivalent( ite->first, name ) ) {
      if( DicomVerb(debugVerb) ) G4cout << " theLineDrawingOptions.begin FOUND " <<  ite->first << " == " << name << G4endl; 
      break;
    }
  }
  if( ite != theLineDrawingOptions.end() ) {
    ldo = (*ite).second;
    //  } else if( !bLineDrawingOptions ) {
    // ldo = new DicomLineDrawingOptions;// default options
    //    G4cout << " LINE NOT FOUND " << name << " NEW " << ldo->GetOption() << " " << ldo->GetTColor() << " " << ldo->GetWidth() << G4endl; //GDEB
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
    //    G4cout << " DRAW LINE " << name <<" "<< line->GetX1() << ", " << line->GetY1() << G4endl; //GDEB
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

//-----------------------------------------------------------------------------
void DicomDrawerROOT::WriteHisto1D( const TH1F* histo )
{
  if( theOutputFile ) {
    theOutputFile->cd();
    histo->Write();
  }
}

#endif
