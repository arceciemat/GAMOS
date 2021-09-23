#include "GmGeneratorMultiIsotopes.hh"
#include "GmGenerVerbosity.hh"
#include "GmMultiIsotopesSource.hh"
#include "GmGenerDistTimeConstantChangeRandom.hh"

#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"
#ifndef WIN32 //WINDEB
#include "GamosCore/GamosSD/include/GmHitsEventMgr.hh"
#endif
#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4RunManager.hh"

//------------------------------------------------------------------------
GmGeneratorMultiIsotopes::GmGeneratorMultiIsotopes()
{
  GmParameterMgr* parmgr = GmParameterMgr::GetInstance(); 
  theProtonProduction = parmgr->GetNumericValue("GmGeneratorMultiIsotopes:ProtonProduction", -1.);
  if( theProtonProduction == -1. ) G4Exception("GmGeneratorMultiIsotopes::GmGeneratorMultiIsotopes",
			   "",
			   FatalErrorInArgument,
			   "No proton production define, please use parameter 'GmGeneratorMultiIsotopes:ProtonProduction'");
  thePreviousProductionTime = parmgr->GetNumericValue("GmGeneratorMultiIsotopes:PreviousProductionTime", 0.);
  thePreviousDecayTime = parmgr->GetNumericValue("GmGeneratorMultiIsotopes:PreviousDecayTime", 0.);

  G4String fnameisp = parmgr->GetStringValue("GmGeneratorMultiIsotopes:IsotopeProductionFileName", "isotopeProd.lis");
  BuildIsotopeList( fnameisp );

  G4String fnameIsots = parmgr->GetStringValue("GmGeneratorMultiIsotopes:IsotopesSourceFileListName", "isotopeFiles.lis");
  BuildIsotopeSources( fnameIsots );
  
  G4String fnameGamma = parmgr->GetStringValue("GmGeneratorMultiIsotopes:GammaSourceFileName", "");
  if( fnameGamma != "" ) BuildGammaSource( fnameGamma );

  bFirstGammaAsPositrons = G4bool(parmgr->GetNumericValue("GmGeneratorMultiIsotopes:FirstGammaAsPositrons", 1));
  bPrintEventTime = G4bool(parmgr->GetNumericValue("GmGeneratorMultiIsotopes:PrintEventTime", 0));
  theFirstEventTime = 0.;
  
  bFirstEvent = true;

  theLastEventTime = 0.;

  theGammaSource = 0;
}

