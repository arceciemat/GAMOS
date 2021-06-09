#include "PETEventClassifierTOFUA.hh"
#include "NuclMed/Base/include/DetV1stHitAlgorithm.hh"
#include "NuclMed/Base/include/DetCountScatteringUA.hh"
#include "PETIOMgrTOF.hh"
#include "NuclMed/Base/include/DetComptonStudyHistosUA.hh"
#include "PETVerbosity.hh"
#include "PETProjDataMgr.hh"

#include "GamosCore/GamosSD/include/GmRecHit.hh"
#include "GamosCore/GamosSD/include/GmHit.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmLine.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosSD/include/GmHitsEventMgr.hh"
#include "GamosCore/GamosUtils/include/GmCone.hh"
#include "GamosCore/GamosAnalysis/include/GmTrajStep.hh"
#include "GamosCore/GamosAnalysis/include/GmCheckOriginalGamma.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserActionMgr.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserEventActionList.hh"

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"
#else
#include "NuclMed/Base/include/Det1stHitAlgorithmFactory.hh"
#endif


#include <math.h>
using namespace CLHEP;

//----------------------------------------------------------------------
PETEventClassifierTOFUA::PETEventClassifierTOFUA()
{
  bDumpLM = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("PET:EvtClass:DumpEventListMode",0));
  if( bDumpLM ) PETIOMgrTOF::GetInstance()->OpenFileOut();

  // MCC 2011. Project Data (STIR or CIEMAT format: more details in PETProjDataMgr class)
  bDumpPD = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("PET:EvtClass:DumpProjData",0));

  BookHistos();

  G4double EPrec511 = GmParameterMgr::GetInstance()->GetNumericValue("PET:EvtClass:511EPrec",0.3);
  the511EPrecMin = GmParameterMgr::GetInstance()->GetNumericValue("PET:EvtClass:511EPrecMin",CLHEP::electron_mass_c2*(1-EPrec511));
  the511EPrecMax = GmParameterMgr::GetInstance()->GetNumericValue("PET:EvtClass:511EPrecMax",CLHEP::electron_mass_c2*(1+EPrec511));

#ifndef GAMOS_NO_VERBOSE
  if( PETVerb(infoVerb) ) G4cout << " the511EPrecMin " << the511EPrecMin << " the511EPrecMax " << the511EPrecMax << G4endl;
#endif
  theCoincidenceTime = GmParameterMgr::GetInstance()->GetNumericValue("PET:EvtClass:CoincidenceTime",DBL_MAX);

  thePETLineDistToVtx  = GmParameterMgr::GetInstance()->GetNumericValue("PET:EvtClass:LineDistToVtx",10.*CLHEP::mm);

  theComptonRecHitsDist  = GmParameterMgr::GetInstance()->GetNumericValue("PET:EvtClass:ComptonRecHitDist",0.*CLHEP::mm);

  G4String the1stHitStr = GmParameterMgr::GetInstance()->GetStringValue("PET:EvtClass:1stHitAlgorithm","Det1stHitByEnergy");
  G4String the1stHit1Str = GmParameterMgr::GetInstance()->GetStringValue("PET:EvtClass:1stHitAlgorithmFirst",the1stHitStr);
  G4String the1stHit2Str = GmParameterMgr::GetInstance()->GetStringValue("PET:EvtClass:1stHitAlgorithmSecond",the1stHitStr);
  
#ifdef ROOT5
  the1stHitAlgo1 = Reflex::PluginService::Create<DetV1stHitAlgorithm*>(the1stHit1Str);
#else
  the1stHitAlgo1 = Det1stHitAlgorithmFactory::get()->create(the1stHit1Str);
#endif
  if( !the1stHitAlgo1 ) { 
    G4Exception("PETEventClassifierTOFUA::PETEventClassifierTOFUA",
		"Wrong 1st hit algorithm",
		FatalErrorInArgument,
		"Check your parameter PET:EvtClass:1stHitAlgorithm or PET:EvtClass:1stHitAlgorithmFirst");
  }
#ifdef ROOT5
  the1stHitAlgo2 = Reflex::PluginService::Create<DetV1stHitAlgorithm*>(the1stHit2Str);
#else
  the1stHitAlgo2 = Det1stHitAlgorithmFactory::get()->create(the1stHit2Str);
#endif
  if( !the1stHitAlgo2 ) { 
    G4Exception("PETEventClassifierTOFUA::PETEventClassifierTOFUA",
		"Wrong 1st hit algorithm",
		FatalErrorInArgument,
		"Check your parameter PET:EvtClass:1stHitAlgorithm or PET:EvtClass:1stHitAlgorithmSecond");
  }

  theHitsEventMgr = GmHitsEventMgr::GetInstance();

  theCountScattering = new DetCountScatteringUA;

}


