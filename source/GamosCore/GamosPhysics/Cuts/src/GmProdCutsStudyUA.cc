#include "G4Run.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4ParticleDefinition.hh"
#include "G4VProcess.hh"
#include "G4RegionStore.hh"

#include "GmCSTrackInfo.hh"
#include "GmProdCutsStudyUA.hh"
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include <math.h>
//----------------------------------------------------------------
GmProdCutsStudyUA::GmProdCutsStudyUA()
{
  theNAccepted = 0;
  theNAcceptedAncestors = 0;
  theCurrentTrackInfo = 0;
}

//----------------------------------------------------------------
void GmProdCutsStudyUA::BeginOfRunAction(const G4Run* )
{
  theAnaMgr = GmAnalysisMgr::GetInstance("prodcuts");
  if( theFilters.size() == 0 ){
    G4Exception("GmProdCutsStudyUA::BeginOfRunAction",
		" No filter defined, all particles will be taken",JustWarning,"");
  }

  G4String hnamf = "N trackInfos per Track in target";
  theAnaMgr->CreateHisto1D(hnamf.c_str(),140,-3,4,980000+91);

  BuildRegionIDs();

  hnamf = "trackInfo Region vs trackInfo Region";
  G4int nbins = theRegionIDs.size();
  theAnaMgr->CreateHisto2D(hnamf.c_str(),nbins,0.,nbins,nbins,0.,nbins,980000+92);
}

//----------------------------------------------------------------
void GmProdCutsStudyUA::BuildRegionIDs()
{
  G4RegionStore* regStore = G4RegionStore::GetInstance();
  theRegionIDs[0] = 0;
  for( unsigned int ii = 0; ii < regStore->size(); ii++ ){
    theRegionIDs[(*regStore)[ii] ] = ii+1;
  }
}
 
//----------------------------------------------------------------
pimmppd GmProdCutsStudyUA::HistosAndStatistics(G4Region* reg, G4ParticleDefinition* part, G4VProcess* proc)
{
  G4int nHreg = 0;
  G4int nHpartProc = 0;
  std::map<G4Region*,G4int>::const_iterator itereg = theRegionIDs.find( reg );
  if( itereg == theRegionIDs.end() ){
    G4Exception("GmProdCutsStudyUA::HistosAndStatistics",
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
    G4Exception("GmProdCutsStudyUA:HistosAndStatistics",
		" Too big number of particles or processes",FatalErrorInArgument,G4String("N regions="+GmGenUtils::itoa(nHreg)+" >? 90  N particle-processes="+GmGenUtils::itoa(nHpartProc)+" >? 1000").c_str());
  }

  G4int nH = 990000 + nHreg*10000 + nHpartProc*10;

  G4bool bHistosDefined = false;
  // G4cout << "GmProdCutsStudyUA:HistosAndStatistics  REGION= " << GetRegionName(reg) << " PARTICLE= " << GetParticleName(part) << " PROCESS= " << GetProcessName(proc)  << G4endl;

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
    // G4cout << " HistosAndStatistics new REGION " << GetRegionName(reg) << G4endl;
    bHistosDefined = true;
    partProcHIDs = new mmppi;
    partProcRanges = new mmppd;
    theHistosIDs.insert( mrmmppi::value_type( reg, partProcHIDs ));
    theRanges.insert( mrmmppd::value_type( reg, partProcRanges ));
  } else {
    // G4cout << " HistosAndStatistics old REGION " << GetRegionName(reg) << G4endl;
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
    // G4cout << partProcRanges << " HistosAndStatistics new PARTICLE " << GetParticleName(part) << " procHID " << procHID << G4endl;
    //--- if particle found, then process is not found, create new record in theHistoIDs
  } else {
    procHID = (*iteppi).second;
    procRange = (*iteppd).second;
    // G4cout << (*iteppd).second << " HistosAndStatistics old PARTICLE " << GetParticleName(part) << (*iteppi).second  << " procHID " << procHID << G4endl;
  }

  mpi::const_iterator itepi;
  mpd::const_iterator itepd;
  G4int ip = 0;
  for(itepi = procHID->begin(), itepd = procRange->begin(); itepi != procHID->end(); itepi++, itepd++ ){
    ip++;
    // G4cout << procHID << " checking process " << GetProcessName((*itepi).first)  << " =? " << GetProcessName(proc) << " " << (*itepi).first << " =? " << proc << G4endl;
    if( (*itepi).first == proc ) break;
  }

  if( itepi == procHID->end() ){
    // G4cout << procRange << " HistosAndStatistics new PROCESS " << GetProcessName(proc) << G4endl;
    (*procHID)[proc] = nH;
    // G4cout << " setting nH " << nH << G4endl;
    (*procRange)[proc] = 1.E99;
    bHistosDefined = true;
  } else {

    // G4cout << (*itepd).second << " HistosAndStatistics old PROCESS " << GetProcessName(proc) << G4endl;
  }

  // G4cout << "GmProdCutsStudyUA:HistosAndStatistics  REGION= " << GetRegionName(reg) << " PARTICLE= " << GetParticleName(part) << " PROCESS= " << GetProcessName(proc) << " HID= " << nH << G4endl;

  if(bHistosDefined) {
    // G4cout << " BookHistos " << G4endl;
    BookHistos( nH, "GmProdCutsStudyUA: " + GetRegionName(reg) + ": " + GetParticleName(part) + ": " + GetProcessName(proc) + ": ");
  }

  return pimmppd(nH, partProcRanges );

}

