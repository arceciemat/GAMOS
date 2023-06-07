#include "GmPDSProcessHelper.hh"
#include "GmPDSInteractionAngleManager.hh"
#include "GmPDSScore.hh"
#include "GmPDSUtils.hh"
#include "GmPDSDetector.hh"
#ifndef WIN32
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#endif

#include "GamosCore/GamosBase/Base/include/GmTrackInfo.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosBase/Base/include/GmVClassifier.hh"
#include "GamosCore/GamosBase/Base/include/GmVFilter.hh"
#ifdef PDS_OP
#include "GamosCore/GamosPhysics/PhysicsList/include/G4VOpDiscreteProcess.hh"
#endif

#include "G4Neutron.hh"
#include "G4Geantino.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SteppingManager.hh"
#include "G4HadronicProcess.hh"
#include "G4Navigator.hh"
#include "G4TransportationManager.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"

#include "G4VEmProcess.hh"

//------------------------------------------------------------------
GmPDSProcessHelper::GmPDSProcessHelper(PDS1aryType ptype ) 
  : the1aryType( ptype )
//GmPDSProcessHelper::GmPDSProcessHelper(const G4String& name, G4SteppingManager* fpSM ): G4VDiscreteProcess( name )5~, fpSteppingManager(fpSM)
{
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
  if(ScoringVerb( testVerb) ) G4cout << " GmPDSProcessHelper::GmPDSProcessHelper " << this << G4endl;
#endif
#endif
  switch (the1aryType) {
  case PDSNeutron:
    theOriginalParticleName = "neutron";
    break;
  case PDSGamma:
    theOriginalParticleName = "gamma";
    break;
#ifdef PDS_OP
  case PDSOpticalPhoton:
    theOriginalParticleName = "opticalphoton";
    break;
#endif
  }

  // USE PARAMETER AND FILEINPATH
  theMinimumWeightForSplitting = log(GetPDSNumericParameter("MinimumWeightForSplitting",theOriginalParticleName,1.E-30));
  theMaximumWeightForSplitting = log(GetPDSNumericParameter("MaximumWeightForSplitting",theOriginalParticleName,1.E-5));
  theMaximumSplitting = GetPDSNumericParameter("GmPDS:MaximumSplitting",theOriginalParticleName,1000.);
  theSplittingFactor = log(theMaximumSplitting)/(theMaximumWeightForSplitting-theMinimumWeightForSplitting);

  theExclusionRadius = GetPDSNumericParameter("ExclusionRadius",theOriginalParticleName,1.*CLHEP::mm);

  bScoreTrueAndGeantino = G4bool(GetPDSNumericParameter("ScoreSeparatelyTrueAndGeantino",theOriginalParticleName,0));

  switch (the1aryType) {
  case PDSNeutron:
    StoreNeutronProcesses();
    ReadEnergyBinsForNeutrons();
    break;
  case PDSGamma:
    StoreGammaProcesses();
    ReadEnergyBinsForGammas();
    break;
#ifdef PDS_OP
  case PDSOpticalPhoton:
    StoreOpticalPhotonProcesses();
    ReadEnergyBinsForOpticalPhotons();
    break;
#endif
  }

  BuildEnergies();

  theNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();

  //  theEmCalculator = new G4EmCalculator;
  theClassifier = 0;

  bClassifierOnTrack= G4bool(GetPDSNumericParameter("ClassifierOnTrack",theOriginalParticleName,1)); 

}

//------------------------------------------------------------------
GmPDSProcessHelper::~GmPDSProcessHelper()
{
}

//------------------------------------------------------------------
void GmPDSProcessHelper::SetDetectors( std::map<G4int, GmPDSDetector*> dets )
{ 
  theDetectors = dets;
  std::map<G4int, GmPDSDetector*>::const_iterator itedet;
  for( itedet = theDetectors.begin(); itedet != theDetectors.end(); itedet++ ){
    theDetectorCentres[(*itedet).first] = (*itedet).second->GetCentrePoint();
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << "GmPDSProcessHelper::SetDetector det " << (*itedet).first << " " << (*itedet).second->GetName() << " centre = " << theDetectorCentres[(*itedet).first] << " detID " << (*itedet).second->GetID() << G4endl;
#endif
#endif
    theDetectorIDs[(*itedet).first] = (*itedet).second->GetID();
  }

}