//------------------------------------------------------------------------
void GmGeneratorMultiIsotopes::BuildIsotopeList( G4String fname )
{
  //--- GET ISOTOPE NAME TO Z 
  GmFileIn finnz = GmFileIn::GetInstance( GmGenUtils::FileInPath( "isotNamesToZ.lis" ),true);
  std::map<G4String,G4int> elemNameToZ;
  std::vector<G4String> wl;
  for( ;; ){
    if( !finnz.GetWordsInLine(wl) ) break;
    elemNameToZ[wl[0]] = GmGenUtils::GetInt(wl[1]);
  }
  
  //---- STORE DIRECTORY WHERE RADIACTIVE DATA IS
  G4String radDir(getenv("G4RADIOACTIVEDATA") );
  
  //---- LOOP TO ISOTOPE LIST
  //  ISOT K37 0.000516111111111
  //  ISOT K36 1.34444444444e-05
  // check if file exists
  GmGenerIsotopeData isotd;
  GmFileIn fin = GmFileIn::GetInstance(fname,true);
  if( GenerVerb(testVerb) )  G4cout << " READING FILE " << fname << G4endl;
  for( ;; ){
    if( !fin.GetWordsInLine(wl) ) break;
    if( GenerVerb(testVerb) ) G4cout << " BuildIsotopeList " << wl[1] << " " << wl.size() << G4endl; 
    if( wl.size() != 3 || wl[0] != "ISOT" ) {
      G4cerr << "READING ";  for(size_t ii=0;ii<wl.size();ii++ ) G4cerr << wl[ii] << " "; G4cerr << G4endl;
      G4Exception("GmGeneratorMultiIsotopes::BuildIsotopeList",
		  "",
		  FatalException,
		  ("Wrong file: "+fname+ ", it should contain three-word lines: 'ISOT' ISOT_NAME ISOT_PER_PROTON").c_str());
    }
    G4String isotName = wl[1];
    isotd.name= isotName;
    if( wl[1] != "gamma" ) {
      //--- EXTRACT DECAY HALFLIFE FROM G4RADIOACTIVEDATA FILE
      //-- DIVIDE ISOT NAME
      G4String elemName = "";
      G4int isotA = 0;
      G4double excit = 0.;
      G4String isotNameNoExcit = isotName;
      size_t ipar = isotName.find("[");
      if( ipar != std::string::npos ) {
	size_t ipar2 = isotName.find("]");
	excit = GmGenUtils::GetValue(isotNameNoExcit.substr(ipar+1,ipar2-ipar-1));
	isotNameNoExcit = isotNameNoExcit.substr(0,ipar);
      }
      if( GenerVerb(testVerb) ) G4cout <<" ISOTNAME " << isotName << " -> " << isotNameNoExcit << G4endl; 
      for( size_t ic = 0; ic < isotNameNoExcit.length(); ic++ ) {
	//      G4cout << ic << " ISINTEGER  " << isotName[ic] << " : " << GmGenUtils::IsInteger(isotName[ic]) << G4endl; //GDEB
	if( GmGenUtils::IsInteger(isotNameNoExcit[ic]) ) {
	  isotA = GmGenUtils::GetInt(isotNameNoExcit.substr(ic,isotNameNoExcit.length()));
	  break;
	} else {
	  elemName += isotNameNoExcit[ic];
	}
	//      G4cout << ic << " ISOTA " << isotA << " ELEMNAME " << elemName << G4endl; //GDEB
      }
      G4int isotZ = elemNameToZ[elemName];
      if( GenerVerb(testVerb) ) G4cout << " ISOTA " << isotA << " ISOTZ " << isotZ << G4endl;
      G4String radDecName = radDir+"/z"+GmGenUtils::itoa(isotZ)+".a"+GmGenUtils::itoa(isotA);
      std::ifstream fint(radDecName);
      if( fint.is_open() ) {
	GmFileIn finRad = GmFileIn::GetInstance(radDir+"/z"+GmGenUtils::itoa(isotZ)+".a"+GmGenUtils::itoa(isotA),true);
	G4double halfLife = 0.;
	std::vector<G4String> wl2;
	for( ;; ){
	  if( !finRad.GetWordsInLine(wl2) ) break;
	  if( wl2[0] == "P" ) {
	    if( GmGenUtils::GetValue(wl2[1]) == excit ) {
	      halfLife = GmGenUtils::GetValue(wl2[3]);
	      break;
	    }
	  }
	}
	isotd.lifeTime = halfLife/log(2.)*CLHEP::second;
	if( GenerVerb(testVerb) ) G4cout << " LIFETIME " << isotd.lifeTime << " HALFLIFE " << halfLife << G4endl;
      } else {  // There is an excited state decaying inmediately with a gamma emission, but base state is not radioactive
	isotd.lifeTime = 0.; // 1.e-99;
      }
    }
    isotd.prodPerTime = GmGenUtils::GetValue(wl[2])*theProtonProduction;
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << " GmGeneratorMultiIsotopes::BuildIsotopeList " << isotd.name << " lifeTime= " << isotd.lifeTime << " prodPerTime " << isotd.prodPerTime << G4endl; 
#endif
    theIsotopeDatae[isotName] = isotd;
    if( GenerVerb(testVerb) ) G4cout << " ADDED theIsotopeDatae " << isotName << " " << isotd.name << G4endl;

  }

  if( theIsotopeDatae.size() == 0 ) {
    G4Exception(" GmGeneratorMultiIsotopes::BuildIsotopeList",
		"",
		FatalException,
		("FILE "+fname+" IS EMPTY").c_str());
  }

}