//----------------------------------------------------------------
void GmProdCutsStudyUA::BookHistos( G4int ih, G4String hnam )
{
  // Create histograms
  G4String hnamf = hnam + " log10(Range)";
  theAnaMgr->CreateHisto1D(hnamf.c_str(),200,-3,7,ih+1);
  hnamf = hnam + " log10(Energy)";
  theAnaMgr->CreateHisto1D(hnamf.c_str(),120,-3,3,ih+2);
  hnamf = hnam + " Safety-Range vs Range)";
  theAnaMgr->CreateHisto2D(hnamf.c_str(),200,-15,5,180,-3,6,ih+3);

}

//----------------------------------------------------------------
void GmProdCutsStudyUA::PreUserTrackingAction(const G4Track* aTrack )
{
  //  GmCSTrackTrajectory * trkTraj 
  if( aTrack->GetParentID() != 0 ) CreateTrackInfo( aTrack );
}

//----------------------------------------------------------------
void GmProdCutsStudyUA::CreateTrackInfo(const G4Track* aTrack )
{
  theCurrentTrackInfo = new GmCSTrackInfo( aTrack );
  theCurrentTrackInfo->SetParent( GmCutsStudyMgr::GetInstance()->GetTrackInfo( aTrack->GetParentID() ));

  /*  GmCSTrackInfo* trkInfo = aTrack->GetUserInformation();
  if( trkInfo == 0 ) {
    trkInfo = new GmCSTrackInfo( aTrack );
  }
  trkInfo->SetCreatorTraj( theCurrentTrackInfo );
  SecondaryaTrack->SetUserInformation( trkInfo );
  */

}

//----------------------------------------------------------------
void GmProdCutsStudyUA::UserSteppingAction(const G4Step* )
{
  //f1or tracking cuts CreateTrackInfo
  SetTrackInfoAccepted();
  //t    KillTrack;
}

