#include "GmHistosGammaAtSD.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4VVisManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"
#include "G4RunManager.hh"

#include <math.h>

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosAnalysis/include/GmCheckOriginalGamma.hh"
#include "GamosCore/GamosAnalysis/include/GmTrajStep.hh"
#include "GamosCore/GamosAnalysis/include/GmEventClassifierByGammaInteraction.hh"
#include "GmSDVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

GmHistosGammaAtSD::GmHistosGammaAtSD()
{
  theAnaMgr = GmAnalysisMgr::GetInstance("gammaSD") ;
  G4String sepa = GmParameterMgr::GetInstance()->GetStringValue("Histos:Separator",":");


  G4String hnam;
  std::vector<G4String> orig(6);
  orig[0] = "ALL"+sepa+" "; 
  orig[1] = "No PE"+sepa+" "; 
  orig[2] = "Only PE"+sepa+" ";
  orig[3] = "PE + 1 Compt"+sepa+" ";
  orig[4] = "PE + 2 Compt"+sepa+" ";
  orig[5] = "PE + >2 Compt"+sepa+" ";
  G4String hgnam = "Gamma At SD"+sepa+" ";
  hnam = G4String("Energy lost ALL (MeV)") ;
  theAnaMgr->CreateHisto1D(hnam,120,0,0.6,103000+91);
  for( unsigned int ii = 0; ii < 6; ii++ ){
    hnam = hgnam + orig[ii] + G4String("Event Type");
    theAnaMgr->CreateHisto1D(hnam,200,0,200,103000+ii*100+1);
    hnam = hgnam + orig[ii] + G4String("N interactions");
    theAnaMgr->CreateHisto1D(hnam,30,0,30,103000+ii*100+2);
    hnam = hgnam + orig[ii] + G4String("N PhotoElec");
    theAnaMgr->CreateHisto1D(hnam,2,0,2,103000+ii*100+3);
    hnam = hgnam + orig[ii] + G4String("N Compton");
    theAnaMgr->CreateHisto1D(hnam,10,0,10,103000+ii*100+4);
    hnam = hgnam + orig[ii] + G4String("N Rayleigh");
    theAnaMgr->CreateHisto1D(hnam,10,0,10,103000+ii*100+5);
    hnam = hgnam + orig[ii] + G4String("N Conversion");
    theAnaMgr->CreateHisto1D(hnam,10,0,10,103000+ii*100+6);
    hnam = hgnam + orig[ii] + G4String("N PhotoElec vs Compton+Rayleigh");
    theAnaMgr->CreateHisto2D(hnam,2,0,2,10,0.,10.,103000+ii*100+7);

    hnam = hgnam + orig[ii] + G4String("Energy at entering SD (MeV)");
    theAnaMgr->CreateHisto1D(hnam,120,0,0.6,103000+ii*100+8);

    hnam = hgnam + orig[ii] + G4String("Energy at entering SD vs energy initial (MeV)");
    theAnaMgr->CreateHisto2D(hnam,120,0,0.6,120,0.,0.6,103000+ii*100+99);

    hnam = hgnam + orig[ii] + G4String("Energy lost PhotoElec (MeV)");
    theAnaMgr->CreateHisto1D(hnam,120,0,0.6,103000+ii*100+11);
    hnam = hgnam + orig[ii] + G4String("Distance to 1st when Compton (mm)") ;
    theAnaMgr->CreateHisto1D(hnam,100,0,1000,103000+ii*100+12);
    hnam = hgnam + orig[ii] + G4String("Distance to 1st when PhotoElec (mm)") ;
    theAnaMgr->CreateHisto1D(hnam,100,0,1000,103000+ii*100+13);
    hnam = hgnam + orig[ii] + G4String("Diff ang when PhotoElec (mrad)");
    theAnaMgr->CreateHisto1D(hnam,100,0,3200,103000+ii*100+14);
    hnam = hgnam + orig[ii] + G4String("Diff energy when PhotoElec (MeV)");
    theAnaMgr->CreateHisto1D(hnam,120,0,0.6,103000+ii*100+15);

    hnam = hgnam + orig[ii] + G4String("Energy lost Compton (MeV)");
    theAnaMgr->CreateHisto1D(hnam,120,0,0.6,103000+ii*100+21);
    hnam = hgnam + orig[ii] + G4String("Angle variation Compton (mrad)");
    theAnaMgr->CreateHisto1D(hnam,100,0,3200.,103000+ii*100+22);

    hnam = hgnam + orig[ii] + G4String("Energy lost Rayleigh (MeV)") ;
    theAnaMgr->CreateHisto1D(hnam,120,0,0.6,103000+ii*100+31);
    hnam = hgnam + orig[ii] + G4String("Angle variation Rayleigh (mrad)");
    theAnaMgr->CreateHisto1D(hnam,100,0,3200.,103000+ii*100+32);

    hnam = hgnam + orig[ii] + G4String("Energy lost Conversion (MeV)") ;
    theAnaMgr->CreateHisto1D(hnam,120,0,0.6,103000+ii*100+41);
    hnam = hgnam + orig[ii] + G4String("Angle variation Conversion (mrad)");
    theAnaMgr->CreateHisto1D(hnam,100,0,3200.,103000+ii*100+42);

    //----- plot track length between interactions
    hnam = hgnam + orig[ii] + G4String("Distance between interactions (mm)");
    theAnaMgr->CreateHisto1D(hnam,500,0,250.,103000+ii*100+51);
    hnam = hgnam + orig[ii] + G4String("Distance between 1st and other interactions (mm)");
    theAnaMgr->CreateHisto1D(hnam,500,0,250.,103000+ii*100+52);
    hnam = hgnam + orig[ii] + G4String("Distance between Compton-Compton interactions (mm)");
    theAnaMgr->CreateHisto1D(hnam,500,0,250.,103000+ii*100+53);
    hnam = hgnam + orig[ii] + G4String("Distance between Compton-PhotElec interactions (mm)");
    theAnaMgr->CreateHisto1D(hnam,500,0,250.,103000+ii*100+54);

  }

  hnam = hgnam + G4String("Number of entering gammas");
  theAnaMgr->CreateHisto1D(hnam,20,0,20.,103000+61);
  hnam = hgnam + G4String("Energy of entering gamma 1 vs gamma 2");
  theAnaMgr->CreateHisto2D(hnam,100,0,600.,100,0.,600.,103000+62);

  theEventClassifier = new GmEventClassifierByGammaInteraction;

  cRAYL = 0;
  cCONV = 0;
  
}


