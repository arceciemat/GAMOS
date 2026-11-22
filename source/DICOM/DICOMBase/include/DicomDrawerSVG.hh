#ifndef DicomDrawerSVG__HH
#define DicomDrawerSVG__HH
//---------------------------------------------------------------------------
// Same as DicomDrawerROOT, but without ROOT: each image slice is drawn as an
// SVG file (GmSVGCanvas), with the same names as the ROOT figures
// ("his"+NAME+".svg"), and written also in the format given by the parameter imageFormat
// (default jpg): png/jpg/gif/bmp/tiff with the built-in renderer of GmSVGCanvas, other formats
// converted from the SVG file if an external converter is found (rsvg-convert, inkscape, magick, convert).
// The colors are those of the ROOT palette set with GmSVGCanvas::SetPalette (parameter palette of the
// DICOM utilities, default 55, kRainBow). Parameters:
//   imageFormat   svg/png/jpg/gif/bmp/tiff/pdf/ps/eps/webp   (svg: no conversion)
//                 (/gamos/SVG/imageFormat, if set, overrides it)
//   SVGConverter  auto/builtin/none/program
//   nContours     number of color levels (default 20, as ROOT)
// plus the same as DicomDrawerROOT: nPixelsX/Y/Z, paletteMin/Max, bFigureTitle, closestPolygon
//---------------------------------------------------------------------------
#include "DicomVDrawer.hh"
#include "DicomMgr.hh"
#include "DicomVLine.hh"
#include <set>

class GmSVGCanvas;
class GmSVGFrame;
class GmHistoWriterCSV;
// GmHisto1/GmHisto2 are ROOT TH1F/TH2F if GAMOS is built with ROOT (as in GmAnalysisMgr.hh)
#ifdef GAMOS_ROOT
class TH1F;
typedef TH1F GmHisto1;
#else
class GmHisto1;
#endif
class DicomParameterMgr;

class DicomDrawerSVG : public DicomVDrawer
{
public:
  DicomDrawerSVG();
  ~DicomDrawerSVG();

public:
  // Without ROOT, histograms are written as text (a '.root' extension is changed to '.txt')
  virtual void SetOutputFile( G4String foutName );
  virtual void CloseOutputFile();

  virtual void DrawXY( DicomVImage*, std::vector<DicomVLineSet*>, G4String extraFileName = "", G4bool bOnlyNonZero = false );
  virtual void DrawXZ( DicomVImage*, std::vector<DicomVLineSet*>, G4String extraFileName = "", G4bool bOnlyNonZero = false );
  virtual void DrawYZ( DicomVImage*, std::vector<DicomVLineSet*>, G4String extraFileName = "", G4bool bOnlyNonZero = false );
  virtual void DrawWithLinesXY( DicomVImage*, DicomVLineList* ){}
  virtual void DrawWithLinesXZ( DicomVImage*, DicomVLineList* ){}
  virtual void DrawWithLinesYZ( DicomVImage*, DicomVLineList* ){}
  virtual void DrawWithLinesXY( DicomVImage*, DicomVLine* ){}
  virtual void DrawWithLinesXZ( DicomVImage*, DicomVLine* ){}
  virtual void DrawWithLinesYZ( DicomVImage*, DicomVLine* ){}
  virtual void DrawLinesXY( DicomVLineSet* lineSet, G4double minZ, G4double maxZ );
  virtual void DrawLinesXZ( DicomVLineSet* lineSet, G4double minY, G4double maxY );
  virtual void DrawLinesYZ( DicomVLineSet* lineSet, G4double minX, G4double maxX );
  virtual void DrawLines( DicomVLineSet* lineSet, G4double minPos, G4double maxPos, size_t ix, size_t iy, DPOrientation orient );
  // ROOT interface of DicomVDrawer, not used
  virtual void DrawLineColorAndStyle( TLine*, G4String, G4bool ) {}
  void DrawSegmentColorAndStyle( G4double x1, G4double y1, G4double x2, G4double y2, G4String name, G4bool bDrawText );

