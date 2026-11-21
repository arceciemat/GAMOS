#include "GmSVGCanvas.hh"
#include "GmSVGPad.hh"

#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <algorithm>
#include <utility>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "zlib.h" // provided by Geant4 (G4zlib) or by the system
#ifdef _WIN32
#include <io.h>
#define GMSVG_ACCESS(path) (_access(path,0) == 0)
#else
#include <unistd.h>
#define GMSVG_ACCESS(path) (access(path,X_OK) == 0)
#endif
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

GmSVGCanvas* GmSVGCanvas::theCurrentCanvas = 0;
std::string GmSVGCanvas::theImageFormat = "";
int GmSVGCanvas::thePalette = 0;
std::vector<unsigned char> GmSVGCanvas::thePaletteTable;

#include "GmSVGPalettes.icc"

namespace {
  //--- format a number for SVG coordinates
  std::string fmt( double val )
  {
    std::ostringstream os;
    os << std::fixed << std::setprecision(2) << val;
    return os.str();
  }

  //--- SVG stroke-dasharray to lengths
  std::vector<double> ParseDash( const std::string& dash )
  {
    std::vector<double> vals;
    std::string str = dash;
    std::replace(str.begin(),str.end(),',',' ');
    std::istringstream is(str);
    double val;
    while( is >> val ) vals.push_back(val);
    return vals;
  }

  //--- figures rendered and written in background threads
  struct FigureJobs {
    std::mutex mtx;
    std::condition_variable cv;
    int nRunning = 0;
    void WaitAll() {
      std::unique_lock<std::mutex> lock(mtx);
      cv.wait( lock, [this]{ return nRunning == 0; } );
    }
    ~FigureJobs() { WaitAll(); } // at the end of the program
  };
  FigureJobs& Jobs()
  {
    static FigureJobs jobs;
    return jobs;
  }
  int MaxFigureThreads()
  {
    static int nThreads = -1;
    if( nThreads < 0 ) {
      const char* env = std::getenv("GM_FIGURE_NTHREADS");
      if( env ) {
	nThreads = std::max(0,std::atoi(env));
      } else {
	unsigned int hw = std::thread::hardware_concurrency();
	nThreads = int(std::min(16u,hw > 1 ? hw-1 : 1u));
      }
    }
    return nThreads;
  }
  void RenderAndWrite( int ww, int wh, const std::vector<GmSVGPrim>& prims, const std::string& fileName,
		       const std::string& format )
  {
    GmSVGRaster raster(ww,wh);
    raster.Render(prims);
    if( raster.Write(fileName,format) ) {
      std::string msg = " GmSVGCanvas: figure written to " + fileName + "\n";
      std::cout << msg << std::flush;
    }
  }

  //--- remove SVG markup tags (for the built-in renderer)
  std::string StripTags( const std::string& text )
  {
    std::string out;
    bool bTag = false;
    for( size_t ii = 0; ii < text.size(); ii++ ) {
      if( text[ii] == '<' ) { bTag = true; continue; }
      if( text[ii] == '>' ) { bTag = false; continue; }
      if( !bTag ) out += text[ii];
    }
    return out;
  }
}

//---------------------------------------------------------------------------
GmSVGCanvas::GmSVGCanvas( int ww, int wh )
  : theWw(ww), theWh(wh)
{
  theCurrentCanvas = this;
  theFullPad = new GmSVGPad("","",0.,0.,1.,1.,this);
  theCurrentPad = theFullPad;
}

GmSVGCanvas::~GmSVGCanvas()
{
  delete theFullPad;
  if( theCurrentCanvas == this ) theCurrentCanvas = 0;
}

void GmSVGCanvas::cd()
{
  theCurrentPad = theFullPad;
  theCurrentCanvas = this;
}

void GmSVGCanvas::Clear()
{
  theElements.clear();
  thePrims.clear();
  theCurrentPad = theFullPad;
}

//---------------------------------------------------------------------------
void GmSVGCanvas::Line( double x1, double y1, double x2, double y2,
			const std::string& color, double width, const std::string& dash )
{
  std::string el = "<line x1=\"" + fmt(x1) + "\" y1=\"" + fmt(y1) + "\" x2=\"" + fmt(x2) + "\" y2=\"" + fmt(y2)
    + "\" stroke=\"" + color + "\" stroke-width=\"" + fmt(width) + "\"";
  if( dash != "" ) el += " stroke-dasharray=\"" + dash + "\"";
  el += "/>";
  theElements.push_back(el);
  GmSVGPrim prim(GmSVGPrim::kLine);
  prim.pts = { x1, y1, x2, y2 };
  prim.stroke = GmSVGRaster::ParseColor(color);
  prim.width = width;
  prim.dash = ParseDash(dash);
  thePrims.push_back(std::move(prim));
}

