#include "PETHistosPositron.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmVVerbosity.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4VVisManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"
#include "G4RunManager.hh"

#include <math.h>
using namespace CLHEP;

//----------------------------------------------------------------
PETHistosPositron::PETHistosPositron()
{

  theAnaMgr = GmAnalysisMgr::GetInstance("pet") ;

  std::string hnam;
  std::string hgnam = "PETPositron: ";

  // Create histograms for positron energy and range
  G4double dmax = 5.;
  hnam = hgnam + "e+ initial energy (keV)";
  theAnaMgr->CreateHisto1D(hnam,100,0,2000,12001);
  hnam = hgnam + "e+ range (mm)";
  theAnaMgr->CreateHisto1D(hnam,1000,0,dmax,12002);
  hnam = hgnam + "e+ energy at annihilation (keV)";
  theAnaMgr->CreateHisto1D(hnam,100,0,1000,12003);
  hnam = hgnam + "e+ time at annihilation (ns)";
  theAnaMgr->CreateHisto1D(hnam,100,0,0.01,12004);
  hnam = hgnam + "e+ distance to origin (mm)";
  theAnaMgr->CreateHisto1D(hnam,1000,0,dmax,12005);
  hnam = hgnam + "e+ distance to origin XY (mm)";
  theAnaMgr->CreateHisto1D(hnam,1000,0,dmax,12006);
  hnam = hgnam + "e+ distance to origin Z (mm)";
  theAnaMgr->CreateHisto1D(hnam,1000,0,dmax,12007);
  hnam = hgnam + "e+ distance to origin / range ";
  theAnaMgr->CreateHisto1D(hnam,1000,0,1.,12008);
  hnam = hgnam + "e+ range - distance to origin (mm)";
  theAnaMgr->CreateHisto1D(hnam,1000,0,dmax,12009);

  hnam = hgnam + "e+ initial energy (keV) vs range (mm)";
  theAnaMgr->CreateHisto2D(hnam,100,0,2000.,200,0,dmax,12011);
  // Create 2D histogram for correlation between secondary gammas momentum and e+ kinetic energy at annihilation
  hnam = hgnam + "e+ gammas vs e+ energy (keV)";
  theAnaMgr->CreateHisto2D(hnam,100,0,2000,100,0,1000,12012);
  hnam = hgnam + "total gamma energy - 2*511keV vs e+ energy (keV)";
  theAnaMgr->CreateHisto2D(hnam,100,-200,200,100,0,1000,12013);

  hnam = hgnam + "e+ initial direction theta (deg) vs distance XY (mm)";
  theAnaMgr->CreateHisto2D(hnam,100,0,180.+1.e-6,200,0,dmax,12014);
  hnam = hgnam + "e+ range (mm) vs distance XY (mm)";
  theAnaMgr->CreateHisto2D(hnam,100,0,dmax,200,0,dmax,12015);
  hnam = hgnam + "e+ range (mm) vs distance (mm)";
  theAnaMgr->CreateHisto2D(hnam,100,0,dmax,200,0,dmax,12016);
  hnam = hgnam + "e+ gammas angle (deg)";
  theAnaMgr->CreateHisto1D(hnam,180,0.,180.+1.e-6,12017);
  hnam = hgnam + "e+ gammas angle (deg) vs e+ energy (keV)";
  theAnaMgr->CreateHisto2D(hnam,180,0.,180.+1.e-6,100,0.,1000.,12018);


}

//----------------------------------------------------------------
void PETHistosPositron::PreUserTrackingAction(const G4Track* aTrack)
{
  if( aTrack->GetParentID() != 0 || aTrack->GetDefinition()->GetParticleName() != "e+" ) return;
  theAnaMgr->GetHisto1( 12001 )->Fill( aTrack->GetKineticEnergy()*1000.); // e+ initial kinetic energy
}


