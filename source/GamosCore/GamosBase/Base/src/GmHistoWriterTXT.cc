#include "GmHistoWriterTXT.hh"
#include "GmHisto1.hh"
#include "GmHisto2.hh"
#include "GmHistoProfile1.hh"
#include "GmHistoProfile2.hh"

G4bool GmHistoWriterTXT::bHistoTXTErrors = TRUE;

//----------------------------------------------------------------------
GmHistoWriterTXT::GmHistoWriterTXT(const G4String& filename)
{
  theFile.open(filename.c_str());
  //  G4cout << "GmHistoWriterTXT: creating histo file " << filename << G4endl;
  //  theFileName = filename;  

}
 
//----------------------------------------------------------------------
GmHistoWriterTXT::~GmHistoWriterTXT()
{
  theFile.close();
}


 
//----------------------------------------------------------------------
#ifndef GAMOS_NO_ROOT
void GmHistoWriterTXT::SaveHisto1( const TH1F* his )
#else  
void GmHistoWriterTXT::SaveHisto1( const GmHisto1* his )
#endif
{
  SaveHisto1D("1D", his );
}


//----------------------------------------------------------------------
#ifndef GAMOS_NO_ROOT
void GmHistoWriterTXT::SaveHisto2( const TH2F* his )
#else 
void GmHistoWriterTXT::SaveHisto2( const GmHisto2* his )
#endif
{
  SaveHisto2D("2D", his);
}

#ifndef GAMOS_NO_ROOT
//----------------------------------------------------------------------
void GmHistoWriterTXT::SaveHistoProf1( const TProfile* his )
{
  SaveHisto1D("Prof1D", his );
}

//----------------------------------------------------------------------
void GmHistoWriterTXT::SaveHistoProf2( const TProfile2D* his )
{
  SaveHisto2D("Prof2D", his );
}
#else
//----------------------------------------------------------------------
void GmHistoWriterTXT::SaveHistoProf1( const GmHistoProfile1* his )
{
  SaveHisto1D("Prof1D", his );
}

//----------------------------------------------------------------------
void GmHistoWriterTXT::SaveHistoProf2( const GmHistoProfile2* his )
{
  SaveHisto2D("Prof2D", his );
}
#endif

#ifndef GAMOS_NO_ROOT
//----------------------------------------------------------------------
void GmHistoWriterTXT::SaveHisto1D(const G4String& hisType, const TH1* his )
{
  G4int nbins = his->GetXaxis()->GetNbins();
  theFile << '"' << hisType << '"' << " " << '"' << his->GetName() << '"' << " " << nbins << " " << his->GetXaxis()->GetXmin() << " " << his->GetXaxis()->GetXmax();
  theFile << " NENT= " << his->GetEntries() << " MEAN= " << his->GetMean();
  if( bHistoTXTErrors ) {
    theFile << " +- " << his->GetMeanError();  
  }
  
  theFile << " RMS= " << his->GetRMS();  
  if( bHistoTXTErrors ) {
    theFile << " +- " << his->GetRMSError();  
  }    

  theFile << " UFLOW= " << his->GetBinContent(0);  
  theFile << " OFLOW= " << his->GetBinContent(nbins+1);  
  theFile << G4endl;  

  for( G4int ii = 1; ii <= nbins; ii++ ){
    theFile << his->GetXaxis()->GetXmin()+his->GetXaxis()->GetBinWidth(ii)*(ii-0.5) << " " << his->GetBinContent(ii);
    if( bHistoTXTErrors ) {
      theFile << " " << his->GetBinError(ii);
    }
    theFile << G4endl;
  }

}

#else
//----------------------------------------------------------------------
void GmHistoWriterTXT::SaveHisto1D(const G4String& hisType, const GmHisto1* his )
{
  G4int nbins = his->GetNbins();
  theFile << '"' << hisType << '"' << " " << '"' << his->GetName() << '"' << " " << nbins << " " << his->GetLowerEdge() << " " << his->GetUpperEdge();

  theFile << " NENT= " << his->GetEntries() << " MEAN= " << his->GetMean();
  if( bHistoTXTErrors ) {
    theFile << " +- " << his->GetMeanError();  
  }
  
  theFile << " RMS= " << his->GetRMS();  
  if( bHistoTXTErrors ) {
    theFile << " +- " << his->GetRMSError();  
  }    

  theFile << " UFLOW= " << his->GetBinContent(0);  
  theFile << " OFLOW= " << his->GetBinContent(nbins+1);  
  theFile << G4endl;  

  for( G4int ii = 0; ii <= nbins+1; ii++ ){
    theFile << his->GetLowerEdge()+his->GetBinWidth(ii)*(ii-0.5) << " " << his->GetBinContent(ii);
    if( bHistoTXTErrors ) {
      theFile << " " << his->GetBinError(ii);
    }
    theFile << G4endl;

  }

}

