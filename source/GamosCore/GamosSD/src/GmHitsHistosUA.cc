#include "GmHitsHistosUA.hh"
#include "GmHitsEventMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosSD/include/GmSDVerbosity.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#ifndef GAMOS_NO_ROOT
#include "TAxis.h"
#endif
#include "G4Event.hh"


//--------------------------------------------------------------------
GmHitsHistosUA::GmHitsHistosUA()
{
}

//--------------------------------------------------------------------
void GmHitsHistosUA::BeginOfRunAction(const G4Run* )
{
  BookHistos();
}


//--------------------------------------------------------------------
void GmHitsHistosUA::EndOfRunAction(const G4Run* )
{
  SumHistos();
}

 
GmHitsHistosUA::~GmHitsHistosUA(){}

//--------------------------------------------------------------------
void GmHitsHistosUA::BookHistos()
{
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) G4cout << "GmHitsHistosUA::BookHistos()" << G4endl;
#endif
  theAnaMgr = GmAnalysisMgr::GetInstance("hits") ;

  G4String hnam;
  theSDTypes = GmGeometryUtils::GetInstance()->GetAllSDTypes();
  std::vector<G4String> orig;
  orig.push_back("SD ALL:");
  std::set<G4String>::const_iterator ites;
  for( ites = theSDTypes.begin(); ites != theSDTypes.end(); ites++ ){
    orig.push_back("SD " + *ites + ": ");  
  }
  G4String hgnam = "Hits: ";
  for( unsigned int ii = 0; ii <= theSDTypes.size(); ii++ ){
    hnam = hgnam + orig[ii] + G4String("Nhits");
    theAnaMgr->CreateHisto1D(hnam,100,0,100,201000+ii*100+1);
    hnam = hgnam + orig[ii] + G4String("Energy (keV)");
    theAnaMgr->CreateHisto1D(hnam,150,0.,1500.,201000+ii*100+2);
    hnam = hgnam + orig[ii] + G4String("Width R3 (mm)");
    theAnaMgr->CreateHisto1D(hnam,100,0,1.,201000+ii*100+3);
    hnam = hgnam + orig[ii] + G4String("Width Z (mm)");
    theAnaMgr->CreateHisto1D(hnam,100,0,1.,201000+ii*100+4);
    hnam = hgnam + orig[ii] + G4String("Width phi (deg)");
    theAnaMgr->CreateHisto1D(hnam,100,0,200.,201000+ii*100+5);
    hnam = hgnam + orig[ii] + G4String("N E depos");
    theAnaMgr->CreateHisto1D(hnam,100,0,100.,201000+ii*100+6);
    hnam = hgnam + orig[ii] + G4String("Time span (ns)");
    theAnaMgr->CreateHisto1D(hnam,100,0,1.,201000+ii*100+7);
    hnam = hgnam + orig[ii] + G4String("Distance between hits (mm)");
    theAnaMgr->CreateHisto1D(hnam,100,0,250.,201000+ii*100+8);
    hnam = hgnam + orig[ii] + G4String("X hit (mm)");
    theAnaMgr->CreateHisto1D(hnam,200,-1000,1000.,201000+ii*100+11);
    hnam = hgnam + orig[ii] + G4String("Y hit (mm)");
    theAnaMgr->CreateHisto1D(hnam,200,-1000,1000.,201000+ii*100+12);
    hnam = hgnam + orig[ii] + G4String("Z hit (mm)");
    theAnaMgr->CreateHisto1D(hnam,200,-1000,1000.,201000+ii*100+13);
    hnam = hgnam + orig[ii] + G4String("R2 hit (mm)");
    theAnaMgr->CreateHisto1D(hnam,200,0,300.,201000+ii*100+14);
    hnam = hgnam + orig[ii] + G4String("PHI hit (deg)");
    theAnaMgr->CreateHisto1D(hnam,360,0.,360.,201000+ii*100+15);
    hnam = hgnam + orig[ii] + G4String("THETA hit (deg)");
    theAnaMgr->CreateHisto1D(hnam,180,0.,180.,201000+ii*100+16);
    hnam = hgnam + orig[ii] + G4String("R3 hit (mm)");
    theAnaMgr->CreateHisto1D(hnam,200,0,1000.,201000+ii*100+17);

  }

}
//--------------------------------------------------------------------
G4int GmHitsHistosUA::GetHitTypeInt(const GmHit* hit )
{
  std::set<G4String>::const_iterator ite;
  G4int nh = 1;
  for( ite = theSDTypes.begin(); ite != theSDTypes.end(); ite++, nh++ ){
    if( hit->GetSDType().substr(1, hit->GetSDType().length()-2) == *ite ) {
      break;
    }
  }
  if( ite == theSDTypes.end() ) {
    for( ite = theSDTypes.begin(); ite != theSDTypes.end(); ite++ ){
      G4cout << "AVAILABLE SD type " << *ite << G4endl;
    }
    G4Exception("GmHitsHistosUA::GetHitTypeInt()",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("SDType not found: " +  hit->GetSDType()).c_str());
  }
  
  return nh;

}