//------------------------------------------------------------------
void GmPDSProcessHelper::BookHistos(G4int index)
{
  theIndices.insert(index);

  if( index == -1 ) BuildHistoNumber();
  // To the GmVHistoBuilder number it will be added
  // 100000* classification index
  // 50000*0 if geantino
  // 50000*1 if neutron
  // 100*   score number (0 for ALL, 1 for true, 2 for geantino) 
  // for 91,92,93  100* detector ID

  bControlHistos = G4bool( GetPDSNumericParameter("ControlHistograms",theOriginalParticleName,1) );
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(warningVerb) ) G4cout << "GmPDSProcessHelper::BookHistos " << index << " " << bControlHistos << G4endl;
#endif
#endif

  if( bControlHistos ) {
    //--- Histos of distance from each volume to detector 
    G4String hnam;
    G4int hDistNbins = 100;
    G4double hDistMin = 0.;
    G4double hDistMax = 1000.;
    G4int hLogWeiNbins = 100;
    G4double hLogWeiMin = -50.;
    G4double hLogWeiMax = 0.;
    G4int hLogEnerNBins = G4int(theEnergies.size());
    G4double hLogEnerMin = log10(*(theEnergies.begin()));
    std::set<G4double>::iterator itee = theEnergies.end();
    itee--;
    G4double hLogEnerMax = log10(*itee);
    G4int hEnerNBins = G4int(theEnergies.size());
    G4double hEnerMin = *(theEnergies.begin());
    G4double hEnerMax = *itee;

    //    G4cout << *(theEnergies.begin())  " PDS HIST " << hLogEnerNBins << " " << hLogEnerMin << " " << hLogEnerMax << G4endl; //GDEB
    G4int histoNumber = theHistoNumber;
    G4String PDIDstr = theOriginalParticleName + ": ";

    if( theClassifier ) {
      if( index != -1 ) {
	PDIDstr = theClassifier->GetIndexName(index) + ": " + PDIDstr;
      } else {
	PDIDstr = "ALL: " + PDIDstr;
      }
    }
    
    //filter ALL
    PDIDstr = "ALL: " + PDIDstr;

    histoNumber = theHistoNumber + 100000 * index;

    hnam = PDIDstr + G4String(" interaction log Energy (MeV)");
    theAnaMgr->CreateHisto1D(hnam,hLogEnerNBins,hLogEnerMin,hLogEnerMax,histoNumber+90);

    std::map<G4int,GmPDSDetector*>::const_iterator itedet;
    for( itedet = theDetectors.begin(); itedet != theDetectors.end(); itedet++ ){
      G4int detID = (*itedet).second->GetID();
      G4int histoNumber2 = theHistoNumber + 100000 * index + 100*detID;
      G4String PDIDstr2 = PDIDstr + " PD" + GmGenUtils::itoa(detID) + ": ";
      
      G4String hnamt = "interaction dist to detector (mm)";
      hnam = PDIDstr2 + hnamt;
      theAnaMgr->CreateHisto1D(hnam,hDistNbins,hDistMin,hDistMax,histoNumber2+91);
      hnam += " vs weight";
      theAnaMgr->CreateHisto2D(hnam,hDistNbins,hDistMin,hDistMax,hLogWeiNbins,hLogWeiMin,hLogWeiMax,histoNumber2+92);
      hnam = PDIDstr2 + hnamt + " weighted by Hstar";
      theAnaMgr->CreateHisto2D(hnam,hDistNbins,hDistMin,hDistMax,hLogWeiNbins,hLogWeiMin,hLogWeiMax,histoNumber2+93);
    
      //----- Histos for particles reaching detector
      G4String hnam0;
      std::map<G4String,GmPDSScore*>::const_iterator itet;
      G4int ii = 0;
      for( itet = theScores->begin(); itet != theScores->end(); itet++, ii++ ) {
	if( ((*itet).second)->GetDetID() != detID ) continue;
	G4String scoreName = (*itet).first;
	if( theScorerHistos.find(scoreName) != theScorerHistos.end() ) continue; // only the new scorers corresponding to the new index
	theScorerHistos.insert( (*itet).first );

	G4int scoreID = ((*itet).second)->GetID();
	if( scoreID >= 500 ) {
	  G4Exception("GmPDSProcessHelper::BookHistos",
		      "More than 500 scores, not enough histograms",
		      JustWarning,
		      "If you really need so many scores, please contact the GAMOS authors");
	} 

	for( size_t kk = 0; kk < 2; kk++ ){ 
	  G4int histoNumber3 = theHistoNumber + index*100000 + kk*50000 + scoreID*100;
	  if( kk == 0 ) {
	    hnam0 = (*itet).first + " At detector : geantino : ";  //histograms for pseudo geantinos 
	  } else if( kk == 1 ) {
	    hnam0 = (*itet).first+ " At detector : " + theOriginalParticleName + " : "; //histograms for neutrons
	  } 
	  hnam = hnam0 + G4String("Energy (MeV)");
	  theAnaMgr->CreateHisto1D(hnam,hLogEnerNBins,hEnerMin,hEnerMax,histoNumber3+1);
	  hnam = hnam0 + G4String("Energy no weighted (MeV)");
	  theAnaMgr->CreateHisto1D(hnam,hEnerNBins,hEnerMin,hEnerMax,histoNumber3+2);
	  hnam = hnam0 + G4String("Energy weighted by Hstar");
	  theAnaMgr->CreateHisto1D(hnam,hEnerNBins,hEnerMin,hEnerMax,histoNumber3+3);

	  hnam = hnam0 + G4String("log10(energy) (MeV)");
	  theAnaMgr->CreateHisto1D(hnam,hLogEnerNBins,hLogEnerMin,hLogEnerMax,histoNumber3+11);
	  hnam = hnam0 + G4String("log10(energy) no weighted (MeV)");
	  theAnaMgr->CreateHisto1D(hnam,hLogEnerNBins,hLogEnerMin,hLogEnerMax,histoNumber3+12);
	  hnam = hnam0 + G4String("log10(energy) weighted by Hstar");
	  theAnaMgr->CreateHisto1D(hnam,hLogEnerNBins,hLogEnerMin,hLogEnerMax,histoNumber3+13);
	  
	  hnam = hnam0 + G4String("log10(weight)");
	  theAnaMgr->CreateHisto1D(hnam,hLogWeiNbins,hLogWeiMin,hLogWeiMax,histoNumber3+21);
	  hnam = hnam0 + G4String("log10(weight) weighted by Hstar");
	  theAnaMgr->CreateHisto1D(hnam,hLogWeiNbins,hLogWeiMin,hLogWeiMax,histoNumber3+22);
	  hnam = hnam0 + G4String("log10(energy) vs log10(weight)");
	  theAnaMgr->CreateHisto2D(hnam,hLogEnerNBins,hLogEnerMin,hLogEnerMax,hLogWeiNbins,hLogWeiMin,hLogWeiMax,histoNumber3+31);
	}
      }
    }
  }
}