//----------------------------------------------------------------------
void PETEventClassifierTOFUA::BookHistos()
{
  theAnaMgr = GmAnalysisMgr::GetInstance("pet") ;

  std::string hnam;

  G4String sepa = GmParameterMgr::GetInstance()->GetStringValue("Histos:Separator",":");
  std::string hgnam = "PETEvtClass"+sepa+" ";
  hnam = hgnam + std::string("PET classification");
  theAnaMgr->CreateHisto1D(hnam,1200,0,1200.,320000+1);
  hnam = hgnam + std::string("PET dist line - vertex (mm)");
  theAnaMgr->CreateHisto1D(hnam,100,0,25.,320000+5);
  hnam = hgnam + std::string("PET dist line - vertex Z (mm)");
  theAnaMgr->CreateHisto1D(hnam,100,0,25.,320000+6);
  hnam = hgnam + std::string("PET dist line - vertex XY(mm)");
  theAnaMgr->CreateHisto1D(hnam,100,0,25.,320000+7);
  hnam = hgnam + std::string("PET RecHit energy (CLHEP::keV)");
  theAnaMgr->CreateHisto1D(hnam,600,0,600.,320000+8);
  hnam = hgnam + std::string("Extra PET RecHit energy (CLHEP::keV)");
  theAnaMgr->CreateHisto1D(hnam,600,0,600.,320000+9);
  hnam = hgnam + std::string("e+ position (X,Y)_ALL");
  theAnaMgr->CreateHisto2D(hnam,300,-200,200.,300,-200,1000,320000+58);
  hnam = hgnam + std::string("e+ position (X,Y)_PET Events");
  theAnaMgr->CreateHisto2D(hnam,300,-200,200.,300,-200,1000,320000+59);

   // DCA histograms for each subclassification type

  std::vector<std::string> str32(2);
  str32[0] = " 2";  
  str32[1] = " 3->2"; 
  std::vector<std::string> strrand(2);
  strrand[0] = " no_random";  
  strrand[1] = " random"; 
  std::vector<std::string> strscat(2);
  strscat[0] = " no_scattered";  
  strscat[1] = " scattered";
  std::vector<std::string> strfar(2);
  strfar[0] = " close_to_vtx";
  strfar[1] = " far_from_vtx";
 
  for( unsigned int i1 = 0; i1 < 2; i1++ ){ 
    for( unsigned int i2 = 0; i2 < 2; i2++ ){ 
      for( unsigned int i3 = 0; i3 < 2; i3++ ){ 
	for( unsigned int i4 = 0; i4 < 2; i4++ ){ 
	  hnam = hgnam + strfar[i1] + str32[i2] + strrand[i3] + strscat[i4] + 
	    G4String("PET dist line - vertex (mm)");
	  theAnaMgr->CreateHisto1D(hnam,100,0,25.,321000+i1+i2*10+i3*100+i4*1000);
	  hnam = hgnam + strfar[i1] + str32[i2] + strrand[i3] + strscat[i4] + 
	    "RecHit energy (keV)";
	  theAnaMgr->CreateHisto1D(hnam,600,0,600.,331000+i1+i2*10+i3*100+i4*1000);
	}
      }
    }
  }

  hnam = hgnam + " ALL far " + "PET dist line - vertex (mm)";
  theAnaMgr->CreateHisto1D(hnam,100,0,25.,341000+1);
  hnam = hgnam + " ALL far" + "RecHit energy (keV)";
  theAnaMgr->CreateHisto1D(hnam,600,0,600.,351000+1);
  hnam = hgnam + " ALL 3->2 " + "PET dist line - vertex (mm)";
  theAnaMgr->CreateHisto1D(hnam,100,0,25.,341000+2);
  hnam = hgnam + " ALL 3->2 " + "RecHit energy (keV)";
  theAnaMgr->CreateHisto1D(hnam,600,0,600.,351000+2);
  hnam = hgnam + " ALL random coincidences " + "PET dist line - vertex (mm)";
  theAnaMgr->CreateHisto1D(hnam,100,0,25.,341000+3);
  hnam = hgnam + " ALL random coincidences" + "RecHit energy (keV)";
  theAnaMgr->CreateHisto1D(hnam,600,0,600.,351000+3);
  hnam = hgnam + " ALL scattered " + "PET dist line - vertex (mm)";
  theAnaMgr->CreateHisto1D(hnam,100,0,25.,341000+4);
  hnam = hgnam + " ALL scattered" + "RecHit energy (keV)";
  theAnaMgr->CreateHisto1D(hnam,600,0,600.,351000+4);
  
}


//----------------------------------------------------------------------
void PETEventClassifierTOFUA::BeginOfRunAction(const G4Run* )
{
  bComptonStudyHistos = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("PET:EvtClass:ComptonStudyHistos",0));
  if( bComptonStudyHistos ) {
    theComptonStudyHistos = new DetComptonStudyHistosUA();
  }
}


//----------------------------------------------------------------------
PETEventClassifierTOFUA::~PETEventClassifierTOFUA()
{
}


//----------------------------------------------------------------------
int64_t PETEventClassifierTOFUA::Classify( gamosRecHitList& rhitList, const G4Event* evt  )
{
  //  int64_t clpet = ClassifyPET( rhitList, evt );
  ClassifyPET( rhitList, evt );

  return theEventClass;
}

