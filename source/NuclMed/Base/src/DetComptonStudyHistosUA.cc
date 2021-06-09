#include "DetComptonStudyHistosUA.hh"
#include "DetVComptAlgorithmVariable.hh"
#include "DetVComptClassificationVariable.hh"
#include "DetCClassifALL.hh"

#include "DetVerbosity.hh"

#include "GamosCore/GamosSD/include/GmRecHit.hh"
#include "GamosCore/GamosSD/include/GmHit.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosAnalysis/include/GmTrajStep.hh"
#include "GamosCore/GamosAnalysis/include/GmCheckOriginalGamma.hh"
#include "GamosCore/GamosUtils/include/GmNumberOfEvent.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"
#else
#include "DetComptAlgorithmVariableFactory.hh"
#include "DetComptClassificationVariableFactory.hh"
#endif

#include <iomanip>

//----------------------------------------------------------------------
DetComptonStudyHistosUA::DetComptonStudyHistosUA()
{
  BookHistos();
}

//----------------------------------------------------------------------
void DetComptonStudyHistosUA::BookHistos()
{
  theAnaMgr = GmAnalysisMgr::GetInstance("compton") ;
  BuildHistoNumber();

  std::vector<G4String> algoVars;
  algoVars = GmParameterMgr::GetInstance()->GetVStringValue("DetComptonStudyHistosUA:AlgorithmVariables",algoVars);
  for( unsigned int ii = 0; ii < algoVars.size(); ii++ ){
    G4String varName = "DetCAlgo"+algoVars[ii];
#ifdef ROOT5
    DetVComptAlgorithmVariable* algo = Reflex::PluginService::Create<DetVComptAlgorithmVariable*>(varName);
#else
    DetVComptAlgorithmVariable* algo = DetComptAlgorithmVariableFactory::get()->create(varName);
#endif
    if( algo == 0 ) {
      G4Exception("DetComptonStudyHistosUA::BookHistos",
		  "Wrong DetVComptAlgorithmVariable name",
		  FatalErrorInArgument,
		  G4String(varName+"Name= "+algoVars[ii]).c_str());
    }
    theCAlgoVars.push_back( algo );
    algo->BookHistos(theHistoNumber+(ii)*10000);
  }
		       
  std::vector<G4String> classifVars;
  classifVars = GmParameterMgr::GetInstance()->GetVStringValue("DetComptonStudyHistosUA:ClassificationVariables",classifVars);
  DetCClassifALL* classif = new DetCClassifALL;
  theCClassifVars.push_back( classif );
  classif->BookHistos(theHistoNumber+0*10000);
  for( unsigned int ii = 0; ii < classifVars.size(); ii++ ){
    G4String varName = "DetCClassif"+classifVars[ii];
#ifdef ROOT5
    DetVComptClassificationVariable* classif2 = Reflex::PluginService::Create<DetVComptClassificationVariable*>(varName );
#else
    DetVComptClassificationVariable* classif2 = DetComptClassificationVariableFactory::get()->create(varName);
#endif
    if( classif2 == 0 ) {
      G4Exception("DetComptonStudyHistosUA::BookHistos",
		  "Wrong DetVComptClassificationVariable name",
		  FatalErrorInArgument,
		  G4String(varName+"Name= "+classifVars[ii]).c_str());
    }
    theCClassifVars.push_back( classif );
    classif2->BookHistos(theHistoNumber+(ii+1)*10000);
  }

  /*  theCClassifVars.push_back( new DetCClassifNRecHit );
  theCClassifVars.push_back( new DetCClassifEnergyMin );
  theCClassifVars.push_back( new DetCClassifXPosMin );
  //  theCClassifVars.push_back( new DetCSHVarHitDistMin );
  //  theCClassifVars.push_back( new DetCSHVarHitDistAver );
  */

  BookOtherHistos();
}

//----------------------------------------------------------------------
void DetComptonStudyHistosUA::BookOtherHistos()
{
  std::string hnam;
  G4String separator = GmParameterMgr::GetInstance()->GetStringValue("Histos:Separator",":");
  std::string hgnam = "DetCompton" + separator + " ";
  hnam = hgnam + std::string("N gamma interactions per event");
  theAnaMgr->CreateHisto1D(hnam,20,0,20,theHistoNumber+11);
  hnam = hgnam + std::string("N rec hits per event");
  theAnaMgr->CreateHisto1D(hnam,20,0,20,theHistoNumber+12);
  hnam = hgnam + std::string("N gamma interactions - N rec hits per event");
  theAnaMgr->CreateHisto1D(hnam,30,-15,15,theHistoNumber+13);
  hnam = hgnam + std::string("N gamma interactions if N rec hits per event");
  theAnaMgr->CreateHisto1D(hnam,20,0,20,theHistoNumber+14);

}