void GmHistosGammaAtSD::BeginOfEventAction( const G4Event* )
{
  theNGammasInSD = 0;
}

void GmHistosGammaAtSD::EndOfEventAction( const G4Event* )
{
  cGammasInSD[theNGammasInSD]++;

  theAnaMgr->GetHisto1(103000+61)->Fill( theEnteringEnergies.size() );

  if( theEnteringEnergies.size() == 2 ) {
    theAnaMgr->GetHisto2(103000+62)->Fill( theEnteringEnergies[0], theEnteringEnergies[1] );
  }

  theEnteringEnergies.clear();

}

void GmHistosGammaAtSD::PreUserTrackingAction(const G4Track* aTrack)
{
  isOriginalGamma = GmCheckOriginalGamma::CheckIfOriginalGamma( aTrack );
  bRAYL = false;
  bCONV = false;
}


void GmHistosGammaAtSD::PostUserTrackingAction(const G4Track* )
{
  if ( isOriginalGamma == 0 ) return;

  if( theInteractions.GetNumberOfSteps() != 0 ) {
    FillHistos();
    theNGammasInSD++;
  }

  if( theInteractions.GetSteps().size() != 0 ) {
    theEnteringEnergies.push_back( theInteractions.GetStep(0)->GetKineticEnergyPre() );
  }

  theInteractions.Clear();

  if( bRAYL ) cRAYL++;
  if( bCONV ) cCONV++;
}