//----------------------------------------------------------------------
int64_t PETEventClassifierTOFUA::ClassifyPET( gamosRecHitList& rhitList, const G4Event* evt )
{
  the511RecHits.clear();

#ifndef GAMOS_NO_VERBOSE
  if( PETVerb(infoVerb) ) G4cout << " PETEventClassifierTOFUA::ClassifyPET() n rechits " << rhitList.size() << G4endl;
#endif

  theEventClass = 0; //!! if no changed = no PET

  if( rhitList.size() == 0) return theEventClass;

  // --- Get time as the one from first hit in this event
  //  std::vector<GmHit*> hits = theHitsEventMgr->GetHitsInEvent(evt->GetEventID());
  //-  if( hits.size() == 0 ) return theEventClass; // no new hits, no classification is needed 
  //-  G4double coincTime = hits[0]->GetTime() - theCoincidenceTime;

  gamosRecHitList::const_iterator itecl1;
  G4int nrechit = 0;
  for( itecl1 = rhitList.begin(); itecl1 != rhitList.end(); itecl1++ ){
    //    GmRecHit* newRecHit = *itecl1;
    nrechit++;

  }
  
  Get511ComptonRecHits( rhitList );

  if( the511RecHits.size() == 2 ) {
    CheckTimeCoincidence(); //delete one if they are not in coincidence time
  }

  if( the511RecHits.size() == 2 ) {

    if( bComptonStudyHistos ) theComptonStudyHistos->FillDist511RecHits(the511RecHits);

    theAnaMgr->GetHisto1(320000+8)->Fill( the511RecHits[0].GetEnergy()/keV );
    theAnaMgr->GetHisto1(320000+8)->Fill( the511RecHits[1].GetEnergy()/keV );

#ifndef GAMOS_NO_VERBOSE
    if( PETVerb(debugVerb) ) G4cout << " PETEventClassifierTOFUA::ClassifyPET  creating line " << the511RecHits[0].GetPosition() << " " << the511RecHits[1].GetPosition() << G4endl;
#endif
    theEventClass += 1;
    G4double linedist;
    if( the511RecHits[1].GetPosition() != the511RecHits[0].GetPosition() ) {
      thePetLine = GmLine( the511RecHits[0].GetPosition(), the511RecHits[1].GetPosition()-the511RecHits[0].GetPosition() );
      G4ThreeVector eventVtx = GetEventVertex( evt );
      linedist = GetPETLineDistanceToVtx( thePetLine, eventVtx );
      theEventClass += int64_t( linedist > thePETLineDistToVtx );
      //      G4cout << " linedist " << linedist << " <? " << thePETLineDistToVtx << " EventClass " << theEventClass << G4endl;
    }
    theEventClass += 100 * CheckDifferentOriginalGammaAndEvent( the511RecHits );
    theEventClass += 1000 * CheckOriginalGammaScattered( the511RecHits );
    FillDCAHistos( linedist );

  }

#ifndef GAMOS_NO_VERBOSE
  if( PETVerb(infoVerb) ) G4cout << " PETEventClassifierTOFUA: classification " << theEventClass << G4endl;
#endif

  theAnaMgr->GetHisto1(320000+1)->Fill(theEventClass);
  theAnaMgr->GetHisto2(320000+58)->Fill(G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetPrimaryVertex(0)->GetPosition().x(),G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetPrimaryVertex(0)->GetPosition().y());

  if( theEventClass != 0 ) {
    theAnaMgr->GetHisto2(320000+59)->Fill(G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetPrimaryVertex(0)->GetPosition().x(),G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetPrimaryVertex(0)->GetPosition().y());
    // G4cout << "!!!! PET_EVENT " << G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID() << " CLASSIFICATION= " << theEventClass << G4endl;
      // G4cout << " VTX X " << G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetPrimaryVertex(0)->GetPosition().x()
	  //     << " VTX Y " << G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetPrimaryVertex(0)->GetPosition().y() << G4endl;
  }

  if( bDumpLM && theEventClass != 0 ) {
    G4PrimaryVertex* pvtx = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetPrimaryVertex(0);
    G4double Evtx = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetPrimaryVertex(0)->GetPrimary(0)->GetKineticEnergy();
    PETIOMgrTOF::GetInstance()->WriteEvent( pvtx->GetPosition(),
					    Evtx,
					    pvtx->GetT0(),
					    the511RecHits[0],
					    the511RecHits[1]);
  }
  

  // MCC 2011. Event to Proj. Data (sinogram)
  if( bDumpPD && theEventClass != 0 ) {
    PETProjDataMgr::GetInstance()->AddEvent( the511RecHits[0].GetPosition(), 
					     the511RecHits[1].GetPosition() );
  }

  return theEventClass;
}

