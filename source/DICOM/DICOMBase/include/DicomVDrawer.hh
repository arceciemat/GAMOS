#ifndef DicomVDrawer__HH
#define DicomVDrawer__HH

#include "DicomVOperator.hh"
#include <vector>

class DicomVImage;
class DicomPolygonSet;
class DicomVLineSet;
class DicomVLineList;
class DicomVLine;
class TLine;

class DicomVDrawer 
{ 
public:
  DicomVDrawer();
  ~DicomVDrawer(){};

public:
  virtual void SetOutputFile( G4String foutName ) = 0;
  virtual void DrawXY( DicomVImage*, std::vector<DicomVLineSet*>, G4String extraFileName = "" ) = 0;
  virtual void DrawXZ( DicomVImage*, std::vector<DicomVLineSet*>, G4String extraFileName = "" ) = 0;
  virtual void DrawYZ( DicomVImage*, std::vector<DicomVLineSet*>, G4String extraFileName = "" ) = 0;
  virtual void DrawWithLinesXY( DicomVImage*, DicomVLineList* ) = 0;
  virtual void DrawWithLinesXZ( DicomVImage*, DicomVLineList* ) = 0;
  virtual void DrawWithLinesYZ( DicomVImage*, DicomVLineList* ) = 0;
  virtual void DrawWithLinesXY( DicomVImage*, DicomVLine* ) = 0;
  virtual void DrawWithLinesXZ( DicomVImage*, DicomVLine* ) = 0;
  virtual void DrawWithLinesYZ( DicomVImage*, DicomVLine* ) = 0;
  //  virtual void DrawLinesXY( DicomVLineSet* lineSet, G4double minZ, G4double maxZ ) = 0;
  virtual void DrawLineColorAndStyle( TLine* line, G4String name, G4bool bDrawText ) = 0;
  virtual G4double GetValueAround(size_t , size_t , size_t , G4double , DicomVImage* , std::vector<G4double>* ) { return 0.;};

  void SetOverSurrounding( G4double val ) {
    theOverSurrounding = val;
    bAverageSurroundingBig = true;
  }
  
protected:
  G4bool bColorsInitialized;
  G4bool bAverageSurroundingBig;
  G4double theOverSurrounding;
  
};

#endif
  
