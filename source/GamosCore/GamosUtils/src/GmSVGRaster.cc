#include "GmSVGRaster.hh"

#include <cmath>
#include <cstring>
#include <mutex>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include "zlib.h" // provided by Geant4 (G4zlib) or by the system
#include "tools/font/lato_regular_ttf"
#include "tools/font/roboto_bold_ttf"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


int GmSVGRaster::theJpegQuality = 90;

namespace {

  //=========================================================================
  // Antialiased polygon filling by exact area coverage: each edge accumulates its
  // signed area contribution on the pixels it crosses; the coverage of a pixel is
  // the running sum along the row (algorithm of font-rs, R. Levien)
  class Accum
  {
  public:
    Accum( int imgW, int imgH, double bx0, double by0, double bx1, double by1 ) {
      theX0 = std::max(0,int(std::floor(bx0))-1);
      theY0 = std::max(0,int(std::floor(by0))-1);
      int x1 = std::min(imgW,int(std::ceil(bx1))+1);
      int y1 = std::min(imgH,int(std::ceil(by1))+1);
      theW = x1-theX0;
      theH = y1-theY0;
      bValid = theW > 0 && theH > 0;
      if( bValid ) theAcc.assign(size_t(theW+2)*size_t(theH),0.f);
    }
    bool IsValid() const { return bValid; }

    void Line( double ax, double ay, double bx, double by ) {
      ax -= theX0; bx -= theX0;
      ay -= theY0; by -= theY0;
      if( ay == by ) return;
      double dir = 1.;
      if( ay > by ) {
	std::swap(ax,bx);
	std::swap(ay,by);
	dir = -1.;
      }
      double ystart = std::max(ay,0.);
      double yend = std::min(by,double(theH));
      if( ystart >= yend ) return;
      double dxdy = (bx-ax)/(by-ay);
      double xx = ax + dxdy*(ystart-ay);
      int iys = int(std::floor(ystart));
      int iye = int(std::ceil(yend));
      const double ww = theW;
      for( int iy = iys; iy < iye; iy++ ) {
	double dy = std::min(iy+1.,yend) - std::max(double(iy),ystart);
	double xnext = xx + dxdy*dy;
	double dd = dy*dir;
	double xa = std::min(xx,xnext);
	double xb = std::max(xx,xnext);
	xa = std::max(0.,std::min(ww,xa));
	xb = std::max(0.,std::min(ww,xb));
	float* row = &theAcc[size_t(iy)*(theW+2)];
	double x0f = std::floor(xa);
	int x0i = int(x0f);
	double x1c = std::ceil(xb);
	int x1i = int(x1c);
	if( x1i <= x0i+1 ) {
	  double xmf = 0.5*(xa+xb) - x0f;
	  row[x0i] += float(dd - dd*xmf);
	  row[x0i+1] += float(dd*xmf);
	} else {
	  double ss = 1./(xb-xa);
	  double x0fr = xa - x0f;
	  double a0 = 0.5*ss*(1.-x0fr)*(1.-x0fr);
	  double x1fr = xb - x1c + 1.;
	  double am = 0.5*ss*x1fr*x1fr;
	  row[x0i] += float(dd*a0);
	  if( x1i == x0i+2 ) {
	    row[x0i+1] += float(dd*(1.-a0-am));
	  } else {
	    double a1 = ss*(1.5-x0fr);
	    row[x0i+1] += float(dd*(a1-a0));
	    for( int xi = x0i+2; xi < x1i-1; xi++ ) row[xi] += float(dd*ss);
	    double a2 = a1 + (x1i-x0i-3)*ss;
	    row[x1i-1] += float(dd*(1.-a2-am));
	  }
	  row[x1i] += float(dd*am);
	}
	xx = xnext;
      }
    }

    void Polygon( const std::vector<double>& pts ) {
      size_t np = pts.size()/2;
      if( np < 3 ) return;
      for( size_t ip = 0; ip < np; ip++ ) {
	size_t jp = (ip+1)%np;
	Line( pts[2*ip], pts[2*ip+1], pts[2*jp], pts[2*jp+1] );
      }
    }

    // composite color with the coverage
    void Fill( std::vector<unsigned char>& rgb, int imgW, unsigned int color ) const {
      if( !bValid ) return;
      float cr = float((color>>24)&255), cg = float((color>>16)&255), cb = float((color>>8)&255);
      float alpha = float(color&255)/255.f;
      for( int iy = 0; iy < theH; iy++ ) {
	const float* row = &theAcc[size_t(iy)*(theW+2)];
	unsigned char* pix = &rgb[(size_t(theY0+iy)*imgW + theX0)*3];
	float sum = 0.f;
	for( int ix = 0; ix < theW; ix++, pix += 3 ) {
	  sum += row[ix];
	  float cov = std::fabs(sum);
	  if( cov < 0.002f ) continue;
	  if( cov > 1.f ) cov = 1.f;
	  cov *= alpha;
	  float rem = 1.f-cov;
	  pix[0] = (unsigned char)(pix[0]*rem + cr*cov + 0.5f);
	  pix[1] = (unsigned char)(pix[1]*rem + cg*cov + 0.5f);
	  pix[2] = (unsigned char)(pix[2]*rem + cb*cov + 0.5f);
	}
      }
    }

  private:
    int theX0, theY0, theW, theH;
    bool bValid;
    std::vector<float> theAcc;
  };

  void PolygonsBBox( const std::vector< std::vector<double> >& polys, double& x0, double& y0, double& x1, double& y1 )
  {
    x0 = y0 = 1.e30;
    x1 = y1 = -1.e30;
    for( size_t ii = 0; ii < polys.size(); ii++ ) {
      for( size_t jj = 0; jj+1 < polys[ii].size(); jj += 2 ) {
	x0 = std::min(x0,polys[ii][jj]); x1 = std::max(x1,polys[ii][jj]);
	y0 = std::min(y0,polys[ii][jj+1]); y1 = std::max(y1,polys[ii][jj+1]);
      }
    }
  }

  double SignedArea( const std::vector<double>& pts )
  {
    double area = 0.;
    size_t np = pts.size()/2;
    for( size_t ip = 0; ip < np; ip++ ) {
      size_t jp = (ip+1)%np;
      area += pts[2*ip]*pts[2*jp+1] - pts[2*jp]*pts[2*ip+1];
    }
    return 0.5*area;
  }

  void ReversePolygon( std::vector<double>& pts )
  {
    size_t np = pts.size()/2;
    for( size_t ip = 0; ip < np/2; ip++ ) {
      std::swap(pts[2*ip],pts[2*(np-1-ip)]);
      std::swap(pts[2*ip+1],pts[2*(np-1-ip)+1]);
    }
  }

  // quadrilateral covering the segment p1-p2 with width ww; ext: extension at both ends
  std::vector<double> SegmentQuad( double x1, double y1, double x2, double y2, double ww, double ext )
  {
    double dx = x2-x1, dy = y2-y1;
    double len = std::sqrt(dx*dx+dy*dy);
    std::vector<double> quad;
    if( len <= 0. ) return quad;
    dx /= len; dy /= len;
    double nx = -dy*ww*0.5, ny = dx*ww*0.5;
    x1 -= dx*ext; y1 -= dy*ext;
    x2 += dx*ext; y2 += dy*ext;
    quad = { x1+nx, y1+ny, x2+nx, y2+ny, x2-nx, y2-ny, x1-nx, y1-ny };
    return quad;
  }

  std::vector<double> CirclePolygon( double cx, double cy, double rr, bool bReverse )
  {
    int nseg = std::max(16,std::min(360,int(2.*M_PI*rr/1.5)));
    std::vector<double> pts(2*nseg);
    for( int ii = 0; ii < nseg; ii++ ) {
      double ang = (bReverse ? -1. : 1.)*2.*M_PI*ii/nseg;
      pts[2*ii] = cx + rr*std::cos(ang);
      pts[2*ii+1] = cy + rr*std::sin(ang);
    }
    return pts;
  }

  //=========================================================================
  // Minimal TrueType font reader: glyph outlines (simple and composite glyphs), advance widths, cmap format 4
  class TTFont
  {
  public:
    struct Pt { double x, y; bool on; };
    typedef std::vector<Pt> Contour;

    TTFont( const unsigned char* data, unsigned int len ) : theData(data), theLen(len), bOK(false) {
      if( !data || len < 12 ) return;
      unsigned int numTables = U16(4);
      size_t head = 0, maxp = 0, hhea = 0, cmap = 0;
      for( unsigned int it = 0; it < numTables; it++ ) {
	size_t rec = 12 + 16*it;
	if( rec+16 > len ) return;
	std::string tag( reinterpret_cast<const char*>(data+rec), 4 );
	size_t off = U32(rec+8);
	if( tag == "head" ) head = off;
	else if( tag == "maxp" ) maxp = off;
	else if( tag == "hhea" ) hhea = off;
	else if( tag == "hmtx" ) theHmtx = off;
	else if( tag == "loca" ) theLoca = off;
	else if( tag == "glyf" ) theGlyf = off;
	else if( tag == "cmap" ) cmap = off;
      }
      if( !head || !maxp || !hhea || !theHmtx || !theLoca || !theGlyf || !cmap ) return;
      theUnitsPerEm = U16(head+18);
      theLocFormat = I16(head+50);
      theNumGlyphs = U16(maxp+4);
      theNumHMetrics = U16(hhea+34);
      //--- cmap: Unicode BMP subtable, format 4
      unsigned int nSub = U16(cmap+2);
      theCmap4 = 0;
      for( unsigned int is = 0; is < nSub; is++ ) {
	unsigned int plat = U16(cmap+4+8*is);
	unsigned int enc = U16(cmap+4+8*is+2);
	size_t off = cmap + U32(cmap+4+8*is+4);
	if( U16(off) != 4 ) continue;
	if( (plat == 3 && enc == 1) || plat == 0 ) {
	  theCmap4 = off;
	  if( plat == 3 ) break;
	}
      }
      bOK = theCmap4 != 0 && theUnitsPerEm != 0;
    }
    bool IsOK() const { return bOK; }
    double UnitsPerEm() const { return theUnitsPerEm; }

