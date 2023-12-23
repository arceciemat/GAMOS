#include "GmVMicroyzUA.hh"
#include "GmDNAVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosAnalysis/include/GmVHistoBuilder.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"
#include "Randomize.hh"
#include "G4SDManager.hh"
#include "G4RandomDirection.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
GmVMicroyzUA::GmVMicroyzUA()  
{
  theParamMgr = GmParameterMgr::GetInstance();
 
}

void GmVMicroyzUA::BeginOfRunAction(const G4Run* )
{
  G4String fileNameOut = theParamMgr->GetStringValue(GetName()+":OutputFileName","GmMicroyz.out");
  G4String suffix = theParamMgr->GetStringValue("GmAnalysisMgr:FileNameSuffix","");
  if( suffix != "" ) {
    fileNameOut += "."+suffix;
  }
  bTextFile = false;
  bBinFile = false;
  theFileOut = 0;
  G4String OutFormat = theParamMgr->GetStringValue(GetName()+":OutputFormat","");
  if( OutFormat == "text" ) {
    bTextFile = true;
    theFileOut= new std::ofstream(fileNameOut);
  } else if( OutFormat == "bin" ) {
    bBinFile = true;
    theFileOut= new std::ofstream(fileNameOut, std::ios::out | std::ios::binary);
  } else if( OutFormat == "" ) {
  } else {
      G4Exception("GMVMicroyzUA::BeginOfRunAction",
		  "",
		  FatalException,
		  ("OutputFormat must be 'text' or 'bin', it is "+OutFormat).c_str());
  }

  bHistos = G4bool(theParamMgr->GetNumericValue(GetName()+":Histos",1));
  if( bHistos ) BookHistos();

  theSumY = 0;
  theSumY2 = 0;
  theSumZ = 0;
  theSumZ2 = 0;
  theSumWei = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GmVMicroyzUA::BookHistos()
{
  theAnaMgr = GmAnalysisMgr::GetInstance(GetName());
  theHistoNumber = GmVHistoBuilder::BuildHistoNumber();
  theAnaMgr->CreateHisto1D("N_hits",140,80/(1*keV),150/(1*keV),theHistoNumber+1);
  theAnaMgr->CreateHisto1D("N_scored_hits",100,0.,100,theHistoNumber+2);
  //  theAnaMgr->CreateHisto1D("E_incident",100,0.9,1.1,theHistoNumber+3);
  std::vector<G4String> theHistoNames {"yf(y)","zf(z)","yd(y)","zd(z)","log10(y)f(y)","log10(z)f(z)","log10(y)d(y)","log10(z)d(z)" };
  theHistoNames = theParamMgr->GetVStringValue(GetName()+":HistoNames", theHistoNames);
  
  G4double ylim = 120.;
  G4double zlim = ylim*0.204/pow(2*15e-3,2); // ylim*0.204/pow(2*theSphereRadius/micrometer,2);
  G4int ozlim = log10(zlim);  zlim = pow(10.,ozlim-1)*(int(zlim/pow(10.,ozlim-1))); // round 2 numbers
    //   G4cout <<  ylim*0.204/pow(2*theSphereRadius/micrometer,2) << " zlim " << zlim << " oz " << ozlim << " " <<  pow(10.,ozlim-1) << "*" << int(zlim/pow(10.,ozlim-1)) << G4endl; //GDEB
  for( std::vector<G4String>::const_iterator ite = theHistoNames.begin(); ite != theHistoNames.end(); ite++ ) {
    if( *ite == "yf(y)" ) {
      theAnaMgr->CreateHisto1D("yf(y)",120,0.,ylim,theHistoNumber+1);
    } else if( *ite == "zf(z)" ) {
      theAnaMgr->CreateHisto1D("zf(z)",125,0.,zlim,theHistoNumber+2);
    } else if( *ite == "yd(y)" ) {
      theAnaMgr->CreateHisto1D("yd(y)",120,0.,ylim*100,theHistoNumber+3);
    } else if( *ite == "zd(z)" ) {
      theAnaMgr->CreateHisto1D("zd(z)",125,0.,zlim*10000,theHistoNumber+4);
    } else if( *ite == "log10(y)f(y)" ) {
      theAnaMgr->CreateHisto1D("log10(y)f(y)",120,-1.,log10(ylim)+1,theHistoNumber+5);
    } else if( *ite == "log10(z)f(z)" ) {
      theAnaMgr->CreateHisto1D("log10(z)f(z)",125,-1,log10(zlim)+1,theHistoNumber+6);
    } else if( *ite == "log10(y)d(y)" ) {
      theAnaMgr->CreateHisto1D("log10(y)d(y)",120,-1.,log10(ylim)+2+1,theHistoNumber+7);
    } else if( *ite == "log10(z)d(z)" ) {
      theAnaMgr->CreateHisto1D("log10(z)d(z)",125,-1.,log10(zlim)+4+1,theHistoNumber+8);
    } else {
      std::ostringstream msg;
      msg << "Histogram name  not found " << *ite << G4endl << "Existing names: ";
      for( std::vector<G4String>::const_iterator ite2 = theHistoNames.begin(); ite2 != theHistoNames.end(); ite2++ ) {
	msg << *ite2 << " ";
      }
      G4Exception("GMVMicroyzUA:BookHistos",
		  "",
		  FatalException,
		  msg);
    }
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GmVMicroyzUA::EndOfEventAction(const G4Event*)
{
  theHits.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//----------------------------------------------------------------
void GmVMicroyzUA::EndOfRunAction( const G4Run* )
{
  G4int nev = GmNumberOfEvent::GetNumberOfEvent();

  /* 
  // sumWei = nev; if no wei=1.
  G4double meanY = GetSumY()/nev;
  G4double sumY2 = GetSumY2();
  G4double meanY2 = GetSumY2()/nev;
  G4double sumY4 = GetSumY4();
  G4double meanZ = GetSumZ()/nev;
  G4double sumZ2 = GetSumZ2();
  G4double meanZ2 = GetSumZ2()/nev;
  G4double sumZ4 = GetSumZ4();
   
  G4double meanYerr = sqrt((sumY2-meanY*meanY*nev)/(nev-1)/nev);
  G4double meanY2err = sqrt((sumY4-meanY2*meanY2*nev)/(nev-1)/nev);
  G4double meanZerr = sqrt((sumZ2-meanZ*meanZ*nev)/(nev-1)/nev);
  G4double meanZ2err = sqrt((sumZ4-meanZ2*meanY2*nev)/(nev-1)/nev);
  G4double meanYerrRel = 0.; */

  G4double meanY = GetSumY()/GetSumWei();
  G4double meanZ = GetSumZ()/GetSumWei();
  G4double meanY2 = GetSumY2()/GetSumWei();
  G4double meanZ2 = GetSumZ2()/GetSumWei();  

  G4double meanYerr = sqrt( (GetSumY2()-meanY*meanY*GetSumWei()) / (GetSumWei()*GetSumWei()*(GetSumWei2()-1/nev*GetSumWei())) );
  G4double meanY2err = sqrt( (GetSumY4()-meanY2*meanY2*GetSumWei()) / (GetSumWei()*GetSumWei()*(GetSumWei2()-1/nev*GetSumWei())) );
  G4double meanZerr = sqrt( (GetSumZ2()-meanZ*meanZ*GetSumWei()) / (GetSumWei()*GetSumWei()*(GetSumWei2()-1/nev*GetSumWei())) );
  G4double meanZ2err = sqrt( (GetSumZ4()-meanZ2*meanZ2*GetSumWei()) / (GetSumWei()*GetSumWei()*(GetSumWei2()-1/nev*GetSumWei())) );
  
  G4double meanYerrRel = 0.;
  if( meanY != 0. ) meanYerrRel = meanYerr/meanY;
  G4double meanY2errRel = 0.;
  if( meanY2 != 0. ) meanY2errRel = meanY2err/meanY2;
  G4double meanZerrRel = 0.;
  if( meanZ != 0. ) meanZerrRel = meanZerr/meanZ;
  G4double meanZ2errRel = 0.;
  if( meanZ2 != 0. ) meanZ2errRel = meanZ2err/meanZ2;
  G4cout << " @#@# Frequency-mean lineal energy (y_F)=  " << meanY << " +-(REL) " << meanYerrRel << G4endl;
  G4cout << " @#@# Dose-mean lineal energy (y_D)=       " << meanY2/meanY << " +-(REL) " << meanY2errRel << G4endl;
  G4cout << " @#@# Frequency-mean specific energy (z_F)= " << meanZ << " +-(REL) " << meanZerrRel << G4endl;
  G4cout << " @#@# Dose-mean specific energy (z_D)=      " << meanZ2/meanZ << " +-(REL) " << meanZ2errRel << G4endl;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmVMicroyzUA::~GmVMicroyzUA()
{
  if( theFileOut ) theFileOut->close();
}


