#include "RTVZRLimitsUA.hh"
#include "RTVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4ios.hh"
#include "G4EventManager.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4VSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4VisExtent.hh"
#include "G4UserSteppingAction.hh"
#include "G4PhysicalVolumeStore.hh"

//---------------------------------------------------------------------
RTVZRLimitsUA::RTVZRLimitsUA()
{
  bRussRoul = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("RTVZRLimitsUA:UseRussianRoulette",0));
  theRussRoulValue = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("RTVZRLimitsUA:RussianRouletteValue",100));
}

//---------------------------------------------------------------------
void RTVZRLimitsUA::BeginOfRunAction( const G4Run* )
{

  BuildXYLimits();

}

//---------------------------------------------------------------------
void RTVZRLimitsUA::GetWorld() 
{
  G4PhysicalVolumeStore* pvs = G4PhysicalVolumeStore::GetInstance();
  G4PhysicalVolumeStore::const_iterator ite;
  //--- Get world logical volume and dimensions
  for(ite = pvs->begin(); ite != pvs->end(); ite++ ){
    if( (*ite)->GetMotherLogical() == 0 ) {
      G4VSolid* solid =  (*ite)->GetLogicalVolume()->GetSolid();
      if( solid->GetEntityType() == "G4Box" ) {
	G4Box* box = (G4Box*)solid;
	theWorldX = box->GetXHalfLength();
	theWorldY = box->GetYHalfLength();
	theWorldZ = box->GetZHalfLength();
      } else if( solid->GetEntityType() == "G4Tubs" ) {
	G4Tubs* tubs = (G4Tubs*)solid;
	theWorldX = tubs->GetOuterRadius();
	theWorldY = tubs->GetOuterRadius();
	theWorldZ = tubs->GetZHalfLength();
      } else {
	G4Exception("RTZRLimitsUA::BeginOfRunAction",
		    " world is not a G4Box or G4Tubs",
		    JustWarning,
		    (G4String(" is is a ")+solid->GetEntityType()).c_str());
	theWorldZ = DBL_MAX;
      }
      theWorldLogical = (*ite)->GetLogicalVolume();
      break;
    } 
  }
}


//---------------------------------------------------------------------
void RTVZRLimitsUA::UserSteppingAction(const G4Step* aStep)
{ 
  //--- Get in which theXYLimits it is in
  G4ThreeVector pos = aStep->GetPostStepPoint()->GetPosition();
  G4ThreeVector dir = aStep->GetPostStepPoint()->GetMomentumDirection();
  std::map<G4double,XYdata>::const_iterator ite = theXYlimits.upper_bound( pos.z() );
  if( ite == theXYlimits.end() ) ite--; // for particles exiting world

  /*  if( dir.z() < 0. ) {
    if( ite != theXYlimits.begin() ) ite--;
    }*/

  XYdata dat = (*ite).second;
  if( pos.x() < dat.xmin || pos.x() > dat.xmax
      || pos.y() < dat.ymin || pos.y() > dat.ymax ){
    G4Track* aTrack = const_cast<G4Track*>(aStep->GetTrack());
    if( bRussRoul ){
      G4double rnd = CLHEP::RandFlat::shoot()*theRussRoulValue;
      if( rnd <= 1. ) {
	aTrack->SetWeight( aTrack->GetWeight()*theRussRoulValue );
      }else {
	aTrack->SetTrackStatus( fStopAndKill );
      }
    } else  {
      aTrack->SetTrackStatus( fStopAndKill );
    }
#ifndef GAMOS_NO_VERBOSE
    if( RTVerb(debugVerb) ) G4cout << "RTVZRLimitsUA::UserSteppingAction KILLING track at " << pos << " dir " << aTrack->GetMomentumDirection() << " XY_z= " << (*ite).first << " XY_dat= " << dat << G4endl;
#endif
    theXYcounts[ (*ite).first ] += 1;
  }

}


//---------------------------------------------------------------------
void RTVZRLimitsUA::EndOfRunAction( const G4Run* )
{
  std::map<G4double,G4String>::const_iterator itename;
  std::map<G4double,G4int>::const_iterator itec;
    G4cout << "%%%%%%%%%%%% Number of killed particles with too big X or Y " << G4endl;
  for( itec = theXYcounts.begin(); itec != theXYcounts.end(); itec++ ){
    
    G4cout << " %%% VOL: " << (*(theXYvolName.find( (*itec).first ))).second << " = " << (*itec).second << "     " << (*(theXYlimits.find( (*itec).first ))).second << G4endl;
  }
}


//---------------------------------------------------------------------
std::ostream& operator<<(std::ostream& out, const RTVZRLimitsUA::XYdata& dat)
{
  out << " DAT: xmin= " << dat.xmin << " " << " xmax= " << dat.xmax << " ymin= " << dat.ymin << " ymax= " << dat.ymax;

  return out;
}
