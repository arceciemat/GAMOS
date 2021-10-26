#include "SPECTEventClassifierUA.hh"
#include "SPECTVerbosity.hh"
#include "SPECTIOMgr.hh"
#include "NuclMed/Base/include/DetV1stHitAlgorithm.hh"
#include "NuclMed/Base/include/DetComptonStudyHistosUA.hh"
#include "NuclMed/Base/include/DetCountScatteringUA.hh"

#include "GamosCore/GamosSD/include/GmRecHit.hh"
#include "GamosCore/GamosSD/include/GmHit.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmLine.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosSD/include/GmHitsEventMgr.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "GamosCore/GamosMovement/include/GmMovementMgr.hh"
#include "GamosCore/GamosAnalysis/include/GmCheckOriginalGamma.hh"

#include <math.h>
#include <iomanip>
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"
#else
#include "NuclMed/Base/include/Det1stHitAlgorithmFactory.hh"
#endif

//----------------------------------------------------------------------
SPECTEventClassifierUA::SPECTEventClassifierUA()
{
  GmParameterMgr* paramMgr = GmParameterMgr::GetInstance();

  bDump = G4bool(paramMgr->GetNumericValue("SPECT:EvtClass:DumpEvent",0));
  if( bDump ) SPECTIOMgr::GetInstance()->OpenFileOut();

  BookHistos();

  if( ! paramMgr->IsParameterInScript("SPECT:EvtClass:GammaEnergy") ){
    G4Exception("SPECTEventClassifierUA::SPECTEventClassifierUA",
		"Parameter not defined ",
		FatalErrorInArgument,
		"A gamma energy should be provided, use command ' /gamos/setParam SPECT:EvtClass:GammaEnergy VALUE ' ");
  }
  theGammaEnergy =paramMgr->GetNumericValue("SPECT:EvtClass:GammaEnergy",-1.);
#ifndef GAMOS_NO_VERBOSE
  if( SPECTVerb(infoVerb) ) G4cout << "SPECTEventClassifierUA  GammaEnergy " << theGammaEnergy << G4endl;
#endif

  G4double EPrec = paramMgr->GetNumericValue("SPECT:EvtClass:EPrec",0.3);
  theEPrecMin = paramMgr->GetNumericValue("SPECT:EvtClass:EPrecMin",theGammaEnergy*(1-EPrec));
  theEPrecMax = paramMgr->GetNumericValue("SPECT:EvtClass:EPrecMax",theGammaEnergy*(1+EPrec));
  
#ifndef GAMOS_NO_VERBOSE
  if( SPECTVerb(infoVerb) ) G4cout << "SPECTEventClassifierUA  EPrecMin " << theEPrecMin << " EPrecMax " << theEPrecMax << G4endl;
#endif
  
  theSPECTLineDistToVtx  = paramMgr->GetNumericValue("SPECT:EvtClass:LineDistToVtx",10.*CLHEP::mm);
  
  theComptonRecHitsDist  = paramMgr->GetNumericValue("SPECT:EvtClass:ComptonRecHitDist",0.*CLHEP::mm);
  
  G4String the1stHitStr = GmParameterMgr::GetInstance()->GetStringValue("SPECT:EvtClass:1stHitAlgorithm","Det1stHitByEnergy");
#ifdef ROOT5
  the1stHitAlgo = Reflex::PluginService::Create<DetV1stHitAlgorithm*>(the1stHitStr);
#else
  the1stHitAlgo = Det1stHitAlgorithmFactory::get()->create(the1stHitStr);
#endif
  if( !the1stHitAlgo ) { 
    G4Exception("SPECTEventClassifierUA::SPECTEventClassifierUA",
		"Wrong 1st hit algorithm",
		FatalErrorInArgument,
		"Check your parameter SPECT:EvtClass:1stHitAlgorithm or SPECT:EvtClass:1stHitAlgorithmFirst");
  }

  theNHits2Reject = G4bool(paramMgr->GetNumericValue("SPECT:EvtClass:RejectIfNRecHits",2));

  theHitsEventMgr = GmHitsEventMgr::GetInstance();

  theCountScattering = new DetCountScatteringUA;

  theMovementMgr = GmMovementMgr::GetInstance();
}

//----------------------------------------------------------------------
void SPECTEventClassifierUA::BeginOfRunAction(const G4Run* )
{
  GetCollimatorCentres();
  bComptonStudyHistos = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("SPECT:EvtClass:DetComptonStudyHistos",0));
  if( bComptonStudyHistos ) {
    theComptonStudyHistos = new DetComptonStudyHistosUA();
  }

}


//----------------------------------------------------------------------
void SPECTEventClassifierUA::GetCollimatorCentres()
{
  GmParameterMgr* paramMgr = GmParameterMgr::GetInstance();

  std::vector<G4double> phcoord;
  /*  phcoord = paramMgr->GetVNumericValue("SPECT:EvtClass:PinholePoint",phcoord);
  if( paramMgr->IsParameterInScript("SPECT:EvtClass:PinholePoint" ) ){
    if( phcoord.size() != 3 ) {
      G4Exception("PECTEventClassifierUA::GetPinholePoint",
		  "Wrong number of pinhole point coordinates",
		  FatalErrorInArgument,
		  G4String("There must be three coordinates, while there are " + GmGenUtils::itoa(phcoord.size() )).c_str());
    } else {
      theCollimatorCentre = G4ThreeVector( phcoord[0], phcoord[1], phcoord[2] );
#ifndef GAMOS_NO_VERBOSE
      if( SPECTVerb(infoVerb) ) G4cout << "SPECTEventClassifierUA::GetCollimatorCentre point set in parameter " << theCollimatorCentre << G4endl;
#endif

    }
  } else {
  */

  G4String collimatorVol = paramMgr->GetStringValue("SPECT:EvtClass:CollimatorVolume","");
  if( collimatorVol == "" ) {
    G4Exception("PECTEventClassifierUA::GetCollimatorCentres",
		"No collimator point defined",
		FatalErrorInArgument,
		"Use command ' /gamos/setParam SPECT:EvtClass:CollimatorVolume VOL X Y Z ' or ' /gamos/setParam SPECT:EvtClass:CollimatorVolume VOL_NAME ");
  } 
  std::vector<GmTouchable*> touchs = GmGeometryUtils::GetInstance()->GetTouchables(collimatorVol);
  if( touchs.size() == 0 ) {
    G4Exception("PECTEventClassifierUA::GetCollimatorCentres",
		"Error in collimator volume name",
		FatalErrorInArgument,
		G4String("No volume found : " + collimatorVol).c_str());
  } 
  for( size_t ii = 0; ii < touchs.size(); ii++ ) {
    theCollimatorLongNamesAndCentres[touchs[ii]->GetLongName()] = touchs[ii]->GetGlobalPosition();

    std::vector<G4LogicalVolume*> lvs = GmGeometryUtils::GetInstance()->GetLogicalVolumes(collimatorVol);
    if( lvs.size() != 1 ) {
       G4Exception("PECTEventClassifierUA::GetCollimatorCentre",
		   "More than one collimator logical volume found",
		   FatalErrorInArgument,
		   G4String("Volume name" + collimatorVol).c_str());
    }
    theCollimatorLogVol = lvs[0];

#ifndef GAMOS_NO_VERBOSE
    if( SPECTVerb(infoVerb) ) G4cout << ii << " SPECTEventClassifierUA::GetCollimatorCentres point from volume " << collimatorVol << " 0 " << touchs[ii]->GetGlobalPosition() << G4endl;
#endif
    delete touchs[ii];
  }

  //  }

}

