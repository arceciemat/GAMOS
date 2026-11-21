#include "DicomDrawerSVG.hh"
#include "DicomVImage.hh"
#include "DicomVLineSet.hh"
#include "DicomVLineList.hh"
#include "DicomVLine.hh"
#include "DicomPolygonSet.hh"
#include "DicomPolygonList.hh"
#include "DicomParameterMgr.hh"
#include "DicomMgr.hh"
#include "DicomVerbosity.hh"
#include "DicomLineDrawingOptions.hh"
#include "DicomOperReflectY.hh"
#include "DicomOperRotateXY180.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh" // GmHisto1, GmHisto2 (TH1F, TH2F if GAMOS is built with ROOT)
#include "GamosCore/GamosBase/Base/include/GmHistoWriterCSV.hh"
#include "GamosCore/GamosUtils/include/GmSVGCanvas.hh"
#include "GamosCore/GamosUtils/include/GmSVGPad.hh"
#include "GamosCore/GamosUtils/include/GmSVGFrame.hh"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Units/PhysicalConstants.h"

#include <cfloat>
#include <cctype>
#include <cmath>
#include <algorithm>

//-----------------------------------------------------------------------------
// 100 bins from 0 to histoMax (as DicomDrawerROOT)
GmHisto1* DicomDrawerSVG::NewHisto1( const G4String& hisName, G4double histoMax )
{
  if( histoMax <= 0. ) histoMax = 1.; // upper edge must be bigger than lower edge
  return new GmHisto1( hisName.c_str(), hisName.c_str(), 100, 0., histoMax );
}

//-----------------------------------------------------------------------------
G4String DicomDrawerSVG::CheckImageFormat( const G4String& imgf )
{
  G4String format = imgf;
  for( size_t ic = 0; ic < format.size(); ic++ ) format[ic] = std::tolower(format[ic]);
  if( format == "jpeg" ) format = "jpg";
  if( format == "tif" ) format = "tiff";
  static const std::set<G4String> validFormats = { "svg", "png", "jpg", "gif", "pdf", "ps", "eps", "tiff", "bmp", "webp" };
  if( validFormats.find(format) == validFormats.end() ) {
    G4String validStr;
    for( std::set<G4String>::const_iterator ite = validFormats.begin(); ite != validFormats.end(); ite++ ) {
      validStr += " " + *ite;
    }
    G4Exception("DicomDrawerSVG::CheckImageFormat",
		"",
		FatalErrorInArgument,
		("-imageFormat: image format not accepted: '" + imgf + "' . Accepted values are:" + validStr).c_str());
  }
  return format;
}

//-----------------------------------------------------------------------------
DicomDrawerSVG::DicomDrawerSVG()
{
  theParamMgr = (DicomParameterMgr*)(DicomParameterMgr::GetInstance());
  theImageFormat = "jpg";
  theCanvasNPixelsX = G4int(theParamMgr->GetNumericValue("nPixelsX",1000));
  theCanvasNPixelsY = G4int(theParamMgr->GetNumericValue("nPixelsY",1000));
  theCanvasNPixelsZ = G4int(theParamMgr->GetNumericValue("nPixelsZ",1000));
  thePaletteMin = theParamMgr->GetNumericValue("paletteMin",-DBL_MAX);
  thePaletteMax = theParamMgr->GetNumericValue("paletteMax",DBL_MAX);
  bFigureTitle = false;
  bFigureTitle = bool(theParamMgr->GetNumericValue("bFigureTitle",bFigureTitle));
  theNContours = std::max(1,G4int(theParamMgr->GetNumericValue("nContours",20)));
  theSVGConverter = theParamMgr->GetStringValue("SVGConverter","auto");
  bReflectY = DicomMgr::GetInstance()->GetReflectY() != 0;
  bRotateXY180 = DicomMgr::GetInstance()->GetRotateXY180() != 0;

  theOverlayOpacity = std::max(0.,std::min(1.,theParamMgr->GetNumericValue("overlayOpacity",0.6)));
  theCTMin = theParamMgr->GetNumericValue("CTMin",-DBL_MAX);
  theCTMax = theParamMgr->GetNumericValue("CTMax",DBL_MAX);
  theCTWindowImage = 0;
  theCTMinUsed = 0.;
  theCTMaxUsed = 1.;
  bFrameTransform = false;

  theOutputFile = 0;
  bDrawHisto1 = false;
  theCanvas = new GmSVGCanvas(theCanvasNPixelsX,theCanvasNPixelsY);
  theFrame = 0;
}

//-----------------------------------------------------------------------------
DicomDrawerSVG::~DicomDrawerSVG()
{
  CloseOutputFile();
  delete theFrame;
  delete theCanvas;
}

//-----------------------------------------------------------------------------
void DicomDrawerSVG::SetOutputFile( G4String foutName )
{
  if( foutName.size() > 5 && foutName.substr(foutName.size()-5) == ".root" ) {
    G4String newName = foutName.substr(0,foutName.size()-5) + ".csv";
    G4cout << " DicomDrawerSVG: histograms are written in the GAMOS .csv file " << newName << " instead of " << foutName << G4endl;
    foutName = newName;
  }
  CloseOutputFile();
  theOutputFile = new GmHistoWriterCSV(foutName);
}

//-----------------------------------------------------------------------------
void DicomDrawerSVG::CloseOutputFile()
{
  if( theOutputFile ) {
    delete theOutputFile; // closes the file
    theOutputFile = 0;
  }
}