//------------------------------------------------------------------
void GmPDSProcessHelper::InitScores(G4int index)
{
  std::map<G4int,GmPDSDetector*>::const_iterator itedet;
  for( itedet = theDetectors.begin(); itedet != theDetectors.end(); itedet++ ){
    G4int detID = (*itedet).second->GetID();
    G4String PDIDstr = theOriginalParticleName + ": PD" + GmGenUtils::itoa(detID) + ": ";

    std::vector<GmVFilter*>::const_iterator itef;
    G4String name2;
    if( theFilters.size() == 0 ) name2 = "ALL";
    for( itef = theFilters.begin(); itef != theFilters.end(); itef++ ) {
      if( name2 != "" ) name2 += "_";
      name2 += (*itef)->GetName();
    }
    name2 += ": ";

    if( theClassifier ) {
      if( index != -1 ) {
	name2 += theClassifier->GetIndexName(index);
      } else {
	name2 += "ALL";
      }
    } else {
      name2 += "ALL";
    }
    PDIDstr = name2 + ": " + PDIDstr;

    AddScore(PDIDstr+"ALL:", detID);

    if( bScoreTrueAndGeantino ) {
      AddScore(PDIDstr+theOriginalParticleName+":", detID);
      AddScore(PDIDstr+"geantino:", detID);
    }

  }  
}


//------------------------------------------------------------------
void GmPDSProcessHelper::ReadEnergyBinsForNeutrons()
{
  G4String flux2DoseFile = GetPDSStringParameter("Flux2DoseFileName","neutron","Flux2Dose.neutron.ICRU57.lis");

  //  std::set<float>* energyBins = new std::set<float>;
  flux2DoseFile = GmGenUtils::FileInPath( flux2DoseFile );
  GmFileIn fin = GmFileIn::GetInstance(flux2DoseFile);
  std::vector<G4String> wl;
  for( ;; ){
    if(! fin.GetWordsInLine( wl ) ) break;
    if( wl.size() != 8 ) {
      G4Exception("GmPDSProcessHelper::ReadEnergyBinsForNeutrons",
		  G4String("Wrong number of words in file "+flux2DoseFile + " line number = " + GmGenUtils::itoa(fin.Nline()\
												     ).c_str()),
                  FatalErrorInArgument,
                  G4String("There must be 8 words, there are "+ GmGenUtils::itoa(wl.size())).c_str());
    }
    Flux2Dose f2d;
    f2d.Hstar = GmGenUtils::GetValue(wl[1]);
    f2d.Hp0 = GmGenUtils::GetValue(wl[2]);
    f2d.Hp15 = GmGenUtils::GetValue(wl[3]);
    f2d.Hp30 = GmGenUtils::GetValue(wl[4]);
    f2d.Hp45 = GmGenUtils::GetValue(wl[5]);
    f2d.Hp60 = GmGenUtils::GetValue(wl[6]);
    f2d.Hp75 = GmGenUtils::GetValue(wl[7]);
    theFlux2Dose[GmGenUtils::GetValue(wl[0])] = f2d;
  }

  fin.Close();

}