    unsigned int GlyphIndex( unsigned int code ) const {
      if( !bOK || code > 0xFFFF ) return 0;
      unsigned int segX2 = U16(theCmap4+6);
      size_t ends = theCmap4+14;
      size_t starts = ends + segX2 + 2;
      size_t deltas = starts + segX2;
      size_t ranges = deltas + segX2;
      for( unsigned int is = 0; is < segX2/2; is++ ) {
	unsigned int endCode = U16(ends+2*is);
	if( endCode < code ) continue;
	unsigned int startCode = U16(starts+2*is);
	if( startCode > code ) return 0;
	unsigned int delta = U16(deltas+2*is);
	unsigned int rangeOff = U16(ranges+2*is);
	if( rangeOff == 0 ) return (code + delta) & 0xFFFF;
	size_t addr = ranges + 2*is + rangeOff + 2*(code-startCode);
	unsigned int gl = U16(addr);
	if( gl == 0 ) return 0;
	return (gl + delta) & 0xFFFF;
      }
      return 0;
    }

    double Advance( unsigned int glyph ) const {
      if( !bOK || theNumHMetrics == 0 ) return 0.;
      unsigned int ig = std::min(glyph,theNumHMetrics-1);
      return U16(theHmtx+4*ig);
    }

    // outline of a glyph in font units (y upwards)
    void GetContours( unsigned int glyph, std::vector<Contour>& contours,
		      double a = 1., double b = 0., double c = 0., double d = 1., double e = 0., double f = 0.,
		      int depth = 0 ) const {
      if( !bOK || glyph >= theNumGlyphs || depth > 8 ) return;
      size_t off0, off1;
      if( theLocFormat == 0 ) {
	off0 = 2*size_t(U16(theLoca+2*glyph));
	off1 = 2*size_t(U16(theLoca+2*glyph+2));
      } else {
	off0 = U32(theLoca+4*glyph);
	off1 = U32(theLoca+4*glyph+4);
      }
      if( off1 <= off0 ) return; // empty glyph (e.g. space)
      size_t gp = theGlyf + off0;
      int nContours = I16(gp);
      if( nContours >= 0 ) {
	//--- simple glyph
	size_t pEnd = gp+10;
	std::vector<unsigned int> endPts(nContours);
	for( int ic = 0; ic < nContours; ic++ ) endPts[ic] = U16(pEnd+2*ic);
	if( nContours == 0 ) return;
	unsigned int nPts = endPts[nContours-1]+1;
	size_t pp = pEnd + 2*nContours;
	unsigned int instrLen = U16(pp);
	pp += 2 + instrLen;
	std::vector<unsigned char> flags(nPts);
	for( unsigned int ip = 0; ip < nPts; ) {
	  unsigned char fl = U8(pp++);
	  flags[ip++] = fl;
	  if( fl & 8 ) {
	    unsigned int nRep = U8(pp++);
	    for( unsigned int ir = 0; ir < nRep && ip < nPts; ir++ ) flags[ip++] = fl;
	  }
	}
	std::vector<int> xs(nPts), ys(nPts);
	int val = 0;
	for( unsigned int ip = 0; ip < nPts; ip++ ) {
	  unsigned char fl = flags[ip];
	  if( fl & 2 ) {
	    int dv = U8(pp++);
	    val += (fl & 16) ? dv : -dv;
	  } else if( !(fl & 16) ) {
	    val += I16(pp);
	    pp += 2;
	  }
	  xs[ip] = val;
	}
	val = 0;
	for( unsigned int ip = 0; ip < nPts; ip++ ) {
	  unsigned char fl = flags[ip];
	  if( fl & 4 ) {
	    int dv = U8(pp++);
	    val += (fl & 32) ? dv : -dv;
	  } else if( !(fl & 32) ) {
	    val += I16(pp);
	    pp += 2;
	  }
	  ys[ip] = val;
	}
	unsigned int ip0 = 0;
	for( int ic = 0; ic < nContours; ic++ ) {
	  Contour cont;
	  for( unsigned int ip = ip0; ip <= endPts[ic] && ip < nPts; ip++ ) {
	    Pt pt;
	    pt.x = a*xs[ip] + c*ys[ip] + e;
	    pt.y = b*xs[ip] + d*ys[ip] + f;
	    pt.on = (flags[ip] & 1) != 0;
	    cont.push_back(pt);
	  }
	  ip0 = endPts[ic]+1;
	  if( cont.size() >= 2 ) contours.push_back(cont);
	}
      } else {
	//--- composite glyph
	size_t pp = gp+10;
	unsigned int fl;
	do {
	  fl = U16(pp);
	  unsigned int comp = U16(pp+2);
	  pp += 4;
	  double dx, dy;
	  if( fl & 1 ) { // ARG_1_AND_2_ARE_WORDS
	    dx = I16(pp); dy = I16(pp+2);
	    pp += 4;
	  } else {
	    dx = (signed char)(U8(pp)); dy = (signed char)(U8(pp+1));
	    pp += 2;
	  }
	  if( !(fl & 2) ) dx = dy = 0.; // point matching not supported
	  double ma = 1., mb = 0., mc = 0., md = 1.;
	  if( fl & 8 ) { // WE_HAVE_A_SCALE
	    ma = md = F2Dot14(pp);
	    pp += 2;
	  } else if( fl & 0x40 ) { // WE_HAVE_AN_X_AND_Y_SCALE
	    ma = F2Dot14(pp); md = F2Dot14(pp+2);
	    pp += 4;
	  } else if( fl & 0x80 ) { // WE_HAVE_A_TWO_BY_TWO
	    ma = F2Dot14(pp); mb = F2Dot14(pp+2); mc = F2Dot14(pp+4); md = F2Dot14(pp+6);
	    pp += 8;
	  }
	  //--- combine: component transform, then parent transform
	  double na = a*ma + c*mb;
	  double nb = b*ma + d*mb;
	  double nc = a*mc + c*md;
	  double nd = b*mc + d*md;
	  double ne = a*dx + c*dy + e;
	  double nf = b*dx + d*dy + f;
	  GetContours( comp, contours, na, nb, nc, nd, ne, nf, depth+1 );
	} while( fl & 0x20 ); // MORE_COMPONENTS
      }
    }

  private:
    unsigned int U8( size_t off ) const { return off < theLen ? theData[off] : 0; }
    unsigned int U16( size_t off ) const { return (U8(off) << 8) | U8(off+1); }
    int I16( size_t off ) const { return short(U16(off)); }
    size_t U32( size_t off ) const { return (size_t(U16(off)) << 16) | U16(off+2); }
    double F2Dot14( size_t off ) const { return I16(off)/16384.; }

    const unsigned char* theData;
    size_t theLen;
    bool bOK;
    size_t theHmtx = 0, theLoca = 0, theGlyf = 0, theCmap4 = 0;
    unsigned int theUnitsPerEm = 0, theNumGlyphs = 0, theNumHMetrics = 0;
    int theLocFormat = 0;
  };

  const TTFont& MainFont()
  {
    static unsigned int len = 0;
    static const unsigned char* data = tools::font::lato_regular_ttf(len);
    static TTFont font(data,len);
    return font;
  }

  const TTFont& GreekFont()
  {
    static unsigned int len = 0;
    static const unsigned char* data = tools::font::roboto_bold_ttf(len);
    static TTFont font(data,len);
    return font;
  }

  //--- decode UTF-8
  std::vector<unsigned int> DecodeUTF8( const std::string& text )
  {
    std::vector<unsigned int> codes;
    for( size_t ii = 0; ii < text.size(); ) {
      unsigned char cc = text[ii];
      unsigned int code = cc;
      int nExtra = 0;
      if( cc >= 0xF0 ) { code = cc & 0x07; nExtra = 3; }
      else if( cc >= 0xE0 ) { code = cc & 0x0F; nExtra = 2; }
      else if( cc >= 0xC0 ) { code = cc & 0x1F; nExtra = 1; }
      ii++;
      for( int ie = 0; ie < nExtra && ii < text.size(); ie++, ii++ ) {
	code = (code << 6) | (((unsigned char)text[ii]) & 0x3F);
      }
      codes.push_back(code);
    }
    return codes;
  }

  // Glyph to draw for a Unicode code: font, glyph index, scale and vertical shift (fraction of font size)
  struct GlyphRef { const TTFont* font; unsigned int glyph; double scale; double rise; };