//
//----------------------------------------------------------------------
void GmHistoWriterTXT::SaveHisto1D(const G4String& hisType, const GmHistoProfile1* his )
{
  G4int nbins = his->GetNbins();
  theFile << '"' << hisType << '"' << " " << '"' << his->GetName() << '"' << " " << nbins << " " << his->GetLowerEdge() << " " << his->GetUpperEdge();

  theFile << " NENT= " << his->GetEntries() << " MEAN= " << his->GetMean();
  if( bHistoTXTErrors ) {
    theFile << " +- " << his->GetMeanError();  
  }
  
  theFile << " RMS= " << his->GetRMS();  
  if( bHistoTXTErrors ) {
    theFile << " +- " << his->GetRMSError();  
  }    

  theFile << " UFLOW= " << his->GetBinContent(0);  
  theFile << " OFLOW= " << his->GetBinContent(nbins+1);  
  theFile << G4endl;  

  for( G4int ii = 0; ii <= nbins+1; ii++ ){
    theFile << his->GetLowerEdge()+his->GetBinWidth(ii)*(ii-0.5) << " " << his->GetBinContent(ii);
    if( bHistoTXTErrors ) {
      theFile << " " << his->GetBinError(ii);
    }
    theFile << G4endl;

  }

}
#endif

#ifndef GAMOS_NO_ROOT
//----------------------------------------------------------------------
void GmHistoWriterTXT::SaveHisto2D(const G4String& hisType, const TH2* his )
{
  G4int nbinsx = his->GetXaxis()->GetNbins();
  G4int nbinsy = his->GetYaxis()->GetNbins();
  theFile << '"' << hisType << '"' << " " << '"' << his->GetName() << '"' << " " << nbinsx << " " << his->GetXaxis()->GetXmin() << " " << his->GetXaxis()->GetXmax() << " " << nbinsy << " " << his->GetYaxis()->GetXmin() << " " << his->GetYaxis()->GetXmax();

  theFile << " NENT= " << his->GetEntries() << " MEAN= " << his->GetMean(1);
  if( bHistoTXTErrors ) {
    theFile << " +- " << his->GetMeanError(1);
  }
  
  theFile << " RMSX= " << his->GetRMS(1);  
  if( bHistoTXTErrors ) {
    theFile << " +- " << his->GetRMSError(1);  
  } 

  theFile << " MEANY= " << his->GetMean(2);
  if( bHistoTXTErrors ) {
    theFile << " +- " << his->GetMeanError(2);
  }
  
  theFile << " RMSY= " << his->GetRMS(2);  
  if( bHistoTXTErrors ) {
    theFile << " " << his->GetRMSError(2);  
  }

  G4double flow;
  G4int ii, jj;
  flow = 0;
  ii = 0;
  for( jj = 0; jj <= nbinsy+1; jj++ ){
    flow += his->GetBinContent(ii,jj);
  }
  theFile << " UFLOWX= " << flow;

  flow = 0.;
  ii = nbinsx+1;
  for( jj = 0; jj <= nbinsy+1; jj++ ){
    flow += his->GetBinContent(ii,jj);
  }
  theFile << " OFLOWX= " << flow;

  flow = 0.;
  jj = 0;
  for( ii = 0; ii <= nbinsx+1; ii++ ){
    flow += his->GetBinContent(ii,jj);
  }
  theFile << " UFLOWY= " << flow;

  flow = 0.;
  jj = nbinsy+1;
  for( ii = 0; ii <= nbinsx+1; ii++ ){
    flow += his->GetBinContent(ii,jj);
  }
  theFile << " OFLOWY= " << flow;
  theFile << G4endl;

  for( ii = 0; ii <= nbinsx+1; ii++ ){
    for( jj = 0; jj <= nbinsy+1; jj++ ){
      theFile << his->GetXaxis()->GetXmin()+his->GetXaxis()->GetBinWidth(ii)*(ii-0.5) 
	      << " " << his->GetYaxis()->GetXmin()+his->GetYaxis()->GetBinWidth(jj)*(jj-0.5) 
	      << " " << his->GetBinContent(ii,jj);
      if( bHistoTXTErrors ) {
	theFile << " " << his->GetBinError(ii,jj);
      } 
      theFile << G4endl;
    }
  }
 
}
#else
//----------------------------------------------------------------------
void GmHistoWriterTXT::SaveHisto2D(const G4String& hisType, const GmHisto2* his )
{
  G4int nbinsx = his->GetNbinsX();
  G4int nbinsy = his->GetNbinsY();
  theFile << '"' << hisType << '"' << " " << '"' << his->GetName() << '"' << " " << nbinsx << " " << his->GetLowerEdgeX() << " " << his->GetUpperEdgeX() << " " << nbinsy << " " << his->GetLowerEdgeY() << " " << his->GetUpperEdgeY();

  theFile << " NENT= " << his->GetEntries() << " MEAN= " << his->GetMean(1);
  if( bHistoTXTErrors ) {
    theFile << " +- " << his->GetMeanError(1);
  }
  
  theFile << " RMSX= " << his->GetRMS(1);  
  if( bHistoTXTErrors ) {
    theFile << " +- " << his->GetRMSError(1);  
  } 

  theFile << " MEANY= " << his->GetMean(2);
  if( bHistoTXTErrors ) {
    theFile << " +- " << his->GetMeanError(2);
  }
  
  theFile << " RMSY= " << his->GetRMS(2);  
  if( bHistoTXTErrors ) {
    theFile << " " << his->GetRMSError(2) << G4endl;  
  }

  G4double flow;
  G4int ii, jj;
  flow = 0;
  ii = 0;
  for( G4int jj = 0; jj <= nbinsy+1; jj++ ){
    flow += his->GetBinContent(ii,jj);
  }
  theFile << " UFLOWX= " << flow;

  flow = 0.;
  ii = nbinsx+1;
  for( G4int jj = 0; jj <= nbinsy+1; jj++ ){
    flow += his->GetBinContent(ii,jj);
  }
  theFile << " OFLOWX= " << flow;

  flow = 0.;
  jj = 0;
  for( G4int ii = 0; ii <= nbinsx+1; ii++ ){
    flow += his->GetBinContent(ii,jj);
  }
  theFile << " UFLOWY= " << flow;

  flow = 0.;
  jj = nbinsy+1;
  for( G4int ii = 0; ii <= nbinsx+1; ii++ ){
    flow += his->GetBinContent(ii,jj);
  }
  theFile << " OFLOWY= " << flow;


  for( G4int ii = 0; ii <= nbinsx+1; ii++ ){
    for( G4int jj = 0; jj <= nbinsy+1; jj++ ){
      theFile << his->GetLowerEdgeX()+his->GetBinWidthX(ii)*(ii-0.5) 
	      << " " << his->GetLowerEdgeY()+his->GetBinWidthY(jj)*(jj-0.5) 
	      << " " << his->GetBinContent(ii,jj);
      if( bHistoTXTErrors ) {
	theFile << " " << his->GetBinError(ii,jj);
      }
      theFile << G4endl;
    }
  }

}

