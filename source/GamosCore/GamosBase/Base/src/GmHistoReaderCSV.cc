#include "GmHistoReaderCSV.hh"
#include "GmAnalysisMgr.hh" // GmHisto1/GmHisto2/GmHistoProfile1/GmHistoProfile2 (or ROOT histos)

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include <cmath>
#include <cstdlib>

//----------------------------------------------------------------------
GmHistoReaderCSV::GmHistoReaderCSV(const G4String& filename)
{
  theFile = GmFileIn::GetInstance(filename, true);
  //  G4cout << "GmHistoReaderCSV: creating histo file " << filename << G4endl;
  theFileName = filename;  
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
    //   G4cout << " GmHistoReaderCSV::ReadFile() " << wl[0] << G4endl;
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
  G4int nbins = G4int( GetNumber(wl,2) ); 
  G4double hmin = GetNumber(wl,3); 
  G4double hmax = GetNumber(wl,4);
  GmHisto1* his = new GmHisto1( wl[1], wl[1], nbins, hmin, hmax );
  FillHisto1D( wl, his );
  theHistos1[wl[1]] = his;
}

//----------------------------------------------------------------------
void GmHistoReaderCSV::FillHistoProfile1D( std::vector<G4String>& wl )
{
  G4int nbins = G4int( GetNumber(wl,2) ); 
  G4double hmin = GetNumber(wl,3); 
  G4double hmax = GetNumber(wl,4);
  GmHistoProfile1* his = new GmHistoProfile1( wl[1], wl[1], nbins, hmin, hmax );
  FillHisto1D( wl, (GmHisto1*)(his) );

  theHistosProfile1[wl[1]] = his;
}

//----------------------------------------------------------------------
void GmHistoReaderCSV::FillHisto1D( std::vector<G4String>& wl, GmHisto1* his )
{
  G4int nbins = his->GetNbinsX();
  G4double hmin = GetNumber(wl,3); //ROOT uses TAxis, GmHisto don't 
  G4double hmax = GetNumber(wl,4);
  G4double hstep = (hmax-hmin)/nbins; 
  // 5 header + (nbins+2) (content,error) + entries, mean, mean error, RMS, RMS error = 2*nbins+14 words
  CheckNFields( wl, 2*nbins+14 );
  for( size_t iw = 5; iw < wl.size(); iw++ ) GetNumber(wl,iw); // all fields must be valid numbers
#ifdef GAMOS_NO_ROOT
  his->SetUseErrors(true);
#endif
  G4int ii = 5; // "1D", his_name,number_of_bins,Xaxis_minimum, Xaxis_maximum, underflow,underflow_error,bin_content, bin_error,overflow, overflow_error (It has indeed number_of_binsX+2 pairs valueerror, as the first one is the underflow (entries below axis_minimum) and the last one is the overflow (entries above axis maximum).
  for( G4int iiX = 0; iiX < nbins+2; iiX++, ii++ ) {
    G4double valX = hmin + (iiX-1+0.5) * hstep;
    his->Fill( valX, GetNumber(wl,ii) );
    //    if( G4String(his->GetName()).find("+000") != std::string::npos ) G4cout << iiX << " " << ii << " " << his->GetName() << " GmHistoReaderCSV::FillHisto1D " << valX << " " << GetNumber(wl,ii) << G4endl; //GDEB
    ii++; 
    his->SetBinError(iiX, GetNumber(wl,ii));
  }

  his->SetEntries(GetNumber(wl,ii)); 

}

//----------------------------------------------------------------------
void GmHistoReaderCSV::FillHisto2D( std::vector<G4String>& wl )
{

  G4int nbinsX = G4int( GetNumber(wl,2) ); 
  G4double hminX = GetNumber(wl,3); 
  G4double hmaxX = GetNumber(wl,4);
  G4int nbinsY = G4int( GetNumber(wl,5) ); 
  G4double hminY = GetNumber(wl,6); 
  G4double hmaxY = GetNumber(wl,7);

  GmHisto2* his = new GmHisto2( wl[1], wl[1], nbinsX, hminX, hmaxX, nbinsY, hminY, hmaxY );

  FillHisto2D( wl, his );

  theHistos2[wl[1]] = his;
}

