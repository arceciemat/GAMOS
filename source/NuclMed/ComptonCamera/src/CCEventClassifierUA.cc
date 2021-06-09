#include "CCEventClassifierUA.hh"
#include "CCVerbosity.hh"
#include "CCIOMgr.hh"

#include "NuclMed/Base/include/DetCountScatteringUA.hh"
#include "NuclMed/Base/include/DetV1stHitAlgorithm.hh"
#include "NuclMed/Base/include/DetComptonStudyHistosUA.hh"


#include "GamosCore/GamosSD/include/GmRecHit.hh"
#include "GamosCore/GamosSD/include/GmHit.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosSD/include/GmHitsEventMgr.hh"
#include "GamosCore/GamosAnalysis/include/GmTrajStep.hh"
#include "GamosCore/GamosAnalysis/include/GmCheckOriginalGamma.hh"

#include <math.h>
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"
#else
#include "NuclMed/Base/include/Det1stHitAlgorithmFactory.hh"
#endif

//----------------------------------------------------------------------
CCEventClassifierUA::CCEventClassifierUA()
{

  bDump = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("CC:EvtClass:DumpEvent",0));
  if( bDump ) CCIOMgr::GetInstance()->OpenFileOut();
  bDumpSingles = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("CC:EvtClass:DumpSingles",0));
bDumpMultiples = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("CC:EvtClass:DumpMultiples",0));
  BookHistos();
  // theGammaEnergy is the energy of the generated gamma rays for Compton imaging  
  theGammaEnergy =GmParameterMgr::GetInstance()->GetNumericValue("CC:EvtClass:GammaEnergy",141.*CLHEP::keV);
  #ifndef GAMOS_NO_VERBOSE
   if( CCVerb(infoVerb) ) G4cout << "CCEventClassifierUA  GammaEnergy " << theGammaEnergy << G4endl;
  #endif

  // Define a photopeak gate
  G4double EPrecPhotopeak = GmParameterMgr::GetInstance()->GetNumericValue("CC:EvtClass:PhotopeakEPrec",0.1);
  thePhotopeakEPrecMin = GmParameterMgr::GetInstance()->GetNumericValue("CC:EvtClass:PhotopeakEPrecMin", theGammaEnergy*(1-EPrecPhotopeak));
  thePhotopeakEPrecMax = GmParameterMgr::GetInstance()->GetNumericValue("CC:EvtClass:PhotopeakEPrecMax",theGammaEnergy*(1+EPrecPhotopeak));
  #ifndef GAMOS_NO_VERBOSE
   if( CCVerb(infoVerb) ) G4cout << " thePhotopeakEPrecMin " << thePhotopeakEPrecMin << " thePhotopeakEPrecMax " << thePhotopeakEPrecMax << G4endl;
  #endif

  theCoincidenceTime = GmParameterMgr::GetInstance()->GetNumericValue("CC:EvtClass:CoincidenceTime",DBL_MAX);
  theComptonRecHitsDistScat  = GmParameterMgr::GetInstance()->GetNumericValue("CC:EvtClass:ComptonRecHitDistScat",0.*CLHEP::mm);
  theComptonRecHitsDistAbs  = GmParameterMgr::GetInstance()->GetNumericValue("CC:EvtClass:ComptonRecHitDistAbs",0.*CLHEP::mm);


  G4String the1stHitStr = GmParameterMgr::GetInstance()->GetStringValue("CC:EvtClass:1stHitAlgorithm","Det1stHitByEnergy");
  G4String the1stHit1Str = GmParameterMgr::GetInstance()->GetStringValue("CC:EvtClass:1stHitAlgorithmFirst",the1stHitStr);
  G4String the1stHit2Str = GmParameterMgr::GetInstance()->GetStringValue("CC:EvtClass:1stHitAlgorithmSecond",the1stHitStr);

#ifdef ROOT5
  the1stHitAlgo1 = Reflex::PluginService::Create<DetV1stHitAlgorithm*>(the1stHit1Str);
#else
  the1stHitAlgo1 = Det1stHitAlgorithmFactory::get()->create(the1stHit1Str);
#endif
  
  if( !the1stHitAlgo1 ) { 
    G4Exception("CCEventClassifierUA::CCEventClassifierUA",
		("Wrong 1st hit algorithm: "+the1stHit1Str).c_str(),
	FatalErrorInArgument,
	"Check your parameter CC:EvtClass:1stHitAlgorithm or CC:EvtClass:1stHitAlgorithmFirst");
 }
#ifdef ROOT5
  the1stHitAlgo2 = Reflex::PluginService::Create<DetV1stHitAlgorithm*>(the1stHit2Str);
#else
  the1stHitAlgo2 = Det1stHitAlgorithmFactory::get()->create(the1stHit2Str);