//----------------------------------------------------------------------
void PETEventClassifierTOFUA::Get511ComptonRecHits( const gamosRecHitList& rhitList )
{
  //----- Join rec hits in sets if they are close in distance ----------
  gamosRecHitList::const_iterator itecl1,itecl2;
  std::set<GmRecHit*>::iterator ites;
  std::set<std::set<GmRecHit*> >::iterator itess;
  std::set<GmRecHit*> recHitUsed;
  mddrh distEnerComptRecHitList; // store in a map usig as key energy distance to electron_mass_c2, so that they can be efficiently delete if there are more than 2
  //  std::map<GmRecHit*,G4double> enerRecHit_inv;

  do {
    DetRecHitSet comptRecHit;
    GmRecHit* seedRecHit = 0;
    //----- Get seed RecHit as the one with highest energy, and that is not used
    // (assume RecHitBuilder have ordered them by energy)
    for( itecl1 = rhitList.begin(); itecl1 != rhitList.end(); itecl1++ ){
      if( recHitUsed.find( *itecl1 ) == recHitUsed.end() ) {
	seedRecHit = *itecl1;
	
	recHitUsed.insert(seedRecHit);
	comptRecHit.AddHit(seedRecHit);
#ifndef GAMOS_NO_VERBOSE
	if( PETVerb(debugVerb) ) G4cout <<"PETEventClassifierTOFUA::Get511ComptonRecHits: insert first not used Rechit in set " << seedRecHit->GetPosition() << " E " << seedRecHit->GetEnergy() << G4endl;
#endif
	break;
      }
    }
    
    //-    if( !seedRecHit ) continue; // no rec hit inside coincidence time left
    
    //----- Add all the hits within a certain distance
    if( theComptonRecHitsDist > 0. ){
      for( itecl1 = rhitList.begin(); itecl1 != rhitList.end(); itecl1++ ){
	GmRecHit* newRecHit = *itecl1;
	if( recHitUsed.find( newRecHit ) == recHitUsed.end() && 
	    newRecHit != seedRecHit ) {	
	  if( fabs(seedRecHit->GetTime()-newRecHit->GetTime()) > theCoincidenceTime ){ 
#ifndef GAMOS_NO_VERBOSE
	    if( PETVerb(debugVerb) )  G4cout << " WRONG coincidence Time " << seedRecHit->GetTime() << " - " << newRecHit->GetTime() << " < " << theCoincidenceTime << G4endl;
#endif
	    continue;
	  }

	  G4double dist = (seedRecHit->GetPosition() 
			   - (newRecHit)->GetPosition() ).mag();
	  if( dist < theComptonRecHitsDist ){
	    recHitUsed.insert(newRecHit);
	    //	  enerRecHit_inv[newRecHit] = newRecHit->GetEnergy();
	    //-	  cRecHit_E += (newRecHit)->GetEnergy();
	    comptRecHit.AddHit(newRecHit);
#ifndef GAMOS_NO_VERBOSE
	    if( PETVerb(debugVerb) ) G4cout 
	      << "PETEventClassifierTOFUA::Get511ComptonRecHits: " 
	      << "insert close Rechit in set "  
	      << newRecHit->GetPosition() 
	      << " E " <<  newRecHit->GetEnergy() 
	      << " dist " << dist << G4endl;
#endif
	  }
	}	
      }
    }
   
    //----- Check the energy of the sum is OK, if not delete them from lists
    if( comptRecHit.GetEnergy() < the511EPrecMin || 
	comptRecHit.GetEnergy() > the511EPrecMax ){
      for( ites = comptRecHit.begin(); ites != comptRecHit.end(); ites++ ){
	if( ites != comptRecHit.begin() ) {
	  recHitUsed.erase(*ites);
	}
      }
      comptRecHit.Initialise();
#ifndef GAMOS_NO_VERBOSE
      if( PETVerb(debugVerb) ) G4cout 
	     << "PETEventClassifierTOFUA::Get511ComptonRecHits: rec hit set Energy BAD " 
	     << comptRecHit.GetEnergy() 
	     << " " << fabs(comptRecHit.GetEnergy() - electron_mass_c2) 
	     << " < " << the511EPrecMin << " > " << the511EPrecMax << G4endl;
#endif
    } else {
      // GOOD set of RecHits 
      G4double distEner = fabs(comptRecHit.GetEnergy() - electron_mass_c2);
      if( distEnerComptRecHitList.find(distEner) != 
	  distEnerComptRecHitList.end()) distEner += 1.*eV;// avoid problems when two hits have exactly the same energy
      distEnerComptRecHitList[distEner] = comptRecHit;

      comptRecHit.Initialise();
      //-     seedRecHit->SetEnergy(cRecHit_E); // set energy of set to the first rec hit, as this will be the one used in the511RecHits
#ifndef GAMOS_NO_VERBOSE
      if( PETVerb(debugVerb) ) G4cout 
	     << "PETEventClassifierTOFUA::Get511ComptonRecHits: rec hit set Energy GOOD " 
	     << comptRecHit.GetEnergy() << G4endl;
#endif
    }
  } while( recHitUsed.size() != rhitList.size() );


#ifndef GAMOS_NO_VERBOSE
  if( PETVerb(debugVerb) ) G4cout 
	 << "PETEventClassifierTOFUA::Get511ComptonRecHits: building of rec hit sets ended. N sets built = " 
	 << distEnerComptRecHitList.size() << G4endl;
#endif

  if( distEnerComptRecHitList.size() == 0 ) return;

  //---- If there are more than take only those that match in coincidence time with another 
  mddrh::iterator itedss, itedss2;

  mddrh distEnerComptRecHitListGoodCT; 
  for( itedss = distEnerComptRecHitList.begin(); itedss != distEnerComptRecHitList.end(); itedss++ ) {
    for( itedss2 = itedss; itedss2 != distEnerComptRecHitList.end(); itedss2++ ) {
      if( itedss == itedss2 ) continue;
      if( CheckTimeCoincidence( itedss, itedss2 ) ) {
	distEnerComptRecHitListGoodCT.insert(*itedss);
	distEnerComptRecHitListGoodCT.insert(*itedss2);
	break;
      }
    }
  }
  distEnerComptRecHitList = distEnerComptRecHitListGoodCT;
  
    //---- If there are more than two delete all except the two closer to 511 keV
  if( distEnerComptRecHitList.size() > 2 ) {
    //    theEventClass += 10;
    itedss = distEnerComptRecHitList.begin();
    itedss++;
    itedss++;
    theAnaMgr->GetHisto1(320000+9)->Fill( (*itedss).second.GetEnergy()/keV );
#ifndef GAMOS_NO_VERBOSE
    if( PETVerb(debugVerb) ) G4cout << "PETEventClassifierTOFUA::Get511ComptonRecHits: deleting rec hit set " << itedss->second.GetEnergy() << G4endl;
#endif
    distEnerComptRecHitList.erase( itedss, distEnerComptRecHitList.end() );
  }

  //----- Calculate position for sets of rec hits -----
  //----- If no two 511 rechits found return
  if( distEnerComptRecHitList.size() < 2 ) return;

  //--- Take the first two sets of rec hits (those closer to 511 keV) and order them by number of rec hits
  itedss = distEnerComptRecHitList.begin();
  DetRecHitSet firstRecHitSet = (*itedss).second;
  DetRecHitSet secondRecHitSet;
  itedss++;
  secondRecHitSet = (*itedss).second;
  if( (*itedss).second.size() < firstRecHitSet.size() ){
    secondRecHitSet = firstRecHitSet;
    firstRecHitSet = (*itedss).second; 
  }

  //---- First rec hit set
  G4ThreeVector pos;
#ifndef GAMOS_NO_VERBOSE
  if( PETVerb(debugVerb) ) G4cout << "PETEventClassifierTOFUA::Get511ComptonRecHits: firstRecHitSet initial pos " << firstRecHitSet.GetPosition() << G4endl;
#endif
  //---- Get position if more than 1 rec hit in set
  if( firstRecHitSet.size() > 1 ) {
    GmRecHit* rhSelected;
    // Use the order by energy
    rhSelected = the1stHitAlgo1->Get1stRecHit( firstRecHitSet, secondRecHitSet );
    pos = rhSelected->GetPosition();
#ifndef GAMOS_NO_VERBOSE
    if( PETVerb(debugVerb) ) G4cout << "PETEventClassifierTOFUA::Get511ComptonRecHits: firstRecHitSet change pos to " << pos << " biggest E  " << G4endl;
#endif
    if( PETVerb(debugVerb) ) firstRecHitSet.Print();
    firstRecHitSet.SetPosition(pos);
  }

#ifndef GAMOS_NO_VERBOSE
  if( PETVerb(debugVerb) ) G4cout << "PETEventClassifierTOFUA::Get511ComptonRecHits: firstRecHitSet  pos final " << firstRecHitSet.GetPosition() << G4endl;
#endif
  //--- Register it as 511 rec hit
  the511RecHits.push_back( firstRecHitSet );

  //---- Second rec hit set (if exists)
  if( distEnerComptRecHitList.size() == 2 ) {
#ifndef GAMOS_NO_VERBOSE
    if( PETVerb(debugVerb) ) G4cout << "PETEventClassifierTOFUA::Get511ComptonRecHits: secondRecHitSet initial pos " << secondRecHitSet.GetPosition() << G4endl;
#endif

    //---- Get position if more than 1 rec hit in set
    if( secondRecHitSet.size() > 1 ) {

      GmRecHit* rhSelected;
      // Use the order by energy
      rhSelected = the1stHitAlgo2->Get1stRecHit( secondRecHitSet, firstRecHitSet );
      pos = rhSelected->GetPosition();
      
#ifndef GAMOS_NO_VERBOSE
      if( PETVerb(debugVerb) ) G4cout << "PETEventClassifierTOFUA::Get511ComptonRecHits: secondRecHitSet change pos " << pos << G4endl;
#endif
      secondRecHitSet.SetPosition(pos);
      if( PETVerb(debugVerb) ) secondRecHitSet.Print();
    }
#ifndef GAMOS_NO_VERBOSE
    if( PETVerb(debugVerb) ) G4cout << "PETEventClassifierTOFUA::Get511ComptonRecHits: secondRecHitSet pos final " << secondRecHitSet.GetPosition() << G4endl;
#endif
    //--- Register it as 511 rec hit
    the511RecHits.push_back( secondRecHitSet );
  }

}