//----------------------------------------------------------------------
DetComptonStudyHistosUA::~DetComptonStudyHistosUA()
{
}

//----------------------------------------------------------------------
void DetComptonStudyHistosUA::BeginOfEventAction(const G4Event* )
{
  mmits::const_iterator itemit;
  for( itemit = theInteractions.begin(); itemit != theInteractions.end(); itemit++ ){
    delete (*itemit).second;
  }
  theInteractions.clear();
}


//----------------------------------------------------------------------
void DetComptonStudyHistosUA::PreUserTrackingAction(const G4Track* aTrack)
{
  isOriginalGamma = GmCheckOriginalGamma::CheckIfOriginalGamma( aTrack );
}

//----------------------------------------------------------------------
void DetComptonStudyHistosUA::UserSteppingAction(const G4Step* aStep)
{ 
  if ( isOriginalGamma == 0) return;

  G4String procName = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
  //  G4cout << " procname " << procName << theComptonName << thePhotoElecName << theRayleighName << theConversionName << G4endl;
  if( procName == "compt" || 
      procName == "phot" ||
      //      procName == "Rayl" || 
      procName == "conv" ) {
    if( aStep->GetTrack()->GetVolume()->GetLogicalVolume()->GetSensitiveDetector() != 0 ) {
      GmTrajStep* ts = new GmTrajStep( aStep );
#ifndef GAMOS_NO_VERBOSE
      if( DetVerb(debugVerb) ) G4cout << " theInteractions " << aStep->GetTrack()->GetTrackID() << " angle " << ts->GetDeltaAngle()/CLHEP::deg << G4endl;
#endif
      theInteractions.insert(mmits::value_type(aStep->GetTrack()->GetTrackID(), ts ) );
         
      G4ThreeVector mompre = ts->GetMomentumPre();
      if( mompre.mag() != 0) mompre /= mompre.mag();
      G4ThreeVector mompost = ts->GetMomentumPost();
      if( mompost.mag() != 0) mompost /= mompost.mag();
      /*      G4double angle = acos( mompre*mompost ) / CLHEP::deg;
            G4cout << " INTERACTION " << std::setprecision(6) 
	     << GmNumberOfEvent::GetNumberOfEvent() << " " 
	     << aStep->GetTrack()->GetTrackID() 
	     << " Elost " << ts->GetDeltaKineticEnergy()/CLHEP::keV 
	     << " XYr " << ts->GetPositionPost().perp() 
	     << " angle " << angle 	     
	     << " " << ts->GetProcessName() << G4endl;
	     G4cout << " INTERACTION " << aStep->GetTrack()->GetTrackID() << " dir0  " << mompre << " dir1 " << mompost <<G4endl; */
    }
  }
}

//----------------------------------------------------------------------
void DetComptonStudyHistosUA::PostUserTrackingAction(const G4Track* aTrack)
{
  if ( isOriginalGamma == 0) return;
  //---- Plot histograms for interactions
  mmits::iterator itemm;
  std::pair<mmits::iterator,mmits::iterator> itemmp = theInteractions.equal_range( aTrack->GetTrackID() );


  std::vector<DetVComptAlgorithmVariable*>::const_iterator ite;
  G4int ii = 0;
  for( ite = theCAlgoVars.begin(); ite != theCAlgoVars.end(); ite++, ii++ ){
    (*ite)->FillHistosInteractions( itemmp, theHistoNumber+ii*10000 );
  }

  //--- Fill other histos
  if( theInteractions.size() ) theAnaMgr->GetHisto1(theHistoNumber+11)->Fill(theInteractions.size());

}