//---------------------------------------------------------------------------
void GmSVGCanvas::Rect( double x1, double y1, double x2, double y2,
			const std::string& stroke, const std::string& fill, double width )
{
  double xx = std::min(x1,x2);
  double yy = std::min(y1,y2);
  theElements.push_back( "<rect x=\"" + fmt(xx) + "\" y=\"" + fmt(yy)
			 + "\" width=\"" + fmt(std::fabs(x2-x1)) + "\" height=\"" + fmt(std::fabs(y2-y1))
			 + "\" stroke=\"" + stroke + "\" fill=\"" + fill + "\" stroke-width=\"" + fmt(width) + "\"/>" );
  GmSVGPrim prim(GmSVGPrim::kRect);
  prim.pts = { x1, y1, x2, y2 };
  prim.stroke = GmSVGRaster::ParseColor(stroke);
  prim.fill = GmSVGRaster::ParseColor(fill);
  prim.width = width;
  thePrims.push_back(std::move(prim));
}

//---------------------------------------------------------------------------
void GmSVGCanvas::Marker( double xx, double yy, int style, const std::string& color, double size )
{
  double rr = 4.*size; // ROOT: marker size 1 is ~8 pixels
  std::string sw = fmt(std::max(1.,size));
  auto polygon = [&]( const std::vector<double>& pts, bool bFill ) {
    std::string el = "<polygon points=\"";
    for( size_t ii = 0; ii+1 < pts.size(); ii += 2 ) {
      el += fmt(xx+pts[ii]*rr) + "," + fmt(yy+pts[ii+1]*rr) + " ";
    }
    el += "\" fill=\"" + (bFill ? color : std::string("none")) + "\" stroke=\"" + color + "\" stroke-width=\"" + sw + "\"/>";
    theElements.push_back(el);
    GmSVGPrim prim(GmSVGPrim::kPolygon);
    for( size_t ii = 0; ii+1 < pts.size(); ii += 2 ) {
      prim.pts.push_back(xx+pts[ii]*rr);
      prim.pts.push_back(yy+pts[ii+1]*rr);
    }
    prim.stroke = GmSVGRaster::ParseColor(color);
    prim.fill = bFill ? prim.stroke : 0;
    prim.width = std::max(1.,size);
    thePrims.push_back(std::move(prim));
  };
  auto circle = [&]( bool bFill ) {
    theElements.push_back( "<circle cx=\"" + fmt(xx) + "\" cy=\"" + fmt(yy) + "\" r=\"" + fmt(rr)
			   + "\" fill=\"" + (bFill ? color : std::string("none")) + "\" stroke=\"" + color + "\" stroke-width=\"" + sw + "\"/>" );
    GmSVGPrim prim(GmSVGPrim::kCircle);
    prim.pts = { xx, yy, rr };
    prim.stroke = GmSVGRaster::ParseColor(color);
    prim.fill = bFill ? prim.stroke : 0;
    prim.width = std::max(1.,size);
    thePrims.push_back(std::move(prim));
  };
  std::vector<double> star;
  for( int ii = 0; ii < 10; ii++ ) {
    double ang = M_PI/2. + ii*M_PI/5.;
    double rad = (ii%2 == 0) ? 1.2 : 0.5;
    star.push_back(rad*std::cos(ang));
    star.push_back(-rad*std::sin(ang));
  }
  std::vector<double> cross = { -0.33,-1, 0.33,-1, 0.33,-0.33, 1,-0.33, 1,0.33, 0.33,0.33,
				0.33,1, -0.33,1, -0.33,0.33, -1,0.33, -1,-0.33, -0.33,-0.33 };
  switch( style ) {
  case 1: case 6: case 7: // dot
    rr = 1.; circle(true); break;
  case 2: // +
    Line( xx-rr, yy, xx+rr, yy, color, 1. ); Line( xx, yy-rr, xx, yy+rr, color, 1. ); break;
  case 5: // x
    Line( xx-rr, yy-rr, xx+rr, yy+rr, color, 1. ); Line( xx-rr, yy+rr, xx+rr, yy-rr, color, 1. ); break;
  case 3: case 31: // *
    Line( xx-rr, yy, xx+rr, yy, color, 1. ); Line( xx, yy-rr, xx, yy+rr, color, 1. );
    Line( xx-rr*0.7, yy-rr*0.7, xx+rr*0.7, yy+rr*0.7, color, 1. ); Line( xx-rr*0.7, yy+rr*0.7, xx+rr*0.7, yy-rr*0.7, color, 1. ); break;
  case 21: case 25:
    polygon( {-1,-1, 1,-1, 1,1, -1,1}, style == 21 ); break;
  case 22: case 26:
    polygon( {0,-1.15, 1.1,0.85, -1.1,0.85}, style == 22 ); break;
  case 23: case 32:
    polygon( {0,1.15, 1.1,-0.85, -1.1,-0.85}, style == 23 ); break;
  case 27: case 33:
    polygon( {0,-1.3, 0.8,0, 0,1.3, -0.8,0}, style == 33 ); break;
  case 28: case 34:
    polygon( cross, style == 34 ); break;
  case 29: case 30:
    polygon( star, style == 29 ); break;
  case 24:
    circle(false); break;
  case 20: default:
    circle(true); break;
  }
}

