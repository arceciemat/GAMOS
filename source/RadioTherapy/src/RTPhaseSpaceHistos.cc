#include "RTPhaseSpaceHistos.hh"
#include "RTPhaseSpaceUA.hh"
#include "RTVerbosity.hh"
#include "EGSPhspParticle.hh"

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserSteppingActionList.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"

#include "G4Run.hh"
#include "G4ios.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4VSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4Trajectory.hh"
#include "G4UserSteppingAction.hh"

//----------------------------------------------------------------
RTPhaseSpaceHistos::RTPhaseSpaceHistos(RTPhaseSpaceUA* ua)
{
  theZstops = ua->GetZStops();

  Initialise();
}


//----------------------------------------------------------------
RTPhaseSpaceHistos::RTPhaseSpaceHistos(G4double zstop)
{
  theZstops.push_back( zstop );

  Initialise();
}
 

//----------------------------------------------------------------
void RTPhaseSpaceHistos::Initialise()
{
  G4String fileName = GmParameterMgr::GetInstance()->GetStringValue("RTPhaseSpaceHistos:FileName","phaseSpace");

  //  G4String suffix = GmParameterMgr::GetInstance()->GetStringValue("GmAnalysisMgr:FileNameSuffix","");
  //  if(suffix != "" ) fileName += suffix;

  //  G4String prefix = GmParameterMgr::GetInstance()->GetStringValue("GmAnalysisMgr:FileNamePrefix","");
  // if(prefix != "" ) fileName = prefix + fileName;

  theAnaMgr = GmAnalysisMgr::GetInstance(fileName);

  DefineHistoLimits();
  /*  BookHistos("ALL"); 
  BookHistos("gamma");
  BookHistos("e-");
  BookHistos("e+");
  bHadrons = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("RTPhaseSpaceHistos:Hadrons",0));

  if( bHadrons ) {
    BookHistos("neutron");
    BookHistos("proton");
   }*/

  theClassifier = 0;

}

//---------------------------------------------------------------------
void RTPhaseSpaceHistos::DefineHistoLimits()
{
  theNbins = G4int( GmParameterMgr::GetInstance()->GetNumericValue("RTPhaseSpaceHistos:Nbins", 100 ) );
  theHisR = 100.;
  theHisAng = 180.;
  theHisE = 10.*CLHEP::MeV;
}