//-----------------------------------------------------------------------------
void DicomDrawerSVG::ApplyImageOperators( DicomVImage* image, G4bool bApplyReflectY )
{
  std::vector<DicomVOperator*> operators = DicomMgr::GetInstance()->GetOperators();
  if( bApplyReflectY && bReflectY && theReflectYImages.find(image) == theReflectYImages.end() ) {
    G4bool bFound = false;
    for( size_t ii = 0; ii < operators.size(); ii++ ) {
      if( dynamic_cast<DicomOperReflectY*>(operators[ii]) != 0 ) {
	dynamic_cast<DicomOperReflectY*>(operators[ii])->Operate(image);
	bFound = true;
	break;
      }
    }
    if( !bFound ) G4Exception("DicomDrawerSVG::ApplyImageOperators","",JustWarning,
			      "ReflectY is set, but no DicomOperReflectY exists: image is not reflected");
    theReflectYImages.insert(image);
  }
  if( bRotateXY180 && theRotateXY180Images.find(image) == theRotateXY180Images.end() ) {
    G4bool bFound = false;
    for( size_t ii = 0; ii < operators.size(); ii++ ) {
      if( dynamic_cast<DicomOperRotateXY180*>(operators[ii]) != 0 ) {
	dynamic_cast<DicomOperRotateXY180*>(operators[ii])->Operate(image);
	bFound = true;
	break;
      }
    }
    if( !bFound ) G4Exception("DicomDrawerSVG::ApplyImageOperators","",JustWarning,
			      "RotateXY180 is set, but no DicomOperRotateXY180 exists: image is not rotated");
    theRotateXY180Images.insert(image);
  }
}

