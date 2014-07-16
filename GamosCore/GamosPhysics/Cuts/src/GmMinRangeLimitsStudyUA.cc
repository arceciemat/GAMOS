#include "G4Run.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4ParticleDefinition.hh"
#include "G4VProcess.hh"
#include "G4RegionStore.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

#include "GmCSTrackStepInfo.hh"
#include "GmMinRangeLimitsStudyUA.hh"
#include "GmMinRangeLimitsStudyCreateInfoUA.hh"
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include <math.h>
//----------------------------------------------------------------
GmMinRangeLimitsStudyUA::GmMinRangeLimitsStudyUA()
{

  theCreateInfoUA = new GmMinRangeLimitsStudyCreateInfoUA( this ); // use this clas without filter, because a new GmCSTrackStepInfo should be created at stepping action despite the filter of the current class

}

//----------------------------------------------------------------
void GmMinRangeLimitsStudyUA::BeginOfRunAction(const G4Run* )
{
  theAnaMgr = GmAnalysisMgr::GetInstance("minrange");

  /* G4String hnamf = "ALL: gamma : log10(Range)";
  theAnaMgr->CreateHisto1D(hnamf.c_str(),180,-3,6,980000+10+1);
  hnamf = "ALL: e- : log10(Range)";
  theAnaMgr->CreateHisto1D(hnamf.c_str(),180,-3,6,980000+20+1);
  hnamf = "ALL: e+ : log10(Range)";
  theAnaMgr->CreateHisto1D(hnamf.c_str(),180,-3,6,980000+30+1);
  hnamf = "ALL: gamma : log10(Energy)";
  theAnaMgr->CreateHisto1D(hnamf.c_str(),140,-3,4,980000+10+2);
  hnamf = "ALL: e- : log10(Energy)";
  theAnaMgr->CreateHisto1D(hnamf.c_str(),140,-3,4,980000+20+2);
  hnamf = "ALL: e+ : log10(Energy)";
  theAnaMgr->CreateHisto1D(hnamf.c_str(),140,-3,4,980000+30+2); */
  G4String hnamf = "N trackInfos per Track in target";
  theAnaMgr->CreateHisto1D(hnamf.c_str(),100,0,100,980000+91);

  BuildRegionIDs();

  hnamf = "trackInfo Region vs trackInfo Region";
  G4int nbins = theRegionIDs.size();
  theAnaMgr->CreateHisto2D(hnamf.c_str(),nbins,0.,nbins,nbins,0.,nbins,980000+92);
}

//----------------------------------------------------------------
void GmMinRangeLimitsStudyUA::BuildRegionIDs()
{
  G4RegionStore* regStore = G4RegionStore::GetInstance();
  theRegionIDs[0] = 0;
  for( unsigned int ii = 0; ii < regStore->size(); ii++ ){
    theRegionIDs[(*regStore)[ii] ] = ii+1;
  }
}
  