#endif
  if( !the1stHitAlgo2 ) { 
    G4Exception("CCEventClassifierUA::CCEventClassifierUA",
		"Wrong 1st hit algorithm",
		FatalErrorInArgument,
		"Check your parameter CC:EvtClass:1stHitAlgorithm or CC:EvtClass:1stHitAlgorithmSecond");
  }

  theSelectPosOrder = G4int(GmParameterMgr::GetInstance()->GetNumericValue("CC:EvtClass:SelectPosOrder",1));
  theHitsEventMgr = GmHitsEventMgr::GetInstance();
  theCountScattering = new DetCountScatteringUA;

}

//----------------------------------------------------------------------
void CCEventClassifierUA::BookHistos()
{
  theAnaMgr = GmAnalysisMgr::GetInstance("compcam") ;
  std::string hnam;
  G4String sepa = GmParameterMgr::GetInstance()->GetStringValue("Histos:Separator",":");
  std::string hgnam = "CCEvtClass"+sepa+" ";
  hnam = hgnam + std::string("CC classification");
  theAnaMgr->CreateHisto1D(hnam,1200,0,1200.,320000+1);
  hnam = hgnam + std::string("Sum energy all coincident events (CLHEP::keV)");
  theAnaMgr->CreateHisto1D(hnam,1000,0,1000.,320000+2);
  hnam = hgnam + std::string("Scatter detector energy all coincident events (CLHEP::keV)");
  theAnaMgr->CreateHisto1D(hnam,1000,0,1000.,320000+3);
  hnam = hgnam + std::string("Absorber detector energy all coincident events (CLHEP::keV)");
  theAnaMgr->CreateHisto1D(hnam,1000,0,1000.,320000+4);
  hnam = hgnam + std::string("Scatter detector energy fully absorbed coincident events (CLHEP::keV)");
  theAnaMgr->CreateHisto1D(hnam, 1000,0,1000.,320000+5);
  hnam = hgnam + std::string("Absorber detector energy fully absorbed coincident events (CLHEP::keV)");
  theAnaMgr->CreateHisto1D(hnam,1000,0,1000.,320000+6);
  hnam = hgnam + std::string("Nb hits in Scatter detector");
  theAnaMgr->CreateHisto1D(hnam,100,0,100.,320000+7);
  hnam = hgnam + std::string("Nb hits in Absorber detector");
  theAnaMgr->CreateHisto1D(hnam,100,0,100.,320000+8);
  hnam = hgnam + std::string("Sum energy all fully absorbed events (CLHEP::keV)");
  theAnaMgr->CreateHisto1D(hnam,1000,0,1000.,320000+9);
  hnam = hgnam + std::string("Sum energy all fully absorbed single_single events (CLHEP::keV)");
  theAnaMgr->CreateHisto1D(hnam,1000,0,1000.,320000+10);
  hnam = hgnam + std::string("Scatter detector energy fully absorbed single_single events (CLHEP::keV)");
  theAnaMgr->CreateHisto1D(hnam, 1000,0,1000.,320000+11);
  hnam = hgnam + std::string("Absorber detector energy fully absorbed single_single events (CLHEP::keV)");
  theAnaMgr->CreateHisto1D(hnam,1000,0,1000.,320000+12);
 hnam = hgnam + std::string("Absorber detector energy single_single events (CLHEP::keV)");
  theAnaMgr->CreateHisto1D(hnam,1000,0,1000.,320000+13);
 hnam = hgnam + std::string("Scatterer detector energy single_single events (CLHEP::keV)");
  theAnaMgr->CreateHisto1D(hnam,1000,0,1000.,320000+14);
  hnam = hgnam + std::string("Sum energy all single_single events (CLHEP::keV)");
  theAnaMgr->CreateHisto1D(hnam,1000,0,1000.,320000+15);
  
}

//----------------------------------------------------------------------
void CCEventClassifierUA::BeginOfRunAction(const G4Run* )
{
  bComptonStudyHistos = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("CC:EvtClass:ComptonStudyHistos",0));
  if( bComptonStudyHistos ) {
    theComptonStudyHistos = new DetComptonStudyHistosUA();
  }
}

//----------------------------------------------------------------------
CCEventClassifierUA::~CCEventClassifierUA()
{
}

//----------------------------------------------------------------------
int64_t CCEventClassifierUA::Classify( gamosRecHitList& rhitList, const G4Event* evt  )
{
  ClassifyCC( rhitList, evt );

  return theEventClass;
}