//----------------------------------------------------------------------
void SPECTEventClassifierUA::BookHistos()
{
  theAnaMgr = GmAnalysisMgr::GetInstance("spect") ;

  std::string hnam;
  G4String sepa = GmParameterMgr::GetInstance()->GetStringValue("Histos:Separator",":");
  std::string hgnam = "SPECTEvtClass"+sepa+" ";
  hnam = hgnam + std::string("dist line - vertex (mm)");
  theAnaMgr->CreateHisto1D(hnam,100,0,25.,320000+2);
  hnam = hgnam + std::string("dist line - vertex Z (mm)");
  theAnaMgr->CreateHisto1D(hnam,100,0,25.,320000+3);
  hnam = hgnam + std::string("dist line - vertex XY (mm)");
  theAnaMgr->CreateHisto1D(hnam,100,0,25.,320000+4);
  hnam = hgnam + std::string("RecHit energy (keV)");
  theAnaMgr->CreateHisto1D(hnam,600,0,600.,320000+5);
  //  hnam = hgnam + std::string("Extra SPECT RecHit energy (keV)");
  //  theAnaMgr->CreateHisto1D(hnam,600,0,600.,320000+9);

  // DCA histograms for each subclassification type

  std::vector<std::string> str32(2);
  str32[0] = " 1";  
  str32[1] = " 2->1"; 
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
	    sepa + G4String(" dist line - vertex (mm)");
	  theAnaMgr->CreateHisto1D(hnam,100,0,25.,321000+i1+i2*10+i3*100+i4*1000);
	  hnam = hgnam + strfar[i1] + str32[i2] + strrand[i3] + strscat[i4] + 
	    sepa +" RecHit energy (CLHEP::keV)";
	  theAnaMgr->CreateHisto1D(hnam,600,0,600.,331000+i1+i2*10+i3*100+i4*1000);
	}
      }
    }
  }

  hnam = hgnam + " ALL far " + sepa + " dist line - vertex (mm)";
  theAnaMgr->CreateHisto1D(hnam,100,0,25.,341000+1);
  hnam = hgnam + " ALL far" + sepa + " RecHit energy (keV)";
  theAnaMgr->CreateHisto1D(hnam,600,0,600.,351000+1);
  hnam = hgnam + " ALL 3->2 " + sepa+" dist line - vertex (mm)";
  theAnaMgr->CreateHisto1D(hnam,100,0,25.,341000+2);
  hnam = hgnam + " ALL 3->2 " + sepa+" RecHit energy (keV)";
  theAnaMgr->CreateHisto1D(hnam,600,0,600.,351000+2);
  hnam = hgnam + " ALL random coincidences " + sepa+" dist line - vertex (mm)";
  theAnaMgr->CreateHisto1D(hnam,100,0,25.,341000+3);
  hnam = hgnam + " ALL random coincidences" + sepa+" RecHit energy (keV)";
  theAnaMgr->CreateHisto1D(hnam,600,0,600.,351000+3);
  hnam = hgnam + " ALL scattered " + sepa+" dist line - vertex (mm)";
  theAnaMgr->CreateHisto1D(hnam,100,0,25.,341000+4);
  hnam = hgnam + " ALL scattered" + sepa+" RecHit energy (keV)";
  theAnaMgr->CreateHisto1D(hnam,600,0,600.,351000+4);
  
}


//----------------------------------------------------------------------
void SPECTEventClassifierUA::EndOfEventAction(const G4Event* evt )
{
  if( theMovementMgr->ApplyMovements() ) {
    GetCollimatorCentres();
  }

  gamosSDRecHitMap rhitMap = theHitsEventMgr->GetRecHits();

  gamosRecHitList rhitList;
  gamosSDRecHitMap::const_iterator ite;
  for( ite = rhitMap.begin(); ite != rhitMap.end(); ite++ ){
    rhitList.insert( rhitList.end(), (*ite).second.begin(), (*ite).second.end() );
  }

  Classify( rhitList, evt );

  CleanCollimatorsTraversed();

}


//----------------------------------------------------------------------
int64_t SPECTEventClassifierUA::Classify( gamosRecHitList& rhitList, const G4Event* evt  )
{
  ClassifySPECT( rhitList, evt );

  return theEventClass;
}