  GlyphRef FindGlyph( unsigned int code )
  {
    const TTFont& mainF = MainFont();
    GlyphRef ref = { &mainF, 0, 1., 0. };
    if( code == '\t' ) code = ' ';
    //--- superscripts (not all in the font): smaller and raised digit / sign, all with the same style
    unsigned int base = 0;
    if( code == 0x2070 ) base = '0';
    else if( code == 0xB9 ) base = '1';
    else if( code == 0xB2 ) base = '2';
    else if( code == 0xB3 ) base = '3';
    else if( code >= 0x2074 && code <= 0x2079 ) base = '4' + (code-0x2074);
    else if( code == 0x207A ) base = '+';
    else if( code == 0x207B ) base = mainF.GlyphIndex(0x2212) ? 0x2212 : '-';
    if( base != 0 ) {
      ref.glyph = mainF.GlyphIndex(base);
      ref.scale = 0.62;
      ref.rise = 0.38;
      return ref;
    }
    unsigned int gl = mainF.GlyphIndex(code);
    if( gl != 0 || code == ' ' ) {
      ref.glyph = gl;
      return ref;
    }
    //--- other characters (Greek letters, ...)
    const TTFont& greekF = GreekFont();
    gl = greekF.GlyphIndex(code);
    if( gl != 0 ) {
      ref.font = &greekF;
      ref.glyph = gl;
      return ref;
    }
    ref.glyph = mainF.GlyphIndex('?');
    return ref;
  }

  //=========================================================================
  // JPEG baseline encoder, derived from TooJpeg 1.x (Stephan Brumme, zlib license, see Geant4 tools/toojpeg).
  // Modified: output to std::string, fixed 4:2:0 sampling, and the macro blocks of uniform color
  // are encoded without DCT (only the DC coefficients)
  namespace jpeg {
    const unsigned char DefaultQuantLuminance[64] =
      { 16, 11, 10, 16, 24, 40, 51, 61, 12, 12, 14, 19, 26, 58, 60, 55,
	14, 13, 16, 24, 40, 57, 69, 56, 14, 17, 22, 29, 51, 87, 80, 62,
	18, 22, 37, 56, 68,109,103, 77, 24, 35, 55, 64, 81,104,113, 92,
	49, 64, 78, 87,103,121,120,101, 72, 92, 95, 98,112,100,103, 99 };
    const unsigned char DefaultQuantChrominance[64] =
      { 17, 18, 24, 47, 99, 99, 99, 99, 18, 21, 26, 66, 99, 99, 99, 99,
	24, 26, 56, 99, 99, 99, 99, 99, 47, 66, 99, 99, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99 };
    const unsigned char ZigZagInv[64] =
      {  0, 1, 8,16, 9, 2, 3,10, 17,24,32,25,18,11, 4, 5,
	12,19,26,33,40,48,41,34, 27,20,13, 6, 7,14,21,28,
	35,42,49,56,57,50,43,36, 29,22,15,23,30,37,44,51,
	58,59,52,45,38,31,39,46, 53,60,61,54,47,55,62,63 };
    const unsigned char DcLuminanceCodesPerBitsize[16] = { 0,1,5,1,1,1,1,1,1,0,0,0,0,0,0,0 };
    const unsigned char DcLuminanceValues[12] = { 0,1,2,3,4,5,6,7,8,9,10,11 };
    const unsigned char AcLuminanceCodesPerBitsize[16] = { 0,2,1,3,3,2,4,3,5,5,4,4,0,0,1,125 };
    const unsigned char AcLuminanceValues[162] =
      { 0x01,0x02,0x03,0x00,0x04,0x11,0x05,0x12,0x21,0x31,0x41,0x06,0x13,0x51,0x61,0x07,0x22,0x71,0x14,0x32,0x81,0x91,0xA1,0x08,
	0x23,0x42,0xB1,0xC1,0x15,0x52,0xD1,0xF0,0x24,0x33,0x62,0x72,0x82,0x09,0x0A,0x16,0x17,0x18,0x19,0x1A,0x25,0x26,0x27,0x28,
	0x29,0x2A,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x53,0x54,0x55,0x56,0x57,0x58,0x59,
	0x5A,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x83,0x84,0x85,0x86,0x87,0x88,0x89,
	0x8A,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xB2,0xB3,0xB4,0xB5,0xB6,
	0xB7,0xB8,0xB9,0xBA,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xE1,0xE2,
	0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA };
    const unsigned char DcChrominanceCodesPerBitsize[16] = { 0,3,1,1,1,1,1,1,1,1,1,0,0,0,0,0 };
    const unsigned char DcChrominanceValues[12] = { 0,1,2,3,4,5,6,7,8,9,10,11 };
    const unsigned char AcChrominanceCodesPerBitsize[16] = { 0,2,1,2,4,4,3,4,7,5,4,4,0,1,2,119 };
    const unsigned char AcChrominanceValues[162] =
      { 0x00,0x01,0x02,0x03,0x11,0x04,0x05,0x21,0x31,0x06,0x12,0x41,0x51,0x07,0x61,0x71,0x13,0x22,0x32,0x81,0x08,0x14,0x42,0x91,
	0xA1,0xB1,0xC1,0x09,0x23,0x33,0x52,0xF0,0x15,0x62,0x72,0xD1,0x0A,0x16,0x24,0x34,0xE1,0x25,0xF1,0x17,0x18,0x19,0x1A,0x26,
	0x27,0x28,0x29,0x2A,0x35,0x36,0x37,0x38,0x39,0x3A,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x53,0x54,0x55,0x56,0x57,0x58,
	0x59,0x5A,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x82,0x83,0x84,0x85,0x86,0x87,
	0x88,0x89,0x8A,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xB2,0xB3,0xB4,
	0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,
	0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA };
    const int CodeWordLimit = 2048;

    struct BitCode {
      BitCode() : code(0), numBits(0) {}
      BitCode( unsigned short cod, unsigned char nb ) : code(cod), numBits(nb) {}
      unsigned short code;
      unsigned char numBits;
    };

    struct BitWriter {
      std::string& out;
      unsigned long long data; // only the lowest numBits bits are pending
      int numBits;
      explicit BitWriter( std::string& ou ) : out(ou), data(0), numBits(0) {}
      void Put( const BitCode& bc ) {
	numBits += bc.numBits;
	data = (data << bc.numBits) | bc.code;
	while( numBits >= 8 ) {
	  numBits -= 8;
	  unsigned char oneByte = (unsigned char)(data >> numBits);
	  out.push_back(char(oneByte));
	  if( oneByte == 0xFF ) out.push_back(char(0));
	}
      }
      void Flush() { Put( BitCode(0x7F,7) ); }
      void Byte( unsigned char bb ) { out += char(bb); }
      void Marker( unsigned char id, unsigned short length ) {
	out += char(0xFF); out += char(id);
	out += char(length >> 8); out += char(length & 0xFF);
      }
    };

    inline float rgb2y( float r, float g, float b ) { return +0.299f*r + 0.587f*g + 0.114f*b; }
    inline float rgb2cb( float r, float g, float b ) { return -0.16874f*r - 0.33126f*g + 0.5f*b; }
    inline float rgb2cr( float r, float g, float b ) { return +0.5f*r - 0.41869f*g - 0.08131f*b; }

    inline void DCT( float* block, int stride )
    {
      const float SqrtHalfSqrt = 1.306562965f;
      const float InvSqrt      = 0.707106781f;
      const float HalfSqrtSqrt = 0.382683432f;
      const float InvSqrtSqrt  = 0.541196100f;
      float& block0 = block[0];
      float& block1 = block[1*stride];
      float& block2 = block[2*stride];
      float& block3 = block[3*stride];
      float& block4 = block[4*stride];
      float& block5 = block[5*stride];
      float& block6 = block[6*stride];
      float& block7 = block[7*stride];
      float add07 = block0 + block7; float sub07 = block0 - block7;
      float add16 = block1 + block6; float sub16 = block1 - block6;
      float add25 = block2 + block5; float sub25 = block2 - block5;
      float add34 = block3 + block4; float sub34 = block3 - block4;
      float add0347 = add07 + add34; float sub07_34 = add07 - add34;
      float add1256 = add16 + add25; float sub16_25 = add16 - add25;
      block0 = add0347 + add1256; block4 = add0347 - add1256;
      float z1 = (sub16_25 + sub07_34) * InvSqrt;
      block2 = sub07_34 + z1; block6 = sub07_34 - z1;
      float sub23_45 = sub25 + sub34;
      float sub12_56 = sub16 + sub25;
      float sub01_67 = sub16 + sub07;
      float z5 = (sub23_45 - sub01_67) * HalfSqrtSqrt;
      float z2 = sub23_45 * InvSqrtSqrt  + z5;
      float z3 = sub12_56 * InvSqrt;
      float z4 = sub01_67 * SqrtHalfSqrt + z5;
      float z6 = sub07 + z3;
      float z7 = sub07 - z3;
      block1 = z6 + z4; block7 = z6 - z4;
      block5 = z7 + z2; block3 = z7 - z2;
    }

    inline int RoundF( float val ) { return int(val + (val >= 0 ? 0.5f : -0.5f)); }

    inline void PutDC( BitWriter& writer, int diff, const BitCode* huffmanDC, const BitCode* codewords )
    {
      if( diff == 0 ) {
	writer.Put( huffmanDC[0] );
      } else {
	const BitCode& bits = codewords[diff];
	writer.Put( huffmanDC[bits.numBits] );
	writer.Put( bits );
      }
    }

    int EncodeBlock( BitWriter& writer, float block64[64], const float scaled[64], int lastDC,
		     const BitCode* huffmanDC, const BitCode* huffmanAC, const BitCode* codewords )
    {
      for( int offset = 0; offset < 8; offset++ ) DCT( block64 + offset*8, 1 );
      for( int offset = 0; offset < 8; offset++ ) DCT( block64 + offset, 8 );
      for( int ii = 0; ii < 64; ii++ ) block64[ii] *= scaled[ii];
      int DC = RoundF(block64[0]);
      int posNonZero = 0;
      int quantized[64];
      for( int ii = 1; ii < 64; ii++ ) {
	quantized[ii] = RoundF(block64[ZigZagInv[ii]]);
	if( quantized[ii] != 0 ) posNonZero = ii;
      }
      PutDC( writer, DC-lastDC, huffmanDC, codewords );
      int offset = 0;
      for( int ii = 1; ii <= posNonZero; ii++ ) {
	while( quantized[ii] == 0 ) {
	  offset += 0x10;
	  if( offset > 0xF0 ) {
	    writer.Put( huffmanAC[0xF0] );
	    offset = 0;
	  }
	  ii++;
	}
	const BitCode& encoded = codewords[quantized[ii]];
	writer.Put( huffmanAC[offset + encoded.numBits] );
	writer.Put( encoded );
	offset = 0;
      }
      if( posNonZero < 63 ) writer.Put( huffmanAC[0x00] );
      return DC;
    }