  void SetImageFormat( G4String imgf ) {
    theImageFormat = CheckImageFormat(imgf);
  }
  // Check that the format is accepted (G4Exception if not); returns it in lower case ("jpeg" -> "jpg")
  static G4String CheckImageFormat( const G4String& imgf );
  void SetCanvasNPixels( G4int cp ) {
    theCanvasNPixelsX = cp;
    theCanvasNPixelsY = cp;
    theCanvasNPixelsZ = cp;
  }
  void SetCanvasNPixelsX( G4int cp ) { theCanvasNPixelsX = cp; }
  void SetCanvasNPixelsY( G4int cp ) { theCanvasNPixelsY = cp; }
  void SetCanvasNPixelsZ( G4int cp ) { theCanvasNPixelsZ = cp; }
  void SetPaletteMin( G4double pmin ) { thePaletteMin = pmin; }
  void SetPaletteMax( G4double pmax ) { thePaletteMax = pmax; }

  virtual G4bool GetFigureTitle() const { return bFigureTitle; }
  virtual void SetFigureTitle( G4bool bst ) { bFigureTitle = bst; }

private:
  //--- 1D histograms of image values (bDrawHisto1)
  GmHisto1* NewHisto1( const G4String& hisName, G4double histoMax );
  void DrawImage2D( const G4String& hisName, const std::vector<G4double>& values,
		    size_t nx, G4double xMin, G4double xMax, size_t ny, G4double yMin, G4double yMax,
		    G4int canvasW, G4int canvasH, G4double marginLeft, G4double marginBottom,
		    G4bool bReverseX, G4bool bReverseY,
		    DicomVImage* image = 0, G4int iAxisU = 0, G4int iAxisV = 1, G4double planePos = 0. );
  // background (CT) image: grey value at a point given in the drawing coordinates of 'image'
  G4bool GetBackgroundGrey( DicomVImage* image, G4ThreeVector pos, G4double& grey );
  // flip a position as the image data have been flipped by the ReflectY/RotateXY180 operators (it is its own inverse)
  void FlipAsImage( G4ThreeVector& pos, DicomVImage* image );
  // drawing coordinate of image (axis 0 = U, 1 = V of the figure) -> frame coordinate (they differ only if there is a background image)
  G4double ToFrame( G4int iuv, G4double val ) const;
  void PrintFigure( const G4String& figName );
  void DrawHisto1( GmHisto1* his );
  void WriteHisto2D( const G4String& hisName, const std::vector<G4double>& values,
		     size_t nx, G4double xMin, G4double xMax, size_t ny, G4double yMin, G4double yMax );
  void WriteHisto1( GmHisto1* his );
  void ApplyImageOperators( DicomVImage* image, G4bool bApplyReflectY );

private:
  G4int theCanvasNPixelsX;
  G4int theCanvasNPixelsY;
  G4int theCanvasNPixelsZ;
  G4String theImageFormat;
  G4String theSVGConverter;
  G4int theNContours;
  G4double thePaletteMin;
  G4double thePaletteMax;
  DicomParameterMgr* theParamMgr;
  G4bool bReflectY;
  G4bool bRotateXY180;
  vpsldo theLineDrawingOptions;
  G4bool bFigureTitle;
  std::set<DicomVImage*> theReflectYImages;
  std::set<DicomVImage*> theRotateXY180Images;
  G4double theOverlayOpacity; // opacity of the colors drawn over the background (CT) image
  G4double theCTMin; // CT values drawn as black (default: minimum of CT image)
  G4double theCTMax; // CT values drawn as white (default: maximum of CT image)
  DicomVImage* theCTWindowImage; // image for which theCTMinUsed/theCTMaxUsed were computed
  G4double theCTMinUsed;
  G4double theCTMaxUsed;
  //--- with a background image the frame is in real coordinates (reversed if the labels are reversed)
  G4bool bFrameTransform;
  G4bool bFrameReverse[2]; // labels reversed in U, V
  G4bool bFrameFlip[2]; // image data flipped in U, V
  G4double theFrameSum[2]; // frame minimum + maximum in U, V
  G4double theFrameImageC2[2]; // 2 * image centre in U, V

  GmSVGCanvas* theCanvas;
  GmSVGFrame* theFrame; // frame of current image
  GmHistoWriterCSV* theOutputFile; // histograms (as DicomDrawerROOT .root file) in GAMOS .csv format
};

#endif
