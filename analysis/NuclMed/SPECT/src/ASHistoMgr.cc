#include "ASHistoMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"


//----------------------------------------------------------------
ASHistoMgr::ASHistoMgr(G4String hname ) : theFileName(hname)
{
  InitializeHistoNames();
}

void ASHistoMgr::InitializeHistoNames()
{
  G4cout << " theFileName " << theFileName << G4endl;
  SetHistoNameAndNumber(theFileName,theFilters,theClassifier);
  BookHistos(99999);
}

void ASHistoMgr::BookHistos(G4int index)
{
  theHistos.insert(index); 
  G4String histoName = theHistoName + "_" + GmGenUtils::itoa(index);
  G4int histoNumber = theHistoNumber+100*index;
  G4String posName; 
  theAnaMgr->CreateHisto1D(histoName+": Energy_"+posName,100,0.,1.,histoNumber+1);

  for( G4int ii = 0; ii <= 2; ii++ ) {
    if( ii == 0 ) {
      posName = "Vtx";
    } else if( ii == 1 ) {
      posName = "1";
    } else if( ii == 2 ) {
      posName = "2";
    }
    histoNumber = theHistoNumber+100*index+20*ii;
    theAnaMgr->CreateHisto1D(histoName+": Position_" + posName + " X ",100,-1000., 1000.,histoNumber+2);
    theAnaMgr->CreateHisto1D(histoName+": Position_" + posName + " Y",100,-1000., 1000.,histoNumber+3);
    theAnaMgr->CreateHisto1D(histoName+": Position_" + posName + " Z",100,-1000., 1000.,histoNumber+4);
    theAnaMgr->CreateHisto1D(histoName+": Position_" + posName + " R2",100,0., 1000.,histoNumber+5);
    theAnaMgr->CreateHisto1D(histoName+": Position_" + posName + " R",100,0., 1000.,histoNumber+6);
    theAnaMgr->CreateHisto1D(histoName+": Position_" + posName + " phi",360,-180.,180.,histoNumber+7);
    theAnaMgr->CreateHisto1D(histoName+": Position_" + posName + " theta",180,0.,180,histoNumber+8);
    theAnaMgr->CreateHisto2D(histoName+": Position_" + posName + " XY",100,-1000., 1000.,100,-1000., 1000.,histoNumber+11);
    theAnaMgr->CreateHisto2D(histoName+": Position_" + posName + " XZ",100,-1000., 1000.,100,-1000., 1000.,histoNumber+12);
    theAnaMgr->CreateHisto2D(histoName+": Position_" + posName + " YZ",100,-1000., 1000.,100,-1000., 1000.,histoNumber+13);
  }

}

//----------------------------------------------------------------
void ASHistoMgr::FillHistos( SPECTOutput& sout )
{
  FillHistos( sout, 99999);
  FillHistos( sout, sout.eventClass);
}

//----------------------------------------------------------------
void ASHistoMgr::FillHistos( SPECTOutput& sout, G4int index )
{
  if( theHistos.find(index) == theHistos.end() ) BookHistos( index );
  G4int histoNumber = theHistoNumber + 100*index;

  G4ThreeVector p1( sout.x1, sout.y1, sout.z1);
  G4ThreeVector p2( sout.x2, sout.y2, sout.z2);
  G4ThreeVector pVtx( sout.xVtx, sout.yVtx, sout.zVtx);

  G4ThreeVector pH;
  theAnaMgr->GetHisto1(histoNumber+1)->Fill(sout.energy);
  for( G4int ii = 0; ii <= 2; ii++ ) {
    if( ii == 0 ) {
      pH = pVtx;
    } else if( ii == 1 ) {
      pH = p1;
    } else if( ii == 2 ) {
      pH = p2;
    }
    histoNumber = theHistoNumber+100*index+20*ii;
   
    theAnaMgr->GetHisto1(histoNumber+2)->Fill(pH.x());
    theAnaMgr->GetHisto1(histoNumber+3)->Fill(pH.y());
    theAnaMgr->GetHisto1(histoNumber+4)->Fill(pH.z());
    theAnaMgr->GetHisto1(histoNumber+5)->Fill(pH.perp());
    theAnaMgr->GetHisto1(histoNumber+6)->Fill(pH.mag());
    theAnaMgr->GetHisto1(histoNumber+7)->Fill(pH.phi());
    theAnaMgr->GetHisto1(histoNumber+8)->Fill(pH.theta());
    theAnaMgr->GetHisto2(histoNumber+11)->Fill(pH.x(),pH.y());
    theAnaMgr->GetHisto2(histoNumber+12)->Fill(pH.x(),pH.z());
    theAnaMgr->GetHisto2(histoNumber+13)->Fill(pH.y(),pH.z());
  }

}