    // block of uniform value: only DC coefficient (= 8*value/quant[0]), all AC are 0
    inline int EncodeUniformBlock( BitWriter& writer, float value, const float scaled[64], int lastDC,
				   const BitCode* huffmanDC, const BitCode* huffmanAC, const BitCode* codewords )
    {
      int DC = RoundF(64.f*value*scaled[0]);
      PutDC( writer, DC-lastDC, huffmanDC, codewords );
      writer.Put( huffmanAC[0x00] );
      return DC;
    }

    void GenerateHuffmanTable( const unsigned char numCodes[16], const unsigned char* values, BitCode result[256] )
    {
      unsigned short huffmanCode = 0;
      for( int numBits = 1; numBits <= 16; numBits++ ) {
	for( int ii = 0; ii < numCodes[numBits-1]; ii++ ) result[*values++] = BitCode(huffmanCode++,(unsigned char)numBits);
	huffmanCode <<= 1;
      }
    }

    bool Write( std::string& out, const unsigned char* pixels, int width, int height, int qualityIn )
    {
      if( width <= 0 || height <= 0 || width > 65535 || height > 65535 ) return false;
      BitWriter writer(out);
      static const unsigned char HeaderJfif[20] =
	{ 0xFF,0xD8, 0xFF,0xE0, 0,16, 'J','F','I','F',0, 1,1,
	  0,        // no density units (as ROOT figures)
	  0,1,0,1,  // density 1:1
	  0,0 };
      for( int ii = 0; ii < 20; ii++ ) writer.Byte(HeaderJfif[ii]);
      int quality = std::max(1,std::min(100,qualityIn));
      quality = quality < 50 ? 5000/quality : 200-quality*2;
      unsigned char quantLuminance[64], quantChrominance[64];
      for( int ii = 0; ii < 64; ii++ ) {
	int lum = (DefaultQuantLuminance[ZigZagInv[ii]]*quality + 50)/100;
	int chr = (DefaultQuantChrominance[ZigZagInv[ii]]*quality + 50)/100;
	quantLuminance[ii] = (unsigned char)(std::max(1,std::min(255,lum)));
	quantChrominance[ii] = (unsigned char)(std::max(1,std::min(255,chr)));
      }
      writer.Marker(0xDB, 2 + 2*(1+64));
      writer.Byte(0x00);
      for( int ii = 0; ii < 64; ii++ ) writer.Byte(quantLuminance[ii]);
      writer.Byte(0x01);
      for( int ii = 0; ii < 64; ii++ ) writer.Byte(quantChrominance[ii]);
      writer.Marker(0xC0, 2+6+3*3);
      writer.Byte(0x08);
      writer.Byte((unsigned char)(height >> 8)); writer.Byte((unsigned char)(height & 0xFF));
      writer.Byte((unsigned char)(width >> 8)); writer.Byte((unsigned char)(width & 0xFF));
      writer.Byte(3);
      for( int id = 1; id <= 3; id++ ) {
	writer.Byte((unsigned char)id);
	writer.Byte(id == 1 ? 0x22 : 0x11); // YCbCr 4:2:0
	writer.Byte(id == 1 ? 0 : 1);
      }
      writer.Marker(0xC4, 2+208+208);
      writer.Byte(0x00);
      for( int ii = 0; ii < 16; ii++ ) writer.Byte(DcLuminanceCodesPerBitsize[ii]);
      for( int ii = 0; ii < 12; ii++ ) writer.Byte(DcLuminanceValues[ii]);
      writer.Byte(0x10);
      for( int ii = 0; ii < 16; ii++ ) writer.Byte(AcLuminanceCodesPerBitsize[ii]);
      for( int ii = 0; ii < 162; ii++ ) writer.Byte(AcLuminanceValues[ii]);
      writer.Byte(0x01);
      for( int ii = 0; ii < 16; ii++ ) writer.Byte(DcChrominanceCodesPerBitsize[ii]);
      for( int ii = 0; ii < 12; ii++ ) writer.Byte(DcChrominanceValues[ii]);
      writer.Byte(0x11);
      for( int ii = 0; ii < 16; ii++ ) writer.Byte(AcChrominanceCodesPerBitsize[ii]);
      for( int ii = 0; ii < 162; ii++ ) writer.Byte(AcChrominanceValues[ii]);
      BitCode huffLumDC[256], huffLumAC[256], huffChrDC[256], huffChrAC[256];
      GenerateHuffmanTable(DcLuminanceCodesPerBitsize, DcLuminanceValues, huffLumDC);
      GenerateHuffmanTable(AcLuminanceCodesPerBitsize, AcLuminanceValues, huffLumAC);
      GenerateHuffmanTable(DcChrominanceCodesPerBitsize, DcChrominanceValues, huffChrDC);
      GenerateHuffmanTable(AcChrominanceCodesPerBitsize, AcChrominanceValues, huffChrAC);
      writer.Marker(0xDA, 2+1+2*3+3);
      writer.Byte(3);
      for( int id = 1; id <= 3; id++ ) {
	writer.Byte((unsigned char)id);
	writer.Byte(id == 1 ? 0x00 : 0x11);
      }
      writer.Byte(0); writer.Byte(63); writer.Byte(0);

      float scaledLum[64], scaledChr[64];
      static const float AanScaleFactors[8] = { 1, 1.387039845f, 1.306562965f, 1.175875602f, 1, 0.785694958f, 0.541196100f, 0.275899379f };
      for( int ii = 0; ii < 64; ii++ ) {
	int row = ZigZagInv[ii]/8;
	int col = ZigZagInv[ii]%8;
	float factor = 1.f/(AanScaleFactors[row]*AanScaleFactors[col]*8.f);
	scaledLum[ZigZagInv[ii]] = factor/quantLuminance[ii];
	scaledChr[ZigZagInv[ii]] = factor/quantChrominance[ii];
      }
      std::vector<BitCode> codewordsArray(2*CodeWordLimit);
      BitCode* codewords = &codewordsArray[CodeWordLimit];
      int numBits = 1;
      int mask = 1;
      for( int value = 1; value < CodeWordLimit; value++ ) {
	if( value > mask ) {
	  numBits++;
	  mask = (mask << 1) | 1;
	}
	codewords[-value] = BitCode((unsigned short)(mask-value),(unsigned char)numBits);
	codewords[+value] = BitCode((unsigned short)value,(unsigned char)numBits);
      }

      //--- color conversion tables
      float tYR[256], tYG[256], tYB[256], tCbR[256], tCbG[256], tCbB[256], tCrR[256], tCrG[256], tCrB[256];
      for( int ii = 0; ii < 256; ii++ ) {
	float vv = float(ii);
	tYR[ii] = rgb2y(vv,0,0); tYG[ii] = rgb2y(0,vv,0); tYB[ii] = rgb2y(0,0,vv) - 128.f;
	tCbR[ii] = rgb2cb(vv,0,0); tCbG[ii] = rgb2cb(0,vv,0); tCbB[ii] = rgb2cb(0,0,vv);
	tCrR[ii] = rgb2cr(vv,0,0); tCrG[ii] = rgb2cr(0,vv,0); tCrB[ii] = rgb2cr(0,0,vv);
      }

      const int maxX = width-1, maxY = height-1;
      int lastYDC = 0, lastCbDC = 0, lastCrDC = 0;
      float Ymcu[256], Cbs[256], Crs[256], Yb[64], Cb[64], Cr[64];
      int xoff[16];
      const unsigned char* prows[16];
      for( int mcuY = 0; mcuY < height; mcuY += 16 ) {
	for( int dy = 0; dy < 16; dy++ ) prows[dy] = &pixels[size_t(std::min(mcuY+dy,maxY))*width*3];
	for( int mcuX = 0; mcuX < width; mcuX += 16 ) {
	  for( int dx = 0; dx < 16; dx++ ) xoff[dx] = std::min(mcuX+dx,maxX)*3;
	  //--- uniform macro block?
	  const unsigned char* p0 = prows[0] + xoff[0];
	  bool bUniform = true;
	  if( mcuX+16 <= width ) { // compare each row with 16 pixels of the first color
	    unsigned char pattern[48];
	    for( int dx = 0; dx < 16; dx++ ) { pattern[3*dx] = p0[0]; pattern[3*dx+1] = p0[1]; pattern[3*dx+2] = p0[2]; }
	    for( int dy = 0; dy < 16 && bUniform; dy++ ) bUniform = std::memcmp(prows[dy]+mcuX*3,pattern,48) == 0;
	  } else for( int dy = 0; dy < 16 && bUniform; dy++ ) {
	    const unsigned char* prow = prows[dy];
	    for( int dx = 0; dx < 16; dx++ ) {
	      const unsigned char* pp = prow + xoff[dx];
	      if( pp[0] != p0[0] || pp[1] != p0[1] || pp[2] != p0[2] ) {
		bUniform = false;
		break;
	      }
	    }
	  }
	  if( bUniform ) {
	    float rr = p0[0], gg = p0[1], bb = p0[2];
	    float yv = rgb2y(rr,gg,bb) - 128.f;
	    for( int ib = 0; ib < 4; ib++ ) {
	      lastYDC = EncodeUniformBlock(writer, yv, scaledLum, lastYDC, huffLumDC, huffLumAC, codewords);
	    }
	    lastCbDC = EncodeUniformBlock(writer, rgb2cb(rr,gg,bb), scaledChr, lastCbDC, huffChrDC, huffChrAC, codewords);
	    lastCrDC = EncodeUniformBlock(writer, rgb2cr(rr,gg,bb), scaledChr, lastCrDC, huffChrDC, huffChrAC, codewords);
	    continue;
	  }
	  //--- color conversion of the 16x16 pixels
	  for( int dy = 0; dy < 16; dy++ ) {
	    const unsigned char* prow = prows[dy];
	    for( int dx = 0; dx < 16; dx++ ) {
	      const unsigned char* pp = prow + xoff[dx];
	      int ii = dy*16+dx;
	      Ymcu[ii] = tYR[pp[0]] + tYG[pp[1]] + tYB[pp[2]];
	      Cbs[ii] = tCbR[pp[0]] + tCbG[pp[1]] + tCbB[pp[2]];
	      Crs[ii] = tCrR[pp[0]] + tCrG[pp[1]] + tCrB[pp[2]];
	    }
	  }
	  //--- four luminance blocks (blocks of uniform value: no DCT)
	  for( int blockY = 0; blockY < 16; blockY += 8 ) {
	    for( int blockX = 0; blockX < 16; blockX += 8 ) {
	      bool bUnif = true;
	      const float y0 = Ymcu[blockY*16+blockX];
	      for( int dy = 0; dy < 8; dy++ ) {
		const float* src = &Ymcu[(blockY+dy)*16+blockX];
		for( int dx = 0; dx < 8; dx++ ) {
		  Yb[dy*8+dx] = src[dx];
		  bUnif = bUnif && (src[dx] == y0);
		}
	      }
	      lastYDC = bUnif ? EncodeUniformBlock(writer, y0, scaledLum, lastYDC, huffLumDC, huffLumAC, codewords)
		: EncodeBlock(writer, Yb, scaledLum, lastYDC, huffLumDC, huffLumAC, codewords);
	    }
	  }
	  //--- chrominance: average of 2x2 pixels
	  bool bUnifCb = true, bUnifCr = true;
	  for( int dy = 0; dy < 8; dy++ ) {
	    for( int dx = 0; dx < 8; dx++ ) {
	      int ia = (2*dy)*16 + 2*dx;
	      Cb[dy*8+dx] = (Cbs[ia]+Cbs[ia+1]+Cbs[ia+16]+Cbs[ia+17])*0.25f;
	      Cr[dy*8+dx] = (Crs[ia]+Crs[ia+1]+Crs[ia+16]+Crs[ia+17])*0.25f;
	      bUnifCb = bUnifCb && (Cb[dy*8+dx] == Cb[0]);
	      bUnifCr = bUnifCr && (Cr[dy*8+dx] == Cr[0]);
	    }
	  }
	  lastCbDC = bUnifCb ? EncodeUniformBlock(writer, Cb[0], scaledChr, lastCbDC, huffChrDC, huffChrAC, codewords)
	    : EncodeBlock(writer, Cb, scaledChr, lastCbDC, huffChrDC, huffChrAC, codewords);
	  lastCrDC = bUnifCr ? EncodeUniformBlock(writer, Cr[0], scaledChr, lastCrDC, huffChrDC, huffChrAC, codewords)
	    : EncodeBlock(writer, Cr, scaledChr, lastCrDC, huffChrDC, huffChrAC, codewords);
	}
      }
      writer.Flush();
      writer.Byte(0xFF); writer.Byte(0xD9); // EOI
      return true;
    }
  } // namespace jpeg