//----------------------------------------------------------------------
G4bool PETEventClassifierTOFUA::CheckTimeCoincidence( mddrh::iterator itedss1, mddrh::iterator itedss2 )
{
  if( theCoincidenceTime <= 0. ) return true;

#ifndef GAMOS_NO_VERBOSE
  if( PETVerb(debugVerb) ) G4cout << " PETEventClassifierTOFUA::CheckTimeCoincidence " << itedss1->second.GetTime() - itedss2->second.GetTime() << " >? " << theCoincidenceTime << " time1= " << itedss1->second.GetTime() << " time2= " << itedss2->second.GetTime() << G4endl;
#endif
  if( fabs(itedss1->second.GetTime() - itedss2->second.GetTime()) < theCoincidenceTime ) {
    return true;
  }

  return false;

}

//----------------------------------------------------------------------
void PETEventClassifierTOFUA::CheckTimeCoincidence()
{
  if( theCoincidenceTime <= 0. ) return;
 
  DetRecHitSetList::iterator ites1 = the511RecHits.begin(); 
  DetRecHitSetList::iterator ites2 = ites1; 
  ites2++;

#ifndef GAMOS_NO_VERBOSE
  if( PETVerb(debugVerb) ) G4cout << " PETEventClassifierTOFUA::CheckTimeCoincidence() " << (*ites1).GetTime() - (*ites2).GetTime() << " >? " << theCoincidenceTime << " time1= " << (*ites1).GetTime() << " time2= " << (*ites2).GetTime() << G4endl;
#endif
  // If time difference is bigger than theCoincidenceTime, delete the one with smaller time 
  if( (*ites1).GetTime() - (*ites2).GetTime() > theCoincidenceTime ){
    the511RecHits.erase(ites2);
    return;
  }

  if( (*ites1).GetTime() - (*ites2).GetTime() < -theCoincidenceTime ){
     the511RecHits.erase(ites1);
    return;
  }

}

      
//----------------------------------------------------------------------
G4ThreeVector PETEventClassifierTOFUA::GetEventVertex( const G4Event* evt )
{
  G4ThreeVector eventVtx;
  //----- Get vertex
  G4int nvtx = evt->GetNumberOfPrimaryVertex();
  for( int ii = 0; ii < nvtx; ii++ ){
    G4PrimaryVertex* vtx = evt->GetPrimaryVertex(ii);
    G4int npart = vtx->GetNumberOfParticle();
#ifndef GAMOS_NO_VERBOSE
    if( PETVerb(debugVerb) ) G4cout << " vertex found " << ii << " npart " << npart << G4endl;
#endif
    for( int jj = 0; jj < npart; jj++ ){
#ifndef GAMOS_NO_VERBOSE
      if( PETVerb(debugVerb) ) G4cout << " part in vertex " << vtx->GetPrimary(jj)->GetPDGcode() << G4endl;
#endif
      //      if( vtx->GetPrimary(jj)->GetPDGcode() == -11 ) {
      //	foundPositron = TRUE;
	eventVtx = vtx->GetPosition();
	//      }
    }
  }

  return eventVtx;
}

	
//----------------------------------------------------------------------
G4double PETEventClassifierTOFUA::GetPETLineDistanceToVtx( const GmLine& line, const G4ThreeVector& eventVtx )
{
  if( the511RecHits.size() != 2 ) {
    G4Exception("PETEventClassifierTOFUA::GetPETLineDistanceToVtx",
		"Error",
		FatalException,
		G4String("Number of 511 recHits !=2 " + GmGenUtils::itoa( the511RecHits.size() ) ).c_str());
  }

  //  if( !foundPositron ) G4Exception("PETEventClassifierTOFUA::GetPETLineDistanceToVtx: no positron found ");

#ifndef GAMOS_NO_VERBOSE
  if( PETVerb(debugVerb) ) G4cout << " PETEventClassifierTOFUA::GetPETLineDistanceToVtx recHit 0 pos " << the511RecHits[0].GetPosition()<< " E " << the511RecHits[0].GetEnergy()<< " recHit 1 pos " << the511RecHits[1].GetPosition()<< " E " << the511RecHits[1].GetEnergy() << G4endl;
#endif

  G4double linedist = line.GetDist( eventVtx );
#ifndef GAMOS_NO_VERBOSE
  if( PETVerb(infoVerb) ) G4cout << " PETEventClassifierTOFUA::GetPETLineDistanceToVtx " << linedist << " ?< " << thePETLineDistToVtx << G4endl;
#endif

#ifndef GAMOS_NO_VERBOSE
  if( linedist > thePETLineDistToVtx*2 ) if( PETVerb(infoVerb) ) G4cout << " BIG LINEDIST " << linedist << " " << G4EventManager::GetEventManager()->GetNonconstCurrentEvent()->GetEventID() << G4endl;
#endif
  theAnaMgr->GetHisto1(320000+5)->Fill( linedist );


  //   if( phantom.GetDist( positronVtx ) < thePETLineDistToPhantom ) {
  //iec = 3;

  G4ThreeVector distp = line.GetDistPoint( eventVtx );
  G4ThreeVector distv = eventVtx - distp;

  theAnaMgr->GetHisto1(320000+6)->Fill( distv.z() );

  /*  G4ThreeVector distXY = distv.cross( G4Thr line.GetDir() );
  distR = distR.unit();
  G4ThreeVector distPHI = distv.cross( distR );
  distPHI = distPHI.unit();
  */
  G4ThreeVector distXY = distv.cross( G4ThreeVector(0.,0.,1.) );
  distXY = distXY.unit();
  G4ThreeVector distvxy(distv.x(), distv.y(),0.);
  //  G4ThreeVector distPHI = distXY.cross( distvxy );
  //  G4ThreeVector distPHI = distvxy.cross( G4ThreeVector(0.,0.,1.) );
  //  G4cout << " DIST " << distv << " z " << distv.z() << " perp " << distv.perp() << " PHI " << distPHI << G4endl;
  //  distPHI = distPHI.unit();
  theAnaMgr->GetHisto1(320000+7)->Fill( distv.perp() );
  
  return linedist;

}

