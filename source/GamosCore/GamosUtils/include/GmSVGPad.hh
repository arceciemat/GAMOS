#ifndef GmSVGPad__HH
#define GmSVGPad__HH
//---------------------------------------------------------------------------
// Part of the GAMOS SVG plotting classes (GmSVGCanvas, GmSVGPad, GmSVGFrame),
// see GmSVGCanvas.hh
//---------------------------------------------------------------------------
#include <string>

class GmSVGCanvas;

//---------------------------------------------------------------------------
// Pad: a rectangular region of a canvas, in NDC
class GmSVGPad
{
public:
  // canvas = 0: current canvas (last created or GmSVGCanvas::cd())
  GmSVGPad( const std::string& name, const std::string& title,
	    double x1, double y1, double x2, double y2, GmSVGCanvas* canvas = 0 );
  void Draw( const char* = "" ) {}
  void cd();
  void SetLogx( int val ) { bLogX = (val != 0); }
  void SetLogy( int val ) { bLogY = (val != 0); }
  int GetLogx() const { return bLogX; }
  int GetLogy() const { return bLogY; }
  GmSVGCanvas* GetCanvas() const { return theCanvas; }

  // pad limits in canvas pixels (y downwards)
  double GetPixXMin() const;
  double GetPixXMax() const;
  double GetPixYTop() const;
  double GetPixYBottom() const;
  double GetPixWidth() const { return GetPixXMax() - GetPixXMin(); }
  double GetPixHeight() const { return GetPixYBottom() - GetPixYTop(); }

  double fX1, fY1, fX2, fY2; // NDC
  bool bLogX, bLogY;

private:
  GmSVGCanvas* theCanvas;
};


#endif
