#ifndef GmSVGFrame__HH
#define GmSVGFrame__HH
//---------------------------------------------------------------------------
// Part of the GAMOS SVG plotting classes (GmSVGCanvas, GmSVGPad, GmSVGFrame),
// see GmSVGCanvas.hh
//---------------------------------------------------------------------------
#include <string>
#include <vector>
class GmSVGPad;

class GmSVGCanvas;

// Frame: axes with user coordinates drawn inside a pad
class GmSVGFrame
{
public:
  GmSVGFrame( const std::string& name, GmSVGPad* pad,
	      double xMin, double xMax, double yMin, double yMax,
	      bool bLogX, bool bLogY,
	      double marginLeft, double marginRight,
	      double marginBottom, double marginTop );

  std::string GetName() const { return theName; }
  double GetXmin() const { return fXMin; }
  double GetXmax() const { return fXMax; }
  double GetMinimum() const { return fYMin; }
  double GetMaximum() const { return fYMax; }

  // user coordinates -> canvas pixels
  double ToPixX( double xx ) const;
  double ToPixY( double yy ) const;
  // fraction of frame (0..1, from left/bottom) -> canvas pixels
  double FracToPixX( double fx ) const { return fPixL + fx*(fPixR-fPixL); }
  double FracToPixY( double fy ) const { return fPixB - fy*(fPixB-fPixT); }
  bool IsInsideX( double xx ) const;
  bool IsInsideY( double yy ) const;
  // clip a segment (canvas pixels) to the frame box; false if it is fully outside
  bool ClipLine( double& x1, double& y1, double& x2, double& y2 ) const;

  // Labels show the axis reversed (as ROOT TGaxis with inverted limits)
  void SetReverseLabelsX( bool val ) { bReverseLabelsX = val; }
  void SetReverseLabelsY( bool val ) { bReverseLabelsY = val; }

  // Draw the frame box, ticks, labels and axis titles. Sizes are fractions of the pad size
  void DrawAxes( const std::string& xTitle, const std::string& yTitle,
		 double xTitleSize, double yTitleSize,
		 double xLabelSize, double yLabelSize,
		 double xTitleOffset, double yTitleOffset );

  // Line in user coordinates, clipped to the frame if bClip (else it may extend outside the frame, as ROOT TLine)
  void DrawLine( double x1, double y1, double x2, double y2,
		 const std::string& color, double width = 1., const std::string& dash = "", bool bClip = false );

  GmSVGPad* GetPad() const { return thePad; }
  GmSVGCanvas* GetCanvas() const;
  double fPixL, fPixR, fPixT, fPixB; // frame box in canvas pixels

  // Nice tick positions between vmin and vmax (about nDiv divisions)
  static std::vector<double> GetTicks( double vmin, double vmax, bool bLog, std::vector<double>& minorTicks, int nDiv = 8 );
  static std::string FormatLabel( double val, double step );

private:
  std::string theName;
  GmSVGPad* thePad;
  double fXMin, fXMax, fYMin, fYMax;
  bool bLogX, bLogY;
  bool bReverseLabelsX, bReverseLabelsY;
};


#endif
