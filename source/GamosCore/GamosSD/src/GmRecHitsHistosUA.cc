#include "GmRecHitsHistosUA.hh"
#include "GmHitsEventMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosSD/include/GmSDVerbosity.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

#include "G4Event.hh"


//--------------------------------------------------------------------
GmRecHitsHistosUA::GmRecHitsHistosUA()
{
}

//--------------------------------------------------------------------
void GmRecHitsHistosUA::BeginOfRunAction(const G4Run* )
{
  BookHistos();
}


//--------------------------------------------------------------------
void GmRecHitsHistosUA::EndOfRunAction(const G4Run* )
{
  SumHistos();
}

 
GmRecHitsHistosUA::~GmRecHitsHistosUA(){}

//--------------------------------------------------------------------
void GmRecHitsHistosUA::BookHistos()
{
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) G4cout << "GmRecHitsHistosUA::BookHistos()" << G4endl;
#endif
  theAnaMgr = GmAnalysisMgr::GetInstance("recHits") ;

  G4String hnam;
  theSDTypes = GmGeometryUtils::GetInstance()->GetAllSDTypes();
  std::vector<G4String> orig;
  orig.push_back("SD ALL:");
  std::set<G4String>::const_iterator ites;
  for( ites = theSDTypes.begin(); ites != theSDTypes.end(); ites++ ){
    orig.push_back("SD " + *ites + ": ");  
  }
  G4String hgnam = "RecHits: ";
  for( unsigned int ii = 0; ii <= theSDTypes.size(); ii++ ){
    hnam = hgnam + orig[ii] + G4String("N rec hits");
    theAnaMgr->CreateHisto1D(hnam,100,0,100,202000+ii*100+1);
    hnam = hgnam + orig[ii] + G4String("Energy (keV)");
    theAnaMgr->CreateHisto1D(hnam,150,0.,1500.,202000+ii*100+2);
    hnam = hgnam + orig[ii] + G4String("Width R3 (mm)");
    theAnaMgr->CreateHisto1D(hnam,100,0,50.,202000+ii*100+3);
    hnam = hgnam + orig[ii] + G4String("Width Z (mm)");
    theAnaMgr->CreateHisto1D(hnam,100,0,50.,202000+ii*100+4);
    hnam = hgnam + orig[ii] + G4String("Width phi (deg)");
    theAnaMgr->CreateHisto1D(hnam,100,0,200.,202000+ii*100+5);
    hnam = hgnam + orig[ii] + G4String("N sim hits");
    theAnaMgr->CreateHisto1D(hnam,100,0,100.,202000+ii*100+6);
    hnam = hgnam + orig[ii] + G4String("Time span (ns)");
    theAnaMgr->CreateHisto1D(hnam,100,0,1.,202000+ii*100+7);
    hnam = hgnam + orig[ii] + G4String("Distance between hits (mm)");
    theAnaMgr->CreateHisto1D(hnam,100,0,250.,202000+ii*100+8);
  }

}

//--------------------------------------------------------------------
G4int GmRecHitsHistosUA::GetHitTypeInt(const GmRecHit* rhit )
{
  GmHit* hit = rhit->GetHits()[0];

  std::set<G4String>::const_iterator ite;
  G4int nh = 1;
  for( ite = theSDTypes.begin(); ite != theSDTypes.end(); ite++, nh++ ){
    if( hit->GetSDType().substr(1, hit->GetSDType().length()-2) == *ite ) {
      break;
    }
  }
  if( ite == theSDTypes.end() ) {
    G4Exception("GmHitsHistosUA::GetHitTypeInt()",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("SDType not found: " +  hit->GetSDType()).c_str());
  }
  
  return nh;
}