  //=========================================================================
  void PutLE16( std::string& out, unsigned int val ) { out += char(val & 255); out += char((val>>8) & 255); }
  void PutLE32( std::string& out, unsigned int val ) { PutLE16(out,val & 0xFFFF); PutLE16(out,(val>>16) & 0xFFFF); }
  void PutBE32( std::string& out, unsigned int val ) {
    out += char((val>>24) & 255); out += char((val>>16) & 255); out += char((val>>8) & 255); out += char(val & 255);
  }

} // namespace

//===========================================================================
//--- pool of image and output buffers, reused by the successive figures (allocating and releasing
//    big buffers for each figure is slow, especially with several threads)
namespace {
  const size_t MaxPoolSize = 16;
  // never deleted: figures may still be written in background threads while the static objects are destroyed
  std::mutex& PoolMutex() { static std::mutex* mtx = new std::mutex; return *mtx; }
  std::vector< std::vector<unsigned char> >& RGBPool() { static auto* pool = new std::vector< std::vector<unsigned char> >; return *pool; }
  std::vector<std::string>& OutPool() { static auto* pool = new std::vector<std::string>; return *pool; }
  template<class T> void TakeFromPool( std::vector<T>& pool, T& buf ) {
    std::lock_guard<std::mutex> lock(PoolMutex());
    if( !pool.empty() ) {
      buf.swap(pool.back());
      pool.pop_back();
    }
  }
  template<class T> void ReturnToPool( std::vector<T>& pool, T& buf ) {
    std::lock_guard<std::mutex> lock(PoolMutex());
    if( pool.size() < MaxPoolSize ) {
      pool.push_back(T());
      pool.back().swap(buf);
    }
  }
}

GmSVGRaster::GmSVGRaster( int ww, int wh )
  : theW(std::max(1,ww)), theH(std::max(1,wh))
{
  TakeFromPool(RGBPool(),theRGB);
  theRGB.assign(size_t(theW)*size_t(theH)*3,255);
}

GmSVGRaster::~GmSVGRaster()
{
  ReturnToPool(RGBPool(),theRGB);
}

//---------------------------------------------------------------------------
void GmSVGRaster::SetJpegQuality( int quality )
{
  theJpegQuality = std::max(1,std::min(100,quality));
}

int GmSVGRaster::GetJpegQuality()
{
  return theJpegQuality;
}

//---------------------------------------------------------------------------
bool GmSVGRaster::IsFormatSupported( const std::string& format )
{
  return format == "png" || format == "jpg" || format == "jpeg" || format == "gif"
    || format == "bmp" || format == "tiff" || format == "tif";
}

//---------------------------------------------------------------------------
unsigned int GmSVGRaster::ParseColor( const std::string& color )
{
  if( color == "" || color == "none" || color == "transparent" ) return 0;
  if( color[0] == '#' ) {
    unsigned int val = 0;
    std::string hex = color.substr(1);
    if( hex.size() == 3 ) hex = std::string() + hex[0] + hex[0] + hex[1] + hex[1] + hex[2] + hex[2];
    if( hex.size() != 6 ) return 0x000000FF;
    for( size_t ii = 0; ii < 6; ii++ ) {
      char cc = char(std::tolower(hex[ii]));
      unsigned int dig = (cc >= '0' && cc <= '9') ? unsigned(cc-'0') : (cc >= 'a' && cc <= 'f') ? unsigned(cc-'a'+10) : 0u;
      val = (val << 4) | dig;
    }
    return (val << 8) | 0xFF;
  }
  if( color == "white" ) return 0xFFFFFFFF;
  if( color == "red" ) return 0xFF0000FF;
  if( color == "green" ) return 0x008000FF;
  if( color == "blue" ) return 0x0000FFFF;
  if( color == "gray" || color == "grey" ) return 0x808080FF;
  return 0x000000FF; // black and unknown names
}

//---------------------------------------------------------------------------
void GmSVGRaster::FillPolygons( const std::vector< std::vector<double> >& polysIn, unsigned int color, bool bNormalize )
{
  if( (color & 255) == 0 || polysIn.empty() ) return;
  double x0, y0, x1, y1;
  PolygonsBBox( polysIn, x0, y0, x1, y1 );
  if( x1 < 0 || y1 < 0 || x0 > theW || y0 > theH ) return;
  Accum acc( theW, theH, x0, y0, x1, y1 );
  if( !acc.IsValid() ) return;
  for( size_t ip = 0; ip < polysIn.size(); ip++ ) {
    if( bNormalize && SignedArea(polysIn[ip]) < 0. ) {
      std::vector<double> poly = polysIn[ip];
      ReversePolygon(poly);
      acc.Polygon(poly);
    } else {
      acc.Polygon(polysIn[ip]);
    }
  }
  acc.Fill( theRGB, theW, color );
}

//---------------------------------------------------------------------------
void GmSVGRaster::StrokeLine( double x1, double y1, double x2, double y2, double width, unsigned int color,
			      const std::vector<double>& dashIn )
{
  if( (color & 255) == 0 || width <= 0. ) return;
  std::vector< std::vector<double> > quads;
  double len = std::sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
  if( len <= 0. ) return;
  std::vector<double> dash = dashIn;
  double dashSum = 0.;
  for( size_t ii = 0; ii < dash.size(); ii++ ) dashSum += std::max(0.,dash[ii]);
  if( dash.size() % 2 == 1 ) dash.insert(dash.end(),dashIn.begin(),dashIn.end()); // SVG: odd list is repeated
  if( dash.empty() || dashSum <= 0. ) {
    quads.push_back( SegmentQuad(x1,y1,x2,y2,width,0.) );
  } else {
    double ux = (x2-x1)/len, uy = (y2-y1)/len;
    double pos = 0.;
    size_t idx = 0;
    bool bOn = true;
    while( pos < len ) {
      double seg = std::min(std::max(0.,dash[idx]),len-pos);
      if( bOn && seg > 0. ) {
	quads.push_back( SegmentQuad(x1+ux*pos,y1+uy*pos,x1+ux*(pos+seg),y1+uy*(pos+seg),width,0.) );
      }
      pos += std::max(seg,1.e-3);
      idx = (idx+1)%dash.size();
      bOn = !bOn;
    }
  }
  FillPolygons( quads, color, true );
}