//------------------------------------------------------------------
void GmPDSProcessHelper::ReadEnergyBinsForGammas()
{
  G4String flux2DoseFile = GetPDSStringParameter("Flux2DoseFileName","gamma","Flux2Dose.gamma.ICRU57.lis");

  //  std::set<float>* energyBins = new std::set<float>;
  flux2DoseFile = GmGenUtils::FileInPath( flux2DoseFile );
  GmFileIn fin = GmFileIn::GetInstance(flux2DoseFile);
  std::vector<G4String> wl;
  for( ;; ){
    if(! fin.GetWordsInLine( wl ) ) break;
    if( wl.size() != 6 ) {
      G4Exception("GmPDSProcessHelper::ReadEnergyBinsForGammas",
		  G4String("Wrong number of words in file "+flux2DoseFile + " line number = " + GmGenUtils::itoa(fin.Nline()\
												     ).c_str()),
                  FatalErrorInArgument,
                  G4String("There must be 6 words, there are "+ GmGenUtils::itoa(wl.size())).c_str());
    }
    Flux2Dose f2d;
    f2d.Hstar = GmGenUtils::GetValue(wl[1]);
    f2d.Hp0 = 0.;
    f2d.Hp15 = 0.;
    f2d.Hp30 = 0.;
    f2d.Hp45 = 0.;
    f2d.Hp60 = 0.;
    f2d.Hp75 = 0.;
    theFlux2Dose[GmGenUtils::GetValue(wl[0])] = f2d;
  }

  fin.Close();

}

#ifdef PDS_OP
//------------------------------------------------------------------
void GmPDSProcessHelper::ReadEnergyBinsForOpticalPhotons()
{
  G4String flux2DoseFile = GetPDSStringParameter("Flux2DoseFileName","opticalphoton","Flux2Dose.opticalphoton.ICRU57.lis");

  //  std::set<float>* energyBins = new std::set<float>;
  flux2DoseFile = GmGenUtils::FileInPath( flux2DoseFile );
  GmFileIn fin = GmFileIn::GetInstance(flux2DoseFile);
  std::vector<G4String> wl;
  for( ;; ){
    if(! fin.GetWordsInLine( wl ) ) break;
    if( wl.size() != 6 ) {
      G4Exception("GmPDSProcessHelper::ReadEnergyBinsForOpticalPhotons",
		  G4String("Wrong number of words in file "+flux2DoseFile + " line number = " + GmGenUtils::itoa(fin.Nline()\
												     ).c_str()),
                  FatalErrorInArgument,
                  G4String("There must be 6 words, there are "+ GmGenUtils::itoa(wl.size())).c_str());
    }
    Flux2Dose f2d;
    f2d.Hstar = GmGenUtils::GetValue(wl[1]);
    f2d.Hp0 = 0.;
    f2d.Hp15 = 0.;
    f2d.Hp30 = 0.;
    f2d.Hp45 = 0.;
    f2d.Hp60 = 0.;
    f2d.Hp75 = 0.;
    theFlux2Dose[GmGenUtils::GetValue(wl[0])] = f2d;
  }

  fin.Close();

}
#endif

//------------------------------------------------------------------
void GmPDSProcessHelper::BuildEnergies()
{

  G4String energiesFile = GetPDSStringParameter("EnergiesFileName",theOriginalParticleName,"Flux2Dose.neutron.ICRU57.lis");
  G4String flux2DoseFile = GetPDSStringParameter("Flux2DoseFileName",theOriginalParticleName,"Flux2Dose.neutron.ICRU57.lis");

  //--- If two files are equal use Flux2Dose, else read file
  if( energiesFile == flux2DoseFile ) {
    if( theFlux2Dose.size() == 0 ) {
      G4Exception("GmPDSProcessHelper::BuildEnergies",
                  "Flux2Dose list is empty",
                  FatalException,
                  G4String("There must be some problem reading file " + flux2DoseFile ).c_str());
    }
    std::map<G4double,Flux2Dose>::const_iterator ite, ite2;
    ite2 = theFlux2Dose.begin();
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << "GmPDSProcessHelper::BuildEnergiesSetFlux2Dose  " << theFlux2Dose.size() << G4endl;
#endif
#endif
    for( ite = theFlux2Dose.begin(); ite != theFlux2Dose.end(); ite++ ){
		 ite2++;
      // logarithmic interpolation with next value
      if( ite2 != theFlux2Dose.end() ) {
       G4double ener = exp( log ( (*ite).first * (*ite2).first ) / 2. );
       theEnergies.insert( ener );
      } else {
        theEnergies.insert( (*ite).first );
      }
    }
       
  } else {
    energiesFile = GmGenUtils::FileInPath( energiesFile );
    GmFileIn fin = GmFileIn::GetInstance(energiesFile);
    std::vector<G4String> wl;
    for( ;; ){
      if(! fin.GetWordsInLine( wl ) ) break;
      theEnergies.insert( GmGenUtils::GetValue(wl[0]) );
    }
  }

}


//------------------------------------------------------------------
void GmPDSProcessHelper::AddScore(const G4String& name, G4int detID )
{
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout <<"  GmPDSProcessHelper::AddScore " << name << " detID " << detID << G4endl;
#endif
#endif
  (*theScores)[name] = new GmPDSScore(name, detID, theEnergies);
}


