#ifndef DicomLineDrawingOptions__HH
#define DicomLineDrawingOptions__HH

#include "G4ThreeVector.hh"
#include <vector>
#include "TColor.h"

class DicomLineDrawingOptions
{ 
public:
  DicomLineDrawingOptions( );
  DicomLineDrawingOptions( std::vector<G4String> wl );
  ~DicomLineDrawingOptions(){};
  
  G4String GetName() const {
    return theName;
  }
  G4int GetOption() const { 
    return theOption;
  }
  G4int GetStyle() const {
    return theStyle;
  }
  EColor GetTColor() const {
    return theTColor;
  }
  G4int GetWidth() const {
    return theWidth;
  }
  G4double GetTextSize() const {
    return theTextSize;
  }
  EColor GetColor( int icol );
  
protected:
  G4String theName;
  G4int theOption; // 0: no drawing, 1: draw line, 2: draw also value
  G4int theStyle;
  EColor theTColor;
  G4int theWidth;
  G4double theTextSize;
};

#endif
  