//---------------------------------------------------------------------
void RTPhaseSpaceHistos::BookHistos(G4String partName, G4int index)
{
  G4int hid = theParticleHistoIDs.size();
  theParticleHistoIDs[partName] = hid;

  G4String sepa = GmParameterMgr::GetInstance()->GetStringValue("Histos:Separator",":");
  G4String hnam;

  for( unsigned int zstopID = 0; zstopID < theZstops.size(); zstopID++ ){
    G4int histoNumber = 760000+10000*zstopID+1000*hid+100*index;
    G4String hgnam = "PhaseSpace"+sepa+" " + GmGenUtils::ftoa(theZstops[zstopID]) + ""+sepa+" " + GetHistoName(index) + ""+sepa+"";
    
    hnam = hgnam + partName + G4String(""+sepa+" X at Zstop");
    theAnaMgr->CreateHisto1D(hnam,theNbins,-theHisR,theHisR,histoNumber+1);
    hnam = hgnam + partName + G4String(""+sepa+" X*10 at Zstop");
    theAnaMgr->CreateHisto1D(hnam,theNbins,-theHisR,theHisR,histoNumber+101);
    hnam = hgnam + partName + G4String(""+sepa+" Y at Zstop");
    theAnaMgr->CreateHisto1D(hnam,theNbins,-theHisR,theHisR,histoNumber+2);
    hnam = hgnam + partName + G4String(""+sepa+" Y*10 at Zstop");
    theAnaMgr->CreateHisto1D(hnam,theNbins,-theHisR,theHisR,histoNumber+102);

    hnam = hgnam + partName + G4String(""+sepa+" XY at Zstop");
    theAnaMgr->CreateHisto2D(hnam,theNbins,-theHisR,theHisR,theNbins,-theHisR,theHisR,histoNumber+9);

    hnam = hgnam + partName + G4String(""+sepa+" R at Zstop");
    theAnaMgr->CreateHisto1D(hnam,theNbins,0.,theHisR,histoNumber+3);
    hnam = hgnam + partName + G4String(""+sepa+" Direction Theta at Zstop");
    theAnaMgr->CreateHisto1D(hnam,theNbins,0,theHisAng/2.,histoNumber+4);
    hnam = hgnam + partName + G4String(""+sepa+" Direction Phi at Zstop");
    theAnaMgr->CreateHisto1D(hnam,theNbins,-theHisAng,theHisAng,histoNumber+5);
    hnam = hgnam + partName + G4String(""+sepa+" Energy at Zstop");
    theAnaMgr->CreateHisto1D(hnam,theNbins,0.,theHisE,histoNumber+6); 
    hnam = hgnam + partName + G4String(""+sepa+" Energy*10 at Zstop");
    theAnaMgr->CreateHisto1D(hnam,theNbins,0.,theHisE,histoNumber+106); 
    hnam = hgnam + partName + G4String(""+sepa+" Vx at Zstop");
    theAnaMgr->CreateHisto1D(hnam,theNbins,0.,1.,histoNumber+7);
    hnam = hgnam + partName + G4String(""+sepa+" Vy at Zstop");
    theAnaMgr->CreateHisto1D(hnam,theNbins,0.,1.,histoNumber+8);
    hnam = hgnam + partName + G4String(""+sepa+" Vz at Zstop");
    theAnaMgr->CreateHisto1D(hnam,theNbins,0.,1.,histoNumber+9);
   
    hnam = hgnam + partName + G4String(""+sepa+" R vs Direction Theta at Zstop");
    theAnaMgr->CreateHisto2D(hnam,theNbins,0,theHisR,theNbins,0.,theHisAng,histoNumber+11);
    hnam = hgnam + partName + G4String(""+sepa+" R vs Energy at Zstop");
    theAnaMgr->CreateHisto2D(hnam,theNbins,0,theHisR,theNbins,0.,theHisE,histoNumber+12);
    hnam = hgnam + partName + G4String(""+sepa+" Direction Theta vs Energy at Zstop");
    theAnaMgr->CreateHisto2D(hnam,theNbins,0,theHisAng,theNbins,0.,theHisE,histoNumber+13);

    hnam = hgnam + partName + G4String(""+sepa+" Weight at Zstop");
    theAnaMgr->CreateHisto1D(hnam,theNbins,-8,2,histoNumber+14);

    hnam = hgnam + partName + G4String(""+sepa+" X vs Energy at Zstop");
    theAnaMgr->CreateHisto2D(hnam,theNbins,0,theHisR,theNbins,0.,theHisE,histoNumber+21);
    hnam = hgnam + partName + G4String(""+sepa+" X vs Vx at Zstop");
    theAnaMgr->CreateHisto2D(hnam,theNbins,0,theHisR,theNbins,0.,1.,histoNumber+22);

    hnam = hgnam + partName + G4String(""+sepa+" X prof weight");
    theAnaMgr->CreateHistoProfile1D(hnam,theNbins,-theHisR,theHisR,histoNumber+30);

    hnam = hgnam + partName + G4String(""+sepa+" X vs log10(Weight)");
    theAnaMgr->CreateHisto2D(hnam,theNbins,-theHisR,theHisR,10,-8,2,histoNumber+91);

  }
}


//---------------------------------------------------------------------
void RTPhaseSpaceHistos::FillHistos( iaea_record_type *p_iaea_record, G4double zstop, const G4Step* aStep )
{
  G4int index;
  if ( aStep && theClassifier ) {
    index = theClassifier->GetIndexFromStep(aStep);
  } else {
    index = 0;
  }

  unsigned int zstopID;
  for( zstopID = 0; zstopID < theZstops.size(); zstopID++ ){
    if( fabs(zstop-theZstops[zstopID]) < 1.E-6 ) break;
  }

  if( theHistos.find(index) == theHistos.end() ) {
    BookHistos("ALL",index);
    BookHistos("gamma",index);
    BookHistos("e-",index);
    BookHistos("e+",index);
    bHadrons = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("RTPhaseSpaceHistos:Hadrons",0));
    
    if( bHadrons ) {
      BookHistos("neutron",index);
      BookHistos("proton",index);
    }
    theHistos.insert(index);
  }

  //  G4int hid = GetHistoID("ALL");
  if( zstopID >= theZstops.size() ) zstopID = 0;
  FillHistos( p_iaea_record, 0, zstopID, index );
  FillHistos( p_iaea_record, p_iaea_record->particle, zstopID, index );
#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(debugVerb) ) G4cout << " RTPhaseSpaceHistos::FillHistos zstop " << zstop << " zstopID " << zstopID << " particle " << p_iaea_record->particle << " Y " << p_iaea_record->y*CLHEP::cm << G4endl;
#endif


}