//-----------------Classify Compton Camera events----------------------------------------
int64_t CCEventClassifierUA::ClassifyCC( gamosRecHitList& rhitList, const G4Event* evt )
{
  theFullRecHits.clear();

  // Initialize variables
  theEventClass = 0; //!! if no changed = no CC
  sumEnergy = 0;
  totalEnergy = 0;
  scatEnergy = 0;
  absEnergy = 0;
  scatHits= 0;
  absHits=0;
  if( rhitList.size() == 0) return theEventClass;

  GetFullComptonRecHits( rhitList); // Includes merging of reconstructed hits

  if( theFullRecHits.size() == 2 ) {
   CheckTimeCoincidence(); //delete one if they are not in coincidence time
  }

 // If there is one unique incident gamma in two sensitive detector types, process the event
  if( theFullRecHits.size()==2 ) {
   GmRecHit* rHit; 
   gamosRecHitList::const_iterator ljh1;
   for( ljh1 = rhitList.begin(); ljh1 != rhitList.end(); ljh1++ ){
    rHit = *ljh1;
     sumEnergy +=rHit->GetEnergy();
     type = rHit->GetSDType();

      if (type=="/Scatterer/") {
        scatHits++;
        scatEnergy +=rHit->GetEnergy();
        scatPosition =rHit->GetPosition();
      } else if (type=="/Absorber/"){
        absHits++;
        absEnergy +=rHit->GetEnergy();
        absPosition =rHit->GetPosition();
      }
   } 

   totalEnergy = theFullRecHits[0].GetEnergy()+theFullRecHits[1].GetEnergy();

   // Currently interested in events with hits in at least two detectors (Coincident events)
   if((scatHits>0) && (absHits>0))
   { 
    theEventClass +=1;
    //Get the total sum of energy from hits
    theAnaMgr->GetHisto1(320000+2)->Fill(sumEnergy/CLHEP::keV);
    theAnaMgr->GetHisto1(320000+3)->Fill(scatEnergy/CLHEP::keV);
    theAnaMgr->GetHisto1(320000+4)->Fill(absEnergy/CLHEP::keV);
    theAnaMgr->GetHisto1(320000+7)->Fill(scatHits);
    theAnaMgr->GetHisto1(320000+8)->Fill(absHits);

    //Process and output those coincident events which sum to the incident energy
    if (sumEnergy < thePhotopeakEPrecMin || sumEnergy > thePhotopeakEPrecMax)
     { 
      #ifndef GAMOS_NO_VERBOSE
       if( CCVerb(debugVerb) ) G4cout << " The energy summed across 2 hits "<<sumEnergy<<" is not within energy gate for event  "<<evt->GetEventID()<<"  where min is "<<thePhotopeakEPrecMin<<G4endl;
      #endif
       if ((scatHits==1) && (absHits==1)) {
         theAnaMgr->GetHisto1(320000+14)->Fill(scatEnergy/CLHEP::keV);
         theAnaMgr->GetHisto1(320000+13)->Fill(absEnergy/CLHEP::keV);
         theAnaMgr->GetHisto1(320000+15)->Fill(sumEnergy/CLHEP::keV);

        }
      } else {
       theEventClass +=1;
       #ifndef GAMOS_NO_VERBOSE
        if( CCVerb(debugVerb) ) G4cout << " CCEventClassifierUA::ClassifyCC Fully absorbed coincident events with scatter detector energy: "<< scatEnergy << " and absorber detector energy: "<<absEnergy<<" for event  "<<evt->GetEventID()<< G4endl;
       #endif

       theAnaMgr->GetHisto1(320000+5)->Fill(scatEnergy/CLHEP::keV);
       theAnaMgr->GetHisto1(320000+6)->Fill(absEnergy/CLHEP::keV);
       theAnaMgr->GetHisto1(320000+9)->Fill(sumEnergy/CLHEP::keV);

       //Write event to file for single single coincident
       if ((scatHits==1) && (absHits==1)) {
        theEventClass +=1;
        theAnaMgr->GetHisto1(320000+10)->Fill(sumEnergy/CLHEP::keV);
        theAnaMgr->GetHisto1(320000+11)->Fill(scatEnergy/CLHEP::keV);
        theAnaMgr->GetHisto1(320000+12)->Fill(absEnergy/CLHEP::keV);
        theAnaMgr->GetHisto1(320000+14)->Fill(scatEnergy/CLHEP::keV);
        theAnaMgr->GetHisto1(320000+13)->Fill(absEnergy/CLHEP::keV);
        theAnaMgr->GetHisto1(320000+15)->Fill(sumEnergy/CLHEP::keV);


        if( bDump && theEventClass != 0) {
         if (bDumpSingles==1) { 
          CCIOMgr::GetInstance()->WriteEvent(theFullRecHits[0].GetPosition(),theFullRecHits[0].GetEnergy()/CLHEP::keV,theFullRecHits[1].GetPosition(), theFullRecHits[1].GetEnergy()/CLHEP::keV);
         }
        }
        singles++;
       } else {
        if(bDump && theEventClass != 0) {
          if (bDumpMultiples==1) {
            CCIOMgr::GetInstance()->WriteEvent(theFullRecHits[0].GetPosition(),theFullRecHits[0].GetEnergy()/CLHEP::keV,theFullRecHits[1].GetPosition(), theFullRecHits[1].GetEnergy()/CLHEP::keV);
          }
        }
        absorbed++;
       }
      }
    coincident++;
   }
   theEventClass += 100 * CheckDifferentOriginalGammaAndEvent( theFullRecHits );
   theEventClass += 1000 * CheckOriginalGammaScattered( theFullRecHits );
  }


  #ifndef GAMOS_NO_VERBOSE
    if( CCVerb(infoVerb) ) G4cout << " CCEventClassifierUA: classification " << theEventClass << G4endl;
  #endif

  theAnaMgr->GetHisto1(320000+1)->Fill(theEventClass);
  return theEventClass;
}