//----------------------------------------------------------------------
int64_t SPECTEventClassifierUA::ClassifySPECT( gamosRecHitList& rhitList, const G4Event* evt )
{
  theGoodRecHits.clear();

#ifndef GAMOS_NO_VERBOSE
  if( SPECTVerb(infoVerb) ) G4cout << " SPECTEventClassifierUA::ClassifySPECT() n rechits " << rhitList.size() << G4endl;
#endif

  theEventClass = 0; //!! if no changed = no SPECT

  if( rhitList.size() == 0) return theEventClass;

  GetGoodComptonRecHits( rhitList );

  if( theGoodRecHits.size() == 1 ) {
    
    if( bComptonStudyHistos ) theComptonStudyHistos->FillDist511RecHits(theGoodRecHits);

   theAnaMgr->GetHisto1(320000+5)->Fill( theGoodRecHits[0].GetEnergy()/CLHEP::keV );
    
    theEventClass += 1;

    mapSTV::const_iterator colliIte = GetClosestCollimator( theGoodRecHits[0].GetPosition() );
    G4ThreeVector collimatorCentre = (*colliIte).second;
    GmLine spectLine = GmLine( theGoodRecHits[0].GetPosition(), collimatorCentre-theGoodRecHits[0].GetPosition() );
#ifndef GAMOS_NO_VERBOSE
    if( SPECTVerb(debugVerb) ) G4cout << " SPECTEventClassifierUA::ClassifySPECT  creating line " << spectLine << G4endl;
#endif
    G4ThreeVector eventVtx = GetEventVertex( evt );
    G4double linedist = GetSPECTLineDistanceToVtx( spectLine, eventVtx );
    linedist = 0;
    theEventClass += int64_t( linedist > theSPECTLineDistToVtx );
    FillDCAHistos( linedist );
 
    G4int doge = CheckDifferentOriginalGammaAndEvent( theGoodRecHits );
    theEventClass += doge;
    theEventClass += 1000 * CheckOriginalGammaScattered( theGoodRecHits );
    theEventClass += 10000 * CheckTraverseCollimator( theGoodRecHits, colliIte, doge );
  
    if( theEventClass != 0 ) {
      theEventClassifierMap[9999] ++;
      if( bDump ) {
	SPECTIOMgr::GetInstance()->WriteEvent( G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetPrimaryVertex(0)->GetPosition(), 
					       theGoodRecHits[0].GetPosition(), 
					       collimatorCentre,
					       theGoodRecHits[0].GetEnergy(),
					       theEventClass);
      }
    }
  }

#ifndef GAMOS_NO_VERBOSE
  if( SPECTVerb(infoVerb) ) G4cout << " SPECTEventClassifierUA::ClassifySPECT classification " << theEventClass << G4endl;
#endif

  theEventClassifierMap[theEventClass] ++;
  
  return theEventClass;
}


