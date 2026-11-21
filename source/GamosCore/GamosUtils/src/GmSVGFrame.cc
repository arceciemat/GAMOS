#include "GmSVGFrame.hh"
#include "GmSVGPad.hh"
#include "GmSVGCanvas.hh"

#include <sstream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <algorithm>

namespace {
  //--- 10^n: plain number when short, else with Unicode superscripts
  std::string powerOfTen( int nn )
  {
    static const char* sup[10] = { "\xE2\x81\xB0", "\xC2\xB9", "\xC2\xB2", "\xC2\xB3", "\xE2\x81\xB4",
				   "\xE2\x81\xB5", "\xE2\x81\xB6", "\xE2\x81\xB7", "\xE2\x81\xB8", "\xE2\x81\xB9" };
    if( nn >= 0 && nn <= 3 ) return std::to_string(int(std::pow(10.,nn)+0.5));
    if( nn < 0 && nn >= -3 ) return "0." + std::string(-nn-1,'0') + "1";
    std::string str = "10";
    if( nn < 0 ) str += "\xE2\x81\xBB"; // superscript minus
    std::string digits = std::to_string(std::abs(nn));
    for( size_t ii = 0; ii < digits.size(); ii++ ) str += sup[digits[ii]-'0'];
    return str;
  }
}

//---------------------------------------------------------------------------
GmSVGCanvas* GmSVGFrame::GetCanvas() const
{
  return thePad->GetCanvas();
}

//-----------------------------------------------------------------------
GmSVGFrame::GmSVGFrame( const std::string& name, GmSVGPad* pad,
			double xMin, double xMax, double yMin, double yMax,
			bool logX, bool logY,
			double marginLeft, double marginRight,
			double marginBottom, double marginTop )
  : theName(name), thePad(pad), fXMin(xMin), fXMax(xMax), fYMin(yMin), fYMax(yMax),
    bLogX(logX), bLogY(logY), bReverseLabelsX(false), bReverseLabelsY(false)
{
  if( bLogX && (fXMin <= 0. || fXMax <= 0.) ) bLogX = false;
  if( bLogY && (fYMin <= 0. || fYMax <= 0.) ) bLogY = false;
  if( fXMax == fXMin ) { fXMin -= 0.5; fXMax += 0.5; }
  if( fYMax == fYMin ) { fYMin -= 0.5; fYMax += 0.5; }
  fPixL = pad->GetPixXMin() + marginLeft*pad->GetPixWidth();
  fPixR = pad->GetPixXMax() - marginRight*pad->GetPixWidth();
  fPixT = pad->GetPixYTop() + marginTop*pad->GetPixHeight();
  fPixB = pad->GetPixYBottom() - marginBottom*pad->GetPixHeight();
}

double GmSVGFrame::ToPixX( double xx ) const
{
  double frac;
  if( bLogX ) {
    if( xx <= 0. ) xx = fXMin*1.e-3;
    frac = std::log(xx/fXMin)/std::log(fXMax/fXMin);
  } else {
    frac = (xx-fXMin)/(fXMax-fXMin);
  }
  return FracToPixX(frac);
}

double GmSVGFrame::ToPixY( double yy ) const
{
  double frac;
  if( bLogY ) {
    if( yy <= 0. ) yy = fYMin*1.e-3;
    frac = std::log(yy/fYMin)/std::log(fYMax/fYMin);
  } else {
    frac = (yy-fYMin)/(fYMax-fYMin);
  }
  return FracToPixY(frac);
}

bool GmSVGFrame::IsInsideX( double xx ) const
{
  double tol = 1.e-9*std::fabs(fXMax-fXMin);
  return xx >= fXMin-tol && xx <= fXMax+tol;
}

bool GmSVGFrame::IsInsideY( double yy ) const
{
  double tol = 1.e-9*std::fabs(fYMax-fYMin);
  return yy >= fYMin-tol && yy <= fYMax+tol;
}

