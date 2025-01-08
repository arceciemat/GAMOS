#include "TCanvas.h"
#include "TColor.h"
#include "TF2.h"

void testPalette( )
{
  int nCount[5] = {0, 1, 2, 3, 4};
  
  TCanvas *c  = new TCanvas("c","Contours",600,0,600,600);
  TF2 *f1 = new TF2("f2","0.1+(1-(x-2)*(x-2))*(1-(y-2)*(y-2))",1,3,1,3);
  UInt_t Number = 3;
  //  double Red[Number] = {1,0.,0};
  //  double Red[Number] = {1,0.,0};
  double Red[3] = { 1.00, 0.00, 0.00};
  Double_t Green[3]  = { 0.00, 1.00, 0.00};
  Double_t Blue[3]   = { 1.00, 0.00, 1.00};
  Double_t Length[3] = { 0.00, 0.50, 1.00 };
  Int_t nb=50;
  TColor::CreateGradientColorTable(Number,Length,Red,Green,Blue,nb);
  //  f1->SetContour(nb);
  
  f1->Draw("surf1z");
  c->Print("testPalette.jpg");

}