//---------------------------------------------------------------------------
std::string GmSVGCanvas::LatexToUnicode( const std::string& textIn )
{
  //--- most common ROOT TLatex symbols
  static const char* conv[][2] = {
    {"#chi","\xCF\x87"}, {"#gamma","\xCE\xB3"}, {"#mu","\xCE\xBC"}, {"#sigma","\xCF\x83"},
    {"#Delta","\xCE\x94"}, {"#delta","\xCE\xB4"}, {"#alpha","\xCE\xB1"}, {"#beta","\xCE\xB2"},
    {"#pm","\xC2\xB1"}, {"#times","\xC3\x97"}, {"#circ","\xC2\xB0"},
    {"^{2}","\xC2\xB2"}, {"^{3}","\xC2\xB3"}, {"^{-1}","\xE2\x81\xBB\xC2\xB9"} };
  std::string text = textIn;
  for( size_t ic = 0; ic < sizeof(conv)/sizeof(conv[0]); ic++ ) {
    std::string from = conv[ic][0];
    size_t ip;
    while( (ip = text.find(from)) != std::string::npos ) text.replace(ip,from.size(),conv[ic][1]);
  }
  return text;
}

//---------------------------------------------------------------------------
void GmSVGCanvas::Text( double xx, double yy, const std::string& textIn, double sizePix,
			const std::string& color, const std::string& anchor, double angle, bool bMarkup )
{
  std::string text = bMarkup ? textIn : LatexToUnicode(textIn);
  //--- for the built-in renderer: original position and anchor (it uses the real text width)
  GmSVGPrim prim(GmSVGPrim::kText);
  prim.pts = { xx, yy };
  prim.text = bMarkup ? StripTags(text) : text;
  prim.size = sizePix;
  prim.fill = GmSVGRaster::ParseColor(color);
  prim.anchor = (anchor == "middle") ? 1 : (anchor == "end" ? 2 : 0);
  prim.angle = angle;
  thePrims.push_back(std::move(prim));

  std::string anchorUsed = anchor;
  if( angle != 0. && anchor != "start" && !bMarkup ) {
    //--- some renderers (e.g. ImageMagick internal) ignore text-anchor in rotated text:
    // move the start point along the text direction instead
    double shift = TextWidth(text,sizePix) * (anchor == "end" ? 1. : 0.5);
    double ang = angle*M_PI/180.;
    xx -= shift*std::cos(ang);
    yy += shift*std::sin(ang);
    anchorUsed = "start";
  }
  std::string el;
  if( angle == 0. ) {
    el = "<text x=\"" + fmt(xx) + "\" y=\"" + fmt(yy) + "\"";
  } else { // translate+rotate: understood by all renderers (rotate(a,cx,cy) is not)
    el = "<text x=\"0\" y=\"0\" transform=\"translate(" + fmt(xx) + " " + fmt(yy) + ") rotate(" + fmt(-angle) + ")\"";
  }
  el += " font-size=\"" + fmt(sizePix) + "\" fill=\"" + color + "\" text-anchor=\"" + anchorUsed + "\"";
  el += ">" + (bMarkup ? text : EscapeXML(text)) + "</text>";
  theElements.push_back(el);
}

//---------------------------------------------------------------------------
double GmSVGCanvas::TextWidth( const std::string& text, double sizePix )
{
  double width = 0.;
  for( size_t ii = 0; ii < text.size(); ii++ ) {
    unsigned char cc = text[ii];
    if( (cc & 0xC0) == 0x80 ) continue; // UTF-8 continuation byte
    if( cc == 'i' || cc == 'l' || cc == 'j' || cc == '.' || cc == ',' || cc == ':' || cc == ';' || cc == '|' || cc == '\'' || cc == '!' ) {
      width += 0.25;
    } else if( cc == 'm' || cc == 'w' || cc == 'M' || cc == 'W' ) {
      width += 0.85;
    } else if( cc == ' ' || cc == 'f' || cc == 't' || cc == 'r' || cc == '(' || cc == ')' || cc == '-' ) {
      width += 0.33;
    } else if( std::isupper(cc) ) {
      width += 0.68;
    } else {
      width += 0.55;
    }
  }
  return width*sizePix;
}

//---------------------------------------------------------------------------
std::string GmSVGCanvas::EscapeXML( const std::string& text )
{
  std::string out;
  for( size_t ii = 0; ii < text.size(); ii++ ) {
    switch( text[ii] ) {
    case '&': out += "&amp;"; break;
    case '<': out += "&lt;"; break;
    case '>': out += "&gt;"; break;
    case '"': out += "&quot;"; break;
    default: out += text[ii];
    }
  }
  return out;
}

//---------------------------------------------------------------------------
std::string GmSVGCanvas::Base64( const std::string& data )
{
  static const char* chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  std::string out;
  out.reserve((data.size()+2)/3*4);
  size_t ii = 0;
  for( ; ii+2 < data.size(); ii += 3 ) {
    unsigned int val = ((unsigned char)data[ii] << 16) | ((unsigned char)data[ii+1] << 8) | (unsigned char)data[ii+2];
    out += chars[(val>>18)&63]; out += chars[(val>>12)&63]; out += chars[(val>>6)&63]; out += chars[val&63];
  }
  if( ii < data.size() ) {
    unsigned int val = (unsigned char)data[ii] << 16;
    if( ii+1 < data.size() ) val |= (unsigned char)data[ii+1] << 8;
    out += chars[(val>>18)&63]; out += chars[(val>>12)&63];
    out += (ii+1 < data.size()) ? chars[(val>>6)&63] : '=';
    out += '=';
  }
  return out;
}