void GmHistosGammaAtSD::UserSteppingAction(const G4Step* aStep)
{ 
  //  G4cout << this << "step isoriginalgamma " << isOriginalGamma << G4endl;
  //----- Only original gammas 
  if ( isOriginalGamma == 0) return;

  if( !IsInterestingStep( aStep ) ) return ;

  theInteractions.AddStep( new GmTrajStep( aStep ) ); 
}

G4bool GmHistosGammaAtSD::IsInterestingStep(const G4Step* aStep)
{
  if( aStep->GetTrack()->GetCurrentStepNumber() == 0 ) return 0;

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << "GmHistosGammaAtSD::IsInterestingStep proc " << aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() << G4endl;
#endif
  //  if( aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() == "Transportation" ) return 0;

  //----- Check if step is in SD 
  G4Track* aTrack = aStep->GetTrack();
  G4VSensitiveDetector* sdet = aTrack->GetVolume()->GetLogicalVolume()->GetSensitiveDetector();
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << "GmHistosGammaAtSD::IsInterestingStep sdet " << sdet << " vol " << aTrack->GetVolume()->GetName() << " mate " <<aTrack->GetMaterial()->GetName() <<  G4endl;
#endif
  if( sdet == 0 ){
    return 0;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(infoVerb) ) G4cout << "GmHistosGammaAtSD::IsInterestingStep sdet " << sdet << " vol " << aTrack->GetVolume()->GetName() << " mate " <<aTrack->GetMaterial()->GetName() <<  G4endl;
#endif
    return 1;
  }

}