//------------------------------------------------------------------
G4int GmPDSProcessHelper::DetectorReached(const G4Step& aStep)
{
  //  const G4Track* aTrack = aStep.GetTrack();
  //  G4Track* aTracknc = const_cast<G4Track*>(aTrack);
  //---- Check if detector has been reached (should be done with a scoring in the future)
  if( ! aStep.GetPostStepPoint()->GetTouchable()->GetVolume() ) return -1; // OutOfWorld

  /*  GmTouchable touch( aStep.GetPostStepPoint()->GetTouchable() );
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout <<"  GmPDSProcessHelper::CheckIfDetectorReached " << touch.GetLongName() << " =? " << theDetectors[touch.GetCopyNo()]->GetName() << G4endl;
#endif
  //  if( touch.GetLongName() == theDetector->GetName()  || aTrack->GetNextVolume()->GetName() == "sphDBlock" ){
  if( touch.GetLongName() == theDetector->GetName() ){
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout <<"  GmPDSProcessHelper::CheckIfDetectorReached return TRUE " << G4endl;
#endif
    return touch.GetCopyNo(); */

  G4VPhysicalVolume* pv = aStep.GetPostStepPoint()->GetPhysicalVolume();
  //  G4cout << " PV " << pv << " " << pv->GetCopyNo() << " size " << theDetectors.size() << G4endl;
  //  G4cout << " PV " << pv << " " << pv->GetName() << " " << pv->GetCopyNo() << " size " << G4endl;
  GmTouchable touch( aStep.GetPostStepPoint()->GetTouchable() );
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
  //  if( ScoringVerb(debugVerb) ) G4cout <<"  GmPDSProcessHelper::DetectorReached " << touch.GetLongName() << G4endl;
#endif
#endif

  //  if( touch.GetLongName() == theDetector->GetName()  || aTrack->GetNextVolume()->GetName() == "sphDBlock" ){
  //  G4cout <<this << " DetectorReached " << (*(theDetectors.begin())).second << " " << theDetectors[1] << " " << theDetectors.size() <<  G4endl;
  if( pv->GetName() == (*theDetectors.begin()).second->GetName() ){ // all detectors have the same name
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
    //    if( ScoringVerb(debugVerb) ) G4cout <<"  GmPDSProcessHelper::DetectorReached " << pv->GetName() << " =? " << theDetectors[pv->GetCopyNo()]->GetName() << G4endl;
    if( ScoringVerb(debugVerb) ) G4cout <<"  GmPDSProcessHelper::DetectorReached returns TRUE " << G4endl;
#endif
#endif
    //-    G4cout << "PD reached " << G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID() << " " << aStep.GetTrack()->GetTrackID() << " " << aStep.GetTrack()->GetParentID() << G4endl;
    return pv->GetCopyNo(); 
  } else {
    return -1;
  }

}


//------------------------------------------------------------------
void GmPDSProcessHelper::FillScores( const G4Track& aTrack, G4bool bGeantino,G4int detID )
{
  G4double ener = aTrack.GetKineticEnergy()/CLHEP::MeV;
  if( ener == 0. ) return;

  G4double wei = GetWeight(&aTrack);

  //  G4cout << "IN DETECTOR " << ener << " " << aTracknc->GetTrackLength() << " " << wei << G4endl;

  std::set<G4double>::const_iterator itee = theEnergies.upper_bound(ener);

  std::set<G4double>::difference_type diffs = std::distance(theEnergies.begin(), itee);
  size_t enerID = G4int(diffs);
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << " GmPDSProcessHelper::FillScores ener " << ener << " set to energy bin " << (*itee) << " enerID " << enerID << G4endl;
#endif
#endif

  Flux2Dose flux2Dose = GetFlux2Dose( ener );
 
  G4String PDIDstr = theOriginalParticleName + ": PD" + GmGenUtils::itoa(detID) + ": ";
  G4int index = -1;
  G4String PDIDstr2; 

  G4String name2; 
  if( theFilters.size() == 0 ) name2 = "ALL";
  std::vector<GmVFilter*>::const_iterator itef;
  for( itef = theFilters.begin(); itef != theFilters.end(); itef++ ) {
    if( name2 != "" ) name2 += "_";
    name2 += (*itef)->GetName();
  }
  name2 += ": ";

  for( G4int ii = 0; ii < 2; ii ++ ) {
    if( ii == 0 || theClassifier ) {
      if( ii == 0 ) {
	PDIDstr2 = name2 + "ALL: " + PDIDstr;
      } else {
	if( theClassifier ) {
	  if( bGeantino ) {
	    index = ( (GmTrackInfo*)(aTrack.GetUserInformation()) )->GetIntValue("ClassifierIndex");
	  } else {
	    index = theClassifier->GetIndexFromTrack(&aTrack);
	    //	    G4cout << " INDEX FROM TRACK " << index << G4endl;
	  }
	  
	  CheckNewIndex(index);

	  PDIDstr2 = name2 + theClassifier->GetIndexName(index) + ": " + PDIDstr;
	} else {
	  PDIDstr2 = name2 + "ALL: " + PDIDstr;
	}

      }

      (*theScores)[PDIDstr2+"ALL:"]->FillEnergy( wei, flux2Dose, enerID );
      if( bControlHistos ) {
	FillControlHistos(ener, wei, bGeantino, (*theScores)[PDIDstr2+"ALL:"],flux2Dose, index);
      }

      if( bScoreTrueAndGeantino ) {
	if( bGeantino ) {
	  (*theScores)[PDIDstr2+"geantino:"]->FillEnergy( wei, flux2Dose, enerID );
	  if( bControlHistos ) {
	    FillControlHistos(ener, wei, bGeantino, (*theScores)[PDIDstr2+"geantino:"], flux2Dose, index);
	  }
	} else {
	  (*theScores)[PDIDstr2+theOriginalParticleName+":"]->FillEnergy( wei, flux2Dose, enerID );
	  if( bControlHistos ) {
	    FillControlHistos(ener, wei, bGeantino, (*theScores)[PDIDstr2+theOriginalParticleName+":"], flux2Dose, index);
	  }
	}
      }
    }
  }
  
}