//----------------------------------------------------------------
pimmppd GmMinRangeLimitsStudyUA::HistosAndStatistics(G4Region* reg, G4ParticleDefinition* part, G4VProcess* proc)
{
  G4int nHreg = 0;
  G4int nHpartProc = 0;
  std::map<G4Region*,G4int>::const_iterator itereg = theRegionIDs.find( reg );
  if( itereg == theRegionIDs.end() ){
    G4Exception("GmMinRangeLimitsStudyUA::HistosAndStatistics",
		"All regions should have an ID",FatalErrorInArgument,reg->GetName());
    //    nHreg = theRegionIDs.size() + 1;
    //  theRegionIDs[reg] = nHreg;
  } else {
    nHreg = (*itereg).second;
  }

  ppp partProc(part,proc);
  std::map<ppp,G4int>::const_iterator itepp = thePartProcIDs.find( partProc );

  if( itepp == thePartProcIDs.end() ){
    nHpartProc = thePartProcIDs.size() + 1;
    thePartProcIDs[partProc] = nHpartProc;
  } else {
    nHpartProc = (*itepp).second;
  }

  if( nHreg > 90 || nHpartProc > 1000 ){
    G4Exception("GmMinRangeLimitsStudyUA:HistosAndStatistics",
		" Too big number of particles or processes",FatalErrorInArgument,G4String("N regions="+GmGenUtils::itoa(nHreg)+" >? 90  N particle-processes="+GmGenUtils::itoa(nHpartProc)+" >? 1000").c_str());
  }

  G4int nH = 990000 + nHreg*10000 + nHpartProc*10;

  G4bool bHistosDefined = false;
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(debugVerb) ) G4cout << "GmMinRangeLimitsStudyUA:HistosAndStatistics  REGION= " << GetRegionName(reg) << " PARTICLE= " << GetParticleName(part) << " PROCESS= " << GetProcessName(proc)  << G4endl;
#endif

  //----- Find histoID corresponding to new particle/process
  //--- Find particle
  mrmmppi::iterator iterppi;
  mrmmppd::iterator iterppd;
  mmppi::iterator iteppi;
  mmppd::iterator iteppd;
  
  for(iterppi = theHistosIDs.begin(), iterppd = theRanges.begin(); iterppi != theHistosIDs.end(); iterppi++,iterppd++ ){
    if( (*iterppi).first == reg ) break;
  }

  mmppi* partProcHIDs;
  mmppd* partProcRanges;
  mpi* procHID ;
  mpd* procRange;
  //--- If region not found create new record in theHistoIDs
  if( iterppi == theHistosIDs.end() ) {
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << " HistosAndStatistics new REGION " << GetRegionName(reg) << G4endl;
#endif
    bHistosDefined = true;
    partProcHIDs = new mmppi;
    partProcRanges = new mmppd;
    theHistosIDs.insert( mrmmppi::value_type( reg, partProcHIDs ));
    theRanges.insert( mrmmppd::value_type( reg, partProcRanges ));
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << " HistosAndStatistics old REGION " << GetRegionName(reg) << G4endl;
#endif
    partProcHIDs = (*iterppi).second;
    partProcRanges = (*iterppd).second;
  }

  for(iteppi = partProcHIDs->begin(), iteppd = partProcRanges->begin(); iteppi != partProcHIDs->end(); iteppi++,iteppd++ ){
    if( (*iteppi).first == part ) break;
  }
  
  //--- if particle not found create new record in partProcIDs
  if( iteppi == partProcHIDs->end() ) {
    bHistosDefined = true;
    procHID = new mpi;
    procRange = new mpd;
    partProcHIDs->insert( mmppi::value_type( part, procHID ));
    partProcRanges->insert( mmppd::value_type( part, procRange )); 
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << partProcRanges << " HistosAndStatistics new PARTICLE " << GetParticleName(part) << G4endl;
#endif
    //--- if particle found, then process is not found, create new record in theHistoIDs
  } else {
    procHID = (*iteppi).second;
    procRange = (*iteppd).second;
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << (*iteppd).second << " HistosAndStatistics old PARTICLE " << GetParticleName(part) << (*iteppi).second  << " procHID " << procHID << G4endl;
#endif
  }

  mpi::const_iterator itepi;
  mpd::const_iterator itepd;
  G4int ip = 0;
  for(itepi = procHID->begin(), itepd = procRange->begin(); itepi != procHID->end(); itepi++, itepd++ ){
    ip++;
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << procHID << " checking process " << GetProcessName((*itepi).first)  << " =? " << GetProcessName(proc) << " " << (*itepi).first << " =? " << proc << G4endl;
#endif
    if( (*itepi).first == proc ) break;
  }

  if( itepi == procHID->end() ){
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << procRange << " HistosAndStatistics new PROCESS " << GetProcessName(proc) << G4endl;
#endif
    (*procHID)[proc] = nH;
    // G4cout << " setting nH " << nH << G4endl;
    (*procRange)[proc] = 1.E99;
    bHistosDefined = true;
  } else {

#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << (*itepd).second << " HistosAndStatistics old PROCESS " << GetProcessName(proc) << G4endl;
#endif
  }

#ifndef GAMOS_NO_VERBOSE
   if( PhysicsVerb(debugVerb) ) G4cout << "GmMinRangeLimitsStudyUA:HistosAndStatistics  REGION= " << GetRegionName(reg) << " PARTICLE= " << GetParticleName(part) << " PROCESS= " << GetProcessName(proc) << " HID= " << nH << G4endl;
#endif

  if(bHistosDefined) {
    // G4cout << " BookHistos " << G4endl;
    BookHistos( nH, "GmMinRangeLimitsStudyUA: " + GetRegionName(reg) + ": " + GetParticleName(part) + ": " + GetProcessName(proc) + ": ");
  }

  return pimmppd(nH, partProcRanges );

}