void GmHistosGammaAtSD::FillHistos()
{
  int nPE = 0, nCOMP = 0, nRAYL = 0, nCONV = 0;
  G4int npoints = theInteractions.GetNumberOfSteps();
  G4ThreeVector dirFirst, posFirst;
  G4double kinEFirst = 0.;

  int icl = 0;
  G4int evtClass = theEventClassifier->Classify( theInteractions.GetSteps() );

  if( evtClass < 10000 ){
    icl = 1; // "No PE: "; 
  } else if( evtClass >= 10000 && (evtClass/100)%100 == 0 ){
    icl = 2;// "Only PE: ";
  } else if( (evtClass/100)%100 == 1 ){
    icl = 3;// "PE + 1 Compt"
  } else if( (evtClass/100)%100 == 2 ){
    icl = 4;// "PE + 2 Compt"
  } else if( (evtClass/100)%100 >= 3 ) {
    icl = 5;// "PE + >2 Compt"
  } else { 
    G4Exception(" GmHistosGammaAtSD::FillHistos ",
		"Wrong classfication",
		FatalException,
		G4String("CLASS= " + GmGenUtils::itoa(icl) + " eventClass= " + GmGenUtils::itoa(evtClass)).c_str());
  }
  
  cClassification[evtClass] ++;

  //  G4cout << " GmHistosGammaAtSD::FillHistos CLASS " << icl << " evtClass " << evtClass << G4endl;
  G4double energyEntering;
  G4double energyLost = 0.;
  for( G4int ii = 0; ii < npoints; ii++ ){
    //----- Fill histograms
    const GmTrajStep* trajsp = theInteractions.GetStep(ii);

    if( ii == 0 ){
      posFirst = trajsp->GetPositionPre();
      dirFirst = trajsp->GetMomentumPre(); dirFirst /= dirFirst.mag();
      kinEFirst = trajsp->GetKineticEnergyPre();
    }
    G4double dener = trajsp->GetDeltaKineticEnergy();
    energyLost += dener;

    if( dener != 0 ) {
      if( trajsp->GetProcessName() == "phot" ||
	  trajsp->GetProcessName() == "compt" || 
	  trajsp->GetProcessName() == "Rayl"  ||
	  trajsp->GetProcessName() == "conv" ){ 
	theAnaMgr->GetHisto1(103000+91)->Fill( dener );
      }
    }
    if( trajsp->GetProcessName() == "phot" ){ 
      nPE++;
      theAnaMgr->GetHisto1(103000+icl*100+11)->Fill( dener );
      theAnaMgr->GetHisto1(103000+11)->Fill( dener );

      if( ii != 0 ) {
	G4double posdiff = (trajsp->GetPositionPost() - posFirst).mag();
	theAnaMgr->GetHisto1(103000+icl*100+13)->Fill( posdiff  );
	theAnaMgr->GetHisto1(103000+13)->Fill( posdiff );

	G4ThreeVector dir = trajsp->GetMomentumPre(); // Post momentum is 0
	dir /= dir.mag();
	G4double dirdiff = acos( dirFirst * dir ) / CLHEP::mrad;
	theAnaMgr->GetHisto1(103000+icl*100+14)->Fill( dirdiff  );
	theAnaMgr->GetHisto1(103000+14)->Fill( dirdiff  );

	G4double kinEdiff = (kinEFirst - trajsp->GetKineticEnergyPre() );
	theAnaMgr->GetHisto1(103000+icl*100+15)->Fill( kinEdiff  );
	theAnaMgr->GetHisto1(103000+15)->Fill( kinEdiff  );
#ifndef GAMOS_NO_VERBOSE
	if( SDVerb(testVerb) ) G4cout << "PE: posdiff " << posdiff << " pos " << trajsp->GetPositionPost() << " posFirst " << posFirst << G4endl 
	       << "PE: dirdiff " << dirdiff << " dir " << dir << " dirFirst " << dirFirst << G4endl
	       << "PE: kinEdiff " << kinEdiff << " kinE " << trajsp->GetKineticEnergyPre() << " kinEFirst " << kinEFirst << G4endl; 
#endif

      }
    }else if( trajsp->GetProcessName() == "compt" ){
      nCOMP++;
      if( icl != 0 ){
	theAnaMgr->GetHisto1(103000+icl*100+21)->Fill( dener );
	theAnaMgr->GetHisto1(103000+21)->Fill( dener );

	G4double ang = trajsp->GetDeltaAngle()/CLHEP::mrad;
	theAnaMgr->GetHisto1(103000+icl*100+22)->Fill( ang ); 
	theAnaMgr->GetHisto1(103000+22)->Fill( ang ); 
#ifndef GAMOS_NO_VERBOSE
	if( SDVerb(testVerb) ) G4cout << "COMPTON: dener " << dener << " ang " << ang << G4endl; 
#endif
      }
      if( ii != 0 ) {
	G4double posdiff = (trajsp->GetPositionPost() - posFirst).mag();
	theAnaMgr->GetHisto1(103000+icl*100+12)->Fill( posdiff  );
	theAnaMgr->GetHisto1(103000+12)->Fill( posdiff );
      }
    }else if( trajsp->GetProcessName() == "Rayl" ){
      nRAYL++;
      bRAYL = true;
      if( ii != 0 ){
	theAnaMgr->GetHisto1(103000+icl*100+31)->Fill( dener );
	theAnaMgr->GetHisto1(103000+31)->Fill( dener );

	G4double ang = trajsp->GetDeltaAngle()/CLHEP::mrad;
	theAnaMgr->GetHisto1(103000+icl*100+32)->Fill( ang );
	theAnaMgr->GetHisto1(103000+32)->Fill( ang );
#ifndef GAMOS_NO_VERBOSE
	if( SDVerb(testVerb) ) G4cout << "RAYLEGIH: dener " << dener << " ang " << ang << G4endl; 
#endif
      }
    }else if( trajsp->GetProcessName() == "conv" ){
      nCONV++;
      bCONV = true;
      if( ii != 0 ){
	theAnaMgr->GetHisto1(103000+icl*100+41)->Fill( dener );
	theAnaMgr->GetHisto1(103000+41)->Fill( dener );
	
	G4double ang = trajsp->GetDeltaAngle()/CLHEP::mrad;
	theAnaMgr->GetHisto1(103000+icl*100+42)->Fill( ang );
	theAnaMgr->GetHisto1(103000+42)->Fill( ang );
#ifndef GAMOS_NO_VERBOSE
	if( SDVerb(testVerb) ) G4cout << "CONVERSION: dener " << dener << " ang " << ang << G4endl; 
#endif
	
      }
    }
    if( ii == 0 ) {
      G4double kinE = trajsp->GetKineticEnergyPre();
      theAnaMgr->GetHisto1(103000+icl*100+8)->Fill(kinE);
      theAnaMgr->GetHisto1(103000+8)->Fill(kinE);
      energyEntering = kinE;
    }

    G4String procNameii = trajsp->GetProcessName();
    if( procNameii == "compt" || 
	procNameii == "phot" ||    
	procNameii == "Rayl" ||    
	procNameii == "conv" ) {
      for( G4int jj = ii+1; jj < npoints; jj++ ){
	const GmTrajStep* trajspjj = theInteractions.GetStep(jj);
	G4String procNamejj = trajspjj->GetProcessName();
	if( procNamejj == "compt" ||
	    procNamejj == "phot" ||    
	    procNamejj == "Rayl" ||    
	    procNamejj == "conv" ) {
	  //-	  if( ii == 0 ) trajsp0 = theInteractions.GetStep(0);
	  G4ThreeVector posii = trajsp->GetPositionPost();
	  G4ThreeVector posjj = trajspjj->GetPositionPost();
	  G4double dist = (posii-posjj).mag();
	  theAnaMgr->GetHisto1(103000+icl*100+51)->Fill(dist);
	  theAnaMgr->GetHisto1(103000+51)->Fill(dist);
	  if( ii == 0 ){
	    theAnaMgr->GetHisto1(103000+icl*100+52)->Fill(dist);
	    theAnaMgr->GetHisto1(103000+52)->Fill(dist);
	  }
	  if( procNameii == "compt" ) {
	    if(  procNamejj == "compt" ){
	      theAnaMgr->GetHisto1(103000+icl*100+53)->Fill(dist);
	      theAnaMgr->GetHisto1(103000+53)->Fill(dist);
	    } else if(  procNamejj == "phot" ){
	      theAnaMgr->GetHisto1(103000+icl*100+54)->Fill(dist);
	      theAnaMgr->GetHisto1(103000+54)->Fill(dist);
	    }
	  }
	}
      }
    }

  }

  theAnaMgr->GetHisto2(103000+icl*100+99)->Fill( energyEntering, energyLost );
  theAnaMgr->GetHisto2(103000+99)->Fill( energyEntering, energyLost );

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(debugVerb) ) G4cout << icl << " EVTCLAS " << evtClass << " npe " << nPE << " ncomp " << nCOMP << " nrayl " << nRAYL << G4endl;
#endif
  theAnaMgr->GetHisto1(103000+icl*100+1)->Fill( float(evtClass/100.+0.01) );
  theAnaMgr->GetHisto1(103000+icl*100+2)->Fill( float(nPE+nCOMP+nRAYL+nCONV+0.01) );
  theAnaMgr->GetHisto1(103000+icl*100+3)->Fill( float(nPE+0.01) );
  theAnaMgr->GetHisto1(103000+icl*100+4)->Fill( float(nCOMP+0.01) );
  theAnaMgr->GetHisto1(103000+icl*100+5)->Fill( float(nRAYL+0.01) );
  theAnaMgr->GetHisto1(103000+icl*100+6)->Fill( float(nCONV+0.01) );
  theAnaMgr->GetHisto2(103000+icl*100+7)->Fill( float(nPE+0.01), float(nCOMP+nRAYL+0.01) );
  theAnaMgr->GetHisto1(103000+1)->Fill( float(evtClass/100.+0.01) );
  theAnaMgr->GetHisto1(103000+2)->Fill( float(nPE+nCOMP+nRAYL+nCONV+0.01) );
  theAnaMgr->GetHisto1(103000+3)->Fill( float(nPE+0.01) );
  theAnaMgr->GetHisto1(103000+4)->Fill( float(nCOMP+0.01) );
  theAnaMgr->GetHisto1(103000+5)->Fill( float(nRAYL+0.01) );
  theAnaMgr->GetHisto1(103000+6)->Fill( float(nCONV+0.01) );
  theAnaMgr->GetHisto2(103000+7)->Fill( float(nPE+0.01), float(nCOMP+nRAYL+0.01) );

}