//----------------------------------------------------------------------
void PETEventClassifierTOFUA::FillDCAHistos( G4double linedist )
{
  //  G4cout << theEventClass << " Fill " << 321000+theEventClass-1 << " " << theAnaMgr->GetHisto1(321000+theEventClass-1)->GetName() << G4endl;
  theAnaMgr->GetHisto1(321000+theEventClass-1)->Fill( linedist );
  theAnaMgr->GetHisto1(331000+theEventClass-1)->Fill( the511RecHits[0].GetEnergy()/keV );
  theAnaMgr->GetHisto1(331000+theEventClass-1)->Fill( the511RecHits[1].GetEnergy()/keV );
  for( unsigned int ii = 1; ii < 5; ii++ ){  
    if( ((theEventClass-1)/int64_t(pow(float(10),int(ii-1)))%10) == 1 ) {
      //      G4cout << " FillALL " << 341000+ii << " " << theAnaMgr->GetHisto1(341000+ii)->GetName() << " " << theEventClass << " " <<  ((theEventClass-1)/int64_t(pow(10,ii-1))%10)  << G4endl;
      theAnaMgr->GetHisto1(341000+ii)->Fill( linedist );
      theAnaMgr->GetHisto1(351000+ii)->Fill( the511RecHits[0].GetEnergy()/keV );
      theAnaMgr->GetHisto1(351000+ii)->Fill( the511RecHits[1].GetEnergy()/keV );
    }
  }

}


//----------------------------------------------------------------------
G4bool PETEventClassifierTOFUA::CheckDifferentOriginalGammaAndEvent( DetRecHitSetList& rhitsets )
{
  G4int origGammaAndEvent = -1;

  std::vector<G4int> eventRhit;
  //----- Loop to all recHits original gammas to check 
  DetRecHitSetList::iterator ites;
  std::set<GmRecHit*>::iterator ite;
  for( ites = rhitsets.begin(); ites != rhitsets.end(); ites++ ){
    std::set<GmRecHit*> rhitset = *ites;
    for( ite = rhitset.begin(); ite != rhitset.end(); ite++ ){
      origGammaAndEvent = -1;
      G4bool chk = CheckRecHitDifferentOriginalGammaAndEvent( (*ite), origGammaAndEvent );
      if( chk ) return TRUE;
      eventRhit.push_back( G4int(origGammaAndEvent/1000) );
    }
  }
  
  if( eventRhit[0] != eventRhit[1] ) {
#ifndef GAMOS_NO_VERBOSE
    if( PETVerb(infoVerb) ) G4cout << "CheckDifferentOriginalGammaAndEvent: same track but different event " << eventRhit[0] << " <> " << eventRhit[1] << G4endl;
#endif
    return TRUE;
  } else {
    return FALSE;  
  }
}

