#ifndef GmSVGRaster__HH
#define GmSVGRaster__HH
//---------------------------------------------------------------------------
// Built-in raster renderer of the GmSVGCanvas figures, so that PNG, JPG, GIF,
// BMP and TIFF files are written without any external program.
//
// GmSVGCanvas keeps, besides the SVG text, a list of the drawn primitives
// (GmSVGPrim); GmSVGRaster paints them with antialiasing (exact area coverage)
// on an RGB image and encodes it.
// Text is drawn with the Lato Regular TrueType font included in Geant4
// (tools/font/lato_regular_ttf), and Roboto Bold for the Greek letters.
// JPEG encoder derived from TooJpeg (Stephan Brumme, zlib license; included in Geant4 as
// tools/toojpeg), modified to encode faster the uniform blocks.
//---------------------------------------------------------------------------
#include <string>
#include <vector>

//---------------------------------------------------------------------------
// Primitive drawn on a GmSVGCanvas, in canvas pixels (origin top-left, y downwards)
struct GmSVGPrim
{
  enum Type { kLine, kRect, kPolygon, kCircle, kText, kImage };
  Type type;
  std::vector<double> pts; // kLine, kRect, kImage: x1,y1,x2,y2; kPolygon: x,y,...; kCircle: cx,cy,r; kText: x,y
  unsigned int stroke; // 0xRRGGBBAA; alpha 0: no stroke
  unsigned int fill;   // 0xRRGGBBAA; alpha 0: no fill
  double width;        // stroke width
  std::vector<double> dash; // dash pattern (lengths in pixels), empty: solid
  //--- text
  std::string text;    // UTF-8
  double size;         // font size (pixels)
  int anchor;          // 0: start, 1: middle, 2: end
  double angle;        // degrees, counterclockwise
  //--- image (RGBA, row 0 at the top)
  int nx, ny;
  std::vector<unsigned char> rgba;

  GmSVGPrim( Type typ ) : type(typ), stroke(0), fill(0), width(1.), size(0.), anchor(0), angle(0.), nx(0), ny(0) {}
};

//---------------------------------------------------------------------------
class GmSVGRaster
{
public:
  GmSVGRaster( int ww, int wh );
  ~GmSVGRaster();
  GmSVGRaster( const GmSVGRaster& ) = delete;
  GmSVGRaster& operator=( const GmSVGRaster& ) = delete;

  // Paint the primitives (on a white background)
  void Render( const std::vector<GmSVGPrim>& prims );
  // Write image file in format png, jpg, gif, bmp or tiff
  bool Write( const std::string& fileName, const std::string& format ) const;
  // Formats that can be written without external program
  static bool IsFormatSupported( const std::string& format );
  // Quality of JPEG files (1-100, default 90)
  static void SetJpegQuality( int quality );
  static int GetJpegQuality();

  // Width of a text (pixels) with the font used for drawing
  static double TextWidth( const std::string& text, double sizePix );
  // Color string "#rrggbb", "#rgb", "none" or a few names to 0xRRGGBBAA (alpha 0 for "none")
  static unsigned int ParseColor( const std::string& color );

  int GetWidth() const { return theW; }
  int GetHeight() const { return theH; }
  const std::vector<unsigned char>& GetRGB() const { return theRGB; }

  //--- primitives (canvas pixels)
  void FillPolygons( const std::vector< std::vector<double> >& polys, unsigned int color, bool bNormalize );
  void StrokeLine( double x1, double y1, double x2, double y2, double width, unsigned int color,
		   const std::vector<double>& dash );
  void FillRect( double xa, double ya, double xb, double yb, unsigned int color ); // axis-aligned, xa<xb, ya<yb
  void DrawRect( double x1, double y1, double x2, double y2, unsigned int stroke, unsigned int fill, double width );
  void DrawPolygon( const std::vector<double>& pts, unsigned int stroke, unsigned int fill, double width );
  void DrawCircle( double cx, double cy, double rr, unsigned int stroke, unsigned int fill, double width );
  void DrawImage( double x1, double y1, double x2, double y2, int nx, int ny, const std::vector<unsigned char>& rgba );
  void DrawText( double xx, double yy, const std::string& text, double sizePix, unsigned int color,
		 int anchor, double angle );

private:
  bool WritePNG( std::string& out ) const;
  bool WriteJPG( std::string& out ) const;
  bool WriteGIF( std::string& out ) const;
  bool WriteBMP( std::string& out ) const;
  bool WriteTIFF( std::string& out ) const;

  int theW, theH;
  std::vector<unsigned char> theRGB;
  static int theJpegQuality;
};

#endif
