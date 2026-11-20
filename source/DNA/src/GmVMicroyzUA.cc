#include <algorithm>
#include "G4Threading.hh"
#include "G4AutoLock.hh"
#include "GamosCore/GamosUtils/include/GmMTMergeMgr.hh"
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
  //--- Multithreading: worker sums are added to the master instance, which prints the means
  SetRunOnMaster(true);
  theFileOut = 0;
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
  if( G4Threading::IsMultithreadedApplication() && G4Threading::IsMasterThread() ) OutFormat = ""; // master: no events, no file
  if( OutFormat == "text" ) {
    bTextFile = true;
    fileNameOut = GmMTMergeMgr::GetThreadFileName(fileNameOut); // Multithreading: one file per thread
    theFileOut= new std::ofstream(fileNameOut);
  } else if( OutFormat == "bin" ) {
    bBinFile = true;
    fileNameOut = GmMTMergeMgr::GetThreadFileName(fileNameOut); // Multithreading: one file per thread
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
  theSumY4 = 0; // were not initialised
  theSumZ = 0;
  theSumZ2 = 0;
  theSumZ4 = 0;
  theSumWei = 0;
  theSumWei2 = 0;
  theSumW2Y = theSumW2Y2 = theSumW2Y3 = theSumW2Y4 = 0.;
  theSumW2Z = theSumW2Z2 = theSumW2Z3 = theSumW2Z4 = 0.;
  theNEntries = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GmVMicroyzUA::AddEntry( G4double y, G4double z, G4double wei )
{
  theSumY += y*wei;
  theSumY2 += y*y*wei;
  theSumY4 += pow(y,4)*wei;
  theSumZ += z*wei;
  theSumZ2 += z*z*wei;
  theSumZ4 += pow(z,4)*wei;
  theSumWei += wei;
  theSumWei2 += wei*wei;
  G4double w2 = wei*wei;
  theSumW2Y += w2*y; theSumW2Y2 += w2*y*y; theSumW2Y3 += w2*y*y*y; theSumW2Y4 += w2*y*y*y*y;
  theSumW2Z += w2*z; theSumW2Z2 += w2*z*z; theSumW2Z3 += w2*z*z*z; theSumW2Z4 += w2*z*z*z*z;
  theNEntries += 1.;
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
  if( G4Threading::IsMultithreadedApplication() && !G4Threading::IsMasterThread() ) {
    GmVMicroyzUA* master = dynamic_cast<GmVMicroyzUA*>(GmUserAction::GetMasterUserAction(GetName()));
    if( master ) {
      G4AutoLock l(&GmUserAction::GetMergeMutex());
      master->theSumY += theSumY; master->theSumY2 += theSumY2; master->theSumY4 += theSumY4;
      master->theSumZ += theSumZ; master->theSumZ2 += theSumZ2; master->theSumZ4 += theSumZ4;
      master->theSumWei += theSumWei; master->theSumWei2 += theSumWei2;
      master->theSumW2Y += theSumW2Y; master->theSumW2Y2 += theSumW2Y2; master->theSumW2Y3 += theSumW2Y3; master->theSumW2Y4 += theSumW2Y4;
      master->theSumW2Z += theSumW2Z; master->theSumW2Z2 += theSumW2Z2; master->theSumW2Z3 += theSumW2Z3; master->theSumW2Z4 += theSumW2Z4;
      master->theNEntries += theNEntries;
    }
    if( theFileOut ) theFileOut->flush();
    return;
  }
  G4int nev = GmNumberOfEvent::GetNumberOfEvent(); (void)nev;

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

  //--- errors of yF = Sum(w*y)/Sum(w) and yD = Sum(w*y^2)/Sum(w*y), ratios of sums over independent events
  //    (delta method): var(A/B) = n/(n-1) * Sum( (a_i - R*b_i)^2 ) / B^2
  //    Checked with 10 jobs of the Geant4 example microyz (1 keV e-, 2 nm): predicted 2.45%, spread of the jobs 2.4-2.6%
  G4double nfac = theNEntries > 1. ? theNEntries/(theNEntries-1.) : 0.;
  G4double meanYerr = 0., meanY2err = 0., meanZerr = 0., meanZ2err = 0.;
  G4double yD = 0., zD = 0.;
  if( GetSumWei() > 0. && GetSumY() > 0. && GetSumZ() > 0. ) {
    G4double W = GetSumWei();
    yD = GetSumY2()/GetSumY();
    zD = GetSumZ2()/GetSumZ();
    meanYerr = sqrt( std::max(0., nfac*(theSumW2Y2 - 2.*meanY*theSumW2Y + meanY*meanY*GetSumWei2())) ) / W;
    meanZerr = sqrt( std::max(0., nfac*(theSumW2Z2 - 2.*meanZ*theSumW2Z + meanZ*meanZ*GetSumWei2())) ) / W;
    meanY2err = sqrt( std::max(0., nfac*(theSumW2Y4 - 2.*yD*theSumW2Y3 + yD*yD*theSumW2Y2)) ) / GetSumY(); // error of yD
    meanZ2err = sqrt( std::max(0., nfac*(theSumW2Z4 - 2.*zD*theSumW2Z3 + zD*zD*theSumW2Z2)) ) / GetSumZ(); // error of zD
  }
  
  G4double meanYerrRel = 0.;
  if( meanY != 0. ) meanYerrRel = meanYerr/meanY;
  G4double meanY2errRel = 0.;
  if( yD != 0. ) meanY2errRel = meanY2err/yD;
  G4double meanZerrRel = 0.;
  if( meanZ != 0. ) meanZerrRel = meanZerr/meanZ;
  G4double meanZ2errRel = 0.;
  if( zD != 0. ) meanZ2errRel = meanZ2err/zD;
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