//-----------------------------------------------------------------------------
// Draw a 2D image as ROOT TH2F::Draw("colz"): frame, color map, axes and palette
void DicomDrawerSVG::DrawImage2D( const G4String& hisName, const std::vector<G4double>& values,
				  size_t nx, G4double xMin, G4double xMax, size_t ny, G4double yMin, G4double yMax,
				  G4int canvasW, G4int canvasH, G4double marginLeft, G4double marginBottom,
				  G4bool bReverseX, G4bool bReverseY,
				  DicomVImage* image, G4int iAxisU, G4int iAxisV, G4double planePos )
{
  //--- background (CT) image: the frame covers both images, in real coordinates
  //    (each image is flipped by the ReflectY/RotateXY180 operators around its own centre, which maps its extent onto itself)
  DicomVImage* bgImage = (image != 0 && theBackgroundImage != 0 && theBackgroundImage != image) ? theBackgroundImage : 0;
  G4double fxMin = xMin, fxMax = xMax, fyMin = yMin, fyMax = yMax;
  bFrameTransform = false;
  if( bgImage ) {
    G4double bgMin[3] = { bgImage->GetMinX(), bgImage->GetMinY(), bgImage->GetMinZ() };
    G4double bgMax[3] = { bgImage->GetMaxX(), bgImage->GetMaxY(), bgImage->GetMaxZ() };
    fxMin = std::min(fxMin,bgMin[iAxisU]);
    fxMax = std::max(fxMax,bgMax[iAxisU]);
    fyMin = std::min(fyMin,bgMin[iAxisV]);
    fyMax = std::max(fyMax,bgMax[iAxisV]);
    G4bool bRefl = theReflectYImages.find(image) != theReflectYImages.end();
    G4bool bRot = theRotateXY180Images.find(image) != theRotateXY180Images.end();
    G4bool bFlipAxis[3] = { bRot, bRefl != bRot, false }; // as FlipAsImage
    G4ThreeVector centre = image->GetCentre();
    G4int iAxes[2] = { iAxisU, iAxisV };
    G4bool bRev[2] = { bReverseX, bReverseY };
    G4double fSum[2] = { fxMin+fxMax, fyMin+fyMax };
    for( G4int ia = 0; ia < 2; ia++ ) {
      bFrameReverse[ia] = bRev[ia];
      theFrameImageC2[ia] = 2.*centre[iAxes[ia]];
      bFrameFlip[ia] = bFlipAxis[iAxes[ia]];
      theFrameSum[ia] = fSum[ia];
    }
    bFrameTransform = true;
    //--- keep the figure aspect ratio equal to the real one
    canvasH = G4int( canvasH * ((fyMax-fyMin)/(yMax-yMin)) / ((fxMax-fxMin)/(xMax-xMin)) );
  }

  theCanvas->SetCanvasSize(canvasW,canvasH);
  theCanvas->Clear();
  theCanvas->cd();
  delete theFrame;
  theFrame = new GmSVGFrame( hisName, theCanvas->GetCurrentPad(), fxMin, fxMax, fyMin, fyMax, false, false,
			     marginLeft, 0.2, marginBottom, 0.1 );
  theFrame->SetReverseLabelsX(bReverseX);
  theFrame->SetReverseLabelsY(bReverseY);

  //--- color scale limits
  G4double zMin = DBL_MAX, zMax = -DBL_MAX;
  for( size_t ii = 0; ii < values.size(); ii++ ) {
    zMin = std::min(zMin,values[ii]);
    zMax = std::max(zMax,values[ii]);
  }
  if( thePaletteMin != -DBL_MAX ) zMin = thePaletteMin;
  if( thePaletteMax != DBL_MAX ) zMax = thePaletteMax;
  if( zMax <= zMin ) zMax = zMin + 1.;
  //--- as ROOT COLZ: empty bins are not painted (unless minimum < 0), bins below minimum neither
  G4bool bPaintZero = (zMin < 0.);

  const G4int nPalColors = GmSVGCanvas::GetPaletteNColors();
  if( !bgImage ) {
    std::vector<unsigned char> rgba(nx*ny*4,0);
    for( size_t iy = 0; iy < ny; iy++ ) {
      size_t row = ny-1-iy; // image row 0 is at the top
      for( size_t ix = 0; ix < nx; ix++ ) {
	G4double zz = values[ix+iy*nx];
	if( zz < zMin || (zz == 0. && !bPaintZero) ) continue; // transparent
	//--- as ROOT THistPainter::PaintColorLevels
	G4int level = G4int(0.01+(zz-zMin)/(zMax-zMin)*theNContours);
	level = std::max(0,std::min(theNContours-1,level));
	G4int icol = std::min(nPalColors-1,G4int((level+0.99)*G4float(nPalColors)/G4float(theNContours)));
	unsigned char* pix = &rgba[(row*nx+ix)*4];
	GmSVGCanvas::PaletteColor( icol, pix[0], pix[1], pix[2] );
	pix[3] = 255;
      }
    }
    theCanvas->Image( theFrame->fPixL, theFrame->fPixT, theFrame->fPixR, theFrame->fPixB, nx, ny, rgba );

  } else {
    //--- image colors over the background image in grey scale, on a grid fine enough for both images
    G4double bgVoxDim[3] = { bgImage->GetVoxelDimX(), bgImage->GetVoxelDimY(), bgImage->GetVoxelDimZ() };
    G4double cellU = std::min( (xMax-xMin)/nx, bgVoxDim[iAxisU] );
    G4double cellV = std::min( (yMax-yMin)/ny, bgVoxDim[iAxisV] );
    size_t nPU = std::max(size_t(1),std::min(size_t(4000),size_t(std::ceil((fxMax-fxMin)/cellU))));
    size_t nPV = std::max(size_t(1),std::min(size_t(4000),size_t(std::ceil((fyMax-fyMin)/cellV))));
    cellU = (fxMax-fxMin)/nPU;
    cellV = (fyMax-fyMin)/nPV;
    G4int iAxisPlane = 3 - iAxisU - iAxisV;
    std::vector<unsigned char> rgba(nPU*nPV*4,0);
    for( size_t iv = 0; iv < nPV; iv++ ) {
      G4double wv = fyMin + (iv+0.5)*cellV; // frame coordinate
      G4double realV = bFrameReverse[1] ? theFrameSum[1] - wv : wv;
      G4double vv = bFrameFlip[1] ? theFrameImageC2[1] - realV : realV; // drawing coordinate of image
      size_t row = nPV-1-iv; // image row 0 is at the top
      for( size_t iu = 0; iu < nPU; iu++ ) {
	G4double wu = fxMin + (iu+0.5)*cellU;
	G4double realU = bFrameReverse[0] ? theFrameSum[0] - wu : wu;
	G4double uu = bFrameFlip[0] ? theFrameImageC2[0] - realU : realU;
	unsigned char* pix = &rgba[(row*nPU+iu)*4];
	//--- background grey
	G4ThreeVector pos;
	pos[iAxisU] = uu;
	pos[iAxisV] = vv;
	pos[iAxisPlane] = planePos;
	G4double grey = 0.;
	G4bool bGrey = GetBackgroundGrey( image, pos, grey );
	//--- image color
	G4bool bColor = false;
	unsigned char rr = 0, gg = 0, bb = 0;
	if( uu >= xMin && uu < xMax && vv >= yMin && vv < yMax ) {
	  size_t ix = std::min(nx-1,size_t((uu-xMin)/(xMax-xMin)*nx));
	  size_t iy = std::min(ny-1,size_t((vv-yMin)/(yMax-yMin)*ny));
	  G4double zz = values[ix+iy*nx];
	  if( !(zz < zMin || (zz == 0. && !bPaintZero)) ) {
	    G4int level = G4int(0.01+(zz-zMin)/(zMax-zMin)*theNContours);
	    level = std::max(0,std::min(theNContours-1,level));
	    G4int icol = std::min(nPalColors-1,G4int((level+0.99)*G4float(nPalColors)/G4float(theNContours)));
	    GmSVGCanvas::PaletteColor( icol, rr, gg, bb );
	    bColor = true;
	  }
	}
	if( bColor && bGrey ) {
	  G4double greyC = 255.*grey*(1.-theOverlayOpacity);
	  pix[0] = (unsigned char)(std::lround(theOverlayOpacity*rr + greyC));
	  pix[1] = (unsigned char)(std::lround(theOverlayOpacity*gg + greyC));
	  pix[2] = (unsigned char)(std::lround(theOverlayOpacity*bb + greyC));
	  pix[3] = 255;
	} else if( bColor ) {
	  pix[0] = rr; pix[1] = gg; pix[2] = bb; pix[3] = 255;
	} else if( bGrey ) {
	  pix[0] = pix[1] = pix[2] = (unsigned char)(std::lround(255.*grey));
	  pix[3] = 255;
	}
      }
    }
    theCanvas->Image( theFrame->fPixL, theFrame->fPixT, theFrame->fPixR, theFrame->fPixB, nPU, nPV, rgba );
  }
  theFrame->DrawAxes( "", "", 0.035, 0.035, 0.035, 0.035, 1., 1. );

  //--- palette axis, at the right of the frame
  GmSVGPad* pad = theFrame->GetPad();
  G4double labelPx = 0.035*std::min(pad->GetPixWidth(),pad->GetPixHeight());
  G4double palX1 = theFrame->fPixR + 0.01*canvasW;
  G4double palX2 = palX1 + 0.05*canvasW;
  G4double frameH = theFrame->fPixB - theFrame->fPixT;
  for( G4int ic = 0; ic < theNContours; ic++ ) {
    G4double py1 = theFrame->fPixB - frameH*ic/theNContours;
    G4double py2 = theFrame->fPixB - frameH*(ic+1)/theNContours;
    unsigned char rr, gg, bb;
    GmSVGCanvas::PaletteColor( std::min(nPalColors-1,G4int((ic+0.99)*G4float(nPalColors)/G4float(theNContours))), rr, gg, bb );
    std::string col = GmSVGCanvas::RGBToSVG(rr/255.,gg/255.,bb/255.);
    theCanvas->Rect( palX1, py2-0.5, palX2, py1, col, col, 0.5 ); // slight overlap avoids gaps
  }
  theCanvas->Rect( palX1, theFrame->fPixT, palX2, theFrame->fPixB, "#000000", "none", 1. );
  std::vector<G4double> minor;
  std::vector<G4double> ticks = GmSVGFrame::GetTicks( zMin, zMax, false, minor, std::max(2,std::min(8,G4int(frameH/(2.5*labelPx)))) );
  G4double step = ticks.size() >= 2 ? ticks[1]-ticks[0] : zMax-zMin;
  for( size_t it = 0; it < ticks.size(); it++ ) {
    G4double py = theFrame->fPixB - frameH*(ticks[it]-zMin)/(zMax-zMin);
    theCanvas->Line( palX2, py, palX2-0.3*(palX2-palX1), py, "#000000", 1. );
    theCanvas->Text( palX2 + 0.3*labelPx, py + 0.35*labelPx, GmSVGFrame::FormatLabel(ticks[it],step), labelPx, "#000000", "start" );
  }

  if( bFigureTitle ) {
    G4double titlePx = 0.04*std::min(pad->GetPixWidth(),pad->GetPixHeight());
    theCanvas->Text( canvasW/2., theFrame->fPixT - 0.6*titlePx, hisName, titlePx, "#000000", "middle" );
  }
}