//----------------------------------------------------------------------
void CCEventClassifierUA::GetFullComptonRecHits( const gamosRecHitList& rhitList)
{

  //----- Join rec hits in sets if they are close in distance ----------
  gamosRecHitList::const_iterator itecl1;
  std::set<GmRecHit*> recHitUsed;
  DetRecHitSet comptRecHit;
  std::map<G4double,DetRecHitSet> distEnerComptRecHitList; // store in a map, so that they can be efficiently delete if there are more than 2
  
  do {
   GmRecHit* seedRecHit;
   //----- Get seed RecHit as the one with highest energy, not used
   // (assume RecHitBuilder have ordered them by energy)
   for( itecl1 = rhitList.begin(); itecl1 != rhitList.end(); itecl1++ ){
     if( recHitUsed.find( *itecl1 ) == recHitUsed.end() ) { 
      seedRecHit = *itecl1;
      recHitUsed.insert(seedRecHit);
      comptRecHit.AddHit(seedRecHit);
      #ifndef GAMOS_NO_VERBOSE
       if( CCVerb(debugVerb) ) G4cout <<"CCEventClassifierUA::GetFullComptonRecHits: insert first not used Rechit in set LOOP1 " << seedRecHit->GetPosition() << " E " << seedRecHit->GetEnergy() << "  ComptRecHitenergy   "<<comptRecHit.GetEnergy() <<G4endl;
      #endif
      break;
     }
   }

   //----- Add all the hits within a certain distance - only implemeted by a user parameter
 
   if(type=="/Scatterer/") {
   theComptonRecHitsDist = theComptonRecHitsDistScat;
   }
   else if (type=="/Absorber/") {
   theComptonRecHitsDist = theComptonRecHitsDistAbs;
   }

   if( theComptonRecHitsDist > 0. ){
    for( itecl1 = rhitList.begin(); itecl1 != rhitList.end(); itecl1++ ){
     GmRecHit* newRecHit = *itecl1;
      if( recHitUsed.find( newRecHit ) == recHitUsed.end() && newRecHit != seedRecHit )  
       {
if( fabs(seedRecHit->GetTime()-newRecHit->GetTime()) > theCoincidenceTime ){
#ifndef GAMOS_NO_VERBOSE
	  if( CCVerb(debugVerb) )  G4cout << " WRONG coincidence Time " << seedRecHit->GetTime() << " - " << newRecHit->GetTime() << " < " << theCoincidenceTime << G4endl;
#endif
	  continue;
	}
        G4double dist = (seedRecHit->GetPosition() - (newRecHit)->GetPosition() ).mag();


	 if( dist < theComptonRecHitsDist ){
	  recHitUsed.insert(newRecHit);
	  comptRecHit.AddHit(newRecHit);
          #ifndef GAMOS_NO_VERBOSE
	   if( CCVerb(debugVerb) ) G4cout 
	    << "CCEventClassifierUA::GetFullComptonRecHits: " 
	    << "insert close Rechit in set LOOP2 "  
	    << newRecHit->GetPosition() 
	    << " E " <<  newRecHit->GetEnergy() 
            << " dist " << dist << G4endl;
	  #endif
        }	
      }
    }   
}
   G4double distEner = fabs(comptRecHit.GetEnergy() - theGammaEnergy);
   // GOOD set of RecHits 
   if( distEnerComptRecHitList.find(distEner) != 
    distEnerComptRecHitList.end()) distEner *= 1.000001;// avoid problems when two hits have exactly the same energy
    distEnerComptRecHitList[distEner] = comptRecHit;
    comptRecHit.Initialise();
    #ifndef GAMOS_NO_VERBOSE
     if( CCVerb(debugVerb) ) G4cout 
        << "CCEventClassifierUA::GetFullComptonRecHits: rec hit set Energy GOOD  " 
	<< comptRecHit.GetEnergy() << G4endl;
    #endif
   } while( recHitUsed.size() != rhitList.size() );
 

  if( distEnerComptRecHitList.size() == 0 ) return;

  std::map<G4double,DetRecHitSet>::iterator itedss;
 
  if ( distEnerComptRecHitList.size() > 2 ) {
   theEventClass +=10;
   return;
  }
  //----- Calculate position for sets of rec hits -----
  //----- If no two rechits found return
  if( distEnerComptRecHitList.size() < 2 ) return;

  //--- Order the two sets of rec hits by time
  itedss = distEnerComptRecHitList.begin();
  DetRecHitSet firstrecHitSet = (*itedss).second;
  itedss++;
  DetRecHitSet secondrecHitSet;
  secondrecHitSet = (*itedss).second;
  if( (*itedss).second.GetTime() < firstrecHitSet.GetTime()){
   secondrecHitSet = firstrecHitSet;
   firstrecHitSet = (*itedss).second; 
  }
  //---- Rec hit set
  G4ThreeVector pos;
  G4double energy1sthit;
  #ifndef GAMOS_NO_VERBOSE
   if( CCVerb(debugVerb) ) G4cout << "CCEventClassifierUA::GetFullComptonRecHits: firstrecHitSet initial pos  LOOP4   " << firstrecHitSet.GetPosition()<< "   firstrecHitSize  "<< firstrecHitSet.size()<<   " with a distenergcomptlistsize " <<distEnerComptRecHitList.size()<<"  and an energy of "<<firstrecHitSet.GetEnergy()/CLHEP::keV<<G4endl;
  #endif
   //---- Get position if more than 1 rec hit in set
  if( firstrecHitSet.size() > 1 ) {
   GmRecHit* rhSelected;
   rhSelected = the1stHitAlgo1->Get1stRecHit( firstrecHitSet, secondrecHitSet );
   pos = rhSelected->GetPosition();
   energy1sthit = rhSelected->GetEnergy();
   #ifndef GAMOS_NO_VERBOSE
    if( CCVerb(debugVerb) ) G4cout << "CCEventClassifierUA::GetFullComptonRecHits: firstrecHitSet change pos to  LOOP5   " << pos << " biggest E  " << G4endl;
   #endif
   firstrecHitSet.SetPosition(pos);
   firstrecHitSet.SetEnergy(energy1sthit);
   if( CCVerb(debugVerb) ) firstrecHitSet.Print();
  }

  #ifndef GAMOS_NO_VERBOSE
   if( CCVerb(debugVerb) ) G4cout << "CCEventClassifierUA::GetFullComptonRecHits: firstrecHitSet  pos final LOOP6  " << firstrecHitSet.GetPosition() << "And what about the size?   "<<firstrecHitSet.size()<< "  energy  "<<firstrecHitSet.GetEnergy()<< G4endl;
  #endif

  //--- Register it as full Rec hit
  theFullRecHits.push_back( firstrecHitSet );

  //---- Second rec hit set
  if( distEnerComptRecHitList.size() >1 ) {

   #ifndef GAMOS_NO_VERBOSE
    if( CCVerb(debugVerb) ) G4cout << "CCEventClassifierUA::GetFullComptonRecHits: secondrecHitSet initial pos " << secondrecHitSet.GetPosition() << G4endl;
   #endif

   //---- Get position if more than 1 rec hit in set
   if( secondrecHitSet.size() > 1 ) {
    GmRecHit* rhSelected;
    // Use the order by energy
    rhSelected = the1stHitAlgo2->Get1stRecHit( secondrecHitSet, firstrecHitSet );
    pos = rhSelected->GetPosition();
    energy1sthit = rhSelected->GetEnergy();
    #ifndef GAMOS_NO_VERBOSE
     if( CCVerb(debugVerb) ) G4cout << "CCEventClassifierUA::GetFullComptonRecHits: secondrecHitSet change pos " << pos << G4endl;
    #endif
    secondrecHitSet.SetPosition(pos);
    secondrecHitSet.SetEnergy(energy1sthit);
     if( CCVerb(debugVerb) ) secondrecHitSet.Print();
   }
 
   #ifndef GAMOS_NO_VERBOSE
    if( CCVerb(debugVerb) ) G4cout << "CCEventClassifierUA::GetFullComptonRecHits: secondrecHitSet pos final " << secondrecHitSet.GetPosition() << G4endl;
   #endif

   //--- Register it as Full rec hit
   theFullRecHits.push_back( secondrecHitSet );
  }
}

