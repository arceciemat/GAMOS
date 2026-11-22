#include "DicomLineDrawingOptions.hh"
#include "DicomVerbosity.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
//#include "ROOTUtilities/PlotData/PlotUtils.C"

//-----------------------------------------------------------------------------
DicomLineDrawingOptions::DicomLineDrawingOptions()
{
  theName = "";
  theOption = 2;
#ifdef GAMOS_ROOT
  theTColor = kBlack;
#endif
  theColorIndex = 1; // kBlack
  theStyle = 1;
  theWidth = 1;
  theTextSize = 0.01;
  
}

//-----------------------------------------------------------------------------
DicomLineDrawingOptions::DicomLineDrawingOptions( std::vector<G4String> wl )
{
  if( wl.size() < 2 || wl.size() > 6 ) {
    G4cerr << " NUMBER OF WORDS " << wl.size() << G4endl;
    for( size_t ii = 0; ii < wl.size(); ii++ ) {
      G4cerr << wl[ii];
      G4cerr << G4endl;
    }
    G4Exception("DicomLineDrawingOptions",
		"Reading LINE DRAWING OPTIONS file",
		FatalException,
		"Line must have between 2 and 5 words : NAME DRAWING_OPTION (COLOR) (LINE_STYLE) (LINE_WIDTH)");
  }
  
  theName = wl[0];
  theOption = GmGenUtils::GetInteger(wl[1]); // 0: no drawing, 1: draw line, 2: draw also value
#ifdef GAMOS_ROOT
  if( wl.size() >= 3 ) {
    theTColor = GetColor(GmGenUtils::GetInteger(wl[2]));
  } else {
    theTColor = kBlack;
  }
#endif
  if( wl.size() >= 3 ) {
    theColorIndex = GetColorIndexFromNumber(GmGenUtils::GetInteger(wl[2]));
  } else {
    theColorIndex = 1; // kBlack
  }
  if( wl.size() >= 4 ) {
    theStyle = GmGenUtils::GetInteger(wl[3]);
  } else {
    theStyle = 1;
  }
  if( wl.size() >= 5 ) {
    theWidth = GmGenUtils::GetValue(wl[4]);
  } else {
    theWidth = 1;
  }
  if( wl.size() >= 6 ) {
    theTextSize = GmGenUtils::GetValue(wl[5]);
  } else {
    theTextSize = 0.01;
  }

  //  G4cout << " SET DRAW LINE OPTIONS " << name << " : " << GetOption() << " " << GetStyle() << " " << GetTColor() << " " << GetWidth() << G4endl; //GDEB

}

#ifdef GAMOS_ROOT
EColor DicomLineDrawingOptions::GetColor( int icol )
{
  // kWhite  = 0,   kBlack  = 1,   kGray    = 920,  kRed    = 632,  kGreen  = 416,
  // kBlue   = 600, kYellow = 400, kMagenta = 616,  kCyan   = 432,  kOrange = 800,
  // kSpring = 820, kTeal   = 840, kAzure   =  860, kViolet = 880,  kPink   = 900

  int icol1 = icol/14;
  int icol2 = icol%14;
  switch (icol2)  {
    case 0:
      return EColor(kBlack+icol1);
    case 1:
      return EColor(kRed);
    case 2:
      return EColor(kGreen);
    case 3:
      return EColor(kBlue);
    case 4:
      return EColor(kMagenta+icol1);
    case 5:
      return EColor(kYellow+icol1+1);
    case 6:
      return EColor(kCyan+icol1);
    case 7:
      return EColor(kViolet+2);
    case 8:
      return EColor(kOrange+icol1+1);
    case 9:
      return EColor(kOrange+3);
    case 10:
      return EColor(kPink+icol1+1);
    case 11:
      //      return EColor(kSpring+icol1+1);
      return EColor(kTeal-7);
    case 12:
      return EColor(kAzure+icol1+1); 
    case 13:
      return EColor(kGray+icol1);
    }

  return EColor(kBlack);
  
}
#endif

//-----------------------------------------------------------------------------
G4int DicomLineDrawingOptions::GetColorIndexFromNumber( int icol )
{
  // Same as GetColor, with ROOT color numbers:
  // kWhite = 0, kBlack = 1, kGray = 920, kRed = 632, kGreen = 416,
  // kBlue = 600, kYellow = 400, kMagenta = 616, kCyan = 432, kOrange = 800,
  // kSpring = 820, kTeal = 840, kAzure = 860, kViolet = 880, kPink = 900
  int icol1 = icol/14;
  int icol2 = icol%14;
  switch (icol2)  {
  case 0: return 1+icol1;     // kBlack+icol1
  case 1: return 632;         // kRed
  case 2: return 416;         // kGreen
  case 3: return 600;         // kBlue
  case 4: return 616+icol1;   // kMagenta+icol1
  case 5: return 400+icol1+1; // kYellow+icol1+1
  case 6: return 432+icol1;   // kCyan+icol1
  case 7: return 880+2;       // kViolet+2
  case 8: return 800+icol1+1; // kOrange+icol1+1
  case 9: return 800+3;       // kOrange+3
  case 10: return 900+icol1+1; // kPink+icol1+1
  case 11: return 840-7;      // kTeal-7
  case 12: return 860+icol1+1; // kAzure+icol1+1
  case 13: return 920+icol1;  // kGray+icol1
  }
  return 1;
}