//---------------------------------------------------------------------
void RTPhaseSpaceHistos::FillHistos( iaea_record_type *p_iaea_record, G4int hid, G4int zstopID, G4int index )
{
  G4double posX = p_iaea_record->x*CLHEP::cm;
  G4double posY = p_iaea_record->y*CLHEP::cm;
  G4double posR = sqrt(p_iaea_record->x*p_iaea_record->x+p_iaea_record->y*p_iaea_record->y)*CLHEP::cm;
  G4double theta = acos(fabs(p_iaea_record->w))/CLHEP::deg;
  G4double phi = atan2(p_iaea_record->u,p_iaea_record->v)/CLHEP::deg;
  //  G4cout << " RTPhaseSpaceHistos::FillHistos theta " << theta << " phi " << phi << G4endl; //GDEB
  //  if( hid == 0 ) G4cout << hid << " " << zstopID << " theta " << theta << " phi " << phi << G4endl;
  G4double energy = fabs(p_iaea_record->energy);
  G4double weight = p_iaea_record->weight;
  //  G4cout << "  RTPhaseSpaceHistos::FillHistos " << hid << " " << posX << " " << posY << " " << posR << " " << theta << " " << phi << " " << energy << G4endl; //GDEB
  G4double dirU = p_iaea_record->u;
  G4double dirV = p_iaea_record->v;
  G4double dirW = p_iaea_record->w;

  G4int histoNumber = 760000+10000*zstopID+1000*hid+100*index;
  FillHistos( posX, posY, posR, theta, phi, energy, dirU, dirV, dirW, weight, histoNumber );

  //  if( RTVerb(debugVerb) ) G4cout << " RTPhaseSpaceHistos::FillHistosp "  << theta<< " " << phi<< " " << dirU<< " " << dirV<< " " << dirW << posX<< " " << posY<< " " << energy<< " " << G4endl; //GDEB

}

//---------------------------------------------------------------------
void RTPhaseSpaceHistos::FillHistos( EGSPhspParticle *particle, G4double zstop )
{
  G4int index = 0;
  if( theHistos.find(index) == theHistos.end() ) {
    BookHistos("ALL",index);
    BookHistos("gamma",index);
    BookHistos("e-",index);
    BookHistos("e+",index);
    if( bHadrons ) {
      BookHistos("neutron",index);
      BookHistos("proton",index);
    }
    theHistos.insert(index);
  }

  unsigned int zstopID;
  for( zstopID = 0; zstopID < theZstops.size(); zstopID++ ){
    if( fabs(zstop-theZstops[zstopID]) < 1.E-6 ) break;
  }

  G4int hid = GetHistoID("ALL");
  FillHistos( particle, hid, zstopID, 0 );
  hid = GetHistoID(particle->theCharge);
  FillHistos( particle, hid, zstopID, 0 );
#ifndef GAMOS_NO_VERBOSE
  if( RTVerb(debugVerb) ) G4cout << " RTPhaseSpaceHistos::FillHistos zstop " << zstop << " zstopID " << zstopID << " particle " << hid << " Y " << particle->thePosY << G4endl;
#endif
}

//----------------------------------------------------- ----------------
void RTPhaseSpaceHistos::FillHistos( EGSPhspParticle * particle, G4int hid,              G4int zstopID, G4int index )
{
  G4double posX = particle->thePosX;
  G4double posY = particle->thePosY;
  G4double posR = sqrt(particle->thePosX*particle->thePosX+particle->thePosY*particle->thePosY);
  G4double theta = acos(fabs(particle->theDirW))/CLHEP::deg;
  G4double phi = atan2(particle->theDirU,particle->theDirV)/CLHEP::deg;
  //  G4cout << " phi " << phi << " phix " << atan2(particle->x,particle->y)/CLHEP::deg  << G4endl;
  G4double energy = fabs(particle->GetKineticEnergy());
  G4double weight = fabs(particle->theWeight);
  G4double dirU = particle->theDirU;
  G4double dirV = particle->theDirV;
  G4double dirW = particle->theDirW;

  G4int histoNumber = 760000+10000*zstopID+1000*hid+100*index;
  FillHistos( posX, posY, posR, theta, phi, energy, dirU, dirV, dirW, weight, histoNumber );
}