//----------------------------------------------------------------------
void DetComptonStudyHistosUA::FillDist511RecHits( DetRecHitSetList& rh511 )
{
  //--- first locate interaction corresponding to each rec hit
  DetRecHitSetList::iterator iterhs;
  std::set<GmRecHit*>::iterator iterh;
  mmits::iterator itemit;
  GmRecHit* rhitClosest;
  mmiterh rhitInterac;
  std::vector<std::pair<GmTrajStep*,GmRecHit*> > rhitInteracOrdered;

  for( iterhs = rh511.begin(); iterhs != rh511.end(); iterhs++ ){
    rhitInteracOrdered.clear();
    rhitInterac.clear();
    DetRecHitSet rhitset = *iterhs;
#ifndef GAMOS_NO_VERBOSE
      if( DetVerb(debugVerb) ) G4cout << " DetComptonStudyHistos::FillDist511RecHits looping rhit set size " << rhitset.size() << G4endl; 
#endif
    
    for( itemit = theInteractions.begin(); itemit != theInteractions.end(); itemit++ ){
      
      //--- Get closer rec hit to interaction = rec hit of same track and closer in distance
      G4double minDist = DBL_MAX;
      G4bool brhitClosest = false;
      
      for( iterh = rhitset.begin(); iterh != rhitset.end(); iterh++ ){ 
	GmRecHit* rhit = (*iterh);
	G4int rhitTrkID = GetTrackID( rhit );
	//--- Get original track ID of rechit == ID with higher number of GmEDepo'
  if( DetVerb(debugVerb) ) 	G4cout << " DetComptonStudyHistosUA::FillDist511RecHits rec hit track ID " << rhitTrkID << " interaction ID " << (*itemit).first << G4endl;	  
	
	if( (*itemit).first != rhitTrkID ) continue; // check if the interaction is of the same track ID than hit

	//	  if( DetVerb(debugVerb) ) G4cout  << " DetComptonStudyHistosUA::FillDist511RecHits N511 " << n511 << " ninter " <<theInteractions.size() << G4endl; 
	G4double dist = ( rhit->GetPosition()
			  - ((*itemit).second)->GetPositionPost()).mag();
	if( dist < minDist ) {	
#ifndef GAMOS_NO_VERBOSE
	  if( DetVerb(debugVerb) ) G4cout << " DetComptonStudyHistosUA::FillDist511RecHits dist to interaction " << rhit << " = " << dist << " minDist " << minDist << G4endl;
#endif
	  minDist = dist;
	  rhitClosest = rhit;
	  brhitClosest = true;
	} 
	
      }

      //---- Store rec hit associated to interaction (closest one)
      //      if( DetVerb(debugVerb) )   G4cout << " DetComptonStudyHistosUA::FillDist511RecHits rhitInterac " << rhit << " <-> " << (*itemitClosest).second << G4endl;	  
      //-      if( minDist == DBL_MAX ) continue;
      if( !brhitClosest ) continue;
#ifndef GAMOS_NO_VERBOSE
        if( DetVerb(debugVerb) ) G4cout  << std::distance( theInteractions.begin(), itemit ) << " DetComptonStudyHistosUA::FillDist511RecHits closest interaction pos " << (*itemit).second->GetPositionPost() << " <-> rhit pos " << rhitClosest->GetPosition() << G4endl;	  
        if( DetVerb(debugVerb) ) G4cout  << std::distance( theInteractions.begin(), itemit ) << " DetComptonStudyHistosUA::FillDist511RecHits closest interaction Energy " << (*itemit).second->GetDeltaKineticEnergy() << " <-> rhit pos " << rhitClosest->GetEnergy() << " " << (*itemit).second << G4endl;	  
#endif

      rhitInterac.insert(std::multimap<GmTrajStep*,GmRecHit*>::value_type((*itemit).second,rhitClosest)); 
      //trhitInterac.insert(std::multimap<mmits::iterator,GmRecHit*>::value_type(itemit,rhit)); 
      
    } // end loop to rec hits in this set
    
    AnalyseRecHitInteractionAssociation( rhitInterac, rhitset );
    
    //--- Fill other histos
    theAnaMgr->GetHisto1(theHistoNumber+12)->Fill(rhitset.size());
    theAnaMgr->GetHisto1(theHistoNumber+13)->Fill(theInteractions.size()-rhitset.size());
    theAnaMgr->GetHisto1(theHistoNumber+14)->Fill(theInteractions.size());

    //--- If two rechits share same track ID, get as first the one that is closer to interaction
    std::pair<mmiterh::iterator,mmiterh::iterator> itepair;
    mmiterh::iterator itemmi;
    //-- Loop to interactions
    for( itemit = theInteractions.begin(); itemit != theInteractions.end(); itemit++ ){
      std::map<G4double,mmiterh::iterator> orderDist;
      std::map<G4double,mmiterh::iterator>::iterator iteor;
      //-- Get list of rec hits associated to this interaction
      itepair = rhitInterac.equal_range((*itemit).second);
      //-- Order rec hits by distance to interaction
      for( itemmi = itepair.first; itemmi != itepair.second; itemmi++ ){
	G4double dist = ( (*itemmi).second->GetPosition()
			  - ((*itemmi).first)->GetPositionPost()).mag();
	//t			  - (*( ((*itemmi).first) )).second->GetPositionPost()).mag();
#ifndef GAMOS_NO_VERBOSE
	  if( DetVerb(debugVerb) ) G4cout  << " DetComptonStudyHistosUA::FillDist511RecHits rechits equal trkID " <<(*itemmi).first << " " << (*itemmi).second << " dist= " << dist << G4endl;
#endif
	orderDist[dist] = itemmi;
      }
      //--- Loop to rec hits ordered by distance and fill the final rhitInteracOrdered
      for( iteor = orderDist.begin(); iteor != orderDist.end();iteor++ ){
	mmiterh::iterator interrh = ((*iteor).second);
	rhitInteracOrdered.push_back( std::pair<GmTrajStep*,GmRecHit*>( 
		  //t (*((*interrh).first)).second,
			  ((*interrh).first), (*interrh).second ) );
      }
    }

    std::vector<DetVComptAlgorithmVariable*>::const_iterator ite;
    G4int ii = 0;
    //    G4cout << " SIZE nrechit " << rhitset.size() << " nrechitORd " << rhitInteracOrdered.size() << "inter " << theInteractions.size() << G4endl;
    for( ite = theCAlgoVars.begin(); ite != theCAlgoVars.end(); ite++, ii++ ){
      (*ite)->FillHistosRHits( rhitInteracOrdered, theHistoNumber+ii*10000 );
    }

  }

}