bool GmSVGFrame::ClipLine( double& x1, double& y1, double& x2, double& y2 ) const
{
  //--- Liang-Barsky clipping of a segment (canvas pixels) to the frame box
  double dx = x2-x1, dy = y2-y1;
  double t0 = 0., t1 = 1.;
  double pp[4] = { -dx, dx, -dy, dy };
  double qq[4] = { x1-fPixL, fPixR-x1, y1-fPixT, fPixB-y1 };
  for( int ii = 0; ii < 4; ii++ ) {
    if( pp[ii] == 0. ) {
      if( qq[ii] < 0. ) return false;
    } else {
      double tt = qq[ii]/pp[ii];
      if( pp[ii] < 0. ) {
	if( tt > t1 ) return false;
	if( tt > t0 ) t0 = tt;
      } else {
	if( tt < t0 ) return false;
	if( tt < t1 ) t1 = tt;
      }
    }
  }
  double nx1 = x1 + t0*dx, ny1 = y1 + t0*dy;
  x2 = x1 + t1*dx; y2 = y1 + t1*dy;
  x1 = nx1; y1 = ny1;
  return true;
}

void GmSVGFrame::DrawLine( double x1, double y1, double x2, double y2,
			   const std::string& color, double width, const std::string& dash, bool bClip )
{
  double px1 = ToPixX(x1), py1 = ToPixY(y1);
  double px2 = ToPixX(x2), py2 = ToPixY(y2);
  if( !bClip || ClipLine(px1,py1,px2,py2) ) GetCanvas()->Line( px1, py1, px2, py2, color, width, dash );
}

//---------------------------------------------------------------------------
std::vector<double> GmSVGFrame::GetTicks( double vmin, double vmax, bool bLog, std::vector<double>& minorTicks, int nDiv )
{
  std::vector<double> ticks;
  minorTicks.clear();
  if( vmax <= vmin ) return ticks;
  if( bLog ) {
    int nmin = int(std::floor(std::log10(vmin)));
    int nmax = int(std::ceil(std::log10(vmax)));
    for( int nn = nmin; nn <= nmax; nn++ ) {
      double dec = std::pow(10.,nn);
      if( dec >= vmin*(1-1.e-9) && dec <= vmax*(1+1.e-9) ) ticks.push_back(dec);
      for( int kk = 2; kk <= 9; kk++ ) {
	double val = kk*dec;
	if( val >= vmin && val <= vmax ) minorTicks.push_back(val);
      }
    }
    if( ticks.size() >= 2 ) return ticks;
    //--- less than two decades: use linear ticks
    minorTicks.clear();
    ticks.clear();
  }
  double range = vmax - vmin;
  double rough = range/std::max(nDiv,1);
  double mag = std::pow(10.,std::floor(std::log10(rough)));
  double norm = rough/mag;
  double step = (norm < 1.5 ? 1. : (norm < 3. ? 2. : (norm < 7. ? 5. : 10.)))*mag;
  double minorStep = step/((std::fabs(step/mag-2.) < 1.e-6) ? 4. : 5.);
  double tol = step*1.e-6;
  for( double val = std::ceil(vmin/minorStep-1.e-6)*minorStep; val <= vmax+tol; val += minorStep ) {
    minorTicks.push_back(val);
  }
  for( double val = std::ceil(vmin/step-1.e-6)*step; val <= vmax+tol; val += step ) {
    ticks.push_back(std::fabs(val) < tol ? 0. : val);
  }
  return ticks;
}

//---------------------------------------------------------------------------
std::string GmSVGFrame::FormatLabel( double val, double step )
{
  if( std::fabs(val) < step*1.e-6 ) val = 0.;
  //--- with very small or big steps the other labels are in scientific notation: write 0 as "0"
  //    (with fixed notation it would have as many decimals as the step and overlap the next label)
  if( val == 0. && (step < 1.e-3 || step >= 1.e5) ) return "0";
  std::ostringstream os;
  double aval = std::fabs(val);
  if( val != 0. && (aval >= 1.e5 || aval < 1.e-3) ) {
    os << std::setprecision(3) << val;
  } else {
    int ndec = 0;
    if( step < 1. ) ndec = int(std::ceil(-std::log10(step)-1.e-9));
    if( ndec < 0 ) ndec = 0;
    os << std::fixed << std::setprecision(ndec) << val;
  }
  return os.str();
}