void GmHistosGammaAtSD::EndOfRunAction( const G4Run* )
{
#ifndef GAMOS_NO_ROOT
  TH1F* hisN = theAnaMgr->GetHisto1(103000+1);
  int nbins = hisN->GetNbinsX();
#else 
  GmHisto1* hisN = theAnaMgr->GetHisto1(103000+1);
  int nbins = hisN->GetNbins();
#endif
  G4double nevents = int(GmNumberOfEvent::GetNumberOfEvent());
  int cInSD = int(hisN->GetEntries());
  std::vector<int> nbinVal(nbins);
  int cPE = 0;
  int cPE0COMP = 0;
  int cPE1COMP = 0;
  int cPE2COMP = 0;
  int cPEnCOMP = 0;
  int cCOMPaver = 0;
  int c1COMP = 0;
  int c2COMP = 0;
  int cnCOMP = 0;
  std::map<G4int,G4int>::const_iterator ite;
  for( ite = cClassification.begin(); ite != cClassification.end(); ite++ ){
    G4int iclass = (*ite).first;
    if( iclass>= 10000 ) cPE += (*ite).second;
    if( iclass>= 10300 ) cPEnCOMP += (*ite).second;
    if( iclass/100%10 >= 1 ) cCOMPaver += (*ite).second * (iclass/100%10);
    if( iclass/100%10 == 1 ) c1COMP += (*ite).second;
    if( iclass/100%10 == 2 ) c2COMP += (*ite).second;
    if( iclass/100%10 > 2 ) cnCOMP += (*ite).second;
    //    G4cout << "CLASS " << iclass << " cPEnCOMP " << cPEnCOMP << "cnCOMP " << cnCOMP << G4endl;
  }
  cPE0COMP = cClassification[10000]+cClassification[10001];
  cPE1COMP = cClassification[10100]+cClassification[10101];
  cPE2COMP = cClassification[10200]+cClassification[10201];

  int cCOMP = c1COMP + c2COMP + cnCOMP ;
  G4cout << G4endl <<  " $$$$$$$$$$ Classification of Gamma Interactions in SD $$$$$$$$ " << G4endl;
  G4cout << " $$GSD: n Events      : " << nevents  << G4endl;
  G4cout << " $$GSD: n gamma in SD : " << cInSD << " : " << 100.* cInSD/nevents/2. << " % " << G4endl;
  G4cout << " $$GSD:  n PE         : " << cPE << " : " << 100.*cPE/cInSD << " % " << G4endl;
  G4cout << " $$GSD:    PE 0 COMP    : " << cPE0COMP << " : " << 100.* cPE0COMP/cPE << " % " << G4endl;
  G4cout << " $$GSD:    PE 1 COMP    : " << cPE1COMP << " : " << 100.* cPE1COMP/cPE  << " % " << G4endl;
  G4cout << " $$GSD:    PE 2 COMP    : " << cPE2COMP << " : " << 100.* cPE2COMP/cPE  << " % " << G4endl;
  G4cout << " $$GSD:    PE >2 COMP   : " << cPEnCOMP << " : " << 100.* cPEnCOMP/cPE << " % " << G4endl;
  G4double c0pecomp = cInSD - cPE - (c1COMP-cPE1COMP) - (c2COMP-cPE2COMP) -(cnCOMP-cPEnCOMP);
  G4cout << " $$GSD: No PE 0 COMP    : " << c0pecomp << " : " << 100.* (c0pecomp)/(cInSD-cPE) << " % " << G4endl;
  G4cout << " $$GSD: No PE 1 COMP    : " << c1COMP-cPE1COMP << " : " << 100.* (c1COMP-cPE1COMP)/(cInSD-cPE)  << " % " << G4endl;
  G4cout << " $$GSD: No PE 2 COMP    : " << c2COMP-cPE2COMP << " : " << 100.* (c2COMP-cPE2COMP)/(cInSD-cPE)  << " % " << G4endl;
  G4cout << " $$GSD: No PE >2 COMP   : " << cnCOMP-cPEnCOMP << " : " << 100.* (cnCOMP-cPEnCOMP)/(cInSD-cPE) << " % " << G4endl;
  G4cout << " $$GSD:  n COMP       : " << cCOMP << " : " << 100.* cCOMP/cInSD << " % " << G4endl;
  G4cout << " $$GSD:    1 COMP       : " << c1COMP << " : " << 100.* c1COMP/cCOMP << " % " << G4endl;
  G4cout << " $$GSD:    2 COMP       : " << c2COMP << " : " << 100.* c2COMP/cCOMP << " % " << G4endl;
  G4cout << " $$GSD:    >2 COMP      : " << cnCOMP << " : " << 100.* cnCOMP/cCOMP << " % " << G4endl;
  G4cout << " $$GSD: nCOMP/gamma     : " << float(cCOMPaver)/cInSD << G4endl;
  G4cout << " $$GSD: n RAYL        : " << cRAYL << " : " << 100.* cRAYL/cInSD << " % " << G4endl;
  G4cout << " $$GSD: n CONV        : " << cCONV << " : " << 100.* cCONV/cInSD << " % " << G4endl;
  G4cout << " $$GSD:  0 Gamma in event : " << cGammasInSD[0] << " : " << 100.* cGammasInSD[0]/nevents << " % " << G4endl;
  G4cout << " $$GSD:  1 Gamma in event : " << cGammasInSD[1] << " : " << 100.* cGammasInSD[1]/nevents << " % " << G4endl; 
  G4cout << " $$GSD:  2 Gamma in event : " << cGammasInSD[2] << " : " << 100.* cGammasInSD[2]/nevents << " % " << G4endl;
  G4int nMoreG = 0;
  for( ite = cGammasInSD.begin(); ite != cGammasInSD.end(); ite++ ){
    if( (*ite).first > 2 ) nMoreG += (*ite).second;
  }
  if( nMoreG > 0 ) G4cout << " $$GSD:  >2 Gamma in event : " << nMoreG << " : " << 100.* nMoreG/nevents << " % " << G4endl;
  //  std::map<G4int,G4int>::iterator ite;
  G4int cP2g = 0;
  for( ite = cGammasInSD.begin(); ite != cGammasInSD.end(); ite++) {
    if( (*ite).first > 2 ) cP2g = (*ite).second;
  }
  if( cP2g > 0 ) {
    G4cout << "     0 Gamma " << cP2g << " : " << 100.* cP2g/nevents << " % " << G4endl;
  }
}


/*
    n out SD : ALL, (ent)/nevents
    n 1 PE : ALL, N PE (1)
    n 1 PE no compton : only PE (ent) 
    n 1 PE 1 compton : 1 compt,N PE (1)
    n 1 PE >1 compton :  > 1 compt, N PE (1)
    N compton aver : all, N compton, (aver)
    N 1 compton  : all, N compton, (1)
    N >1 compton  : all, N compton, (sum >1) */
  