//----------------------------------------------------------------
void GmMinRangeLimitsStudyUA::BookHistos( G4int ih, G4String hnam )
{
  // Create histograms
  G4String hnamf = hnam + " log10(Range)";
  theAnaMgr->CreateHisto1D(hnamf.c_str(),180,-3,6,ih+1);
  hnamf = hnam + " log10(Energy)";
  theAnaMgr->CreateHisto1D(hnamf.c_str(),140,-3,4,ih+2);
  //  hnamf = hnam + " Safety-Range vs Range)";
  //  theAnaMgr->CreateHisto2D(hnamf.c_str(),180,-14,4,160,-3,6,ih+3);

}

//----------------------------------------------------------------
void GmMinRangeLimitsStudyUA::PreUserTrackingAction(const G4Track*  )
{
}

//----------------------------------------------------------------
void GmMinRangeLimitsStudyUA::CreateTrackStepInfo(const G4Track* aTrack )
{
  theCurrentTrackInfo = new GmCSTrackStepInfo( aTrack );
  theCurrentTrackInfo->SetParent( GmCutsStudyMgr::GetInstance()->GetTrackStepInfo( aTrack->GetParentID(), aTrack->GetVertexPosition() )); 

  /*  GmCSTrackInfo* trkInfo = aTrack->GetUserInformation();
  if( trkInfo == 0 ) {
    trkInfo = new GmCSTrackInfo( aTrack );
  }
  trkInfo->SetCreatorTraj( theCurrentTrackInfo );
  SecondaryaTrack->SetUserInformation( trkInfo );
  */

}

//----------------------------------------------------------------
void GmMinRangeLimitsStudyUA::UserSteppingAction(const G4Step* aStep )
{
  //  if( theFilter->Accept( *aStep ) ){
    //f1or tracking cuts CreateTrackInfo
  theCreateInfoUA->SetInfoCreated(true);
  CreateTrackStepInfo( aStep->GetTrack() );

  SetTrackInfoAccepted();
    //t    KillTrack;
    //  }
}

//----------------------------------------------------------------
void GmMinRangeLimitsStudyUA::SetTrackInfoAccepted()
{
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(debugVerb) ) G4cout << "  GmMinRangeLimitsStudyUA::SetTrackInfoAccepted() " << theCurrentTrackInfo << G4endl;
#endif
  GmCSTrackStepInfo* trkStepInfo = (GmCSTrackStepInfo*)theCurrentTrackInfo;
  //  G4ParticleDefinition* particleOriginal = trkStepInfo->GetParticle();

  GmCutsStudyMgr* csMgr = GmCutsStudyMgr::GetInstance();
  std::map< G4ParticleDefinition*, std::map<G4Region*,G4double> > theMinRanges;
  std::map< G4ParticleDefinition*, G4double > minRangeTotal;
  std::map< G4ParticleDefinition*, G4double > minEnergyTotal;
  minRangeTotal[G4Gamma::Gamma()] = 9.E99;
  minRangeTotal[G4Electron::Electron()] = 9.E99;
  minRangeTotal[G4Positron::Positron()] = 9.E99;
  minEnergyTotal[G4Gamma::Gamma()] = 9.E99;
  minEnergyTotal[G4Electron::Electron()] = 9.E99;
  minEnergyTotal[G4Positron::Positron()] = 9.E99;
  G4int nTrkI = 0;
  std::vector<G4Region*> regions; //for region vs region histo
  while( trkStepInfo ){
    //    trkStepInfo->SetAccepted(true); // proper trkInfo is included in the list of trkInfos before
    //--- Accept all the steps previous to this one
    std::vector<GmCSTrackStepInfo*> trkStepInfosBef = csMgr->GetTrackStepInfosBefore( trkStepInfo->GetTrackID(), trkStepInfo->GetTrackLength() ); 
    for( unsigned int ii = 0; ii < trkStepInfosBef.size(); ii++ ){
      GmCSTrackStepInfo* trkI = trkStepInfosBef[ii];
      G4Region* reg = trkI->GetRegion();
      G4ParticleDefinition* part = trkI->GetParticle();
      //---- Get minimum range and energy for this accepted track
      G4double energy = trkI->GetEnergy();
      G4double range = trkI->GetRange();
      if( range < minRangeTotal[part] ) minRangeTotal[part] = range;
      if( energy < minEnergyTotal[part] ) minEnergyTotal[part] = energy;

      //check if there is trki with same particle and same region and smallest energy
      std::map<G4Region*,G4double> MinRanges;
      std::map< G4ParticleDefinition*, std::map<G4Region*,G4double> >::iterator itemr = theMinRanges.find(part);
      if( itemr == theMinRanges.end() ){	
	theMinRanges[part] = MinRanges;
      }else {
	MinRanges = (*itemr).second;
      }
      std::map<G4Region*,G4double>::iterator iter = MinRanges.find( reg );
      if( iter != MinRanges.end() ){
	if( range > (*iter).second ) continue;
      }

      trkI->SetAccepted(true);
      nTrkI++;
      MinRanges[reg] = range;
      regions.push_back(reg);
#ifndef GAMOS_NO_VERBOSE
      if( PhysicsVerb(infoVerb) ) G4cout << " GmMinRangeLimitsStudyUA: ACCEPTED: " << nTrkI;
#endif
      if( PhysicsVerb(-infoVerb) ) trkI->Print();
    }
    
    trkStepInfo = (GmCSTrackStepInfo*)trkStepInfo->GetParent();
  }

  std::map< G4ParticleDefinition*, std::map<G4Region*,G4double> >::iterator itemr;
  for( itemr = theMinRanges.begin(); itemr != theMinRanges.end(); itemr++ ){
    pimmppd ihRange = HistosAndStatistics( 0, (*itemr).first, 0 );
    G4int ihALL = ihRange.first;
    if( minRangeTotal[(*itemr).first] != 9.E99 ){
      theAnaMgr->GetHisto1(ihALL+1)->Fill(log10(minRangeTotal[(*itemr).first]));
      theAnaMgr->GetHisto1(ihALL+2)->Fill(log10(minEnergyTotal[(*itemr).first]));
    }
  }
  theAnaMgr->GetHisto1( 980000+91 )->Fill(float(nTrkI));

  std::vector<G4Region*>::const_iterator itereg1,itereg2; //for region vs region histo
  for(itereg1 = regions.begin(); itereg1 != regions.end(); itereg1++) {
    itereg2 = itereg1; itereg2++;
    for( ;itereg2 != regions.end(); itereg2++) {
      theAnaMgr->GetHisto2( 980000+92 )->Fill(theRegionIDs[*itereg1],theRegionIDs[*itereg2]);
    }
  }
}