//-----------------------------------------------------------------------------
void DicomDrawerSVG::FlipAsImage( G4ThreeVector& pos, DicomVImage* image )
{
  G4bool bRefl = theReflectYImages.find(image) != theReflectYImages.end();
  G4bool bRot = theRotateXY180Images.find(image) != theRotateXY180Images.end();
  G4ThreeVector centre = image->GetCentre();
  if( bRot ) pos.setX( 2.*centre.x() - pos.x() );
  if( bRefl != bRot ) pos.setY( 2.*centre.y() - pos.y() );
}

//-----------------------------------------------------------------------------
G4bool DicomDrawerSVG::GetBackgroundGrey( DicomVImage* image, G4ThreeVector pos, G4double& grey )
{
  DicomVImage* bgImage = theBackgroundImage;
  FlipAsImage( pos, image ); // drawing coordinates of image -> real coordinates
  FlipAsImage( pos, bgImage ); // real coordinates -> data coordinates of background image
  if( pos.x() < bgImage->GetMinX() || pos.x() >= bgImage->GetMaxX()
      || pos.y() < bgImage->GetMinY() || pos.y() >= bgImage->GetMaxY()
      || pos.z() < bgImage->GetMinZ() || pos.z() >= bgImage->GetMaxZ() ) return false;
  size_t nx = bgImage->GetNoVoxelsX();
  size_t ny = bgImage->GetNoVoxelsY();
  size_t nz = bgImage->GetNoVoxelsZ();
  size_t ix = std::min(nx-1,size_t((pos.x()-bgImage->GetMinX())/bgImage->GetVoxelDimX()));
  size_t iy = std::min(ny-1,size_t((pos.y()-bgImage->GetMinY())/bgImage->GetVoxelDimY()));
  size_t iz = std::min(nz-1,size_t((pos.z()-bgImage->GetMinZ())/bgImage->GetVoxelDimZ()));
  std::vector<G4double>* data = bgImage->GetData();
  //--- grey scale window: CTMin/CTMax parameters, or minimum/maximum of the background image
  if( theCTWindowImage != bgImage ) {
    theCTMinUsed = theCTMin;
    theCTMaxUsed = theCTMax;
    if( theCTMinUsed == -DBL_MAX ) theCTMinUsed = *(std::min_element(data->begin(),data->end()));
    if( theCTMaxUsed == DBL_MAX ) theCTMaxUsed = *(std::max_element(data->begin(),data->end()));
    if( theCTMaxUsed <= theCTMinUsed ) theCTMaxUsed = theCTMinUsed + 1.;
    theCTWindowImage = bgImage;
  }
  G4double val = data->at(ix+iy*nx+iz*nx*ny);
  grey = std::max(0.,std::min(1.,(val-theCTMinUsed)/(theCTMaxUsed-theCTMinUsed)));
  return true;
}

//-----------------------------------------------------------------------------
G4double DicomDrawerSVG::ToFrame( G4int iuv, G4double val ) const
{
  if( !bFrameTransform ) return val;
  G4double real = bFrameFlip[iuv] ? theFrameImageC2[iuv] - val : val;
  return bFrameReverse[iuv] ? theFrameSum[iuv] - real : real;
}

//-----------------------------------------------------------------------------
void DicomDrawerSVG::PrintFigure( const G4String& figName )
{
  theCanvas->Print( figName+".svg", theImageFormat, theSVGConverter );
}

//-----------------------------------------------------------------------------
void DicomDrawerSVG::WriteHisto2D( const G4String& hisName, const std::vector<G4double>& values,
				   size_t nx, G4double xMin, G4double xMax, size_t ny, G4double yMin, G4double yMax )
{
  if( !theOutputFile ) return;
  //--- as DicomDrawerROOT: TH2F filled with SetBinContent (errors are sqrt(content))
  GmHisto2* his = new GmHisto2( hisName.c_str(), hisName.c_str(), G4int(nx), xMin, xMax, G4int(ny), yMin, yMax );
  for( size_t iy = 0; iy < ny; iy++ ) {
    for( size_t ix = 0; ix < nx; ix++ ) {
      his->SetBinContent( G4int(ix)+1, G4int(iy)+1, values[ix+iy*nx] );
    }
  }
  theOutputFile->SaveHisto2( his );
  delete his;
}

//-----------------------------------------------------------------------------
void DicomDrawerSVG::WriteHisto1( GmHisto1* his )
{
  if( !theOutputFile ) return;
  theOutputFile->SaveHisto1( his );
}