//----------------------------------------------------------------------
void GmHistoWriterTXT::SaveHisto2D(const G4String& hisType, const GmHistoProfile2* his )
{
  G4int nbinsx = his->GetNbinsX();
  G4int nbinsy = his->GetNbinsY();
  theFile << '"' << hisType << '"' << " " << '"' << his->GetName() << '"' << " " << nbinsx << " " << his->GetLowerEdgeX() << " " << his->GetUpperEdgeX() << " " << nbinsy << " " << his->GetLowerEdgeX() << " " << his->GetUpperEdgeY();

  theFile << " NENT= " << his->GetEntries() << " MEAN= " << his->GetMean(1);
  if( bHistoTXTErrors ) {
    theFile << " +- " << his->GetMeanError(1);
  }
  
  theFile << " RMSX= " << his->GetRMS(1);  
  if( bHistoTXTErrors ) {
    theFile << " +- " << his->GetRMSError(1);  
  } 

  theFile << " MEANY= " << his->GetMean(2);
  if( bHistoTXTErrors ) {
    theFile << " +- " << his->GetMeanError(2);
  }
  
  theFile << " RMSY= " << his->GetRMS(2);  
  if( bHistoTXTErrors ) {
    theFile << " " << his->GetRMSError(2) << G4endl;  
  }

  G4double flow;
  G4int ii, jj;
  flow = 0;
  ii = 0;
  for( G4int jj = 0; jj <= nbinsy+1; jj++ ){
    flow += his->GetBinContent(ii,jj);
  }
  theFile << " UFLOWX= " << flow;

  flow = 0.;
  ii = nbinsx+1;
  for( G4int jj = 0; jj <= nbinsy+1; jj++ ){
    flow += his->GetBinContent(ii,jj);
  }
  theFile << " OFLOWX= " << flow;

  flow = 0.;
  jj = 0;
  for( G4int ii = 0; ii <= nbinsx+1; ii++ ){
    flow += his->GetBinContent(ii,jj);
  }
  theFile << " UFLOWY= " << flow;

  flow = 0.;
  jj = nbinsy+1;
  for( G4int ii = 0; ii <= nbinsx+1; ii++ ){
    flow += his->GetBinContent(ii,jj);
  }
  theFile << " OFLOWY= " << flow;

  for( G4int ii = 0; ii <= nbinsx+1; ii++ ){
    for( G4int jj = 0; jj <= nbinsy+1; jj++ ){
      theFile << " " << his->GetBinContent(ii,jj);
      if( bHistoTXTErrors ) {
	theFile << " " << his->GetBinError(ii,jj);
      }
      theFile << G4endl;
    }
  }

  theFile << " " << his->GetEntries() << " " << his->GetMean(1);
  if( bHistoTXTErrors ) {
    theFile << " " << his->GetMeanError(1);
  }
  
  theFile << " " << his->GetRMS(1);
  if( bHistoTXTErrors ) {
    theFile << " " << his->GetRMSError(1); 
  } 

  theFile << " " << his->GetMean(2);
  if( bHistoTXTErrors ) {
    theFile << " " << his->GetMeanError(2);  
  }
  
  theFile << " " << his->GetRMS(2);  
  if( bHistoTXTErrors ) {
    theFile << " " << his->GetRMSError(2) << G4endl;  
  } else {
    theFile << G4endl;  
  }

}
#endif