//----------------------------------------------------------------------
void DetComptonStudyHistosUA::AnalyseRecHitInteractionAssociation( mmiterh recHitInteraction, DetRecHitSet rhitset )
{
  unsigned int nRecHits = rhitset.size();  
  if( nRecHits > 3 ) nRecHits = 3;

  //----- Check if rhitset position corresponds to first interaction
  //--- Get rec hit from which the rhitset position was taken 
  GmRecHit* setPosRecHit = 0;
  G4ThreeVector setPos = rhitset.GetPosition();
  std::set<GmRecHit*>::iterator iterh;
  for( iterh = rhitset.begin(); iterh != rhitset.end(); iterh++ ){     
    GmRecHit* rhit = (*iterh);
    if( (rhit->GetPosition() - setPos).mag() < 1.E-9 ){
      setPosRecHit = rhit;
      break;
    }
  }
  if( !setPosRecHit ) {
#ifndef GAMOS_NO_VERBOSE
    if( DetVerb(debugVerb) ){
      G4cout << "DetComptonStudyHistosUA::AnalyseRecHitInteractionAssociation: no rech hit found whose pos sets the rhitset pos = " << rhitset.GetPosition() << G4endl;
      for( iterh = rhitset.begin(); iterh != rhitset.end(); iterh++ ){     
	GmRecHit* rhit = (*iterh);
	G4cout << " RHIT pos = " << rhit->GetPosition() << G4endl;
      }
    }
#endif
    return;
  }
  
#ifndef GAMOS_NO_VERBOSE
    if( DetVerb(debugVerb) ) G4cout   << "AnalyseRecHitInteractionAssociation: setPosRecHit pos " << setPosRecHit->GetPosition() << G4endl;
#endif

  //----- CSH variables: set classification index and fill histos of variable
  std::vector<DetVComptClassificationVariable*>::const_iterator ite;
  G4int ii = 0;
  for( ite = theCClassifVars.begin(); ite != theCClassifVars.end(); ite++, ii++ ){
    (*ite)->SetCurrentClassificationAndFillHistos( rhitset, theHistoNumber+ii*10000);
  }
  
  //----- Fill histos distance rec hit - interaction associated
  mmiterh::iterator itemmi;
  for( itemmi = recHitInteraction.begin(); itemmi != recHitInteraction.end(); itemmi++ ){
    //dist rec hit - interaction
    G4double dist = ((*itemmi).first->GetPositionPost() - (*itemmi).second->GetPosition() ).mag();
    
    for( ii = 0, ite = theCClassifVars.begin(); ite != theCClassifVars.end(); ite++, ii++ ){
      (*ite)->FillHisto(theHistoNumber+ii*10000+90,dist);
    }
    
  }
  
  //----- Get first interaction (of same track as rhitset)
  G4int rhitTrkID = GetTrackID( setPosRecHit );
  std::pair<mmits::iterator,mmits::iterator> itemmp = theInteractions.equal_range( rhitTrkID );
  if( itemmp.first == itemmp.second ) {
    G4Exception("DetComptonStudyHistosUA::AnalyseRecHitInteractionAssociation",
		"Gamma that correspond to the current rec hit set does not exist",
		JustWarning,
		"It is likely that you are accumulating hits from different events and the current rec hits set was formed from gammas of previous step. The DetComptonStudy histograms are not meant for the case of accumulating events");
    return;
  }
  GmTrajStep* inter1st = (*((itemmp).first)).second;
#ifndef GAMOS_NO_VERBOSE
    if( DetVerb(debugVerb) ) G4cout   << inter1st << " AnalyseRecHitInteractionAssociation: 1st interaction pos " << inter1st->GetPositionPost() << G4endl;
#endif
  
  //-- Get dist rec hit set - 1st interaction
  G4double dist21st = (inter1st->GetPositionPost() -
		       setPosRecHit->GetPosition()).mag();
  
  //--- Check if this rec hit has associated the first interaction
  std::pair<mmiterh::iterator,mmiterh::iterator> itepair
    = recHitInteraction.equal_range(inter1st);

  //--- Loop to list of rec hits associated to this interaction
  G4bool b1stAssocFound = FALSE;
  G4double minDist = dist21st;
  for( itemmi = itepair.first; itemmi != itepair.second; itemmi++ ){
#ifndef GAMOS_NO_VERBOSE
     if( DetVerb(debugVerb) )  G4cout   << "AnalyseRecHitInteractionAssociation: inter1st assoc to rechit " << (*itemmi).second->GetPosition() << " " << (*itemmi).second << " =? " << setPosRecHit << G4endl; 
#endif
    if( (*itemmi).second == setPosRecHit ){
      b1stAssocFound = TRUE;
    }
    G4double dist = (inter1st->GetPositionPost() - 
		     (*itemmi).second->GetPosition()).mag();
    //-- if there are more than 1 rec hits associated to 1st interaction, check if the closest one is the setPosRecHit
    if(dist < minDist ) {
#ifndef GAMOS_NO_VERBOSE
       if( DetVerb(debugVerb) )  G4cout   << "AnalyseRecHitInteractionAssociation: closer rec hit assoc to inter1st " << (*itemmi).second->GetPosition() << " dist " << dist << " minDist " << minDist << G4endl;
#endif
      minDist = dist;
    }
  }

  for( ii = 0, ite = theCClassifVars.begin(); ite != theCClassifVars.end(); ite++, ii++ ){
    (*ite)->FillHisto(theHistoNumber+ii*10000+91,dist21st);
  }

#ifndef GAMOS_NO_VERBOSE
    if( DetVerb(debugVerb) ) G4cout   << "AnalyseRecHitInteractionAssociation: rec hit pos N " << nRecHits << " " << b1stAssocFound << " " << dist21st << " " << minDist << G4endl;
#endif
  if( !b1stAssocFound ){
    for( ii = 0, ite = theCClassifVars.begin(); ite != theCClassifVars.end(); ite++, ii++ ){
      (*ite)->FillHisto(theHistoNumber+ii*10000+92,dist21st);
    }
  } else {
    for( ii = 0, ite = theCClassifVars.begin(); ite != theCClassifVars.end(); ite++, ii++ ){
      (*ite)->FillHisto(theHistoNumber+ii*10000+93,dist21st);
    }
  }

}