//-----------------------------------------------------------------------------
// Draw a 1D histogram as ROOT TH1F::Draw() with statistics box (SetOptStat(111111))
void DicomDrawerSVG::DrawHisto1( GmHisto1* his )
{
  G4String hisName = his->GetName();
  G4int nBins = his->GetNbinsX();
  G4double xMin = his->GetBinLowEdge(1);
  G4double xMax = his->GetBinLowEdge(nBins)+his->GetBinWidth(nBins);
  G4int canvasW = 700, canvasH = 500; // ROOT default canvas
  theCanvas->SetCanvasSize(canvasW,canvasH);
  theCanvas->Clear();
  theCanvas->cd();
  G4double hMax = std::max(0.,G4double(his->GetMaximum()));
  if( hMax == 0. ) hMax = 1.;
  GmSVGFrame frame( hisName, theCanvas->GetCurrentPad(), xMin, xMax, 0., hMax*1.05, false, false,
		    0.1, 0.1, 0.1, 0.1 );
  frame.DrawAxes( "", "", 0.035, 0.035, 0.035, 0.035, 1., 1. );
  std::string col = GmSVGCanvas::RootColorToSVG(602); // ROOT default histogram line color
  G4double yPrev = 0.;
  for( G4int ib = 1; ib <= nBins; ib++ ) {
    G4double xLow = his->GetBinLowEdge(ib);
    G4double binW = his->GetBinWidth(ib);
    G4double yy = his->GetBinContent(ib);
    frame.DrawLine( xLow, yPrev, xLow, yy, col, 1. );
    frame.DrawLine( xLow, yy, xLow+binW, yy, col, 1. );
    yPrev = yy;
  }
  frame.DrawLine( xMax, yPrev, xMax, 0., col, 1. );

  //--- statistics box
  std::vector<std::pair<G4String,G4String> > lines;
  lines.push_back( std::pair<G4String,G4String>(hisName,"") );
  lines.push_back( std::pair<G4String,G4String>("Entries",GmGenUtils::itoa(G4int(his->GetEntries()))) );
  lines.push_back( std::pair<G4String,G4String>("Mean",GmGenUtils::ftoa(his->GetMean())) );
  lines.push_back( std::pair<G4String,G4String>("Std Dev",GmGenUtils::ftoa(his->GetStdDev())) );
  lines.push_back( std::pair<G4String,G4String>("Underflow",GmGenUtils::ftoa(his->GetBinContent(0))) );
  lines.push_back( std::pair<G4String,G4String>("Overflow",GmGenUtils::ftoa(his->GetBinContent(nBins+1))) );
  G4double textPx = 0.03*canvasH;
  G4double boxW = 0.3*canvasW;
  G4double boxX2 = 0.98*canvasW;
  G4double boxX1 = boxX2 - boxW;
  G4double boxY1 = 0.02*canvasH;
  G4double boxY2 = boxY1 + textPx*1.3*lines.size() + 0.3*textPx;
  theCanvas->Rect( boxX1, boxY1, boxX2, boxY2, "#000000", "#ffffff", 1. );
  for( size_t il = 0; il < lines.size(); il++ ) {
    G4double py = boxY1 + textPx*1.3*(il+1);
    if( il == 0 ) {
      theCanvas->Text( (boxX1+boxX2)/2., py, lines[il].first, textPx, "#000000", "middle" );
      theCanvas->Line( boxX1, py+0.3*textPx, boxX2, py+0.3*textPx, "#000000", 1. );
    } else {
      theCanvas->Text( boxX1+0.3*textPx, py, lines[il].first, textPx, "#000000", "start" );
      theCanvas->Text( boxX2-0.3*textPx, py, lines[il].second, textPx, "#000000", "end" );
    }
  }
  PrintFigure( "his"+hisName );
}

