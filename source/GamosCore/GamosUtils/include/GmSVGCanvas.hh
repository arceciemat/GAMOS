#ifndef GmSVGCanvas__HH
#define GmSVGCanvas__HH
//---------------------------------------------------------------------------
// GmSVGCanvas, GmSVGPad, GmSVGFrame: minimal plotting engine without external dependencies (only zlib, that is
// always available with Geant4), to produce figures when GAMOS is built
// without ROOT (GAMOS_NO_ROOT). It can be used also with ROOT.
//
// It writes the figure as PNG, JPG, GIF, BMP or TIFF with the built-in renderer
// (GmSVGRaster, no external program; the figures are rendered and written in
// background threads, see WaitForFigures), or as an SVG file (plain text, it can be opened
// with any web browser), that is converted to other formats (PDF, PS, EPS, ...)
// calling an external program if it is found in the PATH (rsvg-convert, inkscape, magick or convert).
// The SVG file is only kept if the format is svg or an external program is used.
// Environment variable GM_FIGURE_NTHREADS: maximum number of background threads (default:
// number of cores - 1, at most 16; 0: the figures are written by the main thread).
// The output format can be forced for all figures with SetImageFormat (user command /gamos/SVG/imageFormat).
//
// Coordinates: canvas and pads use NDC (0..1, origin at bottom-left, as ROOT);
// frames (GmSVGFrame) use user coordinates, linear or logarithmic.
// Colors are SVG strings ("#rrggbb"); RootColorToSVG converts ROOT color indices.
//---------------------------------------------------------------------------
#include <string>
#include <vector>
#include "GmSVGRaster.hh"

class GmSVGPad;

// Canvas: collects SVG elements and writes them
class GmSVGCanvas
{
public:
  GmSVGCanvas( int ww = 750, int wh = 750 );
  ~GmSVGCanvas();

  void SetCanvasSize( int ww, int wh ) { theWw = ww; theWh = wh; }
  int GetWw() const { return theWw; }
  int GetWh() const { return theWh; }
  void cd();  // set this canvas and its full pad as current
  void Draw( const char* = "" ) {}
  void SetLogx( int ) {}
  void SetLogy( int ) {}
  void Clear();

  GmSVGPad* GetCurrentPad() const { return theCurrentPad; }
  void SetCurrentPad( GmSVGPad* pad ) { theCurrentPad = pad; theCurrentCanvas = this; }
  static GmSVGCanvas* GetCurrentCanvas() { return theCurrentCanvas; }

  //--- primitives in canvas pixels (origin top-left, y downwards)
  void Line( double x1, double y1, double x2, double y2,
	     const std::string& color, double width = 1., const std::string& dash = "" );
  void Rect( double x1, double y1, double x2, double y2,
	     const std::string& stroke, const std::string& fill, double width = 1. );
  // ROOT-like marker styles: 20-34 (other values are drawn as full circle)
  void Marker( double xx, double yy, int style, const std::string& color, double size );
  // anchor: "start", "middle", "end"; angle in degrees, counterclockwise;
  // bMarkup: text contains SVG markup and is not escaped
  void Text( double xx, double yy, const std::string& text, double sizePix,
	     const std::string& color, const std::string& anchor = "start", double angle = 0.,
	     bool bMarkup = false );
  // Raster image (nx*ny pixels, RGBA, row 0 at the top) drawn in the rectangle (x1,y1)-(x2,y2);
  // it is embedded in the SVG as a compressed PNG
  void Image( double x1, double y1, double x2, double y2, int nx, int ny,
	      const std::vector<unsigned char>& rgba );
  // Approximate width (pixels) of a text in Helvetica/Arial
  static double TextWidth( const std::string& text, double sizePix );

  //--- write SVG file; bEnlargeImages: raster images are enlarged to the drawn size (nearest neighbour),
  //    so that external converters that interpolate images do not blur them
  bool WriteSVG( const std::string& fileName, bool bEnlargeImages = true ) const;
  // Convert SVG file to format (png, jpg, pdf) with an external program:
  //   "auto": look for rsvg-convert, inkscape, magick, convert in PATH
  //   "none": do not convert
  //   otherwise: full path or name of the program (its type is deduced from its name)
  static bool ConvertSVG( const std::string& svgFile, const std::string& format,
			  const std::string& converter = "auto" );
  // Write the figure in format:
  //   png, jpg, gif, bmp, tiff: with the built-in renderer if converter is "auto" or "builtin" (no SVG file)
  //   other formats (pdf, ps, eps, ...) or another converter: SVG file and conversion of it (ConvertSVG)
  //   svg, or converter "none": only the SVG file
  bool Print( const std::string& svgFile, const std::string& format = "png",
	      const std::string& converter = "auto" ) const;
  // Wait until all the figures being written in background threads are finished
  static void WaitForFigures();
  static std::string FindInPath( const std::string& program );
  // Force the output format of all figures (overrides the format passed to Print/ConvertSVG);
  // "" : use the format passed by the caller (default); "svg": no conversion
  static void SetImageFormat( const std::string& format ) { theImageFormat = format; }
  static const std::string& GetImageFormat() { return theImageFormat; }

  //--- utilities
  // convert most common ROOT TLatex symbols (#chi, ^{2}, #pm, ...) to Unicode
  static std::string LatexToUnicode( const std::string& text );
  // ROOT color index (kRed, kBlue+2, ...) to SVG color
  static std::string RootColorToSVG( int color );
  // ROOT line style (1 solid, 2 dashed, 3 dotted, 4 dash-dotted, ...) to SVG stroke-dasharray
  static std::string RootLineStyleToDash( int style, double width = 1. );
  static std::string RGBToSVG( double rr, double gg, double bb );
  //--- color palette, as ROOT TColor::SetPalette: 1 (old rainbow, 50 colors) or 51-113 (255 colors,
  //    e.g. 55 kRainBow, 57 kBird, the default of ROOT 6 and of this class)
  static void SetPalette( int number );
  static int GetPalette();
  static int GetPaletteNColors();
  // color of palette index (0 .. GetPaletteNColors()-1)
  static void PaletteColor( int index, unsigned char& rr, unsigned char& gg, unsigned char& bb );
  // color for a fraction 0..1 (index = int(frac*nColors))
  static void PaletteRGB( double frac, unsigned char& rr, unsigned char& gg, unsigned char& bb );

private:
  static std::string EscapeXML( const std::string& text );
  // PNG of the nx*ny RGBA image enlarged kx*ky times (nearest neighbour); it is written as a palette
  // image if it has at most 256 colors. Rows that repeat the previous one use PNG filter 'Up' (all zeros)
  static std::string EncodePNG( int nx, int ny, const std::vector<unsigned char>& rgba, int kx = 1, int ky = 1 );
  static std::string Base64( const std::string& data );
  std::string ImageElement( const GmSVGPrim& prim, bool bEnlarge ) const;
  int theWw, theWh;
  std::vector<std::string> theElements; // SVG elements ("\x01" + index in thePrims for the images, encoded when writing)
  std::vector<GmSVGPrim> thePrims;      // primitives for the built-in renderer
  GmSVGPad* theFullPad;
  GmSVGPad* theCurrentPad;
  static GmSVGCanvas* theCurrentCanvas;
  static std::string theImageFormat;
  static int thePalette;
  static std::vector<unsigned char> thePaletteTable; // RGB of each color
};

#endif