//---------------------------------------------------------------------
void RTPhaseSpaceHistos::FillHistos( G4double posX, G4double posY, G4double posR, G4double theta, G4double phi, G4double energy, G4double dirU, G4double dirV,  G4double dirW, G4double weight, G4int histoNumber )
{
  //  if( weight != 1. ) G4cout << " weight " << G4endl;
//  G4cout << "  RTPhaseSpaceHistos::FillHistos " << posX << " " << posY << " " << posR << " " << theta << " " << phi << " " << energy << " WEI " << weight << G4endl; //GDEB
//  G4cout << "  RTPhaseSpaceHistos::FillHistos " << posX << " WEI " << weight << G4endl; //GDEB
  theAnaMgr->GetHisto1(histoNumber+1)->Fill( posX, weight );
  /*  theAnaMgr->GetHisto1(histoNumber+1)->FillError( posX, weight );
    TH1F* histoX=  theAnaMgr->GetHisto1(histoNumber+1);
  TAxis* xAxis = theAnaMgr->GetHisto1(histoNumber+1)->GetXaxis();
  for( int ii = 1; ii < xAxis->GetNbins(); ii++ ) {
    if( histoX->GetBinContent(ii) != 0 ) { 
      G4cout << "HISTOX " << ii << " : " <<  histoX->GetBinContent(ii) << " +- " << histoX->GetBinError(ii) << G4endl; //GDEB
    } 
    }*/
  theAnaMgr->GetHisto1(histoNumber+101)->Fill( posX*10, weight );
  theAnaMgr->GetHisto1(histoNumber+2)->Fill( posY, weight );
  theAnaMgr->GetHisto1(histoNumber+102)->Fill( posY*10, weight );

  theAnaMgr->GetHisto1(histoNumber+3)->Fill( posR, weight );
  theAnaMgr->GetHisto1(histoNumber+4)->Fill( theta, weight );
  theAnaMgr->GetHisto1(histoNumber+5)->Fill( phi, weight );
  theAnaMgr->GetHisto1(histoNumber+6)->Fill( energy, weight );
  theAnaMgr->GetHisto1(histoNumber+106)->Fill( energy*10, weight );

  theAnaMgr->GetHisto1(histoNumber+7)->Fill( dirU, weight );
  theAnaMgr->GetHisto1(histoNumber+8)->Fill( dirV, weight );
  theAnaMgr->GetHisto1(histoNumber+9)->Fill( dirW, weight );
  theAnaMgr->GetHisto2(histoNumber+11)->Fill( posR, theta, weight );
  theAnaMgr->GetHisto2(histoNumber+12)->Fill( posR, energy, weight );
  theAnaMgr->GetHisto2(histoNumber+13)->Fill( theta, energy, weight );
  theAnaMgr->GetHisto2(histoNumber+9)->Fill( posX, posY, weight );

  theAnaMgr->GetHisto1(histoNumber+14)->Fill( log10(weight) );

  theAnaMgr->GetHisto2(histoNumber+21)->Fill( posX, energy, weight );
  theAnaMgr->GetHisto2(histoNumber+22)->Fill( posX, dirU, weight );

  theAnaMgr->GetHistoProf1(histoNumber+30)->Fill( posX, log10(weight) );

  theAnaMgr->GetHisto2(histoNumber+91)->Fill( posX, log10(weight) );

}

//---------------------------------------------------------------------
G4int RTPhaseSpaceHistos::GetHistoID(G4String partName)
{
  std::map<G4String,G4int>::const_iterator ite = theParticleHistoIDs.find(partName);
  if( ite != theParticleHistoIDs.end() ){
    return (*ite).second;
  } else { 
    return -1;
  }
}

//---------------------------------------------------------------------
G4String RTPhaseSpaceHistos::GetHistoName(G4int index)
{
  std::map<G4String,G4int>::const_iterator ite;
  for( ite = theParticleHistoIDs.begin(); ite != theParticleHistoIDs.end(); ite++ ){
    if( (*ite).second == index ) {
      return GmGenUtils::itoa((*ite).second);
    }
  }

  return "";
}


//---------------------------------------------------------------------
G4int RTPhaseSpaceHistos::GetHistoID(float charge)
{
  G4String partName;
  if( charge == 0. ) {
    partName = "gamma";
  } else if( charge == -1. ) {
    partName = "e-";
  } else if( charge == 1. ) {
    partName = "e+";
  } else {
    G4Exception(" RTPhaseSpaceHistos::GetHistoID",
		"Wrong particle charge",
		FatalErrorInArgument,
		G4String("charge should 0, -1 or 1, while it is " + GmGenUtils::ftoa(charge)).c_str());
  }

  std::map<G4String,G4int>::const_iterator ite = theParticleHistoIDs.find(partName);
  if( ite != theParticleHistoIDs.end() ){
    return (*ite).second;
  } else { 
    return -1;
  }
}