//---------------------------------------------------------------------------
std::string GmSVGCanvas::EncodePNG( int nx, int ny, const std::vector<unsigned char>& rgba, int kx, int ky )
{
  kx = std::max(1,kx);
  ky = std::max(1,ky);
  const size_t nPix = size_t(nx)*size_t(ny);
  const size_t nxBig = size_t(nx)*kx;
  const size_t nyBig = size_t(ny)*ky;

  //--- palette, if there are at most 256 different colors (usual case: a few color levels)
  std::vector<unsigned int> palette; // RGBA packed
  std::vector<unsigned char> index(nPix);
  bool bPalette = true;
  {
    unsigned int lastCol = 0, lastIdx = 0;
    bool bLast = false;
    for( size_t ip = 0; ip < nPix && bPalette; ip++ ) {
      const unsigned char* pp = &rgba[ip*4];
      unsigned int col = (unsigned int)(pp[0]) << 24 | (unsigned int)(pp[1]) << 16 | (unsigned int)(pp[2]) << 8 | pp[3];
      if( !bLast || col != lastCol ) {
	size_t ic = 0;
	for( ; ic < palette.size(); ic++ ) {
	  if( palette[ic] == col ) break;
	}
	if( ic == palette.size() ) {
	  if( palette.size() == 256 ) {
	    bPalette = false;
	    break;
	  }
	  palette.push_back(col);
	}
	lastCol = col;
	lastIdx = (unsigned int)(ic);
	bLast = true;
      }
      index[ip] = (unsigned char)(lastIdx);
    }
  }
  const size_t bpp = bPalette ? 1 : 4; // bytes per pixel

  //--- raw data: each row starts with filter type; a row equal to the previous one uses
  //  filter 2 ('Up'), so that its bytes are all 0
  const size_t rowLen = 1+bpp*nxBig;
  std::string raw(nyBig*rowLen,'\0');
  for( size_t iy = 0; iy < nyBig; iy++ ) {
    char* row = &raw[iy*rowLen];
    if( iy % ky != 0 ) {
      row[0] = char(2);
      continue;
    }
    row[0] = char(0);
    char* dst = row+1;
    const size_t iy0 = (iy/ky)*size_t(nx);
    if( bPalette ) {
      for( int ix = 0; ix < nx; ix++ ) {
	char val = char(index[iy0+ix]);
	for( int jx = 0; jx < kx; jx++ ) *dst++ = val;
      }
    } else {
      for( int ix = 0; ix < nx; ix++ ) {
	const char* sp = reinterpret_cast<const char*>(&rgba[(iy0+ix)*4]);
	for( int jx = 0; jx < kx; jx++ ) {
	  dst[0] = sp[0]; dst[1] = sp[1]; dst[2] = sp[2]; dst[3] = sp[3];
	  dst += 4;
	}
      }
    }
  }
  //--- compress with Z_RLE strategy: much faster than the default one for these images
  //  (long runs of equal bytes), with similar size
  z_stream zs;
  zs.zalloc = Z_NULL;
  zs.zfree = Z_NULL;
  zs.opaque = Z_NULL;
  if( deflateInit2( &zs, 6, Z_DEFLATED, 15, 8, Z_RLE ) != Z_OK ) {
    std::cerr << " !!! GmSVGCanvas::EncodePNG: compression initialization failed " << std::endl;
    return "";
  }
  std::string comp(deflateBound(&zs,raw.size()),'\0');
  zs.next_in = reinterpret_cast<Bytef*>(&raw[0]);
  zs.avail_in = uInt(raw.size());
  zs.next_out = reinterpret_cast<Bytef*>(&comp[0]);
  zs.avail_out = uInt(comp.size());
  int ret = deflate( &zs, Z_FINISH );
  size_t compLen = zs.total_out;
  deflateEnd(&zs);
  if( ret != Z_STREAM_END ) {
    std::cerr << " !!! GmSVGCanvas::EncodePNG: compression failed " << std::endl;
    return "";
  }
  comp.resize(compLen);

  auto be32 = []( size_t val ) {
    std::string str(4,'\0');
    str[0] = char((val>>24)&255); str[1] = char((val>>16)&255); str[2] = char((val>>8)&255); str[3] = char(val&255);
    return str;
  };
  auto chunk = [&]( const std::string& type, const std::string& data ) {
    std::string td = type + data;
    unsigned int crc = crc32( 0L, reinterpret_cast<const Bytef*>(td.data()), td.size() );
    return be32(data.size()) + td + be32(crc);
  };
  std::string ihdr = be32(nxBig) + be32(nyBig);
  ihdr += char(8); // bit depth
  ihdr += char(bPalette ? 3 : 6); // color type: palette or RGBA
  ihdr += char(0); ihdr += char(0); ihdr += char(0); // compression, filter, interlace
  std::string png = "\x89PNG\r\n\x1a\n";
  png += chunk("IHDR",ihdr);
  if( bPalette ) {
    std::string plte, trns;
    size_t nTrns = 0; // tRNS: alpha of palette entries, up to the last one that is not opaque
    for( size_t ic = 0; ic < palette.size(); ic++ ) {
      plte += char((palette[ic]>>24)&255); plte += char((palette[ic]>>16)&255); plte += char((palette[ic]>>8)&255);
      trns += char(palette[ic]&255);
      if( (palette[ic]&255) != 255 ) nTrns = ic+1;
    }
    png += chunk("PLTE",plte);
    if( nTrns != 0 ) png += chunk("tRNS",trns.substr(0,nTrns));
  }
  png += chunk("IDAT",comp) + chunk("IEND","");
  return png;
}

