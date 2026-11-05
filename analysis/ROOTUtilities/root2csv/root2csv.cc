/*
 */

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmHistoWriterCSV.hh"
#include "TFile.h"
#include "TKey.h"
#include "TH1F.h"
#include "TH2F.h"

//using namespace std;

//___________________________________________________________________________
int main( int argc, char **argv )
{
  
  G4String fileName = argv[1];
  TFile File( fileName );

  TIter ListOfHistograms( File.GetListOfKeys() );
  TKey *HistoKey = 0;
  TH1F *histo1 = 0;
  TH2F *histo2 = 0;

  /*  G4String fNameCSV = fileName.replace("root","csv");
      std::ofstream theFile(fNameCSV.c_str());*/
  std::vector<TH1F*> theHistos1;
  std::vector<TH2F*> theHistos2;
  int nh = 0;
  std::string fn;
  while ( (HistoKey = (TKey*)ListOfHistograms()) ) { 
    nh++;
    std::string hisname = HistoKey->GetName();
    TObject *obj = HistoKey->ReadObj();
    if( dynamic_cast<TH1F*>(obj) ) {
      histo1 = dynamic_cast<TH1F*>(obj);
      theHistos1.push_back(histo1);
    } else if( dynamic_cast<TH2F*>(obj) ) {
      histo2 = dynamic_cast<TH2F*>(obj);
      theHistos2.push_back(histo2);
      G4cout << " root2csv CreateHisto2D " << histo2->GetName() << G4endl; //GDEB
    }
  }

  G4String fileNameCSV = fileName;
  fileNameCSV.replace(fileNameCSV.find("root"), 4, "csv"); 
  GmHistoWriterCSV* hwcsv = new GmHistoWriterCSV(fileNameCSV);
  for( size_t ii = 0; ii < theHistos1.size(); ii++ ) {
    hwcsv->SaveHisto1D("1D",theHistos1[ii]);
  }
  for( size_t ii = 0; ii < theHistos2.size(); ii++ ) {
    hwcsv->SaveHisto2D("2D",theHistos2[ii]);
    G4cout << " root2csv SaveHisto2D " << theHistos2[ii]->GetName() << G4endl; //GDEB
  }
  
}
