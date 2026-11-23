#include "GmDigit.hh"
#include "GmHit.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VVisManager.hh"
#include "GamosCore/GamosSD/include/GmHitsEventMgr.hh"
#include "GamosCore/GamosSD/include/GmSDVerbosity.hh"


//----------------------------------------------------------------------
GmDigit::GmDigit( GmHit* hit )
{
  theSDType = hit->GetSDType();
  theTimeMin = hit->GetTimeMin();
  theTimeMax = hit->GetTimeMax();
  theEnergy = hit->GetEnergy();
  thePosition = hit->GetPosition();
  theHits.push_back( hit );

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " Created GmDigit E " << theEnergy << " pos " << thePosition.mag() << " pos " << thePosition.phi() << " pos " << thePosition.theta() << G4endl;
#endif

  theHitsTimeType = GmHitsEventMgr::GetInstance()->GetHitsTimeType();

}


//----------------------------------------------------------------------
void GmDigit::AddHit( GmHit* hit )
{

  //---- check SDType
  if( theSDType != hit->GetSDType() ) {
    G4Exception("GmDigit::AddHit",
		"Error",
		FatalException,
		"Hit with SD type= ");
    //    G4Exception("GmDigit::AddHit hit with SD type= " + hit->GetSDType + " different as previous hits " + theSDType);
  }

  theTimeMin = std::min( theTimeMin, hit->GetTimeMin() );
  theTimeMax = std::min( theTimeMax, hit->GetTimeMax() );
  //--- position is barycentre of hits
  thePosition = (thePosition*theEnergy + hit->GetPosition()*hit->GetEnergy()) / (theEnergy +  hit->GetEnergy() );

  theHits.push_back( hit );

  theEnergy += hit->GetEnergy();

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " Update GmDigit E " << theEnergy << " pos " << thePosition.mag() << " pos " << thePosition.phi() << " pos " << thePosition.theta() << G4endl;
#endif



  std::vector<GmHit*>::const_iterator ite,ite1;
  for( ite1 = theHits.begin(); ite1 != theHits.end()-1; ite1++ ){
    if( (hit->GetPosition() - (*ite1)->GetPosition() ).mag() > 90. ) {
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

}


//----------------------------------------------------------------------
#include "G4Visible.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

void GmDigit::Draw()
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
G4double GmDigit::GetTime() const 
{
  if( theHitsTimeType == 1 ) {
    return GetTimeMin();
  } else { 
    G4Exception("GmDigit::GetTime",
		"Error",
		FatalException,
		"With SD:Hits:TimeType <> 1 not implemented yet");
    return 0.; // to avoid compilation warning
  }

}


//----------------------------------------------------------------------
void GmDigit::Print(std::ostream& os)
{
  os << " Gamos Digit " 
     << " energy= " << theEnergy
     << " time_min= " << theTimeMin
     << " time_max= " << theTimeMax
     << " pos= " << thePosition.mag() << " " << thePosition.theta() << " " << thePosition.phi()
     << G4endl;
}


//----------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const GmDigit& digi)
{
  GmDigit diginc = digi;
  diginc.Print( os );

  return os;
}