//--------------------------------------------------------------------
void GmHitsHistosUA::EndOfEventAction(const G4Event* )
{
  gamosSDHitMap hitlists = GmHitsEventMgr::GetInstance()->GetHitLists();
 
  gamosSDHitMap::const_iterator itehm;
  hitVector::const_iterator iteh,iteh2;
  G4int nhitsGood = 0;
  std::map<G4int,G4int> hitsInSDType; 
  //---- Loop to hits in this event
  //  hitsInEvent.clear();
  for( itehm = hitlists.begin(); itehm != hitlists.end(); itehm++ ){
    GmHitList* hitlist = (*itehm).second;
    G4int nh;
    const hitVector* hitVector = hitlist->GetHitsCompatibleInTime();
    for( iteh = hitVector->begin(); iteh != hitVector->end(); iteh++ ){
      GmHit* hit = *iteh;
     
      /*  std::vector<GmHit*> hits = GmHitsEventMgr::GetInstance()->GetHitsInEvent( evt->GetEventID() );
	  std::vector<GmHit*>::const_iterator ite,ite2;

#ifndef GAMOS_NO_VERBOSE
	  if( SDVerb(debugVerb) ) G4cout << " GmHitsHistosUA::EndOfEventAction nhits " << hits.size() << G4endl;
#endif
	  
	  //---- Loop to hits in this event
	  for( ite = hits.begin(); ite != hits.end(); ite++ ){
	  GmHit* hit = *ite;
      */
      nh = GetHitTypeInt( hit );
      //---- Plot number of hits
      hitsInSDType[nh]++; //count hits of each SD type
      
      theAnaMgr->GetHisto1(201000+nh*100+2)->Fill( float(hit->GetEnergy()/CLHEP::keV));
      G4ThreeVector depowidth = GetEDepoWidth( hit );
      theAnaMgr->GetHisto1(201000+nh*100+3)->Fill( depowidth.mag()/CLHEP::mm );
      theAnaMgr->GetHisto1(201000+nh*100+4)->Fill( depowidth.z()/CLHEP::mm );
      theAnaMgr->GetHisto1(201000+nh*100+5)->Fill( depowidth.phi()/CLHEP::deg );
      theAnaMgr->GetHisto1(201000+nh*100+6)->Fill( float(hit->GetEDepos().size()) );
      theAnaMgr->GetHisto1(201000+nh*100+7)->Fill( float(hit->GetTimeMax()-hit->GetTimeMin())/CLHEP::ns );

      G4ThreeVector pos = hit->GetPosition();
      theAnaMgr->GetHisto1(201000+nh*100+11)->Fill( pos.x() );
      theAnaMgr->GetHisto1(201000+nh*100+12)->Fill( pos.y() );
      theAnaMgr->GetHisto1(201000+nh*100+13)->Fill( pos.z() );
      theAnaMgr->GetHisto1(201000+nh*100+14)->Fill( pos.perp() );
      theAnaMgr->GetHisto1(201000+nh*100+15)->Fill( pos.phi()/CLHEP::deg );
      theAnaMgr->GetHisto1(201000+nh*100+16)->Fill( pos.theta()/CLHEP::deg );
      theAnaMgr->GetHisto1(201000+nh*100+17)->Fill( pos.mag() );
      
      G4double disthits = -1.;
      for( iteh2 = hitVector->begin(); iteh2 != hitVector->end(); iteh2++ ){
	GmHit* hit2 = *iteh2;
	//	if( hit->GetSDType() != hit2->GetSDType() ) continue;
	G4double dhit = (hit->GetPosition() - hit2->GetPosition()).mag();
	if( dhit > disthits ) disthits = dhit;
#ifndef GAMOS_NO_VERBOSE
	// if( SDVerb(debugVerb) ) G4cout << ii << " " << ii2 << " disthits " << disthits << " dhit " << dhit << " pos1 " << hit->GetPosition() << " pos2 " << hit2->GetPosition() << " diff " << hit->GetPosition()-hit2->GetPosition() << G4endl;
#endif
      }
      if( disthits != -1. ) theAnaMgr->GetHisto1(201000+nh*100+8)->Fill( disthits/CLHEP::mm );
    }
    
  }

  //----- Fill histo of number of hits per type
  std::map<G4int,G4int>::const_iterator itet;
  int nh = 1;
  for( itet = hitsInSDType.begin(); itet != hitsInSDType.end(); itet++, nh++ ){
    theAnaMgr->GetHisto1(201000+nh*100+1)->Fill( float((*itet).second) );
  }
  theAnaMgr->GetHisto1(201000+1)->Fill( float(nhitsGood) );

} 