//----------------------------------------------------------------
void GmMinRangeLimitsStudyUA::EndOfEventAction(const G4Event* )
{
  //Make statistics and histos
  //----- Get list of trackInfos and loop to the accepted ones
  GmCutsStudyMgr* csMgr = GmCutsStudyMgr::GetInstance();
  mmrcs trackInfos = csMgr->GetTrackInfos();
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(infoVerb) ) G4cout << " GmMinRangeLimitsStudyUA::EndOfEventAction n trackInfos " << trackInfos.size() << G4endl;
#endif
  mmrcs::const_iterator ite;
  for( ite = trackInfos.begin(); ite != trackInfos.end(); ite++ ){
    GmCSTrackInfo* trkI = (*ite).second;
    
    if( ! trkI->IsAccepted() ) continue;

    //---- Get particle and process
    G4Region* reg = trkI->GetRegion();
    G4ParticleDefinition* part = trkI->GetParticle();
    G4VProcess* proc = trkI->GetProcess();
    
    //---- Look if histo and statistics for this particle/process is created (if not create them)
    pimmppd ihRange = HistosAndStatistics( reg, part, proc );
    //-- Fill histos
    G4int ih = ihRange.first;
    G4double logrange = log10(trkI->GetRange()); 
    theAnaMgr->GetHisto1( ih+1 )->Fill(logrange);
#ifndef GAMOS_NO_VERBOSE
   //    if( PhysicsVerb(debugVerb) ) G4cout << " FILL " << ih+1 << " : " << log10(trkI->GetRange()) << "=" << trkI->GetRange() << G4endl;
#endif
    theAnaMgr->GetHisto1( ih+2 )->Fill( log10(trkI->GetEnergy()));
    /*    G4double safran = trkI->GetSafety()-trkI->GetRange();
    //    G4int ss = safran > 0  ? 0 : -1;
    G4int ss = -(safran <= 0);
    safran = log10(fabs(safran));
    if( safran < -6 ) safran = -6; // -10-4 to -10+4 is used for negative values of safety-range
    theAnaMgr->GetHisto2( ih+3)->Fill( ss*10+safran,logrange);
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << trkI->GetParticle()->GetParticleName() << " safety-range " << trkI->GetSafety()-trkI->GetRange() << " safety " << trkI->GetSafety() << " range " << trkI->GetRange() << " fill " << ss*10+safran << G4endl;
#endif
    */

    //-- Set minimum range
    mmppd* partProcRanges = ihRange.second;
    mmppd::const_iterator iteppd = partProcRanges->find( part );
    mpd::iterator itepd = (*iteppd).second->find(proc);
    //    mpd::iterator itepd = (*iteppd).second->find( proc );
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << " SETTING RANGE  trkI->GetRang log10 " <<  log10(trkI->GetRange()) << "=" << trkI->GetRange() << " <? " << (*itepd).second << "  " << GetRegionName(reg) << "  " << GetParticleName(part) << "  " << GetProcessName(proc) << G4endl;
#endif
    if( trkI->GetRange() < (*itepd).second ) (*itepd).second = trkI->GetRange();
  }
  
  csMgr->CleanTrackInfos();
}