//-----------------------------------------------------------------------------
void DicomDrawerSVG::DrawXY( DicomVImage* image, std::vector<DicomVLineSet*> lineSets, G4String extraFileName, G4bool bNonZero )
{
  ApplyImageOperators( image, true );

  size_t imNVoxX = image->GetNoVoxelsX();
  size_t imNVoxY = image->GetNoVoxelsY();
  size_t imNVoxZ = image->GetNoVoxelsZ();
  G4double imMinX = image->GetMinX();
  G4double imMinY = image->GetMinY();
  G4double imMaxX = image->GetMaxX();
  G4double imMaxY = image->GetMaxY();
  G4double imMinZ = image->GetMinZ();
  G4double imWidthZ = image->GetVoxelDimZ();
  G4int canvasW = theCanvasNPixelsX;
  G4int canvasH = G4int(theCanvasNPixelsY*(imMaxY-imMinY)/(imMaxX-imMinX));
  if( DicomVerb(debugVerb) ) {
    G4cout << " DicomDrawerSVG::DrawXY IMAGE NAME " << image->GetName() << " " << DicomVImage::GetModalityStr( image->GetModality() ) << G4endl;
  }
  std::vector<G4double>* imgDatap = image->GetData();
  G4double* imgData = &(imgDatap->at(0));

  std::string hisName1All = GmGenUtils::ReplaceChar(image->GetName()+"_XY"+extraFileName,'/',':');
  if( hisName1All.find("_2D") != std::string::npos ) {
    hisName1All.replace(hisName1All.find("_2D"), 3, "_1D.ALL");
  }
  G4double histoMax = thePaletteMax;
  if( histoMax == DBL_MAX ) histoMax = *(std::max_element(imgDatap->begin(),imgDatap->end()));
  GmHisto1* his1All = 0;
  if( bDrawHisto1 ) his1All = NewHisto1( hisName1All, histoMax );
  theImageFormat = CheckImageFormat(theParamMgr->GetStringValue("imageFormat","jpg"));
  G4bool bVoxelsInUseClosest = theParamMgr->GetNumericValue("closestPolygon",1);

  std::vector<G4double> values(imNVoxX*imNVoxY);
  for( size_t iz = 0; iz < imNVoxZ; iz++ ) {
    G4String zstr = GmGenUtils::ftoa(imMinZ+(iz+0.5)*imWidthZ);
    std::string hisName2 = GmGenUtils::ReplaceChar(image->GetName()+"_XY"+extraFileName+"_"+zstr,'/',':');
    G4String hisName1 = hisName2;
    if( hisName1.find("_2D") != std::string::npos ) {
      hisName1.replace(hisName1.find("_2D"), 3, "_1D");
    }
    GmHisto1* his1 = 0;
    if( bDrawHisto1 ) his1 = NewHisto1( hisName1, histoMax );
    for( size_t iy = 0; iy < imNVoxY; iy++ ) {
      for( size_t ix = 0; ix < imNVoxX; ix++ ) {
	G4double data = *imgData;
	if( bAverageSurroundingBig ) {
	  data = GetValueSurrounding(ix,iy,iz, data, image, imgDatap);
	}
	values[ix+iy*imNVoxX] = (data > thePaletteMin) ? data : thePaletteMin;
	if( bDrawHisto1 ) {
	  if( !bNonZero || data != 0. ) his1->Fill(data);
	  if( data != 0. ) his1All->Fill(data);
	}
	imgData++;
      }
    }
    DrawImage2D( hisName2, values, imNVoxX, imMinX, imMaxX, imNVoxY, imMinY, imMaxY,
		 canvasW, canvasH, 0.2, 0.2, bRotateXY180, bReflectY || bRotateXY180,
		 image, 0, 1, imMinZ+(iz+0.5)*imWidthZ );
    WriteHisto2D( hisName2, values, imNVoxX, imMinX, imMaxX, imNVoxY, imMinY, imMaxY );

    //--- Draw lines
    for( size_t ils = 0; ils < lineSets.size(); ils++ ) {
      DicomVLineSet* lineSet = lineSets[ils];
      if( dynamic_cast<DicomPolygonSet*>(lineSet) != 0 && bVoxelsInUseClosest ) {
	lineSet = VoxelsInUseClosest((DicomPolygonSet*)lineSet, image);
      }
      if( bReflectY && !lineSet->HasBeenReflectedY() ) {
	lineSet->ReflectY( image->GetCentre() );
	lineSet->SetHasBeenReflectedY( true );
      }
      if( bRotateXY180 && !lineSet->HasBeenRotatedXY180() ) {
	lineSet->RotateXY180( 180.*CLHEP::deg, image->GetCentre() );
	lineSet->SetHasBeenRotatedXY180( true );
      }
      DrawLinesXY( lineSet, imMinZ+iz*imWidthZ, imMinZ+(iz+1)*imWidthZ);
    }
    PrintFigure( "his"+hisName2 );

    if( bDrawHisto1 ) {
      WriteHisto1( his1 );
      DrawHisto1( his1 );
      delete his1;
    }
  }

  if( bDrawHisto1 ) {
    WriteHisto1( his1All );
    DrawHisto1( his1All );
    delete his1All;
  }
}

//-----------------------------------------------------------------------------
void DicomDrawerSVG::DrawXZ( DicomVImage* image, std::vector<DicomVLineSet*> lineSets, G4String extraFileName, G4bool bNonZero )
{
  ApplyImageOperators( image, false );

  size_t imNVoxX = image->GetNoVoxelsX();
  size_t imNVoxY = image->GetNoVoxelsY();
  size_t imNVoxZ = image->GetNoVoxelsZ();
  size_t imNVoxXY = imNVoxX*imNVoxY;
  G4double imMinX = image->GetMinX();
  G4double imMinY = image->GetMinY();
  G4double imMinZ = image->GetMinZ();
  G4double imMaxX = image->GetMaxX();
  G4double imMaxY = image->GetMaxY();
  G4double imMaxZ = image->GetMaxZ();
  G4double imWidthY = image->GetVoxelDimY();
  G4int canvasW = theCanvasNPixelsX;
  G4int canvasH = G4int(theCanvasNPixelsZ*(imMaxZ-imMinZ)/(imMaxX-imMinX));
  if( DicomVerb(debugVerb) ) {
    G4cout << " DicomDrawerSVG::DrawXZ IMAGE NAME " << image->GetName() << " " << DicomVImage::GetModalityStr( image->GetModality() ) << G4endl;
  }
  std::vector<G4double>* imgDatap = image->GetData();
  G4double* imgData0 = &(imgDatap->at(0));
  std::string hisName1All = GmGenUtils::ReplaceChar(image->GetName()+"_XZ"+extraFileName,'/',':');
  if( hisName1All.find("_2D") != std::string::npos ) {
    hisName1All.replace(hisName1All.find("_2D"), 3, "_1D.ALL");
  }
  G4double histoMax = thePaletteMax;
  if( histoMax == DBL_MAX ) histoMax = *(std::max_element(imgDatap->begin(),imgDatap->end()));
  GmHisto1* his1All = 0;
  if( bDrawHisto1 ) his1All = NewHisto1( hisName1All, histoMax );
  theImageFormat = CheckImageFormat(theParamMgr->GetStringValue("imageFormat","jpg"));

  std::vector<G4double> values(imNVoxX*imNVoxZ);
  for( size_t iy = 0; iy < imNVoxY; iy++ ) {
    G4String ystr;
    if( bRotateXY180 ) {
      ystr = GmGenUtils::ftoa(imMaxY-(iy+0.5)*imWidthY);
    } else {
      ystr = GmGenUtils::ftoa(imMinY+(iy+0.5)*imWidthY);
    }
    std::string hisName2 = GmGenUtils::ReplaceChar(image->GetName()+"_XZ"+extraFileName+"_"+ystr,'/',':');
    G4String hisName1 = hisName2;
    if( hisName1.find("_2D") != std::string::npos ) {
      hisName1.replace(hisName1.find("_2D"), 3, "_1D");
    }
    GmHisto1* his1 = 0;
    if( bDrawHisto1 ) his1 = NewHisto1( hisName1, histoMax );
    G4double* imgData = imgData0 + iy*imNVoxX;
    for( size_t iz = 0; iz < imNVoxZ; iz++ ) {
      for( size_t ix = 0; ix < imNVoxX; ix++ ) {
	G4double data = *imgData;
	if( bAverageSurroundingBig ) {
	  data = GetValueSurrounding(ix,iy,iz, data, image, imgDatap);
	}
	values[ix+iz*imNVoxX] = (data > thePaletteMin) ? data : thePaletteMin;
	if( bDrawHisto1 ) {
	  if( !bNonZero || data != 0. ) his1->Fill(data);
	  if( data != 0. ) his1All->Fill(data);
	}
	imgData++;
      }
      imgData += imNVoxXY - imNVoxX;
    }
    DrawImage2D( hisName2, values, imNVoxX, imMinX, imMaxX, imNVoxZ, imMinZ, imMaxZ,
		 canvasW, canvasH, 0.1, 0.1, bRotateXY180, false,
		 image, 0, 2, imMinY+(iy+0.5)*imWidthY );
    WriteHisto2D( hisName2, values, imNVoxX, imMinX, imMaxX, imNVoxZ, imMinZ, imMaxZ );

    //--- Draw lines
    for( size_t ils = 0; ils < lineSets.size(); ils++ ) {
      if( bRotateXY180 && !lineSets[ils]->HasBeenRotatedXY180() ) {
	lineSets[ils]->RotateXY180( 180.*CLHEP::deg, image->GetCentre() );
	lineSets[ils]->SetHasBeenRotatedXY180( true );
      }
      DrawLinesXZ( lineSets[ils], imMinY+iy*imWidthY, imMinY+(iy+1)*imWidthY);
    }
    PrintFigure( "his"+hisName2 );

    if( bDrawHisto1 ) {
      WriteHisto1( his1 );
      DrawHisto1( his1 );
      delete his1;
    }
  }

  if( bDrawHisto1 ) {
    WriteHisto1( his1All );
    DrawHisto1( his1All );
    delete his1All;
  }
}