//---------------------------------------------------------------------------
// Axis-aligned rectangle, with exact area coverage of the border pixels
void GmSVGRaster::FillRect( double xa, double ya, double xb, double yb, unsigned int color )
{
  xa = std::max(xa,0.); ya = std::max(ya,0.);
  xb = std::min(xb,double(theW)); yb = std::min(yb,double(theH));
  if( xb <= xa || yb <= ya || (color & 255) == 0 ) return;
  const unsigned char cr = (unsigned char)(color>>24), cg = (unsigned char)(color>>16), cb = (unsigned char)(color>>8);
  const float alpha = float(color&255)/255.f;
  int ix0 = int(std::floor(xa)), ix1 = std::min(theW,int(std::ceil(xb)));
  int iy0 = int(std::floor(ya)), iy1 = std::min(theH,int(std::ceil(yb)));
  std::vector<float> covx(ix1-ix0);
  for( int ix = ix0; ix < ix1; ix++ ) covx[ix-ix0] = float(std::min(xb,ix+1.) - std::max(xa,double(ix)));
  for( int iy = iy0; iy < iy1; iy++ ) {
    float covy = float(std::min(yb,iy+1.) - std::max(ya,double(iy)));
    unsigned char* pix = &theRGB[(size_t(iy)*theW + ix0)*3];
    for( int ix = ix0; ix < ix1; ix++, pix += 3 ) {
      float cov = covx[ix-ix0]*covy*alpha;
      if( cov >= 0.999f ) {
	pix[0] = cr; pix[1] = cg; pix[2] = cb;
      } else if( cov >= 0.002f ) {
	float rem = 1.f-cov;
	pix[0] = (unsigned char)(pix[0]*rem + cr*cov + 0.5f);
	pix[1] = (unsigned char)(pix[1]*rem + cg*cov + 0.5f);
	pix[2] = (unsigned char)(pix[2]*rem + cb*cov + 0.5f);
      }
    }
  }
}

//---------------------------------------------------------------------------
void GmSVGRaster::DrawRect( double x1, double y1, double x2, double y2, unsigned int stroke, unsigned int fill, double width )
{
  double xa = std::min(x1,x2), xb = std::max(x1,x2);
  double ya = std::min(y1,y2), yb = std::max(y1,y2);
  if( (fill & 255) != 0 ) FillRect( xa, ya, xb, yb, fill );
  if( (stroke & 255) != 0 && width > 0. ) {
    double hw = 0.5*width;
    if( xb-xa > width && yb-ya > width ) { // four sides, not overlapping
      FillRect( xa-hw, ya-hw, xb+hw, ya+hw, stroke );
      FillRect( xa-hw, yb-hw, xb+hw, yb+hw, stroke );
      FillRect( xa-hw, ya+hw, xa+hw, yb-hw, stroke );
      FillRect( xb-hw, ya+hw, xb+hw, yb-hw, stroke );
    } else {
      FillRect( xa-hw, ya-hw, xb+hw, yb+hw, stroke );
    }
  }
}

//---------------------------------------------------------------------------
void GmSVGRaster::DrawPolygon( const std::vector<double>& pts, unsigned int stroke, unsigned int fill, double width )
{
  size_t np = pts.size()/2;
  if( np < 2 ) return;
  if( (fill & 255) != 0 && np >= 3 ) {
    std::vector< std::vector<double> > polys(1,pts);
    FillPolygons( polys, fill, true );
  }
  if( (stroke & 255) != 0 && width > 0. ) {
    std::vector< std::vector<double> > quads;
    for( size_t ip = 0; ip < np; ip++ ) {
      size_t jp = (ip+1)%np;
      std::vector<double> quad = SegmentQuad( pts[2*ip], pts[2*ip+1], pts[2*jp], pts[2*jp+1], width, 0.5*width );
      if( !quad.empty() ) quads.push_back(quad);
    }
    FillPolygons( quads, stroke, true );
  }
}

//---------------------------------------------------------------------------
void GmSVGRaster::DrawCircle( double cx, double cy, double rr, unsigned int stroke, unsigned int fill, double width )
{
  if( rr <= 0. ) return;
  if( (fill & 255) != 0 ) {
    std::vector< std::vector<double> > polys(1,CirclePolygon(cx,cy,rr,false));
    FillPolygons( polys, fill, false );
  }
  if( (stroke & 255) != 0 && width > 0. ) {
    std::vector< std::vector<double> > polys;
    polys.push_back( CirclePolygon(cx,cy,rr+0.5*width,false) );
    if( rr > 0.5*width ) polys.push_back( CirclePolygon(cx,cy,rr-0.5*width,true) );
    FillPolygons( polys, stroke, false );
  }
}

//---------------------------------------------------------------------------
void GmSVGRaster::DrawImage( double x1, double y1, double x2, double y2, int nx, int ny, const std::vector<unsigned char>& rgba )
{
  if( nx <= 0 || ny <= 0 || rgba.size() < size_t(nx)*size_t(ny)*4 ) return;
  double xa = std::min(x1,x2), xb = std::max(x1,x2);
  double ya = std::min(y1,y2), yb = std::max(y1,y2);
  if( xb <= xa || yb <= ya ) return;
  //--- pixels whose center is inside the rectangle, nearest neighbour sampling
  int px0 = std::max(0,int(std::ceil(xa-0.5)));
  int px1 = std::min(theW,int(std::ceil(xb-0.5)));
  int py0 = std::max(0,int(std::ceil(ya-0.5)));
  int py1 = std::min(theH,int(std::ceil(yb-0.5)));
  if( px1 <= px0 || py1 <= py0 ) return;
  std::vector<int> sxs(px1-px0);
  for( int px = px0; px < px1; px++ ) {
    sxs[px-px0] = std::max(0,std::min(nx-1,int((px+0.5-xa)/(xb-xa)*nx)));
  }
  int prevSy = -1;
  bool bPrevOpaque = false;
  const size_t rowBytes = size_t(px1-px0)*3;
  for( int py = py0; py < py1; py++ ) {
    int sy = std::max(0,std::min(ny-1,int((py+0.5-ya)/(yb-ya)*ny)));
    const unsigned char* srow = &rgba[size_t(sy)*nx*4];
    unsigned char* dst = &theRGB[(size_t(py)*theW + px0)*3];
    //--- same source row as previous pixel row, and opaque: copy it
    if( sy == prevSy && bPrevOpaque ) {
      std::memcpy( dst, dst-size_t(theW)*3, rowBytes );
      continue;
    }
    prevSy = sy;
    bPrevOpaque = true;
    for( int sx = 0; sx < nx; sx++ ) {
      if( srow[sx*4+3] != 255 ) { bPrevOpaque = false; break; }
    }
    for( int px = px0; px < px1; px++, dst += 3 ) {
      const unsigned char* sp = srow + sxs[px-px0]*4;
      unsigned int alpha = sp[3];
      if( alpha == 0 ) continue;
      if( alpha == 255 ) {
	dst[0] = sp[0]; dst[1] = sp[1]; dst[2] = sp[2];
      } else {
	float aa = alpha/255.f, rem = 1.f-aa;
	dst[0] = (unsigned char)(dst[0]*rem + sp[0]*aa + 0.5f);
	dst[1] = (unsigned char)(dst[1]*rem + sp[1]*aa + 0.5f);
	dst[2] = (unsigned char)(dst[2]*rem + sp[2]*aa + 0.5f);
      }
    }
  }
}

//---------------------------------------------------------------------------
double GmSVGRaster::TextWidth( const std::string& text, double sizePix )
{
  std::vector<unsigned int> codes = DecodeUTF8(text);
  double width = 0.;
  for( size_t ic = 0; ic < codes.size(); ic++ ) {
    GlyphRef ref = FindGlyph(codes[ic]);
    width += ref.font->Advance(ref.glyph)/ref.font->UnitsPerEm()*ref.scale*sizePix;
  }
  return width;
}