//----------------------------------------------------------------
void GmMinRangeLimitsStudyUA::EndOfRunAction(const G4Run* )
{
  //---- Sum statistics
  mrmmppd::const_iterator itermppd;
  mmppd::const_iterator itemppd,itemppd1,itemppdALL;
  mpd::iterator itepd,itepd1,itepdALL;
  mrmmppi::const_iterator itermppi;
  mmppi::const_iterator itemppi;
  mpi::iterator itepi;
  mmppd* partProcRanges, *partProcRanges1, *partProcRangesALL;
  G4Region* reg;
  G4ParticleDefinition* part;
  G4VProcess* proc;
  mpd* procRange;
  G4double range;
  G4int ih1,ihALL;
  pimmppd ihRange;

  //----- Loop to all regions
  for( itermppd = theRanges.begin(); itermppd != theRanges.end(); itermppd++ ){
    partProcRanges = (*itermppd).second;
    reg = (*itermppd).first;
    //----- Loop to all particles
    for( itemppd = partProcRanges->begin(); itemppd != partProcRanges->end(); itemppd++ ){
      part = (*itemppd).first;
      //      G4cout << (*itempp).second << " GmMinRangeLimitsStudyUA: nProc " << procRange->size() << G4endl;
      procRange = (*itemppd).second; // map of proc-min range
      //      G4cout << " GmMinRangeLimitsStudyUA: ProcRange " << procRange << " " << procRange->size() << G4endl;

      //--- Fill range of process 'ALL'
      ihRange = HistosAndStatistics( reg, part, 0 );
      ihALL = ihRange.first;
      partProcRangesALL = ihRange.second;
      itemppdALL = partProcRangesALL->find( part );
      itepdALL = (*itemppdALL).second->find(0);
      GmHisto1* hisALLR = theAnaMgr->GetHisto1(ihALL+1);
      GmHisto1* hisALLE = theAnaMgr->GetHisto1(ihALL+2);
      //      GmHisto2* hisALLS = theAnaMgr->GetHisto2(ihALL+3);

      //--- loop to all processes-min range of this region/this particle to get the mininum for process 'ALL', and to add histograms
      for( itepd = procRange->begin(); itepd != procRange->end(); itepd++ ){
	proc = (*itepd).first;
	if( proc == 0 ) continue; // this is the 'ALL' just created

	ihRange = HistosAndStatistics( reg, part, proc );
	ih1 = ihRange.first;
	partProcRanges1 = ihRange.second;
	itemppd1 = partProcRanges1->find( part );
	itepd1 = (*itemppd1).second->find(proc);
	range =  (*itepd1).second;

	if( range < (*itepdALL).second ) (*itepdALL).second = range;
	//       	if( proc != 0 ) G4cout << "GmMinRangeLimitsStudyUA: REGION= " << (*itermppd).first->GetName() << " PARTICLE= " << (*itemppd).first->GetParticleName() << " PROCESS= " << (*itepd1).first->GetProcessName() << " MIN RANGE= " << (*itepd1).second  << " MIN RANGE ALL= " << (*itepdALL).second << G4endl;
       	G4cout << "  GmMinRangeLimitsStudyUA: REGION= " << (*itermppd).first->GetName() << " PARTICLE= " << (*itemppd).first->GetParticleName() << " MIN RANGE= " << (*itepd1).second  << " MIN RANGE ALL= " << (*itepdALL).second << G4endl;

	GmHisto1* hisR = theAnaMgr->GetHisto1(ih1+1);
	GmHisto1* hisE = theAnaMgr->GetHisto1(ih1+2);
	//	GmHisto2* hisS = theAnaMgr->GetHisto2(ih1+3);
	(*hisALLR) = (*hisALLR) + (*hisR);
	(*hisALLE) = (*hisALLE) + (*hisE);
	//	(*hisALLS) = (*hisALLS) + (*hisS);

      }

    }
  }
  
  //Print statistics
  //---- Loop to each region
  // G4cout << "GmMinRangeLimitsStudyUA: nRanges " << theRanges.size() << G4endl;
  G4cout << G4endl << " %%%%% MINIMUM RANGE USER LIMITS STUDY RESULTS " << G4endl;

  for( itermppd = theRanges.begin(); itermppd != theRanges.end(); itermppd++ ){
    partProcRanges = (*itermppd).second;
    // G4cout << (*itermppd).second << " GmMinRangeLimitsStudyUA: nPartProc " << partProcRanges->size() << G4endl;
    for( itemppd = partProcRanges->begin(); itemppd != partProcRanges->end(); itemppd++ ){
      // G4cout << (*itemppd).second << " GmMinRangeLimitsStudyUA: nProc " << procRange->size() << G4endl;
      procRange = (*itemppd).second;
      // G4cout << " GmMinRangeLimitsStudyUA: ProcRange " << procRange << " " << procRange->size() << G4endl;
      for( itepd = procRange->begin(); itepd != procRange->end(); itepd++ ){
	G4cout << "GmMinRangeLimitsStudyUA: REGION= " << GetRegionName((*itermppd).first) << " PARTICLE= " << GetParticleName((*itemppd).first) << " PROCESS= " << GetProcessName((*itepd).first) << " MIN RANGE= " << (*itepd).second << G4endl;
      }
    }
  }


  /*  G4cout << G4endl << " %%%%% RANGE REJECTION STUDY RESULTS (NUMBER OF TRACKS WITH safety > range) " << G4endl;
  //----- Print number of events with safety > range (that would be killed by Raange Rejection technique)
  for( itermppd = theRanges.begin(); itermppd != theRanges.end(); itermppd++ ){
    partProcRanges = (*itermppd).second;
    reg = (*itermppd).first;
    //----- Loop to all particles
    for( itemppd = partProcRanges->begin(); itemppd != partProcRanges->end(); itemppd++ ){
      part = (*itemppd).first;
      procRange = (*itemppd).second; // map of proc-min range
      //--- loop to all processes-min range of this region/this particle to get the mininum for process 'ALL', and to add histograms
      for( itepd = procRange->begin(); itepd != procRange->end(); itepd++ ){
	proc = (*itepd).first;
	ihRange = HistosAndStatistics( reg, part,  proc );
	ihALL = ihRange.first;
	GmHisto2* hisS = theAnaMgr->GetHisto2(ihALL+3);
	//--- count number of entries with x axis > 0 
	TAxis* xaxis = hisS->GetXaxis();
	G4int nbinsX = xaxis->GetNbins();
	G4int nbinsY = hisS->GetYaxis()->GetNbins();
	G4int nent = 0;
	for( G4int ix = 0; ix <= nbinsX+1; ix++ ){
	  if( xaxis->GetBinUpEdge( ix ) <= -6. ) continue;
	  for( G4int iy = 0; iy <= nbinsY+1; iy++ ){
	    nent += G4int(hisS->GetBinContent(ix,iy));
	  }
	}
	G4cout << "RangeRejection: REGION= " << GetRegionName(reg) << " PARTICLE= " << GetParticleName(part) << " PROCESS= " << GetProcessName(proc) << " NTRACKS= " << nent << G4endl;
	
      }
    }
  }
  */
}


//----------------------------------------------------------------
G4String GmMinRangeLimitsStudyUA::GetRegionName(G4Region* reg) 
{
  if( reg == 0 ) {
    return "ALL";
  } else { 
    return reg->GetName();
  }
}

//----------------------------------------------------------------
G4String GmMinRangeLimitsStudyUA::GetParticleName(G4ParticleDefinition* part) 
{
  if( part == 0 ) {
    return "ALL";
  }else {
    return part->GetParticleName();
  }
}
 
//----------------------------------------------------------------
G4String GmMinRangeLimitsStudyUA::GetProcessName(G4VProcess* proc) 
{
  if(proc == 0) {
    return "ALL";
  } else{
    return proc->GetProcessName();
  }
}