//----------------------------------------------------------------------

void CCEventClassifierUA::CheckTimeCoincidence()
{
  if( theCoincidenceTime <= 0. ) return;
 
  DetRecHitSetList::iterator ites1 = theFullRecHits.begin(); 
  DetRecHitSetList::iterator ites2 = ites1; 
  ites2++;

  #ifndef GAMOS_NO_VERBOSE
   if( CCVerb(debugVerb) ) G4cout << " CCEventClassifierUA::CheckTimeCoincidence() " << (*ites1).GetTime() - (*ites2).GetTime() << " >? " << theCoincidenceTime << " time1= " << (*ites1).GetTime() << " time2= " << (*ites2).GetTime() << G4endl;
  #endif

  // If time difference is bigger than theCoincidenceTime, delete the one with smaller time 
  if( (*ites1).GetTime() - (*ites2).GetTime() > theCoincidenceTime ){
   theFullRecHits.erase(ites2);
   return;
  }

  if( (*ites1).GetTime() - (*ites2).GetTime() < -theCoincidenceTime ){
   theFullRecHits.erase(ites1);
   return;
  }
}

//----------------------------------------------------------------------
G4bool CCEventClassifierUA::CheckDifferentOriginalGammaAndEvent( DetRecHitSetList& rhitsets )
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
    eventRhit.push_back( int64_t(origGammaAndEvent/10) );
   }
  }
  
  if( eventRhit[0] != eventRhit[1] ) {
   #ifndef GAMOS_NO_VERBOSE
    if( CCVerb(infoVerb) ) G4cout << "CheckDifferentOriginalGammaAndEvent: same track but different event " << eventRhit[0] << " <> " << eventRhit[1] << G4endl;
   #endif
   return TRUE;
  } else {
   return FALSE;  
  }
}