//---------------------------------------------------------------------------
void GmSVGCanvas::Image( double x1, double y1, double x2, double y2, int nx, int ny,
			 const std::vector<unsigned char>& rgba )
{
  if( nx <= 0 || ny <= 0 || rgba.size() < size_t(nx)*size_t(ny)*4 ) {
    std::cerr << " !!! GmSVGCanvas::Image: wrong image size " << nx << " x " << ny << " data " << rgba.size() << std::endl;
    return;
  }
  //--- the PNG is encoded when the SVG is written (it may be enlarged)
  GmSVGPrim prim(GmSVGPrim::kImage);
  prim.pts = { x1, y1, x2, y2 };
  prim.nx = nx;
  prim.ny = ny;
  prim.rgba.assign( rgba.begin(), rgba.begin()+size_t(nx)*size_t(ny)*4 );
  theElements.push_back( "\x01" + std::to_string(thePrims.size()) );
  thePrims.push_back(std::move(prim));
}

//---------------------------------------------------------------------------
std::string GmSVGCanvas::ImageElement( const GmSVGPrim& prim, bool bEnlarge ) const
{
  double x1 = prim.pts[0], y1 = prim.pts[1], x2 = prim.pts[2], y2 = prim.pts[3];
  int nx = prim.nx, ny = prim.ny;
  //--- enlarge with nearest neighbour to (at least) the drawn size, so that converters
  // that interpolate images (ignoring image-rendering:pixelated) do not blur the voxels
  int kx = 1, ky = 1;
  if( bEnlarge ) {
    kx = std::max(1,int(std::ceil(std::fabs(x2-x1)/nx)));
    ky = std::max(1,int(std::ceil(std::fabs(y2-y1)/ny)));
    kx = std::min(kx,std::max(1,4096/nx));
    ky = std::min(ky,std::max(1,4096/ny));
  }
  std::string png = EncodePNG(nx,ny,prim.rgba,kx,ky);
  if( png == "" ) return "";
  std::string elem = "<image x=\"" + fmt(std::min(x1,x2)) + "\" y=\"" + fmt(std::min(y1,y2))
    + "\" width=\"" + fmt(std::fabs(x2-x1)) + "\" height=\"" + fmt(std::fabs(y2-y1))
    + "\" preserveAspectRatio=\"none\" style=\"image-rendering:pixelated;image-rendering:crisp-edges\""
    + " xlink:href=\"data:image/png;base64,";
  elem += Base64(png);
  elem += "\"/>";
  return elem;
}

//---------------------------------------------------------------------------
std::string GmSVGCanvas::RGBToSVG( double rr, double gg, double bb )
{
  char buf[8];
  snprintf(buf, sizeof(buf), "#%02x%02x%02x",
	   int(std::max(0.,std::min(1.,rr))*255+0.5), int(std::max(0.,std::min(1.,gg))*255+0.5), int(std::max(0.,std::min(1.,bb))*255+0.5));
  return std::string(buf);
}

//---------------------------------------------------------------------------
std::string GmSVGCanvas::RootColorToSVG( int color )
{
  double rr = 0., gg = 0., bb = 0.;
  //--- basic ROOT colors 0-9
  static const double basic[10][3] = { {1,1,1}, {0,0,0}, {1,0,0}, {0,1,0}, {0,0,1},
				       {1,1,0}, {1,0,1}, {0,1,1}, {0.35,0.83,0.33}, {0.35,0.33,0.85} };
  //--- ROOT color wheel: base colors
  static const int wheelIdx[13] = { 400, 416, 432, 600, 616, 632, 800, 820, 840, 860, 880, 900, 920 };
  static const double wheel[13][3] = { {1,1,0}, {0,1,0}, {0,1,1}, {0,0,1}, {1,0,1}, {1,0,0},
				       {1,0.8,0}, {0.8,1,0}, {0,1,0.8}, {0,0.8,1}, {0.8,0,1}, {1,0,0.8}, {0.8,0.8,0.8} };
  if( color >= 0 && color <= 9 ) {
    rr = basic[color][0]; gg = basic[color][1]; bb = basic[color][2];
  } else {
    int ibest = 0;
    for( int ii = 0; ii < 13; ii++ ) {
      if( std::abs(color - wheelIdx[ii]) < std::abs(color - wheelIdx[ibest]) ) ibest = ii;
    }
    int offset = color - wheelIdx[ibest];
    rr = wheel[ibest][0]; gg = wheel[ibest][1]; bb = wheel[ibest][2];
    if( offset > 0 ) { // darker
      double fac = std::max(0.,1.-0.2*offset);
      rr *= fac; gg *= fac; bb *= fac;
    } else if( offset < 0 ) { // lighter
      double fac = std::min(1.,-0.1*offset);
      rr += (1-rr)*fac; gg += (1-gg)*fac; bb += (1-bb)*fac;
    }
  }
  return RGBToSVG(rr,gg,bb);
}