//---------------------------------------------------------------------------
void GmSVGRaster::DrawText( double xx, double yy, const std::string& text, double sizePix, unsigned int color,
			    int anchor, double angle )
{
  if( (color & 255) == 0 || sizePix <= 0. || text.empty() ) return;
  std::vector<unsigned int> codes = DecodeUTF8(text);
  double ang = angle*M_PI/180.;
  double ux = std::cos(ang), uy = -std::sin(ang);  // text direction (pixels, y downwards)
  double vx = -std::sin(ang), vy = -std::cos(ang); // 'up' direction
  double pen = 0.;
  if( anchor != 0 ) {
    double width = TextWidth(text,sizePix);
    pen = -(anchor == 1 ? 0.5 : 1.)*width;
  }
  std::vector< std::vector<double> > polys;
  for( size_t ic = 0; ic < codes.size(); ic++ ) {
    GlyphRef ref = FindGlyph(codes[ic]);
    const TTFont& font = *(ref.font);
    double sc = sizePix*ref.scale/font.UnitsPerEm(); // font units to pixels
    double rise = ref.rise*sizePix;
    std::vector<TTFont::Contour> contours;
    font.GetContours( ref.glyph, contours );
    for( size_t ico = 0; ico < contours.size(); ico++ ) {
      const TTFont::Contour& cont = contours[ico];
      size_t np = cont.size();
      //--- start at an on-curve point (or the middle of two off-curve points)
      size_t istart = 0;
      while( istart < np && !cont[istart].on ) istart++;
      TTFont::Pt start;
      if( istart == np ) {
	start.x = 0.5*(cont[0].x+cont[1].x);
	start.y = 0.5*(cont[0].y+cont[1].y);
	start.on = true;
	istart = 0;
      } else {
	start = cont[istart];
      }
      std::vector<double> fu; // flattened, font units
      fu.push_back(start.x); fu.push_back(start.y);
      TTFont::Pt prev = start;
      bool bCtrl = false;
      TTFont::Pt ctrl = start;
      auto quad = [&]( const TTFont::Pt& p0, const TTFont::Pt& p1, const TTFont::Pt& p2 ) {
	double lenPix = (std::hypot(p1.x-p0.x,p1.y-p0.y) + std::hypot(p2.x-p1.x,p2.y-p1.y))*sc;
	int nseg = std::max(2,std::min(24,int(std::sqrt(lenPix)*1.2)));
	for( int is = 1; is <= nseg; is++ ) {
	  double tt = double(is)/nseg, mt = 1.-tt;
	  fu.push_back( mt*mt*p0.x + 2*mt*tt*p1.x + tt*tt*p2.x );
	  fu.push_back( mt*mt*p0.y + 2*mt*tt*p1.y + tt*tt*p2.y );
	}
      };
      for( size_t kk = 1; kk <= np; kk++ ) {
	const TTFont::Pt& pt = (kk == np && cont[istart].on) ? start : cont[(istart+kk)%np];
	bool bLast = (kk == np);
	if( pt.on ) {
	  if( bCtrl ) {
	    quad(prev,ctrl,pt);
	  } else {
	    fu.push_back(pt.x); fu.push_back(pt.y);
	  }
	  prev = pt;
	  bCtrl = false;
	} else {
	  if( bCtrl ) { // two off-curve points: implied on-curve point in the middle
	    TTFont::Pt mid;
	    mid.x = 0.5*(ctrl.x+pt.x); mid.y = 0.5*(ctrl.y+pt.y); mid.on = true;
	    quad(prev,ctrl,mid);
	    prev = mid;
	  }
	  ctrl = pt;
	  bCtrl = true;
	}
	if( bLast && bCtrl ) quad(prev,ctrl,start); // close the contour
      }
      //--- to pixels
      std::vector<double> poly;
      poly.reserve(fu.size());
      for( size_t ip = 0; ip+1 < fu.size(); ip += 2 ) {
	double uu = pen + fu[ip]*sc;
	double vv = fu[ip+1]*sc + rise;
	poly.push_back( xx + uu*ux + vv*vx );
	poly.push_back( yy + uu*uy + vv*vy );
      }
      if( poly.size() >= 6 ) polys.push_back(poly);
    }
    pen += font.Advance(ref.glyph)*sc;
  }
  FillPolygons( polys, color, false );
}

//---------------------------------------------------------------------------
void GmSVGRaster::Render( const std::vector<GmSVGPrim>& prims )
{
  std::fill(theRGB.begin(),theRGB.end(),(unsigned char)(255));
  for( size_t ip = 0; ip < prims.size(); ip++ ) {
    const GmSVGPrim& prim = prims[ip];
    const std::vector<double>& pp = prim.pts;
    switch( prim.type ) {
    case GmSVGPrim::kLine:
      if( pp.size() >= 4 ) StrokeLine( pp[0], pp[1], pp[2], pp[3], prim.width, prim.stroke, prim.dash );
      break;
    case GmSVGPrim::kRect:
      if( pp.size() >= 4 ) DrawRect( pp[0], pp[1], pp[2], pp[3], prim.stroke, prim.fill, prim.width );
      break;
    case GmSVGPrim::kPolygon:
      DrawPolygon( pp, prim.stroke, prim.fill, prim.width );
      break;
    case GmSVGPrim::kCircle:
      if( pp.size() >= 3 ) DrawCircle( pp[0], pp[1], pp[2], prim.stroke, prim.fill, prim.width );
      break;
    case GmSVGPrim::kText:
      if( pp.size() >= 2 ) DrawText( pp[0], pp[1], prim.text, prim.size, prim.fill, prim.anchor, prim.angle );
      break;
    case GmSVGPrim::kImage:
      if( pp.size() >= 4 ) DrawImage( pp[0], pp[1], pp[2], pp[3], prim.nx, prim.ny, prim.rgba );
      break;
    }
  }
}

//---------------------------------------------------------------------------
bool GmSVGRaster::Write( const std::string& fileName, const std::string& formatIn ) const
{
  std::string format = formatIn;
  for( size_t ic = 0; ic < format.size(); ic++ ) format[ic] = char(std::tolower(format[ic]));
  std::string out;
  TakeFromPool(OutPool(),out);
  out.clear();
  struct OutReturn { std::string& ss; ~OutReturn() { ReturnToPool(OutPool(),ss); } } outReturn{out};
  bool bOK = false;
  if( format == "png" ) bOK = WritePNG(out);
  else if( format == "jpg" || format == "jpeg" ) bOK = WriteJPG(out);
  else if( format == "gif" ) bOK = WriteGIF(out);
  else if( format == "bmp" ) bOK = WriteBMP(out);
  else if( format == "tiff" || format == "tif" ) bOK = WriteTIFF(out);
  else {
    std::cerr << " !!! GmSVGRaster::Write: format not supported: " << formatIn << std::endl;
    return false;
  }
  if( !bOK ) {
    std::cerr << " !!! GmSVGRaster::Write: encoding failed for " << fileName << std::endl;
    return false;
  }
  std::ofstream fout(fileName.c_str(),std::ios::binary);
  if( !fout ) {
    std::cerr << " !!! GmSVGRaster::Write: cannot open file " << fileName << std::endl;
    return false;
  }
  fout.write(out.data(),out.size());
  fout.close();
  return !fout.fail();
}

//---------------------------------------------------------------------------
bool GmSVGRaster::WritePNG( std::string& out ) const
{
  //--- RGB rows; filter 'Up' for rows equal to the previous one (all zeros), 'Sub' for the others
  const size_t rowBytes = size_t(theW)*3;
  const size_t rowLen = 1 + rowBytes;
  std::string raw(size_t(theH)*rowLen,'\0');
  for( int iy = 0; iy < theH; iy++ ) {
    const unsigned char* cur = &theRGB[size_t(iy)*rowBytes];
    char* row = &raw[size_t(iy)*rowLen];
    if( iy > 0 && std::memcmp(cur,cur-rowBytes,rowBytes) == 0 ) {
      row[0] = char(2);
      continue;
    }
    row[0] = char(1);
    for( size_t ib = 0; ib < 3 && ib < rowBytes; ib++ ) row[1+ib] = char(cur[ib]);
    for( size_t ib = 3; ib < rowBytes; ib++ ) row[1+ib] = char((unsigned char)(cur[ib]-cur[ib-3]));
  }
  z_stream zs;
  zs.zalloc = Z_NULL;
  zs.zfree = Z_NULL;
  zs.opaque = Z_NULL;
  if( deflateInit2( &zs, 6, Z_DEFLATED, 15, 8, Z_RLE ) != Z_OK ) return false;
  std::string comp(deflateBound(&zs,raw.size()),'\0');
  zs.next_in = reinterpret_cast<Bytef*>(&raw[0]);
  zs.avail_in = uInt(raw.size());
  zs.next_out = reinterpret_cast<Bytef*>(&comp[0]);
  zs.avail_out = uInt(comp.size());
  int ret = deflate( &zs, Z_FINISH );
  size_t compLen = zs.total_out;
  deflateEnd(&zs);
  if( ret != Z_STREAM_END ) return false;
  comp.resize(compLen);

  auto chunk = [&]( const std::string& type, const std::string& data ) {
    PutBE32(out,(unsigned int)(data.size()));
    std::string td = type + data;
    out += td;
    PutBE32(out,(unsigned int)(crc32( 0L, reinterpret_cast<const Bytef*>(td.data()), uInt(td.size()) )));
  };
  std::string ihdr;
  PutBE32(ihdr,theW);
  PutBE32(ihdr,theH);
  ihdr += char(8); // bit depth
  ihdr += char(2); // color type RGB
  ihdr += char(0); ihdr += char(0); ihdr += char(0);
  out = "\x89PNG\r\n\x1a\n";
  chunk("IHDR",ihdr);
  chunk("IDAT",comp);
  chunk("IEND","");
  return true;
}

//---------------------------------------------------------------------------
bool GmSVGRaster::WriteJPG( std::string& out ) const
{
  out.reserve(size_t(theW)*theH/8);
  return jpeg::Write( out, &theRGB[0], theW, theH, theJpegQuality );
}

//---------------------------------------------------------------------------
bool GmSVGRaster::WriteBMP( std::string& out ) const
{
  const size_t rowBytes = (size_t(theW)*3 + 3) & ~size_t(3);
  const size_t dataSize = rowBytes*theH;
  out = "BM";
  PutLE32(out,(unsigned int)(54+dataSize));
  PutLE32(out,0);
  PutLE32(out,54);
  PutLE32(out,40); // BITMAPINFOHEADER
  PutLE32(out,theW);
  PutLE32(out,theH); // bottom-up
  PutLE16(out,1);
  PutLE16(out,24);
  PutLE32(out,0); // no compression
  PutLE32(out,(unsigned int)(dataSize));
  PutLE32(out,2835); // 72 dpi
  PutLE32(out,2835);
  PutLE32(out,0);
  PutLE32(out,0);
  size_t start = out.size();
  out.resize(start+dataSize,'\0');
  for( int iy = 0; iy < theH; iy++ ) {
    const unsigned char* src = &theRGB[size_t(theH-1-iy)*theW*3];
    char* dst = &out[start+size_t(iy)*rowBytes];
    for( int ix = 0; ix < theW; ix++ ) {
      dst[3*ix] = char(src[3*ix+2]);
      dst[3*ix+1] = char(src[3*ix+1]);
      dst[3*ix+2] = char(src[3*ix]);
    }
  }
  return true;
}