//----------------------------------------------------------------
void GmProdCutsStudyUA::SetTrackInfoAccepted()
{
  // G4cout << "  GmProdCutsStudyUA::SetTrackInfoAccepted() " << theCurrentTrackInfo << G4endl;
  theNAccepted++;
  GmCSTrackInfo* trkInfo = theCurrentTrackInfo;
  G4int nTrkI = 0;
  std::vector<G4Region*> regions; //for region vs region histo
  while( trkInfo ){
    theNAcceptedAncestors++;
    trkInfo->SetAccepted(true);
    nTrkI++;
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(infoVerb) ) G4cout << " GmProdCutsStudyUA: ACCEPTED: ";
#endif
    if( PhysicsVerb(infoVerb) ) trkInfo->Print();
    regions.push_back(trkInfo->GetRegion());  
    trkInfo = trkInfo->GetParent();
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
void GmProdCutsStudyUA::EndOfEventAction(const G4Event* )
{
  //Make statistics and histos
  //----- Get list of trackInfos and loop to the accepted ones
  GmCutsStudyMgr* csMgr = GmCutsStudyMgr::GetInstance();
  mmrcs trackInfos = csMgr->GetTrackInfos();
  // G4cout << " GmProdCutsStudyUA::EndOfEventAction n trackInfos " << trackInfos.size() << G4endl;
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
    // G4cout << " FILL " << ih+1 << " : " << log10(trkI->GetRange()) << "=" << trkI->GetRange() << G4endl;
    theAnaMgr->GetHisto1( ih+2 )->Fill( log10(trkI->GetEnergy()));
    G4double safran = trkI->GetSafety()-trkI->GetRange();
    //    G4int ss = safran > 0  ? 0 : -1;
    G4int ss = -(safran <= 0);
    safran = log10(fabs(safran));
    if( safran < -5 ) safran = -4.99; // 1E-15 to 1E-5 is used for negative values of safety-range
    if( safran > 5 ) safran = 4.99;
    theAnaMgr->GetHisto2( ih+3)->Fill( ss*10+safran,logrange);
    //    G4cout << trkI->GetParticle()->GetParticleName() << " safety-range " << trkI->GetSafety()-trkI->GetRange() << " safety " << trkI->GetSafety() << " range " << trkI->GetRange() << " fill " << ss*10+safran << G4endl;
					
    //-- Set minimum range
    mmppd* partProcRanges = ihRange.second;
    mmppd::const_iterator iteppd = partProcRanges->find( part );
    mpd::iterator itepd = (*iteppd).second->find(proc);
    //    mpd::iterator itepd = (*iteppd).second->find( proc );
    // G4cout << " SETTING RANGE  trkI->GetRang " <<  log10(trkI->GetRange()) << "=" << trkI->GetRange() << " <? " << (*itepd).second << "  " << GetRegionName(reg) << "  " << GetParticleName(part) << "  " << GetProcessName(proc) << G4endl;
    if( trkI->GetRange() < (*itepd).second ) (*itepd).second = trkI->GetRange();
  }
  
  csMgr->CleanTrackInfos();
  theCurrentTrackInfo = 0;
}