//----------------------------------------------------------------
void PETHistosPositron::PostUserTrackingAction(const G4Track* aTrack)
{
  if( aTrack->GetParentID() != 0 || aTrack->GetDefinition()->GetParticleName() != "e+" ) return;

  theAnaMgr->GetHisto2( 12011 )->Fill( aTrack->GetVertexKineticEnergy()*1000., aTrack->GetTrackLength() );

  theAnaMgr->GetHisto2( 12014 )->Fill( aTrack->GetVertexMomentumDirection().theta()/deg, (aTrack->GetPosition() - aTrack->GetVertexPosition()).perp() ); // e+ distance to vertex at annihilation 

  theAnaMgr->GetHisto2( 12015 )->Fill( aTrack->GetTrackLength(), (aTrack->GetPosition() - aTrack->GetVertexPosition()).perp() ); // e+ distance to vertex at annihilation 
  theAnaMgr->GetHisto2( 12016 )->Fill( aTrack->GetTrackLength(), (aTrack->GetPosition() - aTrack->GetVertexPosition()).mag() ); // e+ distance to vertex at annihilation 
  //  G4cout << " dist " << (aTrack->GetPosition() - aTrack->GetVertexPosition()).mag() << " trklen " <<  aTrack->GetTrackLength() << " / " << (aTrack->GetPosition() - aTrack->GetVertexPosition()).mag() /  aTrack->GetTrackLength() << G4endl;

  theAnaMgr->GetHisto1( 12002 )->Fill( aTrack->GetTrackLength() ); // e+ track length
  //theAnaMgr->GetHisto1( 12003 )->Fill( aTrack->GetKineticEnergy()*1000.); // e+ kinetic energy at annihilation;

  // theAnaMgr->GetHisto1( 12003 )->Fill( aStep->GetPreStepPoint()->GetKineticEnergy()*1000.); // e+ kinetic energy at annihilation;

  theAnaMgr->GetHisto1( 12005 )->Fill( (aTrack->GetPosition() - aTrack->GetVertexPosition()).mag() ); // e+ distance to vertex at annihilation
  theAnaMgr->GetHisto1( 12006 )->Fill( (aTrack->GetPosition() - aTrack->GetVertexPosition()).perp() ); // e+ distance to vertex at annihilation
  theAnaMgr->GetHisto1( 12007 )->Fill( (aTrack->GetPosition() - aTrack->GetVertexPosition()).z() ); // e+ distance to vertex at annihilation

  theAnaMgr->GetHisto1( 12008 )->Fill( (aTrack->GetPosition() - aTrack->GetVertexPosition()).mag() / aTrack->GetTrackLength() ); 

  theAnaMgr->GetHisto1( 12009 )->Fill(  aTrack->GetTrackLength() - (aTrack->GetPosition() - aTrack->GetVertexPosition()).mag() ); 
} 