//------------------------------------------------------------------
Flux2Dose GmPDSProcessHelper::GetFlux2Dose( G4double ener )
{
  // it should be an extrapolation of order 4th
  std::map<G4double,Flux2Dose>::const_iterator itee = theFlux2Dose.upper_bound(ener);
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << " GmPDSProcessHelper::GetFlux2DoseID ener " << ener << " flux2dose Hstar " << (*itee).second.Hstar << G4endl;
#endif
#endif

  return (*itee).second;

}


//------------------------------------------------------------------
void GmPDSProcessHelper::FillControlHistos( G4double ener, G4double wei, G4bool bGeantino, GmPDSScore* score, Flux2Dose flux2Dose, G4int index )
{
  // Fill histograms
  G4int nh = theHistoNumber + index*100000 + !bGeantino*50000 + score->GetID()*100;
  theAnaMgr->GetHisto1(nh+1)->Fill(ener,wei);
  theAnaMgr->GetHisto1(nh+2)->Fill(ener);
  theAnaMgr->GetHisto1(nh+11)->Fill(log10(ener),wei);
  theAnaMgr->GetHisto1(nh+12)->Fill(log10(ener));
  G4double hstar = flux2Dose.Hstar;
  theAnaMgr->GetHisto1(nh+3)->Fill(ener,wei*hstar);
  theAnaMgr->GetHisto1(nh+13)->Fill(log10(ener),wei*hstar);
  theAnaMgr->GetHisto1(nh+21)->Fill(log10(wei));
  theAnaMgr->GetHisto1(nh+22)->Fill(log10(wei),wei*hstar);
  theAnaMgr->GetHisto2(nh+31)->Fill(log10(ener),log10(wei));
  //  G4int evtid = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
  //  theAnaMgr->GetHisto2(nh+12)->Fill(log10(ener),float(evtid));
  //  theAnaMgr->GetHisto2(nh+13)->Fill(log10(wei),float(evtid));

#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << " GmPDSProcessHelper::FillScore1Set log10(ener) " << log10(ener) << " ener " << ener << " weight " << wei << G4endl;
#endif
#endif

}


//------------------------------------------------------------------
void GmPDSProcessHelper::PrintResults( std::ostream& out )
{
  std::map<G4String,GmPDSScore*>::const_iterator itet;
  for( itet = theScores->begin(); itet != theScores->end(); itet++ ) {
    ((*itet).second)->PrintResults( out, theDetectors[(*itet).second->GetDetID()] );
  }

}


//------------------------------------------------------------------
void GmPDSProcessHelper::StoreNeutronProcesses()
{
  G4ProcessManager* pmanager = G4Neutron::Neutron()->GetProcessManager();
  G4ProcessVector* pvect = pmanager->GetProcessList();
  int jj, sizproc = pvect->size();
  for( jj = 0; jj < sizproc; jj++ ) {
    G4VProcess* proc = (*pvect)[jj];
    G4HadronicProcess* prochad = dynamic_cast<G4HadronicProcess*>(proc);
    if( prochad ) {
      theNeutronProcesses[prochad->GetProcessName()] = prochad;
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(infoVerb) ) G4cout << "GmPDSGeantinoProcess::StoreNeutronProcesses proc= " << prochad->GetProcessName() << G4endl;     
#endif
#endif
    }
  }
}

//------------------------------------------------------------------
void GmPDSProcessHelper::StoreGammaProcesses()
{
  G4ProcessManager* pmanager = G4Gamma::Gamma()->GetProcessManager();
  G4ProcessVector* pvect = pmanager->GetProcessList();
  int jj, sizproc = pvect->size();
  for( jj = 0; jj < sizproc; jj++ ) {
    G4VProcess* proc = (*pvect)[jj];
    if( proc->GetProcessType() == fElectromagnetic ) {
      G4VEmProcess* procEM = (G4VEmProcess*)(proc);
      theGammaProcesses[proc->GetProcessName()] = procEM;
      //    theGammaProcesses.push_back(proc->GetProcessName());
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(infoVerb) ) G4cout << "GmPDSGeantinoProcess::StoreGammaProcesses proc= " << proc->GetProcessName() << G4endl;
#endif
#endif
    }
  }
}