//----------------------------------------------------------------------
void SPECTEventClassifierUA::GetGoodComptonRecHits( const gamosRecHitList& rhitList )
{
  //----- Join rec hits in sets if they are close in distance ----------
  gamosRecHitList::const_iterator itecl1,itecl2;
  std::set<GmRecHit*>::iterator ites;
  std::set<std::set<GmRecHit*> >::iterator itess;
  std::set<GmRecHit*> recHitUsed;
  std::map<G4double,DetRecHitSet> distEnerComptRecHitList; // store in a map usig as key energy distance to theGammaEnergy, so that they can be efficiently delete if there are more than 1
  //  std::map<GmRecHit*,G4double> enerRecHit_inv;

  do {
    DetRecHitSet comptRecHit;
    GmRecHit* seedRecHit;
    //----- Get seed RecHit as the one with highest energy, not used
    // (assume RecHitBuilder have ordered them by energy)
    for( itecl1 = rhitList.begin(); itecl1 != rhitList.end(); itecl1++ ){
      if( recHitUsed.find( *itecl1 ) == recHitUsed.end() ) {
	seedRecHit = *itecl1;
	recHitUsed.insert(seedRecHit);
	comptRecHit.AddHit(seedRecHit);
#ifndef GAMOS_NO_VERBOSE
	if( SPECTVerb(debugVerb) ) G4cout << "SPECTEventClassifierUA::GetGoodComptonRecHits: insert first not used Rechit in set " << seedRecHit->GetPosition() << " E " << seedRecHit->GetEnergy() << G4endl;
#endif
	break;
      }
    }

    //----- Add all the hits within a certain distance
    if( theComptonRecHitsDist > 0. ){
      for( itecl1 = rhitList.begin(); itecl1 != rhitList.end(); itecl1++ ){
	GmRecHit* newRecHit = *itecl1;
	if( recHitUsed.find( newRecHit ) == recHitUsed.end() && 
	    newRecHit != seedRecHit ) {	
	  G4double dist = (seedRecHit->GetPosition() 
			   - (newRecHit)->GetPosition() ).mag();
	  if( dist < theComptonRecHitsDist ){
	    recHitUsed.insert(newRecHit);
	    //	  enerRecHit_inv[newRecHit] = newRecHit->GetEnergy();
	    //-	  cRecHit_E += (newRecHit)->GetEnergy();
	    comptRecHit.AddHit(newRecHit);
#ifndef GAMOS_NO_VERBOSE
	    if( SPECTVerb(debugVerb) ) G4cout 
		   << "SPECTEventClassifierUA::GetGoodComptonRecHits: " 
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
    if( comptRecHit.GetEnergy() < theEPrecMin || 
	comptRecHit.GetEnergy() > theEPrecMax ){
      for( ites = comptRecHit.begin(); ites != comptRecHit.end(); ites++ ){
	if( ites != comptRecHit.begin() ) {
	  recHitUsed.erase(*ites);
	}
      }
      comptRecHit.Initialise();
#ifndef GAMOS_NO_VERBOSE
      if( SPECTVerb(debugVerb) ) G4cout 
	     << "SPECTEventClassifierUA::GetGoodComptonRecHits: rec hit set Energy BAD " 
	     << comptRecHit.GetEnergy() 
	     << " " << fabs(comptRecHit.GetEnergy() - theGammaEnergy) 
	     << " < " << theEPrecMin << " > " << theEPrecMax << G4endl;
#endif
    } else {
      // GOOD set of RecHits 
      G4double distEner = fabs(comptRecHit.GetEnergy() - theGammaEnergy);
      if( distEnerComptRecHitList.find(distEner) != 
	  distEnerComptRecHitList.end()) distEner *= 1.000001;// avoid problems when two hits have exactly the same energy
      distEnerComptRecHitList[distEner] = comptRecHit;
      comptRecHit.Initialise();
      //-     seedRecHit->SetEnergy(cRecHit_E); // set energy of set to the first rec hit, as this will be the one used in theGoodRecHits
#ifndef GAMOS_NO_VERBOSE
      if( SPECTVerb(debugVerb) ) G4cout 
	     << "SPECTEventClassifierUA::GetGoodComptonRecHits: rec hit set Energy GOOD " 
	     << comptRecHit.GetEnergy() << G4endl;
#endif
    }
  } while( recHitUsed.size() != rhitList.size() );

#ifndef GAMOS_NO_VERBOSE
  if( SPECTVerb(debugVerb) ) G4cout 
	 << "SPECTEventClassifierUA::GetGoodComptonRecHits: List of rec hits set ended " 
	 << distEnerComptRecHitList.size() << G4endl;
#endif

  //----- If no Good rechits found return
  if( distEnerComptRecHitList.size() == 0 ) return;
  
  if( distEnerComptRecHitList.size() > 1 ) {
    if( G4int(distEnerComptRecHitList.size()) >= theNHits2Reject ) return;
    theEventClass += 10;
  }

  //----- Calculate position for sets of rec hits -----

  //--- If there are more than 1 set of rec hits, order them by number of rec hits
  std::map<G4double,DetRecHitSet>::iterator itedss;
  itedss = distEnerComptRecHitList.begin();
  DetRecHitSet recHitSet = (*itedss).second;

  //---- Rec hit set
  G4ThreeVector pos;
#ifndef GAMOS_NO_VERBOSE
  if( SPECTVerb(debugVerb) ) G4cout << "SPECTEventClassifierUA::GetGoodComptonRecHits: RecHitSet initial pos " << recHitSet.GetPosition() << G4endl;
#endif
  //---- Get position if more than 1 rec hit in set
  if( recHitSet.size() > 1 ) {
    // Use the order of the algorithm
    GmRecHit* rhSelected = the1stHitAlgo->Get1stRecHit( recHitSet, recHitSet ); // second set is not used in SPECT
    pos = rhSelected->GetPosition();

    DetRecHitSet::iterator ites3d = recHitSet.begin();    ites3d++;
#ifndef GAMOS_NO_VERBOSE
  if( SPECTVerb(debugVerb) ) G4cout << "SPECTEventClassifierUA::GetGoodComptonRecHits: recHitSet change pos to " << pos << " diff to 1st " << (pos-recHitSet.GetPosition()).mag() << " = " << (recHitSet.GetPosition().x() > (*ites3d)->GetPosition().x() ) <<G4endl;
    if( SPECTVerb(debugVerb) ) recHitSet.Print();
#endif
    recHitSet.SetPosition(pos);
  }

#ifndef GAMOS_NO_VERBOSE
  if( SPECTVerb(debugVerb) ) G4cout << "SPECTEventClassifierUA::GetGoodComptonRecHits: recHitSet  pos final " << recHitSet.GetPosition() << G4endl;
#endif
  //--- Register it as Good rec hit
  theGoodRecHits.push_back( recHitSet );

}
 
//----------------------------------------------------------------------
G4ThreeVector SPECTEventClassifierUA::GetEventVertex( const G4Event* evt )
{
  G4ThreeVector eventVtx;
  //----- Get vertex
  G4int nvtx = evt->GetNumberOfPrimaryVertex();
  for( G4int ii = 0; ii < nvtx; ii++ ){
    G4PrimaryVertex* vtx = evt->GetPrimaryVertex(ii);
    G4int npart = vtx->GetNumberOfParticle();
#ifndef GAMOS_NO_VERBOSE
    if( SPECTVerb(debugVerb) ) G4cout << "SPECTEventClassifierUA::GetEventVertex vertex found " << ii << " npart " << npart << G4endl;
#endif
    for( G4int jj = 0; jj < npart; jj++ ){
      //      if( vtx->GetPrimary(jj)->GetPDGcode() == -11 ) {
      //	foundPositron = TRUE;
	eventVtx = vtx->GetPosition();
#ifndef GAMOS_NO_VERBOSE
	if( SPECTVerb(debugVerb) ) G4cout << "SPECTEventClassifierUA::GetEventVertex part in vertex " << vtx->GetPrimary(jj)->GetPDGcode() << " VTX " << eventVtx << G4endl;
#endif
	//      }
    }
  }

  return eventVtx;
}


//----------------------------------------------------------------------
G4double SPECTEventClassifierUA::GetSPECTLineDistanceToVtx( const GmLine& line, const G4ThreeVector& eventVtx )
{
  if( theGoodRecHits.size() != 1 ) {
    G4Exception("SPECTEventClassifierUA::GetSPECTLineDistanceToVtx:",
		"Number of good rec hits != 1 ",
		FatalErrorInArgument,
		G4String("Number of good recHits = " + GmGenUtils::itoa( theGoodRecHits.size() ) ).c_str());
  }

#ifndef GAMOS_NO_VERBOSE
  if( SPECTVerb(debugVerb) ) G4cout << " SPECTEventClassifierUA::GetSPECTLineDistanceToVtx recHit 0 pos " << theGoodRecHits[0].GetPosition()<< " E " << theGoodRecHits[0].GetEnergy() << G4endl;
#endif

  G4double linedist = line.GetDist( eventVtx );
#ifndef GAMOS_NO_VERBOSE
  if( SPECTVerb(infoVerb) ) G4cout << " SPECTEventClassifierUA::GetSPECTLineDistanceToVtx DIST= " << linedist << " ?< " << theSPECTLineDistToVtx << G4endl;
#endif

#ifndef GAMOS_NO_VERBOSE
  if( linedist > theSPECTLineDistToVtx*2 ) if( SPECTVerb(debugVerb) ) G4cout << " BIG LINEDIST " << linedist << " " << G4EventManager::GetEventManager()->GetNonconstCurrentEvent()->GetEventID() << G4endl;
#endif
  theAnaMgr->GetHisto1(320000+2)->Fill( linedist );

  G4ThreeVector distp = line.GetDistPoint( eventVtx );
  G4ThreeVector distv = eventVtx - distp;

  theAnaMgr->GetHisto1(320000+3)->Fill( distv.z() );

  G4ThreeVector distXY = distv.cross( G4ThreeVector(0.,0.,1.) );
  distXY = distXY.unit();
  G4ThreeVector distvxy(distv.x(), distv.y(),0.);
  //  G4ThreeVector distPHI = distvxy.cross( G4ThreeVector(0.,0.,1.) );
  //  distPHI = distPHI.unit();
  theAnaMgr->GetHisto1(320000+4)->Fill( distv.perp() );
  
  return linedist;

}

//----------------------------------------------------------------------
void SPECTEventClassifierUA::FillDCAHistos( G4double linedist )
{
  //  G4cout << theEventClass << " Fill " << 321000+theEventClass-1 << " " << theAnaMgr->GetHisto1(321000+theEventClass-1)->GetName() << G4endl;
  theAnaMgr->GetHisto1(321000+theEventClass-1)->Fill( linedist );
  theAnaMgr->GetHisto1(331000+theEventClass-1)->Fill( theGoodRecHits[0].GetEnergy()/CLHEP::keV );
  for( unsigned int ii = 1; ii < 5; ii++ ){  
    if( ((theEventClass-1)/int64_t(pow(float(10),int(ii-1)))%10) == 1 ) {
      //      G4cout << " FillALL " << 341000+ii << " " << theAnaMgr->GetHisto1(341000+ii)->GetName() << G4endl;
      theAnaMgr->GetHisto1(341000+ii)->Fill( linedist );
      theAnaMgr->GetHisto1(351000+ii)->Fill( theGoodRecHits[0].GetEnergy()/CLHEP::keV );
    }
  }

}


//----------------------------------------------------------------------
G4bool SPECTEventClassifierUA::CheckDifferentOriginalGammaAndEvent( DetRecHitSetList& rhitsets )
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
      GmRecHit* rhit = *ite;
      G4bool chk = CheckRecHitDifferentOriginalGammaAndEvent( rhit, origGammaAndEvent );
      if( chk ) return TRUE;
      eventRhit.push_back( G4int(origGammaAndEvent/10) );
    }
  }
  
  return FALSE;  
}

//----------------------------------------------------------------------
G4bool SPECTEventClassifierUA::CheckRecHitDifferentOriginalGammaAndEvent( GmRecHit* rhit, G4int& origGammaAndEvent )
{
  if( rhit == 0 ) return FALSE;

  std::vector<GmHit*> hits = rhit->GetHits();
  std::vector<GmHit*>::const_iterator iteh;
  std::set<G4int>::iterator itesi;
#ifndef GAMOS_NO_VERBOSE
  if( SPECTVerb(debugVerb) ) G4cout << origGammaAndEvent << "SPECTEventClassifierUA::CheckRecHitDifferentOriginalGammaAndEvent hits in recHit " << hits.size() << G4endl;
#endif
  //  G4double maxdist = 0.;
  for( iteh = hits.begin(); iteh != hits.end(); iteh++ ){
    std::set<G4int> htidori = (*iteh)->GetOriginalTrackIDs();
#ifndef GAMOS_NO_VERBOSE
    if( SPECTVerb(debugVerb) ) G4cout 
	   << "SPECTEventClassifierUA::CheckRecHitDifferentOriginalGammaAndEvent looping hits n tracks " 
	   << htidori.size() << G4endl;
#endif
    for( itesi = htidori.begin(); itesi != htidori.end(); itesi++ ){
      G4int oge = (*itesi)+10*(*iteh)->GetEventID();
#ifndef GAMOS_NO_VERBOSE
      if( SPECTVerb(debugVerb) ) G4cout << "SPECTEventClassifierUA::CheckRecHitDifferentOriginalGammaAndEvent new track id " << *itesi+10.*(*iteh)->GetEventID() << " ogenew " << oge << " old " << origGammaAndEvent << G4endl;
#endif
      if( origGammaAndEvent != oge ){
	G4bool isDifferentG = TRUE;
	if( origGammaAndEvent == -1 ) {
	  isDifferentG = FALSE;
	} 
	origGammaAndEvent = oge;
#ifndef GAMOS_NO_VERBOSE
	if( SPECTVerb(debugVerb) ) G4cout << "SPECTEventClassifierUA::CheckRecHitDifferentOriginalGammaAndEvent origGammaAndEvent changed " << origGammaAndEvent << G4endl;
#endif
	return isDifferentG;
      }
    }
  }

  return FALSE;

}

//----------------------------------------------------------------------
G4bool SPECTEventClassifierUA::CheckOriginalGammaScattered( DetRecHitSetList& rhitsets )
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
G4bool SPECTEventClassifierUA::CheckRecHitOriginalGammaScattered( GmRecHit* rhit )
{
  if( rhit == 0 ) return FALSE;

  std::vector<GmHit*> hits = rhit->GetHits();
  std::vector<GmHit*>::const_iterator iteh;
  std::set<G4int>::iterator itesi;
#ifndef GAMOS_NO_VERBOSE
  if( SPECTVerb(debugVerb) ) G4cout << "SPECTEventClassifierUA::CheckRecHitOriginalGammaScattered hits in recHit " << hits.size() << G4endl;
#endif

  for( iteh = hits.begin(); iteh != hits.end(); iteh++ ){
    std::set<G4int> htidori = (*iteh)->GetOriginalTrackIDs();
#ifndef GAMOS_NO_VERBOSE
    if( SPECTVerb(debugVerb) ) G4cout << "SPECTEventClassifierUA::CheckRecHitOriginalGammaScattered looping hits n original tracks " << htidori.size() << G4endl;
#endif
    for( itesi = htidori.begin(); itesi != htidori.end(); itesi++ ){
#ifndef GAMOS_NO_VERBOSE
      //-      G4cout << SPECTVerb(5) << " new track id " << *itesi+10.*(*iteh)->GetEventID() << G4endl;
#endif
      if( theCountScattering->GetNScatterOriginalTrack((*itesi) ) ) return TRUE;
    }
  }
  
  return FALSE;

}

//----------------------------------------------------------------------
G4int SPECTEventClassifierUA::CheckTraverseCollimator(DetRecHitSetList& rhitsets, mapSTV::const_iterator colliIteClosest, G4bool diffOrigGamma )
{
  //loop to hits

  std::vector<G4int> eventRhit;
  //----- Loop to all recHits original gammas to check 
  DetRecHitSetList::iterator iterhs;
  std::set<GmRecHit*>::iterator iterh;
  std::vector<GmHit*>::const_iterator iteh;
  std::set<G4int>::iterator itesi;

  for( iterhs = rhitsets.begin(); iterhs != rhitsets.end(); iterhs++ ){
    std::set<GmRecHit*> rhitset = *iterhs;
    for( iterh = rhitset.begin(); iterh != rhitset.end(); iterh++ ){
      //  G4double maxdist = 0.;
      std::vector<GmHit*> hits = (*iterh)->GetHits();
      for( iteh = hits.begin(); iteh != hits.end(); iteh++ ){	
	std::set<G4int> htidori = (*iteh)->GetOriginalTrackIDs();
	for( itesi = htidori.begin(); itesi != htidori.end(); itesi++ ){
	  G4int eventTrk = (*iteh)->GetEventID() * 10 + *itesi;
	  std::map<G4int, mapSTV::const_iterator>::iterator itect = theCollimatorTraversed.find(eventTrk);
#ifndef GAMOS_NO_VERBOSE
	  if( SPECTVerb(debugVerb) ) G4cout 
		 << "SPECTEventClassifierUA::CheckTraverseCollimator eventTrk " << eventTrk << G4endl;
#endif
	  if( itect == theCollimatorTraversed.end() ) { // No collimator traversed
#ifndef GAMOS_NO_VERBOSE
	    if( SPECTVerb(debugVerb) ) G4cout 
		   << "SPECTEventClassifierUA::CheckTraverseCollimator No collimator traversed " << G4endl;
#endif
	    return 2;
	  } else {
#ifndef GAMOS_NO_VERBOSE
	    if( SPECTVerb(debugVerb) ) G4cout 
		   << "SPECTEventClassifierUA::CheckTraverseCollimator real colliTrav "
		   << (*((*itect).second)).first << " ?= closest " << (*colliIteClosest).first 
		   << " " << ((*itect).second != colliIteClosest) << G4endl;
#endif
	    if( (*itect).second != colliIteClosest ) return 1;
	  }
	  if( !diffOrigGamma ) return 0;  // if all come from same gamma/event, do not check the rest
	}
      }
    }
  }

  return 0;  
}

//----------------------------------------------------------------------
void SPECTEventClassifierUA::EndOfRunAction(const G4Run* aRun )
{
  G4int nevt = aRun->GetNumberOfEvent();
  if (nevt == 0) {
    G4cout << "Nevt = 0 !!!" << G4endl;
    return;
  };

  PrintSPECTClassificationTable( nevt );

  if( bDump ) SPECTIOMgr::GetInstance()->CloseFileOut();

}


//----------------------------------------------------------------------
SPAlgo SPECTEventClassifierUA::SelectPosAlgorithm(const G4String& SelectPosStr)
{
  SPAlgo spa;
  if( SelectPosStr == "Energy" ){
    spa = SPByEnergy;
  }else if( SelectPosStr == "XYRadius" || SelectPosStr == "TransaxialPos" ) {
    spa = SPByXYRadius;
  } else if( SelectPosStr == "DistanceTo1st" ){
    spa = SPByDistanceTo1st;
  } else if( SelectPosStr == "ComptonCone" ){
    spa = SPByComptonCone;
  } else if( SelectPosStr == "DistanceToOther" ){
    spa = SPByDistanceToOther;
  } else if( SelectPosStr == "2ComptonCone3XYRadius" ){
    spa = SPBy2ComptonCone3XYRadius;
  } else {
    G4String ErrMsg = " Only available options are: Energy, XYRadius / TransaxialPos, DistanceTo1st, Com\
ptonCone";
    ErrMsg += " You have selected: ";
    ErrMsg += SelectPosStr;
    G4Exception("SPECTEventClassifierUA::SelectPosAlgorithm",
                "Wrong argument to command selecting position algorithm",
                FatalErrorInArgument,
                ErrMsg);
  }

  return spa;
}

//----------------------------------------------------------------------
mapSTV::const_iterator SPECTEventClassifierUA::GetClosestCollimator( G4ThreeVector pos ) 
{
  if( theCollimatorLongNamesAndCentres.size() == 1 ) return theCollimatorLongNamesAndCentres.begin();
  
  mapSTV::const_iterator colliIte;
 
  G4double distMax = DBL_MAX;
  std::map<G4String,G4ThreeVector>::const_iterator ite; 
  for( ite = theCollimatorLongNamesAndCentres.begin(); ite != theCollimatorLongNamesAndCentres.end(); ite++ ){
    G4double dist = ((*ite).second-pos).mag();
    if( dist < distMax ){
      colliIte = ite;
      distMax = dist;
#ifndef GAMOS_NO_VERBOSE
      if( SPECTVerb(debugVerb) ) G4cout << " SPECTEventClassifierUA::GetClosestCollimator centre " << (*ite).first << " " << (*ite).second << " dist " << distMax << G4endl;
#endif
    }
  }

  return colliIte;

}


//----------------------------------------------------------------------
void SPECTEventClassifierUA::UserSteppingAction( const G4Step* aStep )
{
  const G4Track* aTrack = aStep->GetTrack();
  if( !GmCheckOriginalGamma::CheckIfOriginalGamma( aTrack ) ) return;
  
  if( aTrack->GetVolume()->GetLogicalVolume() == theCollimatorLogVol ) {
    G4String touchName = GmGeometryUtils::GetInstance()->GetG4TouchableLongName( (G4TouchableHistory*)(aTrack->GetTouchable()) );
    std::map<G4String,G4ThreeVector>::iterator ite = theCollimatorLongNamesAndCentres.find( touchName );
    if( ite == theCollimatorLongNamesAndCentres.end() ) {
      G4cout << " theCollimatorLongNamesAndCentres " << (*(theCollimatorLongNamesAndCentres.begin())).first << G4endl;
      G4Exception("SPECTEventClassifierUA::UserSteppingAction",
		  "Problem in touchable name. This should not happen, please submit a bug report ",
		  FatalErrorInArgument,
		  G4String("Volume name traversed = " + touchName).c_str());      
    }
    
    theCollimatorTraversed[G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID()*10+aTrack->GetTrackID()] = ite;
    
#ifndef GAMOS_NO_VERBOSE
    if( SPECTVerb(debugVerb) ) G4cout << " SPECTEventClassifierUA::UserSteppingAction  insert collimatorTraversed  eventID " << G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID()*10+aTrack->GetTrackID() << " collID " << std::distance( theCollimatorLongNamesAndCentres.begin(), ite ) << " " << (*ite).first << G4endl;
#endif
    
  }

}


//----------------------------------------------------------------------
void SPECTEventClassifierUA::CleanCollimatorsTraversed()
{
  gamosSDHitMap hits = theHitsEventMgr->GetHitLists();
  gamosSDHitMap::const_iterator iteh;
  GmHitList::const_iterator itehl;

  std::set<G4int> theEvents;
  for( iteh = hits.begin(); iteh != hits.end(); iteh++ ){
    GmHitList* hitlist = (*iteh).second;
    for( itehl = hitlist->begin(); itehl != hitlist->end(); itehl++ ){
      theEvents.insert( (*itehl)->GetEventID() );
      //      G4cout << " SPECTEventClassifierUA::CleanCollimatorsTraversed  theEvents " << (*itehl)->GetEventID() << G4endl;
    }
  }

  std::map<G4int, mapSTV::const_iterator> collimatorTraversedTemp;
  std::map<G4int, mapSTV::const_iterator>::const_iterator itect;
  for( itect = theCollimatorTraversed.begin(); itect != theCollimatorTraversed.end(); itect++ ){
    if( theEvents.find( (*itect).first/10 ) != theEvents.end() ) {
      collimatorTraversedTemp.insert( *itect );
    }
    //    G4cout << " SPECTEventClassifierUA::CleanCollimatorsTraversed  theCollimatorTraversed event " <<  (*itect).first/10 << " events size " <<  theEvents.size() << G4endl;
  }

#ifndef GAMOS_NO_VERBOSE
  if( SPECTVerb(debugVerb) ) G4cout << " SPECTEventClassifierUA::CleanCollimatorsTraversed  deleting " << theCollimatorTraversed.size() - collimatorTraversedTemp.size() << " out of " << theCollimatorTraversed.size() << G4endl;
#endif

  theCollimatorTraversed = collimatorTraversedTemp;

}

//----------------------------------------------------------------------
void SPECTEventClassifierUA::PrintSPECTClassificationTable( int64_t nevt )
{
  std::map<unsigned int, unsigned int>::iterator ite;
  G4int nGOOD = 0;
  G4int nCOINC = 0;
  G4int nSCATT = 0;
  G4int nBADCOLL = 0;
  G4int nNOCOLL = 0;
  G4int nCOINC_SCATT = 0;
  for( ite = theEventClassifierMap.begin(); ite !=  theEventClassifierMap.end(); ite++ ){
    unsigned int evid = (*ite).first;
    if( evid == 0 ) continue;
    if( evid/100 == 0 ) nGOOD += (*ite).second;
    if( evid/100 % 10 == 1 ) nCOINC += (*ite).second;
    if( evid/1000 % 10 == 1 ) nSCATT += (*ite).second;
    if( evid/10000 % 10 == 1 ) nBADCOLL += (*ite).second;
    if( evid/10000 % 10 == 2 ) nNOCOLL += (*ite).second;
    if( evid/100 % 10 == 1 && evid/1000 % 10 == 1 ) nCOINC_SCATT += (*ite).second;
  }

  G4double b_01 = theEventClassifierMap[1]; // SPECT event, 1 recHit of theGammaEnergy CLHEP::keV, LOR < 4mm from source
  G4double b_02 = theEventClassifierMap[2]; // SPECT event, 1 recHit of theGammaEnergy CLHEP::keV, LOR > 4mm from source
  
  G4double b_11 = theEventClassifierMap[11]; // SPECT event, 2->1 recHits of theGammaEnergy CLHEP::keV, LOR < 4mm from source
  G4double b_12 = theEventClassifierMap[12]; // SPECT event, 2->1 recHits of theGammaEnergy CLHEP::keV, LOR > 4mm from source
  

  // Print table for SPECT
  G4double b_SPECT = theEventClassifierMap[9999];
  G4cout << " -------------------------------------------" << G4endl;
  G4cout << " $$SPECT Events                        : " << b_SPECT << "   " << b_SPECT/nevt*100 << " % " << G4endl;
  //  G4cout << " Events SPECT               :  " << nevt-b_00 << "   " << (nevt-b_00)/nevt*100 << " % " << G4endl;

  G4cout << " ---------------- Good SPECT --------------" << G4endl;
  G4cout << "$$SPECT ALL GOOD                         : " << nGOOD << "   " << float(nGOOD)/nevt*100 << " % " << G4endl; 
  G4cout << "(    1) $$SPECT 1 recHit close to vtx    : " << b_01 << "   " << b_01/nevt*100 << " % " << G4endl; 
  G4cout << "(    2) $$SPECT 1 recHit far from vtx    : " << b_02 << "   " << b_02/nevt*100 << " % " << G4endl;
  G4cout << "(   11) $$SPECT 2->1 recHit close to vtx : " << b_11 << "   " << b_11/nevt*100 << " % " << G4endl;
  G4cout << "(   12) $$SPECT 2->1 recHit far from vtx : " << b_12 << "   " << b_12/nevt*100 << " % " << G4endl;
  
  // recHits with hits from different original gammas
  G4double b_101 = theEventClassifierMap[101]; // SPECT event, 1 recHit of theGammaEnergy CLHEP::keV LOR < 4mm from source
  G4double b_102 = theEventClassifierMap[102]; // SPECT event, 1 recHit of theGammaEnergy CLHEP::keV, LOR > 4mm from source
  
  G4double b_111 = theEventClassifierMap[111]; // SPECT event, 2->1 recHits of theGammaEnergy CLHEP::keV, LOR < 4mm from source
  G4double b_112 = theEventClassifierMap[112]; // SPECT event, 2->1 recHits of theGammaEnergy CLHEP::keV, LOR > 4mm from source
  
  G4cout << " --------------- Random Coincidences -------------" << G4endl;
  G4cout << "$$SPECT  ALL RANDOM COINCIDENCES          : " << nCOINC << "   " << float(nCOINC)/nevt*100 << " % " << G4endl; 
  G4cout << "(  101) $$SPECT 1 recHit close to vtx    : " << b_101 << "   " << b_101/nevt*100 << " % " << G4endl; 
  G4cout << "(  102) $$SPECT 1 recHit far from vtx    : " << b_102 << "   " << b_102/nevt*100 << " % " << G4endl;
  G4cout << "(  111) $$SPECT 2->1 recHit close to vtx : " << b_111 << "   " << b_111/nevt*100 << " % " << G4endl;
  G4cout << "(  112) $$SPECT 2->1 recHit far from vtx : " << b_112 << "   " << b_112/nevt*100 << " % " << G4endl;

  // recHits with hits from different original gammas
  G4double b_1001 = theEventClassifierMap[1001]; // SPECT event, 1 recHit of theGammaEnergy CLHEP::keV, LOR < 4mm from source
  G4double b_1002 = theEventClassifierMap[1002]; // SPECT event, 1 recHit of theGammaEnergy CLHEP::keV, LOR > 4mm from source
  
  G4double b_1011 = theEventClassifierMap[1011]; // SPECT event, 2->1 recHits of theGammaEnergy CLHEP::keV, LOR < 4mm from source
  G4double b_1012 = theEventClassifierMap[1012]; // SPECT event, 2->1 recHits of theGammaEnergy CLHEP::keV, LOR > 4mm from source
  
  G4cout << " --------------- Scattered ---------------" << G4endl;
  G4cout << "$$SPECT  ALL SCATTERED                   : " << nSCATT << "   " << float(nSCATT)/nevt*100 << " % " << G4endl; 
  G4cout << "( 1001) $$SPECT 1 recHit close to vtx    : " << b_1001 << "   " << b_1001/nevt*100 << " % " << G4endl; 
  G4cout << "( 1002) $$SPECT 1 recHit far from vtx    : " << b_1002 << "   " << b_1002/nevt*100 << " % " << G4endl;
  G4cout << "( 1011) $$SPECT 2->1 recHit close to vtx : " << b_1011 << "   " << b_1011/nevt*100 << " % " << G4endl;
  G4cout << "( 1012) $$SPECT 2->1 recHit far from vtx : " << b_1012 << "   " << b_1012/nevt*100 << " % " << G4endl;
  
  // recHits with hits from different original gammas
  G4double b_1101 = theEventClassifierMap[1101]; // SPECT event, 1 recHit of theGammaEnergy CLHEP::keV, LOR < 4mm from source
  G4double b_1102 = theEventClassifierMap[1102]; // SPECT event, 1 recHit of theGammaEnergy CLHEP::keV, LOR > 4mm from source
  
  G4double b_1111 = theEventClassifierMap[1111]; // SPECT event, 2->1 recHits of theGammaEnergy CLHEP::keV, LOR < 4mm from source
  G4double b_1112 = theEventClassifierMap[1112]; // SPECT event, 2->1 recHits of theGammaEnergy CLHEP::keV, LOR > 4mm from source
  
  G4cout << " --------------- Random Coincidences and Scattered -------------" << G4endl;
  G4cout << "$$SPECT  ALL COINC. AND SCCATTERED       : " << nCOINC_SCATT << "   " << float(nCOINC_SCATT)/nevt*100 << " % " << G4endl; 
  G4cout << "( 1101) $$SPECT 1 recHit close to vtx    : " << b_1101 << "   " << b_1101/nevt*100 << " % " << G4endl; 
  G4cout << "( 1102) $$SPECT 1 recHit far from vtx    : " << b_1102 << "   " << b_1102/nevt*100 << " % " << G4endl;
  G4cout << "( 1111) $$SPECT 2->1 recHit close to vtx : " << b_1111 << "   " << b_1111/nevt*100 << " % " << G4endl;
  G4cout << "( 1112) $$SPECT 2->1 recHit far from vtx : " << b_1112 << "   " << b_1112/nevt*100 << " % " << G4endl;
  G4cout << " ---------------------------------------------" << G4endl;

  // recHits with hits from different original gammas
  G4double b_10001 = theEventClassifierMap[10001]; // SPECT event, 1 recHit of theGammaEnergy CLHEP::keV, LOR < 4mm from source
  G4double b_10002 = theEventClassifierMap[10002]; // SPECT event, 1 recHit of theGammaEnergy CLHEP::keV, LOR > 4mm from source
  
  G4double b_10011 = theEventClassifierMap[10011]; // SPECT event, 2->1 recHits of theGammaEnergy CLHEP::keV, LOR < 4mm from source
  G4double b_10012 = theEventClassifierMap[10012]; // SPECT event, 2->1 recHits of theGammaEnergy CLHEP::keV, LOR > 4mm from source

  G4cout << " --------------- Wrong collimator assignment -------------" << G4endl;
  G4cout << "$$SPECT  ALL WRONG COLLIMATOR            : " << nBADCOLL << "   " << float(nBADCOLL)/nevt*100 << " % " << G4endl; 
  G4cout << "(10001) $$SPECT 1 recHit close to vtx    : " << b_10001 << "   " << b_10001/nevt*100 << " % " << G4endl; 
  G4cout << "(10002) $$SPECT 1 recHit far from vtx    : " << b_10002 << "   " << b_10002/nevt*100 << " % " << G4endl;
  G4cout << "(10011) $$SPECT 2->1 recHit close to vtx : " << b_10011 << "   " << b_10011/nevt*100 << " % " << G4endl;
  G4cout << "(10012) $$SPECT 2->1 recHit far from vtx : " << b_10012 << "   " << b_10012/nevt*100 << " % " << G4endl;

  G4double b_20001 = theEventClassifierMap[20001]; // SPECT event, 1 recHit of theGammaEnergy CLHEP::keV, LOR < 4mm from source
  G4double b_20002 = theEventClassifierMap[20002]; // SPECT event, 1 recHit of theGammaEnergy CLHEP::keV, LOR > 4mm from source
  
  G4double b_20011 = theEventClassifierMap[20011]; // SPECT event, 2->1 recHits of theGammaEnergy CLHEP::keV, LOR < 4mm from source
  G4double b_20012 = theEventClassifierMap[20012]; // SPECT event, 2->1 recHits of theGammaEnergy CLHEP::keV, LOR > 4mm from source

  G4cout << " --------------- NO collimator traversed -------------" << G4endl;
  G4cout << "$$SPECT  ALL NO COLLIMATOR               : " << nNOCOLL << "   " << float(nBADCOLL)/nevt*100 << " % " << G4endl; 
  G4cout << "(20001) $$SPECT 1 recHit close to vtx    : " << b_20001 << "   " << b_20001/nevt*100 << " % " << G4endl; 
  G4cout << "(20002) $$SPECT 1 recHit far from vtx    : " << b_20002 << "   " << b_20002/nevt*100 << " % " << G4endl;
  G4cout << "(20011) $$SPECT 2->1 recHit close to vtx : " << b_20011 << "   " << b_20011/nevt*100 << " % " << G4endl;
  G4cout << "(20012) $$SPECT 2->1 recHit far from vtx : " << b_20012 << "   " << b_20012/nevt*100 << " % " << G4endl;


  /*  if( G4bool( GmParameterMgr::GetInstance()->GetNumericValue("SPECT:EvtClass:DumpAllTable",1) ) ) {
    G4cout << " ----- FULL SPECT EVENT CLASSIFICATION TABLE -----" << G4endl;
    G4cout << "  ID    N EVENTS" << G4endl;
    for( ite = theEventClassifierMap.begin(); ite !=  theEventClassifierMap.end(); ite++ ){
      if( (*ite).second != 0 ) G4cout << std::setw(6) << (*ite).first << " = " << (*ite).second << G4endl;
    }
    }*/
}  