//---------------------------------------------------------------------------
std::string GmSVGCanvas::RootLineStyleToDash( int style, double width )
{
  //--- ROOT line style patterns (pixels)
  std::string pattern;
  switch( style ) {
  case 2: pattern = "12 12"; break;
  case 3: pattern = "4 8"; break;
  case 4: pattern = "12 16 4 16"; break;
  case 5: pattern = "20 12 4 12"; break;
  case 6: pattern = "20 12 4 12 4 12 4 12"; break;
  case 7: pattern = "20 20"; break;
  case 8: pattern = "20 12 4 12 4 12"; break;
  case 9: pattern = "80 20"; break;
  case 10: pattern = "80 40 4 40"; break;
  default: return ""; // solid
  }
  if( width <= 1. ) return pattern;
  //--- scale with line width
  std::istringstream is(pattern);
  std::string out;
  double val;
  while( is >> val ) out += (out == "" ? "" : " ") + fmt(val*width);
  return out;
}

//---------------------------------------------------------------------------
void GmSVGCanvas::SetPalette( int number )
{
  thePaletteTable.clear();
  if( number == 1 ) {
    //--- ROOT 'pretty palette' (TColor::InitializeColors): 49 hues from violet to red, at lightness 0.5 and saturation 1, and kRed
    auto hls1 = []( double rn1, double rn2, double hue ) {
      if( hue > 360 ) hue -= 360;
      if( hue < 0 ) hue += 360;
      if( hue < 60 ) return rn1 + (rn2-rn1)*hue/60;
      if( hue < 180 ) return rn2;
      if( hue < 240 ) return rn1 + (rn2-rn1)*(240-hue)/60;
      return rn1;
    };
    const double light = 0.5, satur = 1.;
    const double rm2 = light*(1.+satur); // light <= 0.5
    const double rm1 = 2.*light - rm2;
    for( int ii = 0; ii < 49; ii++ ) {
      double hue = 280. - (ii+1)*(280./50.);
      double rgb[3] = { hls1(rm1,rm2,hue+120.), hls1(rm1,rm2,hue), hls1(rm1,rm2,hue-120.) };
      for( int ic = 0; ic < 3; ic++ ) thePaletteTable.push_back( (unsigned char)(std::max(0.,std::min(1.,rgb[ic]))*255.+0.5) );
    }
    thePaletteTable.push_back(255); thePaletteTable.push_back(0); thePaletteTable.push_back(0);
    thePalette = 1;
    return;
  }
  if( number < 51 || number > 113 ) {
    std::cerr << " !!! GmSVGCanvas::SetPalette: palette " << number << " not available (1 or 51-113), using 57 (kBird)" << std::endl;
    number = 57;
  }
  //--- as TColor::CreateGradientColorTable, with 255 colors
  const double (&stops)[3][9] = thePaletteStops[number-51];
  const int nColors = 255;
  for( int ig = 1; ig < 9; ig++ ) {
    int nGrad = int(std::floor(nColors*ig/8.) - std::floor(nColors*(ig-1)/8.));
    for( int ic = 0; ic < nGrad; ic++ ) {
      for( int irgb = 0; irgb < 3; irgb++ ) {
	float val = float( stops[irgb][ig-1] + ic*(stops[irgb][ig]-stops[irgb][ig-1])/nGrad );
	thePaletteTable.push_back( (unsigned char)(std::max(0.f,std::min(1.f,val))*255.f+0.5f) );
      }
    }
  }
  thePalette = number;
}

//---------------------------------------------------------------------------
int GmSVGCanvas::GetPalette()
{
  if( thePaletteTable.empty() ) SetPalette(57);
  return thePalette;
}

//---------------------------------------------------------------------------
int GmSVGCanvas::GetPaletteNColors()
{
  if( thePaletteTable.empty() ) SetPalette(57);
  return int(thePaletteTable.size()/3);
}

//---------------------------------------------------------------------------
void GmSVGCanvas::PaletteColor( int index, unsigned char& rr, unsigned char& gg, unsigned char& bb )
{
  int nColors = GetPaletteNColors();
  index = std::max(0,std::min(nColors-1,index));
  rr = thePaletteTable[3*index];
  gg = thePaletteTable[3*index+1];
  bb = thePaletteTable[3*index+2];
}