//----------------------------------------------------------------------
void GmHistoReaderCSV::FillHistoProfile2D( std::vector<G4String>& wl )
{

  G4int nbinsX = G4int( GetNumber(wl,2) ); 
  G4double hminX = GetNumber(wl,3); 
  G4double hmaxX = GetNumber(wl,4);
  G4int nbinsY = G4int( GetNumber(wl,5) ); 
  G4double hminY = GetNumber(wl,6); 
  G4double hmaxY = GetNumber(wl,7);

  GmHistoProfile2* his = new GmHistoProfile2( wl[1], wl[1], nbinsX, hminX, hmaxX, nbinsY, hminY, hmaxY );

  FillHisto2D( wl, (GmHisto2*)(his) );

  theHistosProfile2[wl[1]] = his;
}

//----------------------------------------------------------------------
void GmHistoReaderCSV::FillHisto2D( std::vector<G4String>& wl, GmHisto2* his )
{
  G4int nbinsX = his->GetNbinsX();
  G4double hminX = GetNumber(wl,3); 
  G4double hmaxX = GetNumber(wl,4);
  G4double hstepX = (hmaxX-hminX)/nbinsX; 
  G4int nbinsY = his->GetNbinsY();
  G4double hminY = GetNumber(wl,6); 
  G4double hmaxY = GetNumber(wl,7);
  G4double hstepY = (hmaxY-hminY)/nbinsY; 

  // 8 header + (nbinsX+2)*(nbinsY+2) (content,error) + entries, (mean, mean error, RMS, RMS error) for X and Y = 2*(nbinsX+2)*(nbinsY+2)+17 words
  CheckNFields( wl, 2*(nbinsX+2)*(nbinsY+2)+17 );
  for( size_t iw = 8; iw < wl.size(); iw++ ) GetNumber(wl,iw); // all fields must be valid numbers
  G4int ii = 8;
  for( G4int iiX = 0; iiX < nbinsX+2; iiX++ ) {
    G4double valX = hminX + (iiX-1+0.5) * hstepX;
    for( G4int iiY = 0; iiY < nbinsY+2; iiY++, ii++ ) {
      G4double valY = hminY + (iiY-1+0.5) * hstepY;
      his->Fill( valX, valY, GetNumber(wl,ii) );
      ii++; 
      his->SetBinError(iiX, iiY, GetNumber(wl,ii));
    }
  }

  his->SetEntries(GetNumber(wl,ii)); 

}



//----------------------------------------------------------------------
G4double GmHistoReaderCSV::GetNumber( const std::vector<G4String>& wl, size_t iw )
{
  const G4String& str = wl[iw];
  char* end = 0;
  G4double val = std::strtod(str.c_str(),&end);
  G4String err;
  if( str.empty() || end == str.c_str() || *end != '\0' ) {
    err = "is not a number: '" + str + "'";
  } else if( !std::isfinite(val) ) {
    err = "is " + str + " (nan and inf are not valid histogram values)";
  }
  if( err != "" ) {
    G4Exception("GmHistoReaderCSV",
		"",
		FatalException,
		("File " + theFileName + " line " + GmGenUtils::itoa(theFile.Nline()) + " histogram '" + wl[1]
		 + "': field " + GmGenUtils::itoa(G4int(iw)+1) + " " + err).c_str());
  }
  return val;
}

//----------------------------------------------------------------------
void GmHistoReaderCSV::CheckNFields( const std::vector<G4String>& wl, size_t nExpected )
{
  if( wl.size() != nExpected ) {
    G4Exception("GmHistoReaderCSV",
		"",
		FatalException,
		("File " + theFileName + " line " + GmGenUtils::itoa(theFile.Nline()) + " histogram '" + wl[1]
		 + "': it has " + GmGenUtils::itoa(G4int(wl.size())) + " fields, while it should have "
		 + GmGenUtils::itoa(G4int(nExpected)) + " (GAMOS .csv histograms are always written with errors)").c_str());
  }
}