//--------------------------------------------------------------------
void GmRecHitsHistosUA::EndOfEventAction(const G4Event* )
{
  gamosSDRecHitMap rechitsMap = GmHitsEventMgr::GetInstance()->GetRecHits();
  gamosSDRecHitMap::iterator item;
  std::vector<GmRecHit*>::const_iterator ite,ite2;
  std::vector<GmHit*>::const_iterator iteh,iteh2;
  std::map<G4int,G4int> hitsInSDType;

  for( item = rechitsMap.begin(); item != rechitsMap.end(); item++ ){
    std::vector<GmRecHit*> rhits = (*item).second;
    G4double disthits = -1.;

    //---- Loop to rec hits in this event
    for( ite = rhits.begin(); ite != rhits.end(); ite++ ){
      GmRecHit* rhit = *ite;
      int nh = GetHitTypeInt( rhit );
      hitsInSDType[nh]++; //count hits of each SD type
      //      G4cout << " Rechit hitsInSDType " << nh << " = " << hitsInSDType[nh] << G4endl;
      
      theAnaMgr->GetHisto1(202000+nh*100+2)->Fill( float(rhit->GetEnergy()/CLHEP::keV));
      G4ThreeVector depowidth = GetEDepoWidth( rhit );
      theAnaMgr->GetHisto1(202000+nh*100+3)->Fill( depowidth.mag()/CLHEP::mm );
      theAnaMgr->GetHisto1(202000+nh*100+4)->Fill( depowidth.z()/CLHEP::mm );
      theAnaMgr->GetHisto1(202000+nh*100+5)->Fill( depowidth.phi()/CLHEP::deg );
      theAnaMgr->GetHisto1(202000+nh*100+6)->Fill( float(rhit->GetHits().size()));

      //---- Get time difference: loop to hits in this recHit   
      std::vector<GmHit*> hits = rhit->GetHits();
      G4double timemin = DBL_MAX;
      G4double timemax = -DBL_MAX;
      for( iteh = hits.begin(); iteh != hits.end(); iteh++ ){
	GmHit* hit = *iteh;
	if( hit->GetTimeMin() < timemin ) timemin = hit->GetTimeMin();
	if( hit->GetTimeMax() > timemax ) timemax = hit->GetTimeMax();
      }
      theAnaMgr->GetHisto1(202000+nh*100+7)->Fill( (timemax-timemin)/CLHEP::ns );
      for( ite2 = rhits.begin(); ite2 != rhits.end(); ite2++ ){
	GmRecHit* rhit2 = *ite2;
	//	if( hit->GetSDType() != hit2->GetSDType() ) continue;
	G4double dhit = (rhit->GetPosition() - rhit2->GetPosition()).mag();
	if( dhit > disthits ) disthits = dhit;
      }
      if( disthits != -1. ) theAnaMgr->GetHisto1(202000+nh*100+8)->Fill( disthits/CLHEP::mm ); 
    }
  }

  //----- Fill histo of number of hits per type
  std::map<G4int,G4int>::const_iterator itet;
  int nh = 1;
  for( itet = hitsInSDType.begin(); itet != hitsInSDType.end(); itet++, nh++ ){
    theAnaMgr->GetHisto1(202000+nh*100+1)->Fill( float((*itet).second) );
  }
} 

//--------------------------------------------------------------------
void GmRecHitsHistosUA::SumHistos()
{
  for( unsigned int jj = 1; jj <= 6; jj++ ){
#ifndef GAMOS_NO_ROOT
    TH1F* hisAll = theAnaMgr->GetHisto1D(202000+jj);
    for( unsigned int ii = 1; ii <= theSDTypes.size(); ii++ ){
      TH1F* his = theAnaMgr->GetHisto1D(202000+ii*100+jj);
      if( hisAll->GetXaxis()->GetNbins() != his->GetXaxis()->GetNbins() ||
	  hisAll->GetXaxis()->GetXmax() != hisAll->GetXaxis()->GetXmax() || 
	  hisAll->GetXaxis()->GetXmin() != his->GetXaxis()->GetXmin() ) {
#else 
    GmHisto1* hisAll = theAnaMgr->GetHisto1D(202000+jj);
    for( unsigned int ii = 1; ii <= theSDTypes.size(); ii++ ){
      GmHisto1* his = theAnaMgr->GetHisto1D(202000+ii*100+jj);
      if( hisAll->GetNbins() != his->GetNbins() ||
	  hisAll->GetLowerEdge() != his->GetLowerEdge() || 
	  hisAll->GetUpperEdge() != hisAll->GetUpperEdge() ) {
#endif
	G4Exception("GmRecHitsHistos::SumHistos",
		    "Histograms have different dimensions, they cannot be summed up!",
		    FatalException,
		    (G4String("HISTOGRAM 1: ")+hisAll->GetName()+" HISTOGRAM 2: "+ his->GetName()).c_str());
      }
      *hisAll = *hisAll + *his;
    }
  }
}

//--------------------------------------------------------------------
G4ThreeVector GmRecHitsHistosUA::GetEDepoWidth( GmRecHit* rhit )
{
  G4ThreeVector minpos(DBL_MAX,DBL_MAX,DBL_MAX);
  G4ThreeVector maxpos(-DBL_MAX,-DBL_MAX,-DBL_MAX);
  //---- Loop to hits in this recHit   
  std::vector<GmHit*> hits = rhit->GetHits();
  std::vector<GmHit*>::const_iterator iteh;
  for( iteh = hits.begin(); iteh != hits.end(); iteh++ ){
    GmHit* hit = *iteh;
    std::vector<GmEDepo*> edepos = hit->GetEDepos();
    int isiz = edepos.size();
    for( int ii= 0; ii<isiz;ii++){
      G4ThreeVector pos = edepos[ii]->GetPosition();
      if( pos.x() < minpos.x() ) minpos.setX( pos.x() );
      if( pos.y() < minpos.y() ) minpos.setY( pos.y() );
      if( pos.z() < minpos.z() ) minpos.setZ( pos.z() );
      if( pos.x() > maxpos.x() ) maxpos.setX( pos.x() );
      if( pos.y() > maxpos.y() ) maxpos.setY( pos.y() );
      if( pos.z() > maxpos.z() ) maxpos.setZ( pos.z() );
    }

  }
  return maxpos-minpos;
}