//----------------------------------------------------------------
void GmProdCutsStudyUA::EndOfRunAction(const G4Run* )
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
      //      G4cout << (*itempp).second << " GmProdCutsStudyUA: nProc " << procRange->size() << G4endl;
      procRange = (*itemppd).second; // map of proc-min range
      //      G4cout << " GmProdCutsStudyUA: ProcRange " << procRange << " " << procRange->size() << G4endl;

      //--- Fill range of process 'ALL'
      ihRange = HistosAndStatistics( reg, part, 0 );
      ihALL = ihRange.first;
      partProcRangesALL = ihRange.second;
      itemppdALL = partProcRangesALL->find( part );
      itepdALL = (*itemppdALL).second->find(0);
      GmHisto1* hisALLR = theAnaMgr->GetHisto1(ihALL+1);
      GmHisto1* hisALLE = theAnaMgr->GetHisto1(ihALL+2);
      GmHisto2* hisALLS = theAnaMgr->GetHisto2(ihALL+3);

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
	//	if( proc != 0 ) G4cout << "GmProdCutsStudyUA: REGION= " << (*itermppd).first->GetName() << " PARTICLE= " << (*itemppd).first->GetParticleName() << " PROCESS= " << (*itepd1).first->GetProcessName() << " MIN RANGE= " << (*itepd1).second  << " MIN RANGE ALL= " << (*itepdALL).second << G4endl;

	GmHisto1* hisR = theAnaMgr->GetHisto1(ih1+1);
	GmHisto1* hisE = theAnaMgr->GetHisto1(ih1+2);
	GmHisto2* hisS = theAnaMgr->GetHisto2(ih1+3);
	(*hisALLR) = (*hisALLR) + (*hisR);
	(*hisALLE) = (*hisALLE) + (*hisE);
	(*hisALLS) = (*hisALLS) + (*hisS);

      }

    }
  }
  
  //Print statistics
  //---- Loop to each region
  // G4cout << "GmProdCutsStudyUA: nRanges " << theRanges.size() << G4endl;
  G4cout << G4endl << " %%%%% PRODUCTION CUTS STUDY RESULTS " << G4endl;

  for( itermppd = theRanges.begin(); itermppd != theRanges.end(); itermppd++ ){
    partProcRanges = (*itermppd).second;
    // G4cout << (*itermppd).second << " GmProdCutsStudyUA: nPartProc " << partProcRanges->size() << G4endl;
    for( itemppd = partProcRanges->begin(); itemppd != partProcRanges->end(); itemppd++ ){
      // G4cout << (*itemppd).second << " GmProdCutsStudyUA: nProc " << procRange->size() << G4endl;
      procRange = (*itemppd).second;
      // G4cout << " GmProdCutsStudyUA: ProcRange " << procRange << " " << procRange->size() << G4endl;
      for( itepd = procRange->begin(); itepd != procRange->end(); itepd++ ){
	G4cout << "GmProdCutsStudyUA: REGION= " << GetRegionName((*itermppd).first) << " PARTICLE= " << GetParticleName((*itemppd).first) << " PROCESS= " << GetProcessName((*itepd).first) << " MIN RANGE= " << (*itepd).second << G4endl;
      }
    }
  }


  G4cout << G4endl << " %%%%% RANGE REJECTION STUDY RESULTS (NUMBER OF TRACKS WITH safety > range) " << G4endl;
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
	G4int nbinsX = hisS->GetNbinsX();
	G4int nbinsY = hisS->GetNbinsY();
	G4int nent = 0;
	for( G4int ix = 0; ix <= nbinsX+1; ix++ ){
#ifdef GAMOS_NO_ROOT
	  if( hisS->GetBinUpEdgeX( ix ) <= -5. ) continue;
#else 
	  if( hisS->GetXaxis()->GetBinUpEdge( ix ) <= -5. ) continue;
#endif
	  for( G4int iy = 0; iy <= nbinsY+1; iy++ ){
	    nent += G4int(hisS->GetBinContent(ix,iy));
	  }
	}
	G4cout << "RangeRejection: REGION= " << GetRegionName(reg) << " PARTICLE= " << GetParticleName(part) << " PROCESS= " << GetProcessName(proc) << " NTRACKS= " << nent << G4endl;
	
      }
    }
  }

  G4cout << "NUMBER OF TRACKS ACCEPTED " << theNAccepted << " N ANCESTORS " << theNAcceptedAncestors << G4endl;
}
 

//----------------------------------------------------------------
G4String GmProdCutsStudyUA::GetRegionName(G4Region* reg) 
{
  if( reg == 0 ) {
    return "ALL";
  } else { 
    return reg->GetName();
  }
}

//----------------------------------------------------------------
G4String GmProdCutsStudyUA::GetParticleName(G4ParticleDefinition* part) 
{
  if( part == 0 ) {
    return "ALL";
  }else {
    return part->GetParticleName();
  }
}
 
//----------------------------------------------------------------
G4String GmProdCutsStudyUA::GetProcessName(G4VProcess* proc) 
{
  if(proc == 0) {
    return "ALL";
  } else{
    return proc->GetProcessName();
  }
}