//----------------------------------------------------------------------
G4bool CCEventClassifierUA::CheckRecHitDifferentOriginalGammaAndEvent( GmRecHit* rhit, G4int& origGammaAndEvent )
{
  if( rhit == 0 ) return FALSE;

  std::vector<GmHit*> hits = rhit->GetHits();
  std::vector<GmHit*>::const_iterator iteh;
  std::set<G4int>::iterator itesi;
  #ifndef GAMOS_NO_VERBOSE
   if( CCVerb(debugVerb) ) G4cout << origGammaAndEvent << "CCEventClassifierUA::CheckRecHitDifferentOriginalGammaAndEvent hits in recHit " << hits.size() << G4endl;
  #endif
   for( iteh = hits.begin(); iteh != hits.end(); iteh++ ){
   std::set<G4int> htidori = (*iteh)->GetOriginalTrackIDs();
   #ifndef GAMOS_NO_VERBOSE
    if( CCVerb(debugVerb) ) G4cout 
       << "CCEventClassifierUA::CheckRecHitDifferentOriginalGammaAndEvent looping hits n tracks " 
       << htidori.size() << G4endl;
   #endif
   for( itesi = htidori.begin(); itesi != htidori.end(); itesi++ ){
    G4int oge = (*itesi)+10*(*iteh)->GetEventID();
   #ifndef GAMOS_NO_VERBOSE
    if( CCVerb(debugVerb) ) G4cout << " new track id " << *itesi+10.*(*iteh)->GetEventID() << " ogenew " << oge << " old " << origGammaAndEvent << G4endl;
   #endif
    if( origGammaAndEvent != oge ){
     G4bool isDifferentG = TRUE;
      if( origGammaAndEvent == -1 ) {
       isDifferentG = FALSE;
      } 
     origGammaAndEvent = oge;
     #ifndef GAMOS_NO_VERBOSE
      if( CCVerb(debugVerb) ) G4cout << "CCEventClassifierUA::CheckRecHitDifferentOriginalGammaAndEvent origGammaAndEvent changed " << origGammaAndEvent << G4endl;
     #endif
     return isDifferentG;
    }
   }
 }
 return FALSE;
}

//----------------------------------------------------------------------
G4bool CCEventClassifierUA::CheckOriginalGammaScattered( DetRecHitSetList& rhitsets )
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
G4bool CCEventClassifierUA::CheckRecHitOriginalGammaScattered( GmRecHit* rhit )
{
  if( rhit == 0 ) return FALSE;

  std::vector<GmHit*> hits = rhit->GetHits();
  std::vector<GmHit*>::const_iterator iteh;
  std::set<G4int>::iterator itesi;
  #ifndef GAMOS_NO_VERBOSE
   if( CCVerb(debugVerb) ) G4cout << "CCEventClassifierUA::CheckRecHitOriginalGammaScattered hits in recHit " << hits.size() << G4endl;
  #endif

  for( iteh = hits.begin(); iteh != hits.end(); iteh++ ){
   std::set<G4int> htidori = (*iteh)->GetOriginalTrackIDs();
   #ifndef GAMOS_NO_VERBOSE
    if( CCVerb(debugVerb) ) G4cout << "CCEventClassifierUA::CheckRecHitOriginalGammaScattered looping hits n original tracks " << htidori.size() << G4endl;
   #endif
   for( itesi = htidori.begin(); itesi != htidori.end(); itesi++ ){
     if( theCountScattering->GetNScatterOriginalTrack((*itesi) ) ) return TRUE;
   }
  }
  return FALSE;
}
 
//----------------------------------------------------------------------
void CCEventClassifierUA::EndOfRunAction(const G4Run* aRun )
{
  G4int nevt = aRun->GetNumberOfEvent();
  if (nevt == 0) {
   G4cout << "Nevt = 0 !!!" << G4endl;
   return;
  };

  PrintCCClassificationTable( nevt );
  if( bDump ) CCIOMgr::GetInstance()->CloseFileOut();
}