//----------------------------------------------------------------------
G4bool PETEventClassifierTOFUA::CheckRecHitDifferentOriginalGammaAndEvent( GmRecHit* rhit, G4int& origGammaAndEvent )
{
  if( rhit == 0 ) return FALSE;

  std::vector<GmHit*> hits = rhit->GetHits();
  std::vector<GmHit*>::const_iterator iteh;
  std::set<G4int>::iterator itesi;
#ifndef GAMOS_NO_VERBOSE
  if( PETVerb(debugVerb) ) G4cout << origGammaAndEvent << "PETEventClassifierTOFUA::CheckRecHitDifferentOriginalGammaAndEvent hits in recHit " << hits.size() << G4endl;
#endif
  //  G4double maxdist = 0.;
  for( iteh = hits.begin(); iteh != hits.end(); iteh++ ){
    std::set<G4int> htidori = (*iteh)->GetOriginalTrackIDs();
#ifndef GAMOS_NO_VERBOSE
    if( PETVerb(debugVerb) ) G4cout 
	   << "PETEventClassifierTOFUA::CheckRecHitDifferentOriginalGammaAndEvent looping hits n tracks " 
	   << htidori.size() << G4endl;
#endif
    for( itesi = htidori.begin(); itesi != htidori.end(); itesi++ ){
      G4int oge = (*itesi)+1000*(*iteh)->GetEventID();
#ifndef GAMOS_NO_VERBOSE
      if( PETVerb(debugVerb) ) G4cout << " new track id " << *itesi+1000.*(*iteh)->GetEventID() << " ogenew " << oge << " old " << origGammaAndEvent << G4endl;
#endif
      if( origGammaAndEvent != oge ){
	G4bool isDifferentG = TRUE;
	if( origGammaAndEvent == -1 ) {
	  isDifferentG = FALSE;
	} 
	origGammaAndEvent = oge;
#ifndef GAMOS_NO_VERBOSE
	if( PETVerb(debugVerb) ) G4cout << "PETEventClassifierTOFUA::CheckRecHitDifferentOriginalGammaAndEvent origGammaAndEvent changed " << origGammaAndEvent << G4endl;
#endif
	return isDifferentG;
      }
    }
  }

  return FALSE;

}

//----------------------------------------------------------------------
G4bool PETEventClassifierTOFUA::CheckOriginalGammaScattered( DetRecHitSetList& rhitsets )
{
  //----- Loop to all recHits original gammas to check 
  DetRecHitSetList::iterator ites;
  std::set<GmRecHit*>::iterator ite;
  for( ites = rhitsets.begin(); ites != rhitsets.end(); ites++ ){
    std::set<GmRecHit*> rhitset = *ites;
    for( ite = rhitset.begin(); ite != rhitset.end(); ite++ ){
      if( CheckRecHitOriginalGammaScattered( *ite ) ){
	return TRUE;
      }
    }
  }

  return FALSE;
}


//----------------------------------------------------------------------
G4bool PETEventClassifierTOFUA::CheckRecHitOriginalGammaScattered( GmRecHit* rhit )
{
  if( rhit == 0 ) return FALSE;

  std::vector<GmHit*> hits = rhit->GetHits();
  std::vector<GmHit*>::const_iterator iteh;
  std::set<G4int>::iterator itesi;
#ifndef GAMOS_NO_VERBOSE
  if( PETVerb(debugVerb) ) G4cout << "PETEventClassifierTOFUA::CheckRecHitOriginalGammaScattered hits in recHit " << hits.size() << G4endl;
#endif

  for( iteh = hits.begin(); iteh != hits.end(); iteh++ ){
    std::set<G4int> htidori = (*iteh)->GetOriginalTrackIDs();
#ifndef GAMOS_NO_VERBOSE
    if( PETVerb(debugVerb) ) G4cout << "PETEventClassifierTOFUA::CheckRecHitOriginalGammaScattered looping hits n original tracks " << htidori.size() << G4endl;
#endif
    for( itesi = htidori.begin(); itesi != htidori.end(); itesi++ ){
#ifndef GAMOS_NO_VERBOSE
      //-      G4cout << PETVerb(5) << " new track id " << *itesi+10.*(*iteh)->GetEventID() << G4endl;
#endif
      if( theCountScattering->GetNScatterOriginalTrack((*itesi) ) ) return TRUE;
    }
  }
  
  return FALSE;

}


//----------------------------------------------------------------------
void PETEventClassifierTOFUA::EndOfRunAction(const G4Run* aRun )
{
  G4int nevt = aRun->GetNumberOfEvent();
  if (nevt == 0) {
    G4cout << "Nevt = 0 !!!" << G4endl;
    return;
  };

  PrintPETClassificationTable( nevt );

  if( bDumpLM ) PETIOMgrTOF::GetInstance()->CloseFileOut();

  if( bDumpPD ) PETProjDataMgr::GetInstance()->WriteInterfile();

}

