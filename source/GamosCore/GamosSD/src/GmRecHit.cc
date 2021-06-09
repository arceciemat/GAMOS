#include "GmRecHit.hh"
#include "GmHit.hh"
#include "GmHitsEventMgr.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VVisManager.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosSD/include/GmSDVerbosity.hh"


//----------------------------------------------------------------------
GmRecHit::GmRecHit( GmHit* hit )
{
  theSDType = hit->GetSDType();
  theTimeMin = hit->GetTimeMin();
  theTimeMax = hit->GetTimeMax();
  theEnergy = hit->GetEnergy();
  thePosition = hit->GetPosition();
  theHits.push_back( hit );
  
  theMaxSimHitEnergy = theEnergy;
  
  G4String sdtype = theSDType.substr(1,theSDType.size()-2);

  bPosAtBarycentre = G4bool(GmParameterMgr::GetInstance()->GetNumericValue("SD:RecHit:PosAtBarycentre:"+sdtype,0));

  theHitsTimeType = GmHitsEventMgr::GetInstance()->GetHitsTimeType();
  theHitsTimeType = 1;
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " Created GmRecHit E " << theEnergy << " posXYZ " << thePosition.x() << " pos " << thePosition.y() << " pos " << thePosition.z() << " time " << GetTime() << G4endl;
#endif

}


//----------------------------------------------------------------------
void GmRecHit::AddHit( GmHit* hit )
{

  //---- check SDType
  if( theSDType != hit->GetSDType() ) {
    G4Exception("GmRecHit::AddHit",
		"Wrong argument",
		FatalErrorInArgument,
		"Hit with SD type");
    //    G4Exception("GmRecHit::AddHit hit with SD type= " + hit->GetSDType + " different as previous hits " + theSDType);
  }

  theTimeMin = std::min( theTimeMin, hit->GetTimeMin() );
  theTimeMax = std::max( theTimeMax, hit->GetTimeMax() );
  //--- position is barycentre of hits
  if (bPosAtBarycentre) {
    thePosition = (thePosition*theEnergy + hit->GetPosition()*hit->GetEnergy()) / (theEnergy +  hit->GetEnergy() );
  } else {
    if( hit->GetEnergy() > theMaxSimHitEnergy ){ 
      theMaxSimHitEnergy = hit->GetEnergy();
      thePosition = hit->GetPosition();
    }
  }
  theHits.push_back( hit );

  theEnergy += hit->GetEnergy();

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " Update GmRecHit E " << theEnergy 
	 << " pos " << thePosition.x() << " pos " << thePosition.y() << " pos " << thePosition.z() << " time " << GetTime() << G4endl;
#endif
  //<< thePosition.mag() << " pos " << thePosition.phi() << " pos " << thePosition.theta() << G4endl;


  /*
  std::vector<GmHit*>::const_iterator ite,ite1;
  for( ite1 = theHits.begin(); ite1 != theHits.end()-1; ite1++ ){
    if( (hit->GetPosition() - (*ite1)->GetPosition() ).mag() > 90.*mm ) {
#ifndef GAMOS_NO_VERBOSE
      if( SDVerb(infoVerb) ) G4cout << " !!! TOO BIG HITS DISTANCE " <<  (hit->GetPosition() - (*ite1)->GetPosition() ).mag() << " " << theHits.size() << G4endl; 
#endif
      for( ite = theHits.begin(); ite != theHits.end(); ite++ ){
#ifndef GAMOS_NO_VERBOSE
	if( SDVerb(infoVerb) ) G4cout << (*ite)->GetDetUnitID() << " HIT: " << (*ite)->GetPosition() << G4endl;
#endif
      }
    }
  }
  */
}



//----------------------------------------------------------------------
#include "G4Visible.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

void GmRecHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager) {
    G4Circle circle(thePosition);
    circle.SetScreenSize(0.4);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}


//----------------------------------------------------------------------
G4double GmRecHit::GetTime() const 
{

  if( theHitsTimeType == 1 ) {
    return GetTimeMin();
  } else if( theHitsTimeType == 2 ) {
    return GetTimeMax();
  } else {
    return GetTimeMin();
    G4Exception("GmRecHit::GetTime",
		"Wrong argument",
		FatalErrorInArgument,
		"SD:Hits:TimeType <> 1 not implemented yet");
    return 0.; // to avoid compilation warning
  }

}


//----------------------------------------------------------------------
void GmRecHit::Print(std::ostream& os, G4bool bVerbose)
{
  if( bVerbose ) os << " Gamos RecHit;  energy= ";
  os << theEnergy << " ";
  if( bVerbose ) os << " time_min= ";
  os << (unsigned long long)(theTimeMin) << " ";
  if( bVerbose ) os << " time_max= ";
  os << (unsigned long long)(theTimeMax) << " "; 
  if( bVerbose ) os << " posXYZ= ";
  os << thePosition.x() << " " << thePosition.y() << " " << thePosition.z() << " ";
  if( bVerbose ) os << " N SimHits= ";
  os << theHits.size() << G4endl;
}


//----------------------------------------------------------------------
void GmRecHit::Print(FILE* fout, G4bool )
{
  GmRecHitPrint rhp;
  rhp.energy = theEnergy;
  rhp.timeMin = (unsigned long long)(theTimeMin);
  rhp.timeMax = (unsigned long long)(theTimeMax);
  G4ThreeVector pos = thePosition;
  rhp.posX = pos.x();
  rhp.posY = pos.y();
  rhp.posZ = pos.z();
  rhp.nhits = theHits.size();
  fwrite (&rhp, sizeof(struct GmRecHitPrint),1,fout);
}


//----------------------------------------------------------------------
G4ThreeVector GmRecHit::GetPositionOfMaxEnergy() const 
{ 
  std::vector<GmHit*>::const_iterator ite;
  GmHit* maxHit = 0;
  G4double maxE = 0.;
  for( ite = theHits.begin(); ite != theHits.end(); ite++ ){
    if( (*ite)->GetEnergy() > maxE ){
      maxHit = *ite;
      maxE = (*ite)->GetEnergy();
    }
  }
 
  return maxHit->GetPosition();
}

//----------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const GmRecHit& rhit)
{
  GmRecHit rhitnc = rhit;
  rhitnc.Print( os );

  return os;
}