//------------------------------------------------------------------------
void GmGeneratorMultiIsotopes::BuildIsotopeSources( G4String fnamesfl )
{
  G4bool bRecycle = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmGeneratorMultiIsotopes:Recycle", 1));
  bOnlyDecay = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmGeneratorMultiIsotopes:OnlyDecay", 0));
  GmFileIn flIn = GmFileIn::GetInstance(fnamesfl,true);
  std::vector<G4String> wl;
  for( ;; ){
    if( !flIn.GetWordsInLine(wl) ) break;
    G4cout << fnamesfl << " LOOP TO ISOTOPE FILES " << wl[0] << G4endl; //GDEB
    //-- FIND THE ISOTOPE IN THE NAME
    std::map<G4String,GmGenerIsotopeData>::iterator itei;
    G4String fName;
    for( itei = theIsotopeDatae.begin(); itei != theIsotopeDatae.end(); itei++ ) {
      G4String isotName = itei->second.name;
      //      G4cout << " LOOP theIsotopeDatae " << isotName << " " << std::distance(theIsotopeDatae.begin(),itei) << " " << G4endl; //GDEB
      // FILE NAMES DO NOT CONTAIN [ ], but : :
      size_t ibra = isotName.find('[');
      if( ibra != std::string::npos ) {
	isotName = isotName.replace(ibra,1,":");
	isotName = isotName.replace(isotName.find(']'),1,":");
      }
      fName = wl[0];
      if( fName.find(isotName) != std::string::npos ) {
	// CHECK IT IS NOT AN EXCITED STATE
	//	if( fName.find(":") * isotName.find(":") >= 0 ) {
	if( (fName.find(":") != std::string::npos && isotName.find(":") != std::string::npos)
	    || (fName.find(":") == std::string::npos && isotName.find(":") == std::string::npos ) ) {
	  //CREATE SOURCE
#ifndef GAMOS_NO_VERBOSE
	  if( GenerVerb(debugVerb) ) G4cout << " GmGeneratorMultiIsotopes::BuildIsotopeSources " << itei->second.name << "=" << isotName << " " << fName << G4endl; 
#endif
	  GmMultiIsotopesSource* source = new GmMultiIsotopesSource("GmMultiIsotopesSource_"+isotName);
	  G4bool bSourceOK = true;
	  source->SetRecycleFile(bRecycle);
	  if( GenerVerb(testVerb) ) G4cout << " BuildIsotopeSources CREATE theMISources " << "GmMultiIsotopesSource_"+isotName << G4endl;
	  source->SetIsotopeData(itei->second);

	  if( !bOnlyDecay ) {
	    if( GenerVerb(testVerb) ) G4cout << " CALL BuildGenerTimeDecayProd " << source << G4endl;
	    source->BuildGenerTimeDecayProd(thePreviousProductionTime);
	  } else {
	    if( GenerVerb(testVerb) ) G4cout << " CALL BuildGenerTimeDecay " << source << G4endl;
	    bSourceOK = source->BuildGenerTimeDecay(thePreviousProductionTime, thePreviousDecayTime);
	  }
	  GmFileIn fin = GmFileIn::GetInstance(fName,true);
	  fin.SetSeparator(',');
	  source->SetFileIn(fin);
	  if( bSourceOK ) theMISources.push_back( source );
	  G4double nEventsInFile = GmGenUtils::GetInt(wl[1]);
	  source->SetNEventsInFile(nEventsInFile);
	  break;
	}
      }
    }
    if( itei == theIsotopeDatae.end() ) {
      G4Exception("GmGeneratorMultiIsotopes::BuildIsotopeSources",
		  "",
		  FatalException,
		  ("Wrong file: "+fName+ ", it does not correspond to any isotope in GAMOS parameter 'GmGeneratorMultiIsotopes:IsotopeProductionFileName'").c_str());
    }	
  }

  if( theMISources.size() == 0 ) {
    G4Exception(" GmGeneratorMultiIsotopes::BuildIsotopeSources",
		"",
		FatalException,
		("FILE "+fnamesfl+" IS EMPTY").c_str());
  }
}