//---------------------------------------------------------------------------
void GmSVGCanvas::PaletteRGB( double frac, unsigned char& rr, unsigned char& gg, unsigned char& bb )
{
  frac = std::max(0.,std::min(1.,frac));
  PaletteColor( int(frac*GetPaletteNColors()), rr, gg, bb );
}

//---------------------------------------------------------------------------
bool GmSVGCanvas::WriteSVG( const std::string& fileName, bool bEnlargeImages ) const
{
  std::ofstream fout(fileName.c_str());
  if( !fout ) {
    std::cerr << " !!! GmSVGCanvas::WriteSVG: cannot open file " << fileName << std::endl;
    return false;
  }
  fout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
       << "<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" width=\"" << theWw << "\" height=\"" << theWh
       << "\" viewBox=\"0 0 " << theWw << " " << theWh << "\" font-family=\"Helvetica, Arial, sans-serif\">\n"
       << "<rect x=\"0\" y=\"0\" width=\"" << theWw << "\" height=\"" << theWh << "\" fill=\"#ffffff\"/>\n";
  for( size_t ii = 0; ii < theElements.size(); ii++ ) {
    const std::string& elem = theElements[ii];
    if( !elem.empty() && elem[0] == '\x01' ) { // image
      size_t ip = std::stoul(elem.substr(1));
      if( ip < thePrims.size() ) fout << ImageElement(thePrims[ip],bEnlargeImages) << "\n";
    } else {
      fout << elem << "\n";
    }
  }
  fout << "</svg>\n";
  fout.close();
  std::cout << " GmSVGCanvas: figure written to " << fileName << std::endl;
  return true;
}

//---------------------------------------------------------------------------
bool GmSVGCanvas::Print( const std::string& svgFile, const std::string& formatIn,
			 const std::string& converter ) const
{
  //--- format forced by user command /gamos/SVG/imageFormat
  std::string format = (theImageFormat != "") ? theImageFormat : formatIn;
  for( size_t ic = 0; ic < format.size(); ic++ ) format[ic] = char(std::tolower(format[ic]));
  if( format == "jpeg" ) format = "jpg";
  bool bNoConversion = format == "" || format == "svg" || converter == "none" || converter == "NONE";
  bool bBuiltin = !bNoConversion && GmSVGRaster::IsFormatSupported(format)
    && (converter == "" || converter == "auto" || converter == "AUTO" || converter == "builtin" || converter == "BUILTIN");
  //--- SVG file only if no conversion or conversion by an external program (the images are enlarged only for the external converters)
  if( !bBuiltin ) {
    if( !WriteSVG(svgFile, !bNoConversion) ) return false;
  }
  if( bNoConversion ) return true;
  if( !bBuiltin ) {
    ConvertSVG( svgFile, format, (converter == "builtin" || converter == "BUILTIN") ? "auto" : converter );
    return true;
  }

  //--- built-in renderer
  std::string outFile = svgFile;
  size_t idot = outFile.rfind(".svg");
  if( idot != std::string::npos && idot+4 == outFile.size() ) outFile = outFile.substr(0,idot);
  outFile += "." + format;
  int nThreads = MaxFigureThreads();
  if( nThreads == 0 ) {
    RenderAndWrite( theWw, theWh, thePrims, outFile, format );
    return true;
  }
  GmSVGRaster::TextWidth("0",1.); // initialize the fonts before the jobs manager (so they are destroyed after it)
  FigureJobs& jobs = Jobs();
  {
    std::unique_lock<std::mutex> lock(jobs.mtx);
    jobs.cv.wait( lock, [&]{ return jobs.nRunning < nThreads; } );
    jobs.nRunning++;
  }
  std::shared_ptr< std::vector<GmSVGPrim> > prims = std::make_shared< std::vector<GmSVGPrim> >(thePrims);
  int ww = theWw, wh = theWh;
  std::thread th( [ww,wh,prims,outFile,format,&jobs]() {
      RenderAndWrite( ww, wh, *prims, outFile, format );
      std::lock_guard<std::mutex> lock(jobs.mtx);
      jobs.nRunning--;
      jobs.cv.notify_all();
    } );
  th.detach();
  return true;
}

//---------------------------------------------------------------------------
void GmSVGCanvas::WaitForFigures()
{
  Jobs().WaitAll();
}

//---------------------------------------------------------------------------
std::string GmSVGCanvas::FindInPath( const std::string& program )
{
#ifdef _WIN32
  const char sep = ';';
  const char dirSep = '\\';
  std::vector<std::string> exts = { ".exe", ".bat", ".com", "" };
#else
  const char sep = ':';
  const char dirSep = '/';
  std::vector<std::string> exts = { "" };
#endif
  //--- program given with a path
  if( program.find('/') != std::string::npos || program.find('\\') != std::string::npos ) {
    for( size_t ie = 0; ie < exts.size(); ie++ ) {
      if( GMSVG_ACCESS((program+exts[ie]).c_str()) ) return program+exts[ie];
    }
    return "";
  }
  const char* pathEnv = std::getenv("PATH");
  if( !pathEnv ) return "";
  std::string path(pathEnv);
  size_t start = 0;
  while( start <= path.size() ) {
    size_t end = path.find(sep,start);
    if( end == std::string::npos ) end = path.size();
    std::string dir = path.substr(start,end-start);
    if( dir != "" ) {
      for( size_t ie = 0; ie < exts.size(); ie++ ) {
	std::string full = dir + dirSep + program + exts[ie];
	if( GMSVG_ACCESS(full.c_str()) ) return full;
      }
    }
    start = end+1;
  }
  return "";
}