//-----------------------------------------------------------------------------
void DicomDrawerSVG::DrawYZ( DicomVImage* image, std::vector<DicomVLineSet*> lineSets, G4String extraFileName, G4bool bNonZero )
{
  ApplyImageOperators( image, true );

  size_t imNVoxX = image->GetNoVoxelsX();
  size_t imNVoxY = image->GetNoVoxelsY();
  size_t imNVoxZ = image->GetNoVoxelsZ();
  G4double imMinX = image->GetMinX();
  G4double imMinY = image->GetMinY();
  G4double imMinZ = image->GetMinZ();
  G4double imMaxX = image->GetMaxX();
  G4double imMaxY = image->GetMaxY();
  G4double imMaxZ = image->GetMaxZ();
  G4double imWidthX = image->GetVoxelDimX();
  G4int canvasW = theCanvasNPixelsY;
  G4int canvasH = G4int(theCanvasNPixelsZ*(imMaxZ-imMinZ)/(imMaxY-imMinY));
  if( DicomVerb(debugVerb) ) {
    G4cout << " DicomDrawerSVG::DrawYZ IMAGE NAME " << image->GetName() << " " << DicomVImage::GetModalityStr( image->GetModality() ) << G4endl;
  }
  std::vector<G4double>* imgDatap = image->GetData();
  G4double* imgData0 = &(imgDatap->at(0));
  std::string hisName1All = GmGenUtils::ReplaceChar(image->GetName()+"_YZ"+extraFileName,'/',':');
  if( hisName1All.find("_2D") != std::string::npos ) {
    hisName1All.replace(hisName1All.find("_2D"), 3, "_1D.ALL");
  }
  G4double histoMax = thePaletteMax;
  if( histoMax == DBL_MAX ) histoMax = *(std::max_element(imgDatap->begin(),imgDatap->end()));
  GmHisto1* his1All = 0;
  if( bDrawHisto1 ) his1All = NewHisto1( hisName1All, histoMax );
  theImageFormat = CheckImageFormat(theParamMgr->GetStringValue("imageFormat","jpg"));

  std::vector<G4double> values(imNVoxY*imNVoxZ);
  for( size_t ix = 0; ix < imNVoxX; ix++ ) {
    G4double* imgData = imgData0 + ix;
    // as in DicomDrawerROOT (where the bRotateXY180 test overrides the bReflectY one)
    G4String xstr = bRotateXY180 ? GmGenUtils::ftoa(imMaxX-(ix+0.5)*imWidthX)
      : GmGenUtils::ftoa(imMinX+(ix+0.5)*imWidthX);
    std::string hisName2 = GmGenUtils::ReplaceChar(image->GetName()+"_YZ"+extraFileName+"_"+xstr,'/',':');
    G4String hisName1 = hisName2;
    if( hisName1.find("_2D") != std::string::npos ) {
      hisName1.replace(hisName1.find("_2D"), 3, "_1D");
    }
    GmHisto1* his1 = 0;
    if( bDrawHisto1 ) his1 = NewHisto1( hisName1, histoMax );
    for( size_t iz = 0; iz < imNVoxZ; iz++ ) {
      for( size_t iy = 0; iy < imNVoxY; iy++ ) {
	G4double data = *imgData;
	if( bAverageSurroundingBig ) {
	  data = GetValueSurrounding(ix,iy,iz, data, image, imgDatap);
	}
	values[iy+iz*imNVoxY] = (data > thePaletteMin) ? data : thePaletteMin;
	if( bDrawHisto1 ) {
	  if( !bNonZero || data != 0. ) his1->Fill(data);
	  if( data != 0. ) his1All->Fill(data);
	}
	imgData += imNVoxX;
      }
    }
    DrawImage2D( hisName2, values, imNVoxY, imMinY, imMaxY, imNVoxZ, imMinZ, imMaxZ,
		 canvasW, canvasH, 0.1, 0.1, bReflectY || bRotateXY180, false,
		 image, 1, 2, imMinX+(ix+0.5)*imWidthX );
    WriteHisto2D( hisName2, values, imNVoxY, imMinY, imMaxY, imNVoxZ, imMinZ, imMaxZ );

    //--- Draw lines
    for( size_t ils = 0; ils < lineSets.size(); ils++ ) {
      if( bReflectY && !lineSets[ils]->HasBeenReflectedY() ) {
	lineSets[ils]->ReflectY( image->GetCentre() );
	lineSets[ils]->SetHasBeenReflectedY( true );
      }
      if( bRotateXY180 && !lineSets[ils]->HasBeenRotatedXY180() ) {
	lineSets[ils]->RotateXY180( 180.*CLHEP::deg, image->GetCentre() );
	lineSets[ils]->SetHasBeenRotatedXY180( true );
      }
      DrawLinesYZ( lineSets[ils], imMinX+ix*imWidthX, imMinX+(ix+1)*imWidthX);
    }
    PrintFigure( "his"+hisName2 );

    if( bDrawHisto1 ) {
      WriteHisto1( his1 );
      DrawHisto1( his1 );
      delete his1;
    }
  }

  if( bDrawHisto1 ) {
    WriteHisto1( his1All );
    DrawHisto1( his1All );
    delete his1All;
  }
}

