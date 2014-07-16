#include "GmHistoReaderCSV.hh"
#include "GmHisto1.hh"
#include "GmHisto2.hh"
#include "GmHistoProfile1.hh"
#include "GmHistoProfile2.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

G4bool GmHistoReaderCSV::bHistoCSVErrors = TRUE;

//----------------------------------------------------------------------
GmHistoReaderCSV::GmHistoReaderCSV(const G4String& filename)
{
  theFile = GmFileIn::GetInstance(filename);
  //  G4cout << "GmHistoReaderCSV: creating histo file " << filename << G4endl;
  //  theFileName = filename;  
  theFile.SetSeparator(',');
  ReadFile();
}

//----------------------------------------------------------------------
GmHistoReaderCSV::~GmHistoReaderCSV()
{
  theFile.Close();
}

//----------------------------------------------------------------------
void GmHistoReaderCSV::ReadFile()
{
  std::vector<G4String> wl;
  for(;;) {
    if( ! theFile.GetWordsInLine(wl) ) break;
    G4cout << " GmHistoReaderCSV::ReadFile() " << wl[0] << G4endl;
    if( wl[0] == "1D" ) {
      FillHisto1D(wl);
    } else if( wl[0] == "2D" ) {
      FillHisto2D(wl);
    } else if( wl[0] == "Prof1D" ) {
      FillHistoProfile1D(wl);
    } else if( wl[0] == "Prof2D" ) {
      FillHistoProfile2D(wl);
    }
  }
    
}

//----------------------------------------------------------------------
void GmHistoReaderCSV::FillHisto1D( std::vector<G4String>& wl )
{
  G4int nbins = G4int( GmGenUtils::GetValue(wl[2]) ); 
  G4double hmin = GmGenUtils::GetValue(wl[3]); 
  G4double hmax = GmGenUtils::GetValue(wl[4]);
  GmHisto1* his = new GmHisto1( wl[1], wl[1], nbins, hmin, hmax );
  FillHisto1D( wl, his );
  theHistos1[wl[1]] = his;
}

//----------------------------------------------------------------------
void GmHistoReaderCSV::FillHistoProfile1D( std::vector<G4String>& wl )
{
  G4int nbins = G4int( GmGenUtils::GetValue(wl[2]) ); 
  G4double hmin = GmGenUtils::GetValue(wl[3]); 
  G4double hmax = GmGenUtils::GetValue(wl[4]);
  GmHistoProfile1* his = new GmHistoProfile1( wl[1], wl[1], nbins, hmin, hmax );
  FillHisto1D( wl, (GmHisto1*)(his) );

  theHistosProfile1[wl[1]] = his;
}

//----------------------------------------------------------------------
void GmHistoReaderCSV::FillHisto1D( std::vector<G4String>& wl, GmHisto1* his )
{
  G4int nbins = his->GetNbinsX();
  G4double hmin = GmGenUtils::GetValue(wl[3]); //ROOT uses TAxis, GmHisto don't 
  G4double hmax = GmGenUtils::GetValue(wl[4]);
  G4double hstep = (hmax-hmin)/nbins; 
  G4bool bHistoCSVErrors = (G4int(wl.size()) > nbins+8);
  G4int ii = 5;
  for( ; ii < 5+nbins+1; ii++ ) {
    G4double valX = hmin + (ii-0.5) * hstep;
    his->Fill( valX, GmGenUtils::GetValue(wl[ii]) );
    if( bHistoCSVErrors ) {
      ii++; 
      his->SetBinError(ii, GmGenUtils::GetValue(wl[ii]));
    }
  }

  his->SetEntries(GmGenUtils::GetValue(wl[ii])); 

}

//----------------------------------------------------------------------
void GmHistoReaderCSV::FillHisto2D( std::vector<G4String>& wl )
{

  G4int nbinsX = G4int( GmGenUtils::GetValue(wl[2]) ); 
  G4double hminX = GmGenUtils::GetValue(wl[3]); 
  G4double hmaxX = GmGenUtils::GetValue(wl[4]);
  G4int nbinsY = G4int( GmGenUtils::GetValue(wl[5]) ); 
  G4double hminY = GmGenUtils::GetValue(wl[6]); 
  G4double hmaxY = GmGenUtils::GetValue(wl[7]);

  GmHisto2* his = new GmHisto2( wl[1], wl[1], nbinsX, hminX, hmaxX, nbinsY, hminY, hmaxY );

  FillHisto2D( wl, his );

  theHistos2[wl[1]] = his;
}