//----------------------------------------------------------------
void PETHistosPositron::UserSteppingAction(const G4Step* aStep)
{
  const G4Track* aTrack = aStep->GetTrack();


  /*
if( G4EventManager::GetEventManager()->GetNonconstCurrentEvent()->GetEventID() > 100000) {
GmVVerbosity::FindManager("GmSDVerbosity")->SetFilterLevel(5);
  if( aTrack->GetDefinition()->GetParticleName() == "gamma" ) {

G4TouchableHistory* touch = (G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable());

for( int ii = 0; ii < touch->GetHistoryDepth(); ii++ ){
G4VPhysicalVolume* pv = touch->GetVolume( ii );
//      G4cout << " copyNO " << pv->GetCopyNo() << " name " << pv->GetName() << G4endl;
}
}
  
  if( aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() == "Compt" && fabs( aStep->GetPreStepPoint()->GetKineticEnergy()-0.511 ) < 0.001 ) {
    G4double angle = acos( aStep->GetPreStepPoint()->GetMomentumDirection()*aStep->GetPostStepPoint()->GetMomentumDirection());
    G4cout << " angle " << angle/deg << " E diff " << aStep->GetDeltaEnergy() << " pre dir " << aStep->GetPreStepPoint()->GetMomentumDirection() << " post dir " << aStep->GetPostStepPoint()->GetMomentumDirection()<< G4endl;
  } 
}
  */

  /*  std::string processName = "";
  if(aStep->GetPostStepPoint()->GetProcessDefinedStep() != NULL){
    processName = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
    }

  if( processName != "annihil" ) return;
  */

  //G4cout << " mate " <<  aStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetMaterial()->GetName() << G4endl;
  if( aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() != "annihil" ) return;

  theAnaMgr->GetHisto1( 12003 )->Fill( aStep->GetPreStepPoint()->GetKineticEnergy()*1000.); // e+ kinetic energy at annihilation;  
  
  theAnaMgr->GetHisto1( 12004 )->Fill( aTrack->GetLocalTime()*ns ); // e+ time at annihilation;
  //  G4cout << " e+ time " <<  aTrack->GetLocalTime()*ns << G4endl;

  G4bool bPrintPos = FALSE;
  if( bPrintPos ) G4cout << " e+POS " << aTrack->GetPosition().x() << " " <<aTrack->GetPosition().y() << " " <<aTrack->GetPosition().z() << " VTX " <<
aTrack->GetVertexPosition().x() << " " << aTrack->GetVertexPosition().y() << " " <<aTrack->GetVertexPosition().z() << " " << G4endl;
 
  G4TrackVector* secondaries = fpTrackingManager->GimmeSecondaries();
  unsigned int siz = secondaries->size();
  G4double eGammas = 0.;
  std::vector<G4ThreeVector> dirGamma;
  for( unsigned int ii = siz-2; ii < siz; ii++){
    //   G4cout << siz << "secondary gamma P "<<(*secondaries)[ii]->GetMomentum() << G4endl;
    dirGamma.push_back( (*secondaries)[ii]->GetMomentumDirection() );
    eGammas += (*secondaries)[ii]->GetKineticEnergy();
    theAnaMgr->GetHisto2( 12012 )->Fill( (*secondaries)[ii]->GetKineticEnergy()/keV, aStep->GetPreStepPoint()->GetKineticEnergy()/keV);

    //    if( fabs((*secondaries)[ii]->GetKineticEnergy()-0.511) > 0.01 ) G4cout << " gamma energy " << (*secondaries)[ii]->GetKineticEnergy() << " e+ pre " <<  aStep->GetPreStepPoint()->GetKineticEnergy()/keV << " e+ post " <<  aStep->GetPostStepPoint()->GetKineticEnergy()/keV << G4endl;
  }
  //  if( aStep->GetPreStepPoint()->GetKineticEnergy()/keV > 0.001 ) G4cout << "BIG energy at e+ annihil " <<  aStep->GetPreStepPoint()->GetKineticEnergy()/keV << G4endl;
  theAnaMgr->GetHisto2(12013)->Fill(( eGammas-(1.022)-(aStep->GetPreStepPoint()->GetKineticEnergy()))/keV, aStep->GetPreStepPoint()->GetKineticEnergy()/keV);

  if( siz >= 2 ) {
    G4double gAngle = acos( dirGamma[0]*dirGamma[1] );
    theAnaMgr->GetHisto1(12017)->Fill( gAngle/deg );
    //if( G4EventManager::GetEventManager()->GetNonconstCurrentEvent()->GetEventID() > 100000 )  G4cout << " gamma Angle " << gAngle/deg << " dir1 " << dirGamma[0] << " dir2 " << dirGamma[1] << G4endl;
    
    theAnaMgr->GetHisto2(12018)->Fill( gAngle/deg, aStep->GetPreStepPoint()->GetKineticEnergy()/keV );
  }else {
//    G4cout << " Less than two gammas from e+ !!! " << siz << G4endl;
  }
				    
}