//-----------------------------------------------------------------------------
void DicomDrawerSVG::DrawLinesXY( DicomVLineSet* lineSet, G4double minZ, G4double maxZ )
{
  DrawLines( lineSet, minZ, maxZ, 0, 1, DPOrientXY);
}

//-----------------------------------------------------------------------------
void DicomDrawerSVG::DrawLinesXZ( DicomVLineSet* lineSet, G4double minY, G4double maxY )
{
  DrawLines( lineSet, minY, maxY, 0, 2, DPOrientXZ);
}

//-----------------------------------------------------------------------------
void DicomDrawerSVG::DrawLinesYZ( DicomVLineSet* lineSet, G4double minX, G4double maxX )
{
  DrawLines( lineSet, minX, maxX, 1, 2, DPOrientYZ);
}

//-----------------------------------------------------------------------------
void DicomDrawerSVG::DrawLines( DicomVLineSet* lineSet, G4double minPos, G4double maxPos, size_t ix, size_t iy, DPOrientation orient )
{
  if( !theFrame ) return;
  theLineDrawingOptions = DicomMgr::GetInstance()->GetLineDrawingOptions();
  if( DicomVerb(debugVerb) ) {
    G4cout << " DicomDrawerSVG::DrawLines lineSet " << lineSet->GetName() << " " << DicomVLine::GetOrientationName(lineSet->GetOrientation()) << " =? " << DicomVLine::GetOrientationName(orient) << G4endl;
  }
  if( lineSet->GetOrientation() != orient ) return;
  std::vector<DicomVLineList*> lineLists = lineSet->GetLineLists();
  for( size_t ill = 0; ill < lineLists.size(); ill++ ) {
    std::vector<DicomVLine*> lines = lineLists[ill]->GetLines();
    for( size_t il = 0; il < lines.size(); il++ ) {
      G4double planePos = lines[il]->GetPlanePosition();
      if( planePos >= minPos && planePos <= maxPos ) {
	std::vector<G4ThreeVector> points = lines[il]->GetPoints();
	G4int ns4 = points.size()/4;
	G4int ntxt = G4int(CLHEP::RandFlat::shoot()*ns4); // point where to draw the text
	for( size_t ip = 1; ip < points.size(); ip++ ) {
	  G4bool bDrawText = (ns4 == 0 && ip == 1);
	  if( ns4 != 0 ) bDrawText = bDrawText || (G4int(ip%ns4) == ntxt);
	  DrawSegmentColorAndStyle( points[ip-1](ix), points[ip-1](iy), points[ip](ix), points[ip](iy),
				    lines[il]->GetName(), bDrawText );
	}
      }
    }
  }
}

//-----------------------------------------------------------------------------
void DicomDrawerSVG::DrawSegmentColorAndStyle( G4double x1, G4double y1, G4double x2, G4double y2, G4String name, G4bool bDrawText )
{
  static DicomLineDrawingOptions defaultOptions;
  DicomLineDrawingOptions* ldo = &defaultOptions;
  vpsldo::const_iterator ite;
  for( ite = theLineDrawingOptions.begin(); ite != theLineDrawingOptions.end(); ite++ ) {
    if( GmGenUtils::AreWordsEquivalent( ite->first, name ) ) {
      ldo = (*ite).second;
      break;
    }
  }
  x1 = ToFrame(0,x1); x2 = ToFrame(0,x2); // to frame coordinates (only different if there is a background image)
  y1 = ToFrame(1,y1); y2 = ToFrame(1,y2);
  std::string color = GmSVGCanvas::RootColorToSVG(ldo->GetColorIndex());
  if( ldo->GetOption() >= 1 ) {
    theFrame->DrawLine( x1, y1, x2, y2, color, ldo->GetWidth(),
			GmSVGCanvas::RootLineStyleToDash(ldo->GetStyle(),ldo->GetWidth()), false ); // not clipped to the frame, as in ROOT
  }
  if( bDrawText && ldo->GetOption() == 2 ) {
    G4double px1 = theFrame->ToPixX(x1), py1 = theFrame->ToPixY(y1);
    G4double px2 = theFrame->ToPixX(x2), py2 = theFrame->ToPixY(y2);
    G4double angle = std::atan2( -(py2-py1), px2-px1 )*180./CLHEP::pi; // angle as seen in the figure
    GmSVGPad* pad = theFrame->GetPad();
    G4double sizePx = ldo->GetTextSize()*std::min(pad->GetPixWidth(),pad->GetPixHeight());
    theCanvas->Text( px1, py1, name, sizePx, color, "start", angle );
  }
}
