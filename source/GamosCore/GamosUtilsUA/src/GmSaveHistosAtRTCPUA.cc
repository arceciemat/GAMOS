#include "G4Run.hh"
#include "G4RunManager.hh"

#include "GmSaveHistosAtRTCPUA.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosGenerator/include/GmGenerator.hh"
#include "GamosCore/GamosGenerator/include/RTPlanMgr.hh"
 
//----------------------------------------------------------------
GmSaveHistosAtRTCPUA::GmSaveHistosAtRTCPUA()
{
}

//------------------ ----------------------------------------------
void GmSaveHistosAtRTCPUA::BeginOfRunAction( const G4Run* )
{
  G4VUserPrimaryGeneratorAction* gener = const_cast<G4VUserPrimaryGeneratorAction*>(G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  GmGenerator* gmgener = dynamic_cast<GmGenerator*>(gener);
  if( !gmgener ) {
    G4Exception("GmSaveHistosAtRTCPUA::BeginOfRunAction",
		"",
		FatalException,
		"primary generator has to be of type GmGenerator");
  }
  std::vector<GmParticleSource*> gmsources = gmgener->GetSources(); 
  if( gmsources.size() != 1 ) {
    G4Exception("GmSaveHistosAtRTCPUA::BeginOfRunAction",
		"",
		FatalException,
		"There can be only 1 GmSource particle source");
  }

  theRTMgr = RTPlanMgr::GetInstance();

  theBeam = 0;
  theControlPoint = 0;
  theBeamState = 0;
  theParamMgr = GmParameterMgr::GetInstance(); 
  theOriginalSuffix = theParamMgr->GetStringValue("GmAnalysisMgr:FileNameSuffix","");
  bChangeAtBeam = G4bool(theParamMgr->GetNumericValue("GmSaveHistosAtRTCPUA:ChangeAtBeam",1));
  bChangeAtControlPoint = G4bool(theParamMgr->GetNumericValue("GmSaveHistosAtRTCPUA:ChangeAtControlPoint",0));
  bChangeAtBeamState = G4bool(theParamMgr->GetNumericValue("GmSaveHistosAtRTCPUA:ChangeAtBeamState",0));
  bSaveEvent = G4bool(theParamMgr->GetNumericValue("GmSaveHistosAtRTCPUA:SaveEvent",0));
  bDeleteHistos = G4bool(theParamMgr->GetNumericValue("GmSaveHistosAtRTCPUA:DeleteHistos",0));
}

//----------------------------------------------------------------
void GmSaveHistosAtRTCPUA::EndOfEventAction( const G4Event* event )
{
  G4bool bSave = false;
  if( theRTMgr->IsChangedBeam() && bChangeAtBeam ) {
    bSave = true;
  }
  if ( theRTMgr->IsChangedControlPoint() && bChangeAtControlPoint ) {
    bSave = true;
  }
  if( theRTMgr->IsChangedBeamState() && bChangeAtBeamState ) {
    bSave = true;
  }

  //  G4cout << "GmSaveHistosAtRTCPUA::EndOfEventAction SAVE " << bSave << G4endl; //GDEB
  //  G4cout << "GmSaveHistosAtRTCPUA::EndOfEventAction BCHANGE : " << bChangeAtBeam << " " << bChangeAtControlPoint << " " << bChangeAtBeamState << G4endl; //GDEB
  //  G4cout  << "GmSaveHistosAtRTCPUA::EndOfEventAction RTCHANGED : " << theRTMgr->IsChangedBeam() << " " << theRTMgr->IsChangedControlPoint() << " " << theRTMgr->IsChangedBeamState() << G4endl; //GDEB
  if( !bSave ) return;

  G4String name = "B"+GmGenUtils::itoa(theBeam)+"CP"+GmGenUtils::itoa(theControlPoint)+"SP"+GmGenUtils::itoa(theBeamState);
  if( bSaveEvent ) {
    name += "EVT"+GmGenUtils::itoa(event->GetEventID());
  }
  G4cout << "GmSaveHistosAtRTCPUA::EndOfEventAction SAVED OK " << name << G4endl; //GDEB

  /*  std::vector<G4String> vparam{theOriginalSuffix+name};
  theParamMgr->AddParamOfDefinedType("GmAnalysisMgr:FileNameSuffix",vparam, PTstring);

  G4cout << "GmSaveHistosAtRTCPUA::EndOfEventAction SAVED SUFFIX " << theParamMgr->GetStringValue("GmAnalysisMgr:FileNameSuffix",theOriginalSuffix) << G4endl; //GDEB
  */ 

  GmAnalysisMgr* anaMgr = GmAnalysisMgr::GetInstance("");
  std::map<G4String,GmAnalysisMgr*> anaMgrs = anaMgr->GetAllInstances();
  std::map<G4String,GmAnalysisMgr*>::const_iterator itea;
  std::set<G4String> fileFormats = anaMgr->GetFileFormats();
  std::set<G4String>::const_iterator ites;
  for( itea = anaMgrs.begin(); itea != anaMgrs.end(); itea++ ){
    GmAnalysisMgr* anaMgr2 = (*itea).second;
    for( ites = fileFormats.begin(); ites != fileFormats.end(); ites++) {
      anaMgr2->Save( anaMgr2->GetFileName()+name, *ites );
    }
    if( bDeleteHistos ) {
      mih1 histos1 = anaMgr2->GetHistos1D();
      for (mih1::iterator ite = histos1.begin(); ite != histos1.end(); ite++) {
	ite->second->Reset();
      }
      mih2 histos2 = anaMgr2->GetHistos2D();
      for (mih2::iterator ite = histos2.begin(); ite != histos2.end(); ite++) {
	ite->second->Reset();
      }
      mihp1 histosp1 = anaMgr2->GetHistosProf1();
      for (mihp1::iterator ite = histosp1.begin(); ite != histosp1.end(); ite++) {
	ite->second->Reset();
      }
      mihp2 histosp2 = anaMgr2->GetHistosProf2();
      for (mihp2::iterator ite = histosp2.begin(); ite != histosp2.end(); ite++) {
	ite->second->Reset();
      }
    }
  }

  if( theRTMgr->IsChangedBeam() && bChangeAtBeam ) {
    theBeam ++;
    theControlPoint = 0;
    theBeamState = 0;
  }
  if ( theRTMgr->IsChangedControlPoint() && bChangeAtControlPoint ) {
    theControlPoint ++;
    theBeamState = 0;
  }
  if( theRTMgr->IsChangedBeamState() &&  bChangeAtBeamState ) {
    theBeamState ++;
  }
}
