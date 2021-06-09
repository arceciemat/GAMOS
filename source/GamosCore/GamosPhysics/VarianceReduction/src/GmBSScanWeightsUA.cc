#define _USE_MATH_DEFINES
#include <cmath>

#include "GmBSScanWeightsUA.hh"
#include "GmBSTrajectory.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"

GmBSScanWeightsUA::GmBSScanWeightsUA()
{
  theAnaMgr = GmAnalysisMgr::GetInstance("BSweights");

  thePlaneZ = GmParameterMgr::GetInstance()->GetNumericValue("GmBSScanWeightsUA:PlaneZ", 1.*m );
  thePlaneHalfWidth = GmParameterMgr::GetInstance()->GetNumericValue("GmBSScanWeightsUA:PlaneHalfWidth", 10.*cm );

  bPrimary = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("GmBSScanWeightsUA:ConsiderPrimary", 0));

}

//---------------------------------------------------------------------
void GmBSScanWeightsUA::BeginOfRunAction( const G4Run* )
{  
}

//---------------------------------------------------------------------
void GmBSScanWeightsUA::BeginOfEventAction( const G4Event* evt )
{
  if( evt->GetEventID() == 0 ) {
    DefineHistoLimits();
    BookHistos("All");
    BookHistos("ReachZParent");  
    BookHistos("ReachZGammaAncestors");  
  }

  theTrajectories.clear();

}

//---------------------------------------------------------------------
void GmBSScanWeightsUA::DefineHistoLimits()
{
  theNbins = G4int( GmParameterMgr::GetInstance()->GetNumericValue("GmBSScanWeightsUA:Nbins", 100 ) );
  G4Box* world = (G4Box*)(GmGeometryUtils::GetInstance()->GetTopLV()->GetSolid());

  theHisXY = std::max(world->GetXHalfLength(),world->GetYHalfLength())/2.;
  theHisZ = world->GetZHalfLength();
  G4double mom = fpEventManager->GetConstCurrentEvent()->GetPrimaryVertex(0)->GetPrimary(0)->GetMomentum().mag();
  G4double mass = fpEventManager->GetConstCurrentEvent()->GetPrimaryVertex(0)->GetPrimary(0)->GetMass();
  theHisE = sqrt(mom*mom+mass*mass)-mass;
}

//---------------------------------------------------------------------
void GmBSScanWeightsUA::BookHistos(G4String type)
{

  G4String hnam;
  G4String hgnam = "GmBSScanWeights: ";

  G4int hid = 0; 
  if( type == "All" ) {
    hid = 0;
  }else  if( type == "ReachZParent" ) {
    hid = 1;
  }else  if( type == "ReachZGammaAncestors" ) {
    hid = 2;
  }

  hnam = hgnam + type + G4String(": X");
  theAnaMgr->CreateHisto1D(hnam,theNbins,-theHisXY,theHisXY,34000+hid*100+1);
  hnam = hgnam + type + G4String(": Y");
  theAnaMgr->CreateHisto1D(hnam,theNbins,-theHisXY,theHisXY,34000+hid*100+2);
  hnam = hgnam + type + G4String(": Z");
  theAnaMgr->CreateHisto1D(hnam,theNbins,-theHisZ,theHisZ,34000+hid*100+3);
  hnam = hgnam + type + G4String(": R");
  theAnaMgr->CreateHisto1D(hnam,theNbins,0.,theHisXY,34000+hid*100+4);
  hnam = hgnam + type + G4String(": Direction Theta");
  theAnaMgr->CreateHisto1D(hnam,theNbins,0.,180.,34000+hid*100+5);
  hnam = hgnam + type + G4String(": Direction Phi");
  theAnaMgr->CreateHisto1D(hnam,theNbins,-180.,180.,34000+hid*100+6);
  hnam = hgnam + type + G4String(": Energy");
  theAnaMgr->CreateHisto1D(hnam,theNbins,0.,20.*MeV,34000+hid*100+7);
    
  hnam = hgnam + type + G4String(": R vs Z");
  theAnaMgr->CreateHisto2D(hnam,theNbins,0,theHisXY,theNbins,0.,theHisE,34000+hid*100+11);
  hnam = hgnam + type + G4String(": R vs Direction Theta");
  theAnaMgr->CreateHisto2D(hnam,theNbins,0,theHisXY,theNbins,-M_PI/2.,M_PI/2.,34000+hid*100+12);

}

//---------------------------------------------------------------------
void GmBSScanWeightsUA::PreUserTrackingAction(const G4Track* aTrack )
{
  //  if( aTrack->GetDefinition() == G4Gamma::Gamma() ){
    theTrajectories.push_back( new GmBSTrajectory( aTrack ) );
    //  }
}