//------------------------------------------------------------------------
void GmGeneratorMultiIsotopes::BuildGammaSource( G4String fName )
{
  G4bool bRecycle = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmGeneratorMultiIsotopes:Recycle", 1));
  GmFileIn fin = GmFileIn::GetInstance(fName,true);

  //-- FIND THE GAMMA ISOTOPE DATA
  std::map<G4String,GmGenerIsotopeData>::const_iterator itei = theIsotopeDatae.find("gamma");
  if( itei == theIsotopeDatae.end() ) {
    G4String fnameisp = GmParameterMgr::GetInstance()->GetStringValue("GmGeneratorMultiIsotopes:IsotopeProductionFileName", "isotopeProd.lis");
    G4Exception("GmGeneratorMultiIsotopes::BuildGammaSource",
		"",
		FatalException,
		("No gamma in "+fnameisp).c_str());
  } 
  GmMultiIsotopesSource* source = new GmMultiIsotopesSource("GmMultiIsotopesSource_gamma");
  source->SetRecycleFile(bRecycle);
  theMISources.push_back( source );
  theGammaSource = source;
  source->SetIsotopeData(itei->second);   
  source->BuildGenerTimeConstantChangeRandom(thePreviousProductionTime+thePreviousDecayTime);
  fin.SetSeparator(',');
  source->SetFileIn(fin);
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGeneratorMultiIsotopes::BuildGammaSource " << fName << " PRODPERPROTON " << itei->second.prodPerTime << G4endl; 
#endif
}

//------------------------------------------------------------------------
GmGeneratorMultiIsotopes::~GmGeneratorMultiIsotopes()
{
  //  theFileIn.Close();
}