//----------------------------------------------------------------------
void CCEventClassifierUA::PrintCCClassificationTable( int64_t nevt )
{
  theAnaMgr = GmAnalysisMgr::GetInstance("compcam") ;

  GmHisto1* his = theAnaMgr->GetHisto1(320000+1);   // CC classification
  // recHits with ALL hits from original gammas 
  
  G4double b_00 = his->GetBinContent (1); // Not Coincident events
  G4double b_01 = his->GetBinContent (2); // coincidence events, not absorbed
  G4double b_02 = his->GetBinContent (3); // absorbed events, multiples
  G4double b_03 = his->GetBinContent (4); // Useful Compton coincidence events, 2 recHits in scatter and absorber detector

  // More than two RecHitSets 
  G4double b_10 = his->GetBinContent (11); // Not Coincident events, more than two rechitsets
  G4double b_11 = his->GetBinContent (12); // coincidence events, not absorbed, more than two rechitsets
  G4double b_12 = his->GetBinContent (13); // absorbed events, multiples, more than two rechitsets
  G4double b_13 = his->GetBinContent (14); // Useful Compton coincidence events, 2 recHits in scatter and absorber detector, more than two rechitsets

  //Random events
  G4double b_100 = his->GetBinContent (101); // Not Coincident events, random
  G4double b_101 = his->GetBinContent (102); // Coincidence events, absorbed multiples , random
  G4double b_102 = his->GetBinContent (103); // absorbed events, multiples
  G4double b_103 = his->GetBinContent (104); // Useful Compton coincidence events, 2 recHits in scatter and absorber detector, random

  //Random events with more than two rechitsets
  G4double b_110 = his->GetBinContent (111); // Not Coincident events, random, more than two rechitsets
  G4double b_111 = his->GetBinContent (112); // Ccoincidence events, absorbed multiples , random, more than two rechitsets
  G4double b_112 = his->GetBinContent (113); // absorbed events, multiples, more than two rechitsets
  G4double b_113 = his->GetBinContent (114); // Useful Compton coincidence events, 2 recHits in scatter and absorber detector, random, more than two rechitsets

  //Scattered events
  G4double b_1000 = his->GetBinContent (1001); // Not Coincident events, scattered
  G4double b_1001 = his->GetBinContent (1002); // coincidence events, not absorbed, scattered
  G4double b_1002 = his->GetBinContent (1003); // absorbed events, multiples, scattered
  G4double b_1003 = his->GetBinContent (1004); // Useful Compton coincidence events, 2 recHits in scatter and absorber detector, scattered
  //Scattered events, more than two rec hits
  G4double b_1010 = his->GetBinContent (1011); // Not Coincident events, scattered, more than two rechitsets
  G4double b_1011 = his->GetBinContent (1012); // coincidence events, not absorbed, scattered, more than two rechitsets
  G4double b_1012 = his->GetBinContent (1013); // absorbed events, multiples, scattered, more than two rechitsets
  G4double b_1013 = his->GetBinContent (1014); // Useful Compton coincidence events, 2 recHits in scatter and absorber detector, scattered, more than two rechitsets

  //Scattered events, random
  G4double b_1100 = his->GetBinContent (1101); // Not Coincident events, scattered, random
  G4double b_1101 = his->GetBinContent (1102); // coincidence events, not absorbed, scattered, random
  G4double b_1102 = his->GetBinContent (1103); // absorbed events, multiples, scattered, random
  G4double b_1103 = his->GetBinContent (1104); // Useful Compton coincidence events, 2 recHits in scatter and absorber detector, scattered, random

  //Scattered events, random, more than two rechitsets
  G4double b_1110 = his->GetBinContent (1111); // Not Coincident events, scattered, random, more than two rechitsets
  G4double b_1111 = his->GetBinContent (1112); // coincidence events, not absorbed, scattered, random, more than two rechitsets
  G4double b_1112 = his->GetBinContent (1113); // absorbed events, multiples, scattered, random, more than two rechitsets
  G4double b_1113 = his->GetBinContent (1114); // Useful Compton coincidence events, 2 recHits in scatter and absorber detector, scattered, random, more than two rechitsets

  // Print table for Events
  //-  G4double b_CC = his->GetEntries()-his->GetBinContent(1);
  G4cout << " ---------------------------------------------------------" << G4endl;
  G4cout <<" "<<G4endl;
  G4cout << " --------------- True Compton imaging events -------------" << G4endl;
  G4cout << "$$CC  ALL GOOD                           :  " << b_00+b_01+b_02+b_03+b_10+b_11+b_12+b_13<< "   " << (b_00+b_01+b_02+b_03+b_10+b_11+b_12+b_13)/nevt*100 << " % " << G4endl; 
  G4cout << "(   0) $$CC Not coincident events        :  " <<b_00<<"  " <<(b_00)/nevt*100 << " % " <<G4endl; 
  G4cout << "(   1) $$CC Coincident events            :  " << b_01+b_02+b_03 << "   " << (b_01+b_02+b_03)/nevt*100 << " % " << G4endl; 
  G4cout << "(   2) $$CC Fully absorbed coinc. event  :  " << b_02+b_03 << "   " << (b_02+b_03)/nevt*100 << " % " << G4endl; 
  G4cout << "(   3) $$CC Single Single events         :  " << b_03 << "   " << b_03/nevt*100 << " % " << G4endl; 
  G4cout << "(  10) $$CC More than 2 rec. hits found  :  " <<b_10+b_11+b_12+b_13 <<"  " <<(b_10+b_11+b_12+b_13 )/nevt*100 << " % " << G4endl;
  G4cout <<" "<<G4endl;

  G4cout << " --------------- Random Coincidences -------------" << G4endl;
  G4cout << "$$CC  ALL COINCIDENCES                               :  " << b_100+b_101+b_102+b_103+b_110+b_111+b_112+b_113 << "   " << (b_100+b_101+b_102+b_103+b_110+b_111+b_112+b_113)/nevt*100 << " % " << G4endl; 
  G4cout << "( 100) $$CC Not coincident events        :  " <<b_100<<"  " <<(b_100)/nevt*100 << " % " <<G4endl; 
  G4cout << "( 101) $$CC Coincidence events           :  " << b_101+b_102+b_103 << "   " << (b_101+b_102+b_103)/nevt*100 << " % " << G4endl;
  G4cout << "( 102) $$CC Fully absorbed coinc. event  :  " << b_102+b_103 << "   " << (b_102+b_103)/nevt*100 << " % " << G4endl; 
  G4cout << "( 103) $$CC Single Single events         :  " << b_103 << "   " << b_103/nevt*100 << " % " << G4endl; 
  G4cout << "( 110) $$CC More than 2 rec. hits found  :  " <<b_110+b_111+b_112+b_113 <<"  " <<(b_110+b_111+b_112+b_113 )/nevt*100 << " % " << G4endl;
  G4cout <<" "<<G4endl;

  G4cout << " --------------- Scattered -------------" << G4endl;
  G4cout << "$$CC  ALL SCATTERED                      :  " <<  b_1000+b_1001+b_1002+b_1003+b_1010+b_1011+b_1012+b_1013 <<" " << (b_1000+b_1001+b_1002+b_1003+b_1010+b_1011+b_1012+b_1013)/nevt*100 << " % " << G4endl; 
  G4cout << "(1000) $$CC Not coincident events        :  " << b_1000<<"  " <<(b_1000)/nevt*100 << " % " <<G4endl; 
  G4cout << "(1001) $$CC Coincidence events    :  " << b_1001+b_1002+b_1003 << "   " << (b_1001+b_1002+b_1003)/nevt*100 << " % " << G4endl;
  G4cout << "(1002) $$CC Fully absorbed coinc. events :  " << b_1002+b_1003 << "   " << (b_1002+b_1003)/nevt*100 << " % " << G4endl; 
  G4cout << "(1003) $$CC Single Single events         :  " << b_1003 << "   " << b_1003/nevt*100 << " % " << G4endl; 
  G4cout << "(1010) $$CC More than 2 rec. hits found  :  "<<b_1010+b_1011+b_1012+b_1013<<"  " <<(b_1010+b_1011+b_1012+b_1013)/nevt*100 << " % " <<G4endl;
  G4cout <<" "<<G4endl;
 
  G4cout << " --------------- Scattered and Random Coincidences -------------" << G4endl;
  G4cout << "$$CC  ALL COINC. AND SCATTERED           :  " << b_1100+b_1101+b_1102+b_1103+b_1110+b_1111+b_1112+b_1113 << "   " << ( b_1100+b_1101+b_1102+b_1103+b_1110+b_1111+b_1112+b_1113 )/nevt*100 << " % " << G4endl; 
  G4cout << "(1100) $$CC Not coincident events        :  " << b_1100<<"  " <<(b_1100)/nevt*100 << " % " <<G4endl;
  G4cout << "(1101) $$CC Coincidence events           :  " << b_1101+b_1102+b_1103 << "   " << (b_1101+b_1102+b_1103)/nevt*100 << " % " << G4endl;
  G4cout << "(1102) $$CC Fully absorbed coinc. event  :  " << b_1102+b_1103 << "   " << (b_1102+b_1103)/nevt*100 << " % " << G4endl; 
  G4cout << "(1103) $$CC Single Single events         :  " << b_1103 << "   " << b_1103/nevt*100 << " % " << G4endl; 
  G4cout << "(1110) $$CC More than 2 rec. hits found  :  " <<b_1110+b_1111+b_1112+b_1113 <<"  " <<(b_1110+b_1111+b_1112+b_1113 )/nevt*100 << " % " << G4endl;
}  

//----------------------------------------------------------------------
void CCEventClassifierUA::EndOfEventAction(const G4Event* evt )
{
  gamosSDRecHitMap rhitMap = theHitsEventMgr->GetRecHits();

  gamosRecHitList rhitList;
  gamosSDRecHitMap::const_iterator ite;
  for( ite = rhitMap.begin(); ite != rhitMap.end(); ite++ ){
    rhitList.insert( rhitList.end(), (*ite).second.begin(), (*ite).second.end() );
  }

 Classify( rhitList, evt );
}