//---------------------------------------------------------------------
void GmBSScanWeightsUA::UserSteppingAction(const G4Step* aStep)
{
  G4ThreeVector posPre = aStep->GetPreStepPoint()->GetPosition();
  G4ThreeVector posPost = aStep->GetPostStepPoint()->GetPosition();
  if( posPre.z() < thePlaneZ && posPost.z() >= thePlaneZ ){
    G4double rat = (thePlaneZ-posPre.z())/(posPost.z()-posPre.z());
    G4double posX=posPre.x()+(posPost.x()-posPre.x())*rat;
    G4double posY=posPre.y()+(posPost.y()-posPre.y())*rat;
    if( fabs(posX) < thePlaneHalfWidth && fabs(posY) < thePlaneHalfWidth ){
      //---- Look for all ancestor trajectories that are gamma
      G4cout << "GmBSScanWeightsUA::UserSteppingAction  plane reached " << G4endl;
      GmBSTrajectory* traj = *(theTrajectories.rbegin()); //current track trajectory
      G4int ii = 0;
      while( traj != 0){ 
	if( traj->GetParticleName() == "gamma") {
	  FillHistos(traj, 2);
	}
	traj = GetParent(traj);
	ii++;
	if( ii == 1 ) FillHistos(traj, 1);
      }
    }
  }
}
//---------------------------------------------------------------------
GmBSTrajectory* GmBSScanWeightsUA::GetParent(GmBSTrajectory* traj) const
{

  G4int parentID = traj->GetParentID();
  GmBSTrajectory* parentTraj = 0;
  std::vector<GmBSTrajectory*>::const_iterator ite;
  for( ite = theTrajectories.begin(); ite != theTrajectories.end(); ite++ ){
    if( (*ite)->GetTrackID() == parentID ){
      parentTraj = *ite;
      break;
    }
  }

  return parentTraj;
}

//---------------------------------------------------------------------
void GmBSScanWeightsUA::EndOfEventAction( const G4Event* )
{
  std::vector<GmBSTrajectory*>::const_iterator ite;
  for( ite = theTrajectories.begin(); ite != theTrajectories.end(); ite++ ){
    if( (*ite)->GetParticleName() == "gamma" ){
      FillHistos( *ite, 0 );
    }
  }

}


//---------------------------------------------------------------------
void GmBSScanWeightsUA::FillHistos( GmBSTrajectory* traj, G4int hid )
{
  if( traj == 0 ){
    G4cout << "TRAJ IS = 0 " << G4endl; 
    std::vector<GmBSTrajectory*>::const_iterator ite;
    for( ite = theTrajectories.begin(); ite != theTrajectories.end(); ite++ ){
      G4cout << "TRAJ: " << traj->GetTrackID() << " parent " << traj->GetParentID() << G4endl;
    }
    return; 
  }

  if( !bPrimary && traj->GetParentID() == 0 ) return;

  G4double posX = traj->GetPosX();
  G4double posY = traj->GetPosY();
  G4double posZ = traj->GetPosZ();
  G4double posR = sqrt(posX*posX+posY*posY);
  G4double theta = traj->GetDirectionTheta();
  G4double phi = traj->GetDirectionPhi();
  G4double energy = traj->GetEnergy();
  //  G4cout << "  GmBSScanWeightsUA::FillHistos theta " << hid << " " << theta << " " << (theta)/deg << " " << traj->GetInitialMomentum() << G4endl;
    // G4cout << "  GmBSScanWeightsUA::FillHistos " << hid << " " << posX << " " << posY << " " << posR << " " << theta << " " << phi << " " << energy << G4endl;

  theAnaMgr->GetHisto1(34000+100*hid+1)->Fill( posX );
  theAnaMgr->GetHisto1(34000+100*hid+2)->Fill( posY );
  theAnaMgr->GetHisto1(34000+100*hid+3)->Fill( posZ );
  theAnaMgr->GetHisto1(34000+100*hid+4)->Fill( posR );
  theAnaMgr->GetHisto1(34000+100*hid+5)->Fill( theta/deg );
  theAnaMgr->GetHisto1(34000+100*hid+6)->Fill( phi/deg );
  theAnaMgr->GetHisto1(34000+100*hid+7)->Fill( energy );
  theAnaMgr->GetHisto2(34000+100*hid+11)->Fill( posR, posZ );
  theAnaMgr->GetHisto2(34000+100*hid+12)->Fill( posR, theta );
  
}