//---------------------------------------------------------------------------
bool GmSVGRaster::WriteTIFF( std::string& out ) const
{
  //--- baseline TIFF, RGB, PackBits compression (each row packed separately), one strip
  std::string data;
  const size_t rowBytes = size_t(theW)*3;
  for( int iy = 0; iy < theH; iy++ ) {
    const unsigned char* row = &theRGB[size_t(iy)*rowBytes];
    size_t ii = 0;
    while( ii < rowBytes ) {
      //--- run of equal bytes
      size_t run = 1;
      while( ii+run < rowBytes && run < 128 && row[ii+run] == row[ii] ) run++;
      if( run >= 3 ) {
	data += char(1-int(run));
	data += char(row[ii]);
	ii += run;
	continue;
      }
      //--- literal bytes, until a run of 3 equal bytes
      size_t lit = 0;
      while( ii+lit < rowBytes && lit < 128 ) {
	if( ii+lit+2 < rowBytes && row[ii+lit] == row[ii+lit+1] && row[ii+lit] == row[ii+lit+2] ) break;
	lit++;
      }
      data += char(lit-1);
      data.append(reinterpret_cast<const char*>(row+ii),lit);
      ii += lit;
    }
  }
  const unsigned int nTags = 12;
  const unsigned int ifdOffset = 8;
  const unsigned int ifdSize = 2 + nTags*12 + 4;
  const unsigned int bpsOffset = ifdOffset + ifdSize;
  const unsigned int xresOffset = bpsOffset + 6;
  const unsigned int yresOffset = xresOffset + 8;
  const unsigned int dataOffset = yresOffset + 8;
  out = "II";
  PutLE16(out,42);
  PutLE32(out,ifdOffset);
  PutLE16(out,nTags);
  auto tag = [&]( unsigned int id, unsigned int type, unsigned int count, unsigned int value ) {
    PutLE16(out,id); PutLE16(out,type); PutLE32(out,count);
    if( type == 3 && count == 1 ) { PutLE16(out,value); PutLE16(out,0); }
    else PutLE32(out,value);
  };
  tag(256, 4, 1, theW);          // ImageWidth
  tag(257, 4, 1, theH);          // ImageLength
  tag(258, 3, 3, bpsOffset);     // BitsPerSample 8,8,8
  tag(259, 3, 1, 32773);         // Compression: PackBits
  tag(262, 3, 1, 2);             // PhotometricInterpretation: RGB
  tag(273, 4, 1, dataOffset);    // StripOffsets
  tag(277, 3, 1, 3);             // SamplesPerPixel
  tag(278, 4, 1, theH);          // RowsPerStrip
  tag(279, 4, 1, (unsigned int)(data.size())); // StripByteCounts
  tag(282, 5, 1, xresOffset);    // XResolution
  tag(283, 5, 1, yresOffset);    // YResolution
  tag(296, 3, 1, 2);             // ResolutionUnit: inch
  PutLE32(out,0); // no more IFDs
  PutLE16(out,8); PutLE16(out,8); PutLE16(out,8);
  PutLE32(out,72); PutLE32(out,1);
  PutLE32(out,72); PutLE32(out,1);
  out += data;
  return true;
}

//---------------------------------------------------------------------------
bool GmSVGRaster::WriteGIF( std::string& out ) const
{
  //--- palette: the image colors if they are at most 256, else the 256 most frequent ones
  //    (the other colors, mainly from antialiasing, are mapped to the closest one)
  const size_t nPix = size_t(theW)*theH;
  std::unordered_map<unsigned int,unsigned int> counts;
  for( size_t ip = 0; ip < nPix; ip++ ) {
    unsigned int col = (unsigned int)(theRGB[3*ip]) << 16 | (unsigned int)(theRGB[3*ip+1]) << 8 | theRGB[3*ip+2];
    counts[col]++;
  }
  std::vector< std::pair<unsigned int,unsigned int> > sorted; // (count, color)
  sorted.reserve(counts.size());
  for( std::unordered_map<unsigned int,unsigned int>::const_iterator ite = counts.begin(); ite != counts.end(); ite++ ) {
    sorted.push_back( std::make_pair(ite->second,ite->first) );
  }
  std::sort(sorted.begin(),sorted.end(),
	    []( const std::pair<unsigned int,unsigned int>& a, const std::pair<unsigned int,unsigned int>& b ) {
	      return a.first > b.first || (a.first == b.first && a.second < b.second); } );
  size_t nPal = std::min(size_t(256),sorted.size());
  std::vector<unsigned int> palette(256,0);
  std::unordered_map<unsigned int,unsigned char> index;
  for( size_t ic = 0; ic < nPal; ic++ ) {
    palette[ic] = sorted[ic].second;
    index[sorted[ic].second] = (unsigned char)(ic);
  }
  for( size_t ic = nPal; ic < sorted.size(); ic++ ) {
    unsigned int col = sorted[ic].second;
    int rr = (col>>16)&255, gg = (col>>8)&255, bb = col&255;
    int best = 0;
    long bestD = -1;
    for( size_t jc = 0; jc < nPal; jc++ ) {
      int dr = rr-int((palette[jc]>>16)&255), dg = gg-int((palette[jc]>>8)&255), db = bb-int(palette[jc]&255);
      long dd = 2L*dr*dr + 4L*dg*dg + 3L*db*db;
      if( bestD < 0 || dd < bestD ) {
	bestD = dd;
	best = int(jc);
      }
    }
    index[col] = (unsigned char)(best);
  }
  std::vector<unsigned char> pix(nPix);
  unsigned int lastCol = 0xFFFFFFFF;
  unsigned char lastIdx = 0;
  for( size_t ip = 0; ip < nPix; ip++ ) {
    unsigned int col = (unsigned int)(theRGB[3*ip]) << 16 | (unsigned int)(theRGB[3*ip+1]) << 8 | theRGB[3*ip+2];
    if( col != lastCol ) {
      lastIdx = index[col];
      lastCol = col;
    }
    pix[ip] = lastIdx;
  }

  //--- header, logical screen with global color table of 256 entries
  out = "GIF89a";
  PutLE16(out,theW);
  PutLE16(out,theH);
  out += char(0xF7); // global color table, 8 bits, 256 entries
  out += char(0);    // background color index
  out += char(0);    // aspect ratio
  for( size_t ic = 0; ic < 256; ic++ ) {
    out += char((palette[ic]>>16)&255); out += char((palette[ic]>>8)&255); out += char(palette[ic]&255);
  }
  //--- image descriptor
  out += char(0x2C);
  PutLE16(out,0); PutLE16(out,0);
  PutLE16(out,theW); PutLE16(out,theH);
  out += char(0); // no local color table, not interlaced
  //--- LZW compressed data
  const int minCodeSize = 8;
  out += char(minCodeSize);
  const int clearCode = 1 << minCodeSize;
  const int eoiCode = clearCode+1;
  std::string block;
  unsigned int bitBuf = 0;
  int nBits = 0;
  auto putCode = [&]( int code, int codeSize ) {
    bitBuf |= (unsigned int)(code) << nBits;
    nBits += codeSize;
    while( nBits >= 8 ) {
      block += char(bitBuf & 255);
      bitBuf >>= 8;
      nBits -= 8;
      if( block.size() == 255 ) {
	out += char(255);
	out += block;
	block.clear();
      }
    }
  };
  //--- dictionary: hash table (prefix code, byte) -> code
  const int hashSize = 5003;
  std::vector<int> hashKey(hashSize,-1), hashCode(hashSize,0);
  int codeSize = minCodeSize+1;
  int nextCode = eoiCode+1;
  putCode(clearCode,codeSize);
  int prefix = pix.empty() ? 0 : pix[0];
  for( size_t ip = 1; ip < nPix; ip++ ) {
    int cc = pix[ip];
    int key = (prefix << 8) | cc;
    int hh = (key ^ (key >> 12)) % hashSize;
    bool bFound = false;
    while( hashKey[hh] != -1 ) {
      if( hashKey[hh] == key ) {
	bFound = true;
	break;
      }
      hh = (hh+1) % hashSize;
    }
    if( bFound ) {
      prefix = hashCode[hh];
      continue;
    }
    putCode(prefix,codeSize);
    if( nextCode < 4096 ) {
      hashKey[hh] = key;
      hashCode[hh] = nextCode;
      if( nextCode == (1 << codeSize) ) codeSize++;
      nextCode++;
    } else { // dictionary full: restart
      putCode(clearCode,codeSize);
      std::fill(hashKey.begin(),hashKey.end(),-1);
      codeSize = minCodeSize+1;
      nextCode = eoiCode+1;
    }
    prefix = cc;
  }
  putCode(prefix,codeSize);
  putCode(eoiCode,codeSize);
  if( nBits > 0 ) {
    block += char(bitBuf & 255);
    if( block.size() == 255 ) {
      out += char(255);
      out += block;
      block.clear();
    }
  }
  if( !block.empty() ) {
    out += char(block.size());
    out += block;
  }
  out += char(0);    // end of image data
  out += char(0x3B); // trailer
  return true;
}