//------------------------------------------------------------------
void GmPDSProcessHelper::StoreOpticalPhotonProcesses()
{
  G4ProcessManager* pmanager = G4OpticalPhoton::OpticalPhoton()->GetProcessManager();
  G4ProcessVector* pvect = pmanager->GetProcessList();
  int jj, sizproc = pvect->size();
  for( jj = 0; jj < sizproc; jj++ ) {
#ifdef PDS_OP
    G4VProcess* proc = (*pvect)[jj];
    if( proc->GetProcessType() == fElectromagnetic ) {
      G4VOpDiscreteProcess* procDis = dynamic_cast<G4VOpDiscreteProcess*>(proc);
      if( ! procDis ) continue;
      theOpticalPhotonProcesses[proc->GetProcessName()] = procDis;
      //    theOpticalPhotonProcesses.push_back(proc->GetProcessName());
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(infoVerb) ) G4cout << "GmPDSGeantinoProcess::StoreOpticalPhotonProcesses proc= " << proc->GetProcessName() << G4endl;
#endif
#endif
    }
#endif
  }

}


//------------------------------------------------------------------
G4double GmPDSProcessHelper::GetCrossSection( const G4Step* aStep )
{
  switch (the1aryType) {
  case PDSNeutron:
    return GetCrossSectionForNeutron( aStep );
    break;
  case PDSGamma:
    return GetCrossSectionForGamma( aStep );
    break;
#ifdef PDS_OP
  case PDSOpticalPhoton:
    return GetCrossSectionForOpticalPhoton( aStep );
    break;
#endif
  default:
    return 0.;
  }

  
}

//------------------------------------------------------------------
G4double GmPDSProcessHelper::GetCrossSectionForNeutron( const G4Step* aStep )
{
  const G4Track* aTrack = aStep->GetTrack();
  G4Track aTrackNeutron ( *aTrack );
  G4DynamicParticle* dpart = const_cast<G4DynamicParticle*>(aTrackNeutron.GetDynamicParticle());
  dpart->SetDefinition(G4Neutron::Neutron());
  aTrackNeutron.SetStep( aStep );

  std::map<G4String,G4HadronicProcess*>::const_iterator iten;

  /*  G4double invmfp = 0.;

  //-  G4ParticleDefinition* partNeutron = G4Neutron::Neutron();
  //--- Get the total mean free path by adding the microscopic cross sections (1./mfp)
  G4ForceCondition* cond = new G4ForceCondition;
  for( iten = theNeutronProcesses.begin(); iten != theNeutronProcesses.end(); iten++ ){
    G4double mfpt = ((*iten).second)->GetMeanFreePath(aTrackNeutron,0.,cond );
    //-    (*iten).second->GetCrossSectionDataStore()->DumpPhysicsTable( *partNeutron );
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << "GmPDSProcessHelper::GetCrossSectionForNeutron  MeanFreePath = " << mfpt << " XS " << 1./mfpt << " process " << (*iten).first << " E= " << aTrackNeutron.GetKineticEnergy() << " MATE= " << aTrackNeutron.GetMaterial()->GetName() << G4endl;
#endif
    invmfp += 1. / mfpt;
  }
  delete cond;
  */
 
  // Get microscopic XS
  G4Material* aMaterial = aTrackNeutron.GetMaterial();
  G4int nElements = aMaterial->GetNumberOfElements();
  const G4DynamicParticle * dynParticle = aTrackNeutron.GetDynamicParticle();
  G4double xSection = 0.;
  G4double xSectionAll = 0.;
  const G4double* atomsPerVolumeVector = aMaterial->GetVecNbOfAtomsPerVolume();
  //  const G4double* atomFractions = aMaterial->GetFractionVector();
  for( iten = theNeutronProcesses.begin(); iten != theNeutronProcesses.end(); iten++ ){
    xSection = 0.;
    for( G4int ii=0; ii<nElements; ii++ ) {
      G4double mcs = ((*iten).second)->GetMicroscopicCrossSection( dynParticle, (*aMaterial->GetElementVector())[ii], aMaterial);
      xSection += mcs * atomsPerVolumeVector[ii];
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
      if( ScoringVerb(debugVerb) ) G4cout << "GmPDSProcessHelper::GetCrossSectionForNeutron ELEMENT " << ii << " MicroscopicXS = " << mcs << " process " << (*iten).first << " E= " << aTrackNeutron.GetKineticEnergy() << " MATE= " << aTrackNeutron.GetMaterial()->GetName() << G4endl;
#endif
#endif
    }
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
    if( ScoringVerb(debugVerb) ) G4cout << "GmPDSProcessHelper::GetCrossSectionForNeutron  MicroscopicXS = " << xSection << " process " << (*iten).first << " E= " << aTrackNeutron.GetKineticEnergy() << " MATE= " << aTrackNeutron.GetMaterial()->GetName() << G4endl;
#endif
#endif
    xSectionAll += xSection;
  }


  // G4cout << " INVMFP " << invmfp << " XS " << xSectionAll << G4endl;
  
  //  return invmfp;
  return xSectionAll;
}