//----------------------------------------------------------------------
void GmHistoReaderCSV::FillHistoProfile2D( std::vector<G4String>& wl )
{

  G4int nbinsX = G4int( GmGenUtils::GetValue(wl[2]) ); 
  G4double hminX = GmGenUtils::GetValue(wl[3]); 
  G4double hmaxX = GmGenUtils::GetValue(wl[4]);
  G4int nbinsY = G4int( GmGenUtils::GetValue(wl[5]) ); 
  G4double hminY = GmGenUtils::GetValue(wl[6]); 
  G4double hmaxY = GmGenUtils::GetValue(wl[7]);

  GmHistoProfile2* his = new GmHistoProfile2( wl[1], wl[1], nbinsX, hminX, hmaxX, nbinsY, hminY, hmaxY );

  FillHisto2D( wl, (GmHisto2*)(his) );

  theHistosProfile2[wl[1]] = his;
}

//----------------------------------------------------------------------
void GmHistoReaderCSV::FillHisto2D( std::vector<G4String>& wl, GmHisto2* his )
{
  G4int nbinsX = his->GetNbinsX();
  G4double hminX = GmGenUtils::GetValue(wl[3]); 
  G4double hmaxX = GmGenUtils::GetValue(wl[4]);
  G4double hstepX = (hmaxX-hminX)/nbinsX; 
  G4int nbinsY = his->GetNbinsY();
  G4double hminY = GmGenUtils::GetValue(wl[6]); 
  G4double hmaxY = GmGenUtils::GetValue(wl[7]);
  G4double hstepY = (hmaxY-hminY)/nbinsY; 

  G4bool bHistoCSVErrors = (G4int(wl.size()) > nbinsX*nbinsY+13);
  G4int ii = 8;
  for( G4int iiX = 0; iiX < nbinsX+1; iiX++ ) {
    G4double valX = hminX + (iiX-0.5) * hstepX;
    for( G4int iiY = 0; ii < nbinsY+1; iiY++, ii++ ) {
      G4double valY = hminY + (iiY-0.5) * hstepY;
      his->Fill( valX, valY, GmGenUtils::GetValue(wl[ii]) );
      if( bHistoCSVErrors ) {
	ii++; 
	his->SetBinError(iiX, iiY, GmGenUtils::GetValue(wl[ii]));
      }
    }
  }

  his->SetEntries(GmGenUtils::GetValue(wl[ii])); 

}



//----------------------------------------------------------------------
GmHisto1* GmHistoReaderCSV::GetHisto1( const G4String& histoName )
{

  GmHisto1* his = 0;

  std::map<G4String,GmHisto1*>::const_iterator ite = theHistos1.find( histoName );
  if( ite != theHistos1.end() ) {
    his = (*ite).second;
  }

  if( his == 0 ) {
    for( ite = theHistos1.begin(); ite != theHistos1.end(); ite++) {
      G4cout << "HISTO1 " << his->GetName() << " <> " << histoName << ":" << G4endl;
    }
  }
  return his;
}

//----------------------------------------------------------------------
GmHisto2* GmHistoReaderCSV::GetHisto2( const G4String& histoName )
{
  GmHisto2* his = 0;

  std::map<G4String,GmHisto2*>::const_iterator ite = theHistos2.find( histoName );
  if( ite != theHistos2.end() ) {
    his = (*ite).second;
  }

  return his;
}
//----------------------------------------------------------------------
GmHistoProfile1* GmHistoReaderCSV::GetHistoProfile1( const G4String& histoName ){

  GmHistoProfile1* his = 0;

  std::map<G4String,GmHistoProfile1*>::const_iterator ite = theHistosProfile1.find( histoName );
  if( ite != theHistosProfile1.end() ) {
    his = (*ite).second;
  }

  return his;
}

//----------------------------------------------------------------------
GmHistoProfile2* GmHistoReaderCSV::GetHistoProfile2( const G4String& histoName )
{

  GmHistoProfile2* his = 0;

  std::map<G4String,GmHistoProfile2*>::const_iterator ite = theHistosProfile2.find( histoName );
  if( ite != theHistosProfile2.end() ) {
    his = (*ite).second;
  }

  return his;
}