//---------------------------------------------------------------------------
bool GmSVGCanvas::ConvertSVG( const std::string& svgFile, const std::string& formatIn,
			      const std::string& converter )
{
  //--- format forced by user command /gamos/SVG/imageFormat
  std::string format = (theImageFormat != "") ? theImageFormat : formatIn;
  for( size_t ic = 0; ic < format.size(); ic++ ) format[ic] = std::tolower(format[ic]);
  if( converter == "none" || converter == "NONE" || format == "" || format == "svg" ) return false;
  if( format == "jpeg" ) format = "jpg";

  //--- find converter
  std::string prog;
  if( converter == "auto" || converter == "AUTO" || converter == "" ) {
    std::vector<std::string> candidates = { "rsvg-convert", "inkscape", "magick" };
#ifndef _WIN32
    candidates.push_back("convert"); // in Windows 'convert' is a system program (disk conversion)
#endif
    //--- prefer a converter that can write the requested format
    //    (rsvg-convert: png/pdf/ps, inkscape: png/pdf/ps/eps, ImageMagick: any, e.g. jpg, gif, tiff)
    bool bRsvgOK = (format == "png" || format == "pdf" || format == "ps");
    bool bInkOK = bRsvgOK || format == "eps";
    std::string progFallback;
    for( size_t ii = 0; ii < candidates.size(); ii++ ) {
      std::string pp = FindInPath(candidates[ii]);
      if( pp == "" ) continue;
      if( (candidates[ii] == "rsvg-convert" && !bRsvgOK)
	  || (candidates[ii] == "inkscape" && !bInkOK) ) {
	if( progFallback == "" ) progFallback = pp;
	continue;
      }
      prog = pp;
      break;
    }
    if( prog == "" ) prog = progFallback; // it will write png
    if( prog == "" ) {
      std::cout << " GmSVGCanvas: no SVG converter found (rsvg-convert, inkscape, magick, convert),"
		<< " only " << svgFile << " is written" << std::endl;
      return false;
    }
  } else {
    prog = FindInPath(converter);
    if( prog == "" ) {
      std::cerr << " !!! GmSVGCanvas: SVG converter not found: " << converter
		<< " , only " << svgFile << " is written" << std::endl;
      return false;
    }
  }

  //--- program type from its name
  std::string base = prog;
  size_t isl = base.find_last_of("/\\");
  if( isl != std::string::npos ) base = base.substr(isl+1);

  std::string outFile = svgFile;
  size_t idot = outFile.rfind(".svg");
  if( idot != std::string::npos ) outFile = outFile.substr(0,idot);
  std::string cmd;
  if( base.find("rsvg-convert") != std::string::npos ) {
    if( format != "png" && format != "pdf" && format != "ps" ) {
      std::cout << " GmSVGCanvas: rsvg-convert cannot write " << format << " , writing png" << std::endl;
      format = "png";
    }
    outFile += "." + format;
    cmd = "\"" + prog + "\" -f " + format + " -b white -o \"" + outFile + "\" \"" + svgFile + "\"";
  } else if( base.find("inkscape") != std::string::npos ) {
    if( format != "png" && format != "pdf" && format != "ps" && format != "eps" ) {
      std::cout << " GmSVGCanvas: inkscape cannot write " << format << " , writing png" << std::endl;
      format = "png";
    }
    outFile += "." + format;
    cmd = "\"" + prog + "\" \"" + svgFile + "\" --export-type=" + format + " --export-filename=\"" + outFile
      + "\" --export-background=white --export-background-opacity=1";
  } else { // ImageMagick: magick or convert
    static bool bWarnedIM = false;
    if( !bWarnedIM ) {
      bWarnedIM = true;
      std::cout << " GmSVGCanvas: converting with ImageMagick, whose own SVG renderer may lose some characters"
		<< " (e.g. superscripts of 10^-n axis labels); the .svg file is correct."
		<< " For better results install rsvg-convert (package librsvg2-bin) or inkscape" << std::endl;
    }
    outFile += "." + format;
    cmd = "\"" + prog + "\" \"" + svgFile + "\"";
    if( format != "pdf" ) cmd += " -background white -flatten";
    cmd += " \"" + outFile + "\"";
  }
#ifdef _WIN32
  cmd = "\"" + cmd + "\""; // cmd.exe strips the outer quotes
#endif
  int ret = std::system(cmd.c_str());
  if( ret != 0 ) {
    std::cerr << " !!! GmSVGCanvas: SVG conversion failed (" << ret << "): " << cmd << std::endl;
    std::remove(outFile.c_str()); // do not leave an empty or partial file
    return false;
  }
  std::cout << " GmSVGCanvas: figure converted to " << outFile << " with " << base << std::endl;
  return true;
}