//------------------------------------------------------------------
G4double GmPDSProcessHelper::GetCrossSectionForGamma( const G4Step* aStep )
{
  const G4Track* aTrack = aStep->GetTrack();

  G4double energy = aStep->GetPreStepPoint()->GetKineticEnergy();
  G4LogicalVolume* lv = aTrack->GetVolume()->GetLogicalVolume();
  const G4MaterialCutsCouple* cutsC = lv->GetMaterialCutsCouple();
  //  G4Region* region = lv->GetRegion();

  G4double XStot = 0.;
  std::map<G4String,G4VEmProcess*>::const_iterator iteg;
  //  std::vector<G4String>::const_iterator iteg;
  for( iteg = theGammaProcesses.begin(); iteg != theGammaProcesses.end(); iteg++ ){
    //    G4double XS = theEmCalculator->GetCrossSectionPerVolume(energy, G4Gamma::Gamma(), (*iteg).first, aMaterial, region ); // it returns 0. for phot
    G4double XS = (*iteg).second->CrossSectionPerVolume( energy, cutsC );
    XStot += XS;
    //      G4cout << "GmPDSProcessHelper::GetCrossSectionForGamma  CrossSection EM = " << procEM->CrossSectionPerVolume( energy, lv->GetMaterialCutsCouple() ) 
    //    << " ener " << energy 
    //     << " mate " << lv->GetMaterial()->GetName() << " process " << (*iteg).first << G4endl;
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << "GmPDSProcessHelper::GetCrossSectionForGamma  CrossSection = " << XS << " ener " << energy << " mate " << lv->GetMaterial()->GetName() << " process " << (*iteg).first << G4endl;
#endif
#endif

  }
  
  return XStot;
}

#ifdef PDS_OP
//------------------------------------------------------------------
G4double GmPDSProcessHelper::GetCrossSectionForOpticalPhoton( const G4Step* aStep )
{
  /*const G4Track* aTrack = aStep->GetTrack();
  G4double energy = aStep->GetPreStepPoint()->GetKineticEnergy();
  G4LogicalVolume* lv = aTrack->GetVolume()->GetLogicalVolume();
  const G4MaterialCutsCouple* cutsC = lv->GetMaterialCutsCouple();
   G4Region* region = lv->GetRegion();
  */
  
  G4double XStot = 0.;
  G4ForceCondition* dummyFC = 0;
  std::map<G4String,G4VOpDiscreteProcess*>::const_iterator iteg;
  //  std::vector<G4String>::const_iterator iteg;
  for( iteg = theOpticalPhotonProcesses.begin(); iteg != theOpticalPhotonProcesses.end(); iteg++ ){
    //    G4double XS = theEmCalculator->GetCrossSectionPerVolume(energy, G4OpticalPhoton::OpticalPhoton(), (*iteg).first, aMaterial, region ); // it returns 0. for phot
    G4double XS = 1./(*iteg).second->GetMeanFreePath( *aTrack, 0, dummyFC );
    XStot += XS;
    //      G4cout << "GmPDSProcessHelper::GetCrossSectionForOpticalPhoton  CrossSection EM = " << procEM->CrossSectionPerVolume( energy, lv->GetMaterialCutsCouple() ) 
    //    << " ener " << energy 
    //     << " mate " << lv->GetMaterial()->GetName() << " process " << (*iteg).first << G4endl;
#ifndef WIN32
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) G4cout << "GmPDSProcessHelper::GetCrossSectionForOpticalPhoton  CrossSection = " << XS << " ener " << energy << " mate " << lv->GetMaterial()->GetName() << " process " << (*iteg).first << G4endl;
#endif
#endif
  }
  
  return XStot;
}
#endif

//------------------------------------------------------------------
G4double GmPDSProcessHelper::GetDistanceToDetectorIncm( const G4ThreeVector pos, G4int detID ) 
{
  return (theDetectorCentres[detID] - pos).mag()/CLHEP::cm;
  
}

//------------------------------------------------------------------
GmPDSScore* GmPDSProcessHelper::GetScore( G4String name )
{
  return (*theScores)[theOriginalParticleName+":"+name];
}

//------------------------------------------------------------------
void GmPDSProcessHelper::CheckNewIndex(G4int index)
{
  if( theIndices.find( index ) == theIndices.end() ){
    InitScores(index);
    BookHistos(index);
    theIndices.insert( index );
  }

}