//----------------------------------------------------------------------
G4int DetComptonStudyHistosUA::GetTrackID( GmRecHit* rhit )
{
  G4int eventID = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
  G4int rhitTrkID;
  std::vector<GmHit*>::iterator iteh;
  std::set<G4int>::iterator itesi; 
  std::vector<GmHit*> hits = rhit->GetHits();
  std::map<G4int,G4int> idmap;
  for( iteh = hits.begin(); iteh != hits.end(); iteh++ ){
    if( (*iteh)->GetEventID() != eventID ) continue;
    std::set<G4int> htidori = (*iteh)->GetOriginalTrackIDs();
    for( itesi = htidori.begin(); itesi != htidori.end(); itesi++ ){
      idmap[(*itesi)] ++;	  
#ifndef GAMOS_NO_VERBOSE
     if( DetVerb(debugVerb) )  G4cout << " DetComptonStudyHistosUA::FillDist511RecHits adding track ID to hit " << rhit << " " << *iteh <<  " ID= " << *itesi << " nid= " << idmap[(*itesi)] << G4endl;	  
#endif
    }
  }
  std::map<G4int,G4int>::iterator iteid;
  G4int maxID = 0;
 
  for( iteid = idmap.begin(); iteid != idmap.end(); iteid++ ){
    if( (*iteid).second > maxID ) {
      maxID = (*iteid).second;
      rhitTrkID = (*iteid).first;
    }
  }

  return rhitTrkID;
}