//----------------------------------------------------------------------
void PETEventClassifierTOFUA::PrintPETClassificationTable( int64_t nevt )
{
  theAnaMgr = GmAnalysisMgr::GetInstance("pet") ;

  GmHisto1* his = theAnaMgr->GetHisto1(320000+1);   // PET classification
  // recHits with ALL hits from original gammas (from e+ annihilation) 
  
  //  G4double b_00 = his->GetBinContent (1); // events NOT PET (bin 0 is underflow)
  
  G4double b_01 = his->GetBinContent (2); // PET events, 2 recHits of 511 keV, LOR < 4mm from source
  G4double b_02 = his->GetBinContent (3); // PET events, 2 recHits of 511 keV, LOR > 4mm from source
  
  G4double b_11 = his->GetBinContent (12); // PET events, 3->2 recHits of 511 keV, LOR < 4mm from source
  G4double b_12 = his->GetBinContent (13); // PET events, 3->2 recHits of 511 keV, LOR > 4mm from source
  

  // Print table for PET
  G4double b_PET = his->GetEntries()-his->GetBinContent(1);
  G4cout << " ---------------------------------------------" << G4endl;
  G4cout << "$$PET Events                          :  " << b_PET << "   " << (b_PET)/nevt*100 << " % " << G4endl;
  //  G4cout << " Events PET               :  " << nevt-b_00 << "   " << (nevt-b_00)/nevt*100 << " % " << G4endl;

  G4cout << " --------------- Good PET -------------" << G4endl;
  G4cout << "$$PET  ALL                            :  " << b_01+b_02+b_11+b_12 << "   " << (b_01+b_02+b_11+b_12)/nevt*100 << " % " << G4endl; 
  G4cout << "(   1) $$PET 2 recHits close to vtx   :  " << b_01 << "   " << b_01/nevt*100 << " % " << G4endl; 
  G4cout << "(   2) $$PET 2 recHits far from vtx   :  " << b_02 << "   " << b_02/nevt*100 << " % " << G4endl;
  G4cout << "(  11) $$PET 3->2 recHit close to vtx :  " << b_11 << "   " << b_11/nevt*100 << " % " << G4endl;
  G4cout << "(  12) $$PET 3->2 recHit far from vtx :  " << b_12 << "   " << b_12/nevt*100 << " % " << G4endl;
  
  // recHits with hits from different original gammas
  G4double b_101 = his->GetBinContent (102); // PET events, 2 recHits of 511 keV, LOR < 4mm from source
  G4double b_102 = his->GetBinContent (103); // PET events, 2 recHits of 511 keV, LOR > 4mm from source
  
  G4double b_111 = his->GetBinContent (112); // PET events, 3->2 recHits of 511 keV, LOR < 4mm from source
  G4double b_112 = his->GetBinContent (113); // PET events, 3->2 recHits of 511 keV, LOR > 4mm from source
  
  G4cout << " --------------- Random Coincidences -------------" << G4endl;
  G4cout << "$$PET  ALL COINCIDENCES               :  " << b_101+b_102+b_111+b_112 << "   " << (b_101+b_102+b_111+b_112)/nevt*100 << " % " << G4endl; 
  G4cout << "( 101) $$PET 2 recHits close to vtx   :  " << b_101 << "   " << b_101/nevt*100 << " % " << G4endl; 
  G4cout << "( 102) $$PET 2 recHits far from vtx   :  " << b_102 << "   " << b_102/nevt*100 << " % " << G4endl;
  G4cout << "( 111) $$PET 3->2 recHit close to vtx :  " << b_111 << "   " << b_111/nevt*100 << " % " << G4endl;
  G4cout << "( 112) $$PET 3->2 recHit far from vtx :  " << b_112 << "   " << b_112/nevt*100 << " % " << G4endl;

  // recHits with hits from different original gammas
  G4double b_1001 = his->GetBinContent (1002); // PET events, 2 recHits of 511 keV, LOR < 4mm from source
  G4double b_1002 = his->GetBinContent (1003); // PET events, 2 recHits of 511 keV, LOR > 4mm from source
  
  G4double b_1011 = his->GetBinContent (1012); // PET events, 3->2 recHits of 511 keV, LOR < 4mm from source
  G4double b_1012 = his->GetBinContent (1013); // PET events, 3->2 recHits of 511 keV, LOR > 4mm from source
  
  G4cout << " --------------- Scattered -------------" << G4endl;
  G4cout << "$$PET  ALL SCATTERED                  :  " << b_1001+b_1002+b_1011+b_1012 << "   " << (b_1001+b_1002+b_1011+b_1012)/nevt*100 << " % " << G4endl; 
  G4cout << "(1001) $$PET 2 recHits close to vtx   :  " << his->GetBinContent(1002) << "   " << his->GetBinContent(1002)/nevt*100 << " % " << G4endl; 
  G4cout << "(1002) $$PET 2 recHits far from vtx   :  " << his->GetBinContent(1003) << "   " << his->GetBinContent(1003)/nevt*100 << " % " << G4endl;
  G4cout << "(1011) $$PET 3->2 recHit close to vtx :  " << his->GetBinContent(1012) << "   " << his->GetBinContent(1012)/(G4double(nevt))*100 << " % " << G4endl;
  G4cout << "(1012) $$PET 3->2 recHit far from vtx :  " << his->GetBinContent(1013) << "   " << his->GetBinContent(1013)/(G4double(nevt))*100 << " % " << G4endl;
  
  // recHits with hits from different original gammas
  G4double b_1101 = his->GetBinContent (1102); // PET events, 2 recHits of 511 keV, LOR < 4mm from source
  G4double b_1102 = his->GetBinContent (1103); // PET events, 2 recHits of 511 keV, LOR > 4mm from source
  
  G4double b_1111 = his->GetBinContent (1112); // PET events, 3->2 recHits of 511 keV, LOR < 4mm from source
  G4double b_1112 = his->GetBinContent (1113); // PET events, 3->2 recHits of 511 keV, LOR > 4mm from source
  
  G4cout << " --------------- Scattered and Random Coincidences -------------" << G4endl;
  G4cout << "$$PET  ALL COINC. AND SCCATTERED      :  " << b_1101+b_1102+b_1111+b_1112 << "   " << (b_1101+b_1102+b_1111+b_1112)/nevt*100 << " % " << G4endl; 
  G4cout << "(1101) $$PET 2 recHits close to vtx   :  " << his->GetBinContent(1102) << "   " << his->GetBinContent(1102)/nevt*100 << " % " << G4endl; 
  G4cout << "(1102) $$PET 2 recHits far from vtx   :  " << his->GetBinContent(1103) << "   " << his->GetBinContent(1103)/nevt*100 << " % " << G4endl;
  G4cout << "(1111) $$PET 3->2 recHit close to vtx :  " << his->GetBinContent(1112) << "   " << his->GetBinContent(1112)/(G4double(nevt))*100 << " % " << G4endl;
  G4cout << "(1112) $$PET 3->2 recHit far from vtx :  " << his->GetBinContent(1113) << "   " << his->GetBinContent(1113)/(G4double(nevt))*100 << " % " << G4endl;
  
}  


//----------------------------------------------------------------------
void PETEventClassifierTOFUA::EndOfEventAction(const G4Event* evt )
{
  gamosSDRecHitMap rhitMap = theHitsEventMgr->GetRecHits();

  gamosRecHitList rhitList;
  gamosSDRecHitMap::const_iterator ite;
  for( ite = rhitMap.begin(); ite != rhitMap.end(); ite++ ){
    rhitList.insert( rhitList.end(), (*ite).second.begin(), (*ite).second.end() );
  }

  Classify( rhitList, evt );
}
