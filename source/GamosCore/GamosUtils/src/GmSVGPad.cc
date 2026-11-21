#include "GmSVGPad.hh"
#include "GmSVGCanvas.hh"
#include <iostream>

//---------------------------------------------------------------------------
GmSVGPad::GmSVGPad( const std::string&, const std::string&,
		    double x1, double y1, double x2, double y2, GmSVGCanvas* canvas )
  : fX1(std::min(x1,x2)), fY1(std::min(y1,y2)), fX2(std::max(x1,x2)), fY2(std::max(y1,y2)),
    bLogX(false), bLogY(false), theCanvas(canvas)
{
  if( fX2 - fX1 <= 0. ) { fX1 = 0.; fX2 = 1.; } // invalid pad: use full width
  if( fY2 - fY1 <= 0. ) { fY1 = 0.; fY2 = 1.; }
  if( theCanvas == 0 ) theCanvas = GmSVGCanvas::GetCurrentCanvas();
  if( theCanvas == 0 ) {
    std::cerr << " !!! GmSVGPad created without any GmSVGCanvas, creating one " << std::endl;
    theCanvas = new GmSVGCanvas();
  }
}

void GmSVGPad::cd()
{
  theCanvas->SetCurrentPad(this);
}

double GmSVGPad::GetPixXMin() const { return fX1*theCanvas->GetWw(); }
double GmSVGPad::GetPixXMax() const { return fX2*theCanvas->GetWw(); }
double GmSVGPad::GetPixYTop() const { return (1.-fY2)*theCanvas->GetWh(); }
double GmSVGPad::GetPixYBottom() const { return (1.-fY1)*theCanvas->GetWh(); }
