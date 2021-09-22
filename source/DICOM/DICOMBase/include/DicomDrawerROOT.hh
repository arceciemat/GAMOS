#ifndef DicomDrawerROOT__HH
#define DicomDrawerROOT__HH

#include "DicomVDrawer.hh"
#include "DicomMgr.hh"
#include "DicomVLine.hh"
class TLine;
class TFile;
//class DicomOperLog;
class DicomParameterMgr;
#include <set>

class DicomDrawerROOT : public DicomVDrawer
{ 
public:
  DicomDrawerROOT();
  ~DicomDrawerROOT(){};

public:
  virtual void SetOutputFile( G4String foutName );

  virtual void DrawXY( DicomVImage*, std::vector<DicomVLineSet*>, G4String extraFileName = "" );
  virtual void DrawXZ( DicomVImage*, std::vector<DicomVLineSet*>, G4String extraFileName = "" );
  virtual void DrawYZ( DicomVImage*, std::vector<DicomVLineSet*>, G4String extraFileName = "" );
  virtual void DrawWithLinesXY( DicomVImage*, DicomVLineList* );
  virtual void DrawWithLinesXZ( DicomVImage*, DicomVLineList* );
  virtual void DrawWithLinesYZ( DicomVImage*, DicomVLineList* );
  virtual void DrawWithLinesXY( DicomVImage*, DicomVLine* );
  virtual void DrawWithLinesXZ( DicomVImage*, DicomVLine* );
  virtual void DrawWithLinesYZ( DicomVImage*, DicomVLine* );
  virtual void DrawLinesXY( DicomVLineSet* lineSet, G4double minZ, G4double maxZ );
  virtual void DrawLinesXZ( DicomVLineSet* lineSet, G4double minZ, G4double maxZ );
  virtual void DrawLinesYZ( DicomVLineSet* lineSet, G4double minZ, G4double maxZ );
  virtual void DrawLines( DicomVLineSet* lineSet, G4double minPos, G4double maxPos, size_t ix, size_t iy, DPOrientation orient );
  void DrawLineColorAndStyle( TLine* line, G4String name, G4bool bDrawText );

  void SetImageFormat( G4String imgf ) {
    theImageFormat = imgf;
  }
  void SetCanvasNPixels( G4int cp ) {
    theCanvasNPixelsX = cp;
    theCanvasNPixelsY = cp;
    theCanvasNPixelsZ = cp;
  }
  void SetCanvasNPixelsX( G4int cp ) {
    theCanvasNPixelsX = cp;
  }
  void SetCanvasNPixelsY( G4int cp ) {
    theCanvasNPixelsY = cp; 
  }
    void SetCanvasNPixelsZ( G4int cp ) {
    theCanvasNPixelsZ = cp;
  }

  void SetPaletteMin( G4double pmin ) {
    thePaletteMin = pmin;
  }
  void  SetPaletteMax( G4double pmax ) {
    thePaletteMax = pmax;
  }

private:
  DicomPolygonSet* VoxelsInUseClosest( DicomPolygonSet* polySet, DicomVImage* image );
  virtual G4double GetValueSurrounding(size_t ix, size_t iy, size_t iz, G4double value, DicomVImage* image, std::vector<G4double>* imgDatap );
  
private:
  G4int theCanvasNPixelsX;
  G4int theCanvasNPixelsY;
  G4int theCanvasNPixelsZ;
  G4String theImageFormat;  
  G4double thePaletteMin;
  G4double thePaletteMax;
  //  DicomOperLog* theOperatorLog;
  DicomParameterMgr* theParamMgr;
  G4bool bRotateXY180;
  msldo theLineDrawingOptions;
  G4bool bLineDrawingOptions;
  G4bool bShowTitle;
  std::set<DicomVImage*> theRotateXY180Images;

  TFile* theOutputFile;
};

#endif
  