//------------------------------------------------------------------------
void GmGeneratorMultiIsotopes::GeneratePrimaries(G4Event* evt)
{
  std::vector<GmMultiIsotopesSource*>::iterator ite;
  std::set<GmMultiIsotopesSource*> currentSources;
  std::set<GmMultiIsotopesSource*>::const_iterator ite2;
  //----- Select smaller time
  G4double timeMin = DBL_MAX;
  G4double timeMinPositrons = DBL_MAX;
  G4double timeGamma = 0;
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "GmGeneratorMultiIsotopes::GeneratePrimaries, Nsources " << theMISources.size() << G4endl;
#endif
  std::multimap<G4double, GmMultiIsotopesSource*> times;
  for( ite = theMISources.begin(); ite != theMISources.end(); ite++ ){
    //    G4cout << " GeneratePrimaries LOOPING theMISources " << (*ite)->GetName() << G4endl; //GDEB
    GmMultiIsotopesSource* source = *ite;
    source->SetWeight( 1. );
    G4double tim = source->GenerateTime();
    if( bFirstEvent && bFirstGammaAsPositrons ) {
      if( source == theGammaSource ) {
	timeGamma = tim;
      } else {
	if( tim < timeMinPositrons ) timeMinPositrons = tim;
      }
    } else {
      if( tim < timeMin ) timeMin = tim;
    }
    times.insert(std::multimap<G4double, GmMultiIsotopesSource*>::value_type( tim, *ite));
    #ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << *ite << " GmGeneratorMultiIsotopes::GeneratePrimaries, source " << (*ite)->GetName() << " time proposed " << tim << " <? " << timeMin << G4endl;
    #endif
  }
  if( bFirstEvent && bFirstGammaAsPositrons ) {
    timeMin = timeMinPositrons;
  }

  //---- For the current source set last time to 0. and for the other sources add the time to the last time active
  std::multimap<G4double, GmMultiIsotopesSource*>::iterator itets;
  if( theGammaSource ) {
    if( bFirstEvent && bFirstGammaAsPositrons ) { // avoid first gamma event to be counted twice
      std::multimap<G4double, GmMultiIsotopesSource*>::iterator itetsf = times.find(timeGamma);
      if( theGammaSource != itetsf->second ) {
	G4Exception("GmGeneratorMultiIsotopes::GeneratePrimaries",
		    "",
		    FatalException,
		    ("Source with timeGamma is not gamma source, but "+itetsf->second->GetName()+ ", please contact GAMOS developers").c_str());
      }
      times.erase(itetsf);
      //-- Set gamma time to the one after timeMinPositrons
      GmGenerDistTimeConstantChangeRandom* gammaTD = dynamic_cast<GmGenerDistTimeConstantChangeRandom*>(theGammaSource->GetTimeDistribution());
      G4double interval = gammaTD->GetInterval();
      G4double gammaTime = interval*(G4int(timeMinPositrons/interval)+1);
      theGammaSource->SetTime(gammaTime);
      times.insert(std::multimap<G4double, GmMultiIsotopesSource*>::value_type(gammaTime, theGammaSource));    
    }
  }

  for( itets = times.begin(); itets != times.end() ; itets++ ){
    G4double tim = (*itets).first;
    GmMultiIsotopesSource* source = (*itets).second;
    if( tim <= timeMin ) { // select both sources

#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(debugVerb) ) G4cout << "GmGeneratorMultiIsotopes::GeneratePrimaries, in event " << evt->GetEventID() << " add source " << source->GetName() << " time proposed " << tim << " <? " << timeMin << G4endl;
#endif
      currentSources.insert( source );
      source->LastTimeActive(0.);
    } else {
      source->LastTimeNotActive( timeMin );
    }
  }
  //#ifndef WIN32 //WINDEB
  if( theLastEventTime == 0. ) {
    if ( bOnlyDecay ) {
      theLastEventTime += thePreviousDecayTime;
      GmHitsEventMgr::GetInstance()->SetEventTime(thePreviousDecayTime); // thePreviousProductionTime is only used to get the number of isotopes produced before starting the decay
      G4cout << " GmHitsEventMgr::GetInstance()->SetEventTime( " << thePreviousDecayTime << G4endl; //GDEB
      theLastEventTime = thePreviousDecayTime;
    } else {
      theLastEventTime = thePreviousProductionTime;
      GmHitsEventMgr::GetInstance()->SetEventTime(thePreviousProductionTime+thePreviousDecayTime );
      G4cout << " GmHitsEventMgr::GetInstance()->SetEventTime( " << thePreviousProductionTime+thePreviousDecayTime << G4endl; //GDEB
    }
  }
  //#endif
  theLastEventTime += timeMin;

  if( bPrintEventTime ) {
    if ( theFirstEventTime == 0.) theFirstEventTime = theLastEventTime;
    //    G4cout << "GmGeneratorMultiIsotopes::GeneratePrimaries  event_time= " << theLastEventTime << " increase= " << timeMin << " FirstEventTime " << theFirstEventTime << G4endl;
    G4cout << "GmGeneratorMultiIsotopes::GeneratePrimaries  event_time= " << theLastEventTime-theFirstEventTime << " increase= " << timeMin << " FirstEventTime " << theFirstEventTime << G4endl;
  }

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "GmGeneratorMultiIsotopes::GeneratePrimaries  event time= " << theLastEventTime << " increase= " << timeMin << " N_SOURCES= " << currentSources.size() << G4endl;
#endif

  for( ite2 = currentSources.begin(); ite2 != currentSources.end(); ite2++ ){
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << "GmGeneratorMultiIsotopes::GenerateVertices of source " <<  (*ite2)->GetName() << G4endl;
#endif
    std::vector<G4PrimaryVertex*> vtxs = (*ite2)->GenerateVertices( theLastEventTime );
    for( size_t iv = 0; iv < vtxs.size(); iv++ ){
      G4PrimaryVertex* vtx = vtxs[iv];
      evt->AddPrimaryVertex( vtx );
    }
  }

  bFirstEvent = false;

}

//------------------------------------------------------------------------
void GmGeneratorMultiIsotopes::GenerateParticle( const GenerTextFileData& evtDat, G4Event* evt )
{
  G4PrimaryVertex* vtx = new G4PrimaryVertex( G4ThreeVector(evtDat.posx,evtDat.posy,evtDat.posz), evtDat.time );
  evt->AddPrimaryVertex( vtx );
  G4ParticleDefinition* partDef = GmGetParticleMgr::GetInstance()->GetG4Particle( evtDat.partName, false );
  if( !partDef ) {
    GmGetParticleMgr::GetInstance()->CreateIon( evtDat.partName );
    partDef = GmGetParticleMgr::GetInstance()->GetG4Particle( evtDat.partName, true );
  }

#ifndef GAMOS_NO_VERBOSE
  if( !partDef ) if( GenerVerb(debugVerb) ) G4cout << " partDef " << partDef << " partName " << evtDat.partName << G4endl;
#endif
  G4PrimaryParticle* particle = new G4PrimaryParticle( partDef, evtDat.momx, evtDat.momy, evtDat.momz);
  particle->SetWeight( evtDat.weight );  

  vtx->SetPrimary(particle);

}