//---------------------------------------------------------------------------
void GmSVGFrame::DrawAxes( const std::string& xTitle, const std::string& yTitle,
			   double xTitleSize, double yTitleSize,
			   double xLabelSize, double yLabelSize,
			   double xTitleOffset, double yTitleOffset )
{
  GmSVGCanvas* canvas = GetCanvas();
  const std::string black = "#000000";
  double padSize = std::min(thePad->GetPixWidth(),thePad->GetPixHeight());
  double xLabelPx = xLabelSize*padSize;
  double yLabelPx = yLabelSize*padSize;
  double xTitlePx = xTitleSize*padSize;
  double yTitlePx = yTitleSize*padSize;
  double frameW = fPixR - fPixL;
  double frameH = fPixB - fPixT;
  double majorX = 0.03*frameH;
  double majorY = 0.03*frameW;

  canvas->Rect( fPixL, fPixT, fPixR, fPixB, black, "none", 1. );

  //--- X axis: at most 8 divisions, at least ~5 label sizes between labels
  std::vector<double> minor;
  int nDivX = std::max(2,std::min(8,int(frameW/(5.*xLabelPx))));
  std::vector<double> ticks = GetTicks( fXMin, fXMax, bLogX, minor, nDivX );
  double step = ticks.size() >= 2 ? ticks[1]-ticks[0] : (fXMax-fXMin);
  for( size_t ii = 0; ii < minor.size(); ii++ ) {
    double px = ToPixX( bReverseLabelsX ? fXMin+fXMax-minor[ii] : minor[ii] );
    canvas->Line( px, fPixB, px, fPixB-majorX/2., black, 1. );
  }
  for( size_t ii = 0; ii < ticks.size(); ii++ ) {
    double px = ToPixX( bReverseLabelsX ? fXMin+fXMax-ticks[ii] : ticks[ii] );
    canvas->Line( px, fPixB, px, fPixB-majorX, black, 1. );
    std::string label;
    if( bLogX && ticks.size() >= 2 && minor.size() != 0 ) {
      label = powerOfTen( int(std::floor(std::log10(ticks[ii])+0.5)) );
    } else {
      label = FormatLabel(ticks[ii],step);
    }
    canvas->Text( px, fPixB + xLabelPx*1.2, label, xLabelPx, black, "middle" );
  }
  if( xTitle != "" ) {
    double py = fPixB + xLabelPx*1.3 + xTitlePx*1.3*xTitleOffset;
    canvas->Text( fPixR, py, xTitle, xTitlePx, black, "end" );
  }

  //--- Y axis
  int nDivY = std::max(2,std::min(8,int(frameH/(2.5*yLabelPx))));
  ticks = GetTicks( fYMin, fYMax, bLogY, minor, nDivY );
  step = ticks.size() >= 2 ? ticks[1]-ticks[0] : (fYMax-fYMin);
  double maxLabelWidth = 0.;
  for( size_t ii = 0; ii < minor.size(); ii++ ) {
    double py = ToPixY( bReverseLabelsY ? fYMin+fYMax-minor[ii] : minor[ii] );
    canvas->Line( fPixL, py, fPixL+majorY/2., py, black, 1. );
  }
  for( size_t ii = 0; ii < ticks.size(); ii++ ) {
    double py = ToPixY( bReverseLabelsY ? fYMin+fYMax-ticks[ii] : ticks[ii] );
    canvas->Line( fPixL, py, fPixL+majorY, py, black, 1. );
    std::string label;
    if( bLogY && ticks.size() >= 2 && minor.size() != 0 ) {
      label = powerOfTen( int(std::floor(std::log10(ticks[ii])+0.5)) );
    } else {
      label = FormatLabel(ticks[ii],step);
    }
    canvas->Text( fPixL - yLabelPx*0.4, py + yLabelPx*0.35, label, yLabelPx, black, "end" );
    maxLabelWidth = std::max(maxLabelWidth, GmSVGCanvas::TextWidth(label,yLabelPx));
  }
  if( yTitle != "" ) {
    // rotated text: glyphs extend ~0.8*size to the left of px: keep them inside the canvas
    double px = fPixL - yLabelPx*0.4 - maxLabelWidth - yTitlePx*0.3*yTitleOffset;
    if( px < 0.8*yTitlePx ) px = 0.8*yTitlePx;
    canvas->Text( px, fPixT, yTitle, yTitlePx, black, "end", 90. );
  }
}