//----------------------------------------------------------------------
GmHisto1* GmHistoReaderCSV::GetHisto1( const G4String& histoName, G4bool bExists )
{
  GmHisto1* his = 0;

  std::map<G4String,GmHisto1*>::const_iterator ite = theHistos1.find( histoName );
  if( ite != theHistos1.end() ) {
    his = (*ite).second;
  }

  if( his == 0 ) {
    for( ite = theHistos1.begin(); ite != theHistos1.end(); ite++) {
      G4cerr << "HISTO1 " << ite->first << " <> " << histoName << ":" << G4endl;
    }
    G4ExceptionSeverity excepSever = FatalException;
    if ( ! bExists ) {
      excepSever = JustWarning;
    }
    G4Exception("GmHistoReaderCSV::GetHisto1",
		"",
		excepSever,
		("!!! Histogram "+histoName+" not found in "+theFileName).c_str());
  }
  return his;
}

//----------------------------------------------------------------------
GmHisto2* GmHistoReaderCSV::GetHisto2( const G4String& histoName, G4bool bExists )
{
  GmHisto2* his = 0;

  std::map<G4String,GmHisto2*>::const_iterator ite = theHistos2.find( histoName );
  if( ite != theHistos2.end() ) {
    his = (*ite).second;
  }
  if( his == 0 ) {
    for( ite = theHistos2.begin(); ite != theHistos2.end(); ite++) {
      G4cerr << "HISTO2 " << ite->first << " <> " << histoName << ":" << G4endl;
    }
    G4ExceptionSeverity excepSever = FatalException;
    if ( ! bExists ) {
      excepSever = JustWarning;
    }
    G4Exception("GmHistoReaderCSV::GetHisto2",
		"",
		excepSever,
		("!!! Histogram "+histoName+" not found in "+theFileName).c_str());
  }
  
  return his;
}
//----------------------------------------------------------------------
GmHistoProfile1* GmHistoReaderCSV::GetHistoProfile1( const G4String& histoName, G4bool bExists)
{

  GmHistoProfile1* his = 0;

  std::map<G4String,GmHistoProfile1*>::const_iterator ite = theHistosProfile1.find( histoName );
  if( ite != theHistosProfile1.end() ) {
    his = (*ite).second;
  }
  if( his == 0 ) {
    for( ite = theHistosProfile1.begin(); ite != theHistosProfile1.end(); ite++) {
      G4cerr << "HISTO_PROFILE1 " << ite->first << " <> " << histoName << ":" << G4endl;
    }
    G4ExceptionSeverity excepSever = FatalException;
    if ( ! bExists ) {
      excepSever = JustWarning;
    }
    G4Exception("GmHistoReaderCSV::GetHistoProfile1",
		"",
		excepSever,
		("!!! Histogram "+histoName+" not found in "+theFileName).c_str());
  }

  return his;
}

//----------------------------------------------------------------------
GmHistoProfile2* GmHistoReaderCSV::GetHistoProfile2( const G4String& histoName, G4bool bExists )
{

  GmHistoProfile2* his = 0;

  std::map<G4String,GmHistoProfile2*>::const_iterator ite = theHistosProfile2.find( histoName );
  if( ite != theHistosProfile2.end() ) {
    his = (*ite).second;
  }
  if( his == 0 ) {
    for( ite = theHistosProfile2.begin(); ite != theHistosProfile2.end(); ite++) {
      G4cerr << "HISTO1 " << ite->first << " <> " << histoName << ":" << G4endl;
    }
    G4ExceptionSeverity excepSever = FatalException;
    if ( ! bExists ) {
      excepSever = JustWarning;
    }
    G4Exception("GmHistoReaderCSV::GetHistoProfile2",
		"",
		excepSever,
		("!!! Histogram "+histoName+" not found in "+theFileName).c_str());
  }
  
  return his;
}