//--------------------------------------------------------------------
void GmHitsHistosUA::SumHistos()
{
  for( unsigned int jj = 1; jj <= 17; jj++ ){
    if( jj == 8 || jj == 9 || jj == 10 ) continue;
#ifndef GAMOS_NO_ROOT
    TH1F* hisAll = theAnaMgr->GetHisto1D(201000+jj);
    for( unsigned int ii = 1; ii <= theSDTypes.size(); ii++ ){
      TH1F* his = theAnaMgr->GetHisto1D(201000+ii*100+jj);
      if( hisAll->GetXaxis()->GetNbins() != his->GetXaxis()->GetNbins() ||
	  hisAll->GetXaxis()->GetXmax() != hisAll->GetXaxis()->GetXmax() || 
	  hisAll->GetXaxis()->GetXmin() != his->GetXaxis()->GetXmin() ) {
#else
    GmHisto1* hisAll = theAnaMgr->GetHisto1D(201000+jj);
    for( unsigned int ii = 1; ii <= theSDTypes.size(); ii++ ){
      GmHisto1* his = theAnaMgr->GetHisto1D(201000+ii*100+jj);
      if( hisAll->GetNbins() != his->GetNbins() ||
	  hisAll->GetLowerEdge() != his->GetLowerEdge() || 
	  hisAll->GetUpperEdge() != hisAll->GetUpperEdge() ) {
#endif
	G4Exception("GmHitsHistos::SumHistos",
		    "Histograms have different dimensions, they cannot be summed up!",
		    FatalException,
		    (G4String("HISTOGRAM 1: ")+hisAll->GetName()+" HISTOGRAM 2: "+ his->GetName()).c_str());
      }
      *hisAll = *hisAll + *his;
    }
  }
}

//--------------------------------------------------------------------
G4ThreeVector GmHitsHistosUA::GetEDepoWidth( GmHit* hit )
{
  G4ThreeVector minpos(DBL_MAX,DBL_MAX,DBL_MAX);
  G4ThreeVector maxpos(-DBL_MAX,-DBL_MAX,-DBL_MAX);
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
  return maxpos-minpos;
}
