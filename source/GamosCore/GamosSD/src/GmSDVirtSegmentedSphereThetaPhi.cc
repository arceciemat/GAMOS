#define _USE_MATH_DEFINES
#include <cmath>

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Sphere.hh"
#include "GmSDVirtSegmentedSphereThetaPhi.hh"
#include "GmHit.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosSD/include/GmSDVerbosity.hh"

#include "CLHEP/Random/RandGauss.h"


//-------------------------------------------------------------------
GmSDVirtSegmentedSphereThetaPhi::GmSDVirtSegmentedSphereThetaPhi(G4String sdtype ): GmSDVirtSegmentedSphere( sdtype )
{
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << sdtype << "GmSDVirtSegmentedSphereThetaPhi:->"<<SensitiveDetectorName << G4endl;
#endif

  GmParameterMgr* parmgr = GmParameterMgr::GetInstance();
  theRResolution =  parmgr->GetNumericValue("SD:VirtSegmentedSphereThetaPhi:Rresolution:"+sdtype, 0.*CLHEP::ns); // save it here for efficiency

}

//-------------------------------------------------------------------
GmSDVirtSegmentedSphereThetaPhi::~GmSDVirtSegmentedSphereThetaPhi()
{
}

//-------------------------------------------------------------------
void GmSDVirtSegmentedSphereThetaPhi::DefineSegmentation(const G4String& sdtype)
{
  DefineThetaPhiSegmentation(sdtype);
}

//-------------------------------------------------------------------
void GmSDVirtSegmentedSphereThetaPhi::DefineThetaPhiSegmentation(const G4String& sdtype)
{

  GmParameterMgr* parmgr = GmParameterMgr::GetInstance();
  G4double cellThetaPhi = 10.*CLHEP::mm;
  cellThetaPhi = parmgr->GetNumericValue("SD:cellThetaPhi:"+sdtype, 10.*CLHEP::mm);

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << sdtype << " SD cellThetaPhi " << cellThetaPhi << G4endl;
#endif

  if( cellThetaPhi == 0. ) {
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(errorVerb) )  G4cerr << sdtype << " SD cellThetaPhi " << cellThetaPhi << G4endl;
#endif
    G4Exception("GmSDVirtSegmentedSphereThetaPhi::DefineThetaPhiSegmentation",
		"Wrong argument",
		FatalErrorInArgument,
		"CellThetaPhi cannot be 0. ");
  }

#ifndef GAMOS_NO_VERBOSE
  //  if( SDVerb(infoVerb) ) G4cout << " GmSDVirtSegmentedSphereThetaPhi::DefineThetaPhiSegmentation() for an Sphere centered at 0 " << theLogVol->GetName() << G4endl;
#endif

  if( theLogVol->GetSolid()->GetEntityType() != "G4Sphere" ) {
    G4Exception("GmSDVirtSegmentedSphereThetaPhi::DefineThetaPhiSegmentation",
		"Wrong argument",
		FatalErrorInArgument,
		"For a volume that it is not an sphere ");
  }
  G4Sphere* sphere = (G4Sphere*)(theLogVol->GetSolid());
  theRadius = sphere->GetInsideRadius();

  //- Get first angle as the one that make a casquette of L*L
  //-  ang.push_back( acos(cellThetaPhi*cellThetaPhi/(2*M_PI*R*R) ) );
  // Get how many rings can be accomodated
  float nring = M_PI * theRadius / cellThetaPhi;
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << "sphere radius " << theRadius << " nring " << nring << G4endl;
#endif
  // make it the closest integer so that the percentage of increase/decrease is minimum (1.02 bigger heights is better than 0.91)
  float ext = nring - int(nring);
  ( ext / (1-ext) < nring/(nring+1) ) ? nring = int(nring) : nring = int(nring+1);
  theThetaAngle = M_PI / nring;
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " theThetaAngle " << theThetaAngle << " nring " << nring << G4endl;
#endif
  MAXNDIV = G4int(parmgr->GetNumericValue("SD:VirtSegmented/MaxNdivisions", 2000));
  if( nring > MAXNDIV ) {
#ifndef GAMOS_NO_VERBOSE
     if( SDVerb(errorVerb) )  G4cerr << "!!! Too big numbers of theta divisions = " << nring << " > " << MAXNDIV << G4endl;
#endif
     G4Exception("GmSDVirtSegmentedSphereThetaPhi::DefineThetaPhiSegmentation",
		 "Wrong argument",
		 FatalErrorInArgument,
		 G4String("nring " + GmGenUtils::ftoa(nring) + " > MAXNDIV " + GmGenUtils::ftoa(MAXNDIV)).c_str());
  }

  // calculate how many cells can be accomated per ring
  for( int ii = 0; ii < nring; ii++ ){
    // ThetaPhi radius at center of ring
    float ang = theThetaAngle*(ii+0.5);
    float rad = theRadius*sin(ang);
    float nring2 = 2.*M_PI*rad/cellThetaPhi;
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(infoVerb) ) G4cout << "ring radius " << rad << " nring " << nring2 << G4endl;
#endif
    // make it the closest integer so that the percentage of increase/decrease is minimum (1.02 bigger heights is better than 0.91)
    ext = nring2 - int(nring2);
    //    G4cout << " nring1 " << ext / (1-ext) << " nring2 " << nring/(nring+1) << G4endl;
    ( ext / (1-ext) < nring2/(nring2+1) ) ? nring2 = int(nring2) : nring2 = int(nring2+1);
    thePhiAngles.push_back( 2.*M_PI/nring2 );
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(infoVerb) ) G4cout << ii << " thePhiAngle " << thePhiAngles[ii] << " nring " << nring2 << G4endl; 
#endif
    if( nring2 > MAXNDIV ) {
#ifndef GAMOS_NO_VERBOSE
      if( SDVerb(errorVerb) ) G4cerr << "!!! Too big numbers of phi divisions at ring " << ii << " = " << nring2 << " > " << MAXNDIV << G4endl;
#endif
      G4Exception("GmSDVirtSegmentedSphereThetaPhi::DefineSegmentation",
		  "Wrong argument",
		  FatalErrorInArgument,
		  G4String("nring " + GmGenUtils::ftoa(nring2) + " > MAXNDIV " + GmGenUtils::ftoa(MAXNDIV)).c_str());
    }
  }
}


//-------------------------------------------------------------------
unsigned long long GmSDVirtSegmentedSphereThetaPhi::GetDetUnitID( G4Step* aStep )
{
  return GetDetUnitIDThetaPhi( aStep );
}

//-------------------------------------------------------------------
unsigned long long GmSDVirtSegmentedSphereThetaPhi::GetDetUnitIDThetaPhi( G4Step* aStep )
{
  
  G4ThreeVector pos = aStep->GetTrack()->GetPosition();

  G4int ntheta = int(pos.theta() / theThetaAngle);
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " GetDetUnitID ntheta " << ntheta << " theta "<< pos.theta() << " theThetaAngle " << theThetaAngle << G4endl;
#endif
  G4double phi = pos.phi(); if ( phi < 0. ) phi += 2*M_PI;
  G4int nphi = int(phi / thePhiAngles[ntheta]);
 
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " GetDetUnitIDThetaPhi " << ntheta*MAXNDIV+nphi << " nTheta " << ntheta << " nPhi " << nphi << G4endl;
#endif
  return ntheta*MAXNDIV+nphi;
}


//-------------------------------------------------------------------
void GmSDVirtSegmentedSphereThetaPhi::CalculateAndSetPosition( GmHit* hit, G4Step* )
{
  // get it as the center of the cell in theta and phi, smear the R  
  G4int id = hit->GetDetUnitID();
  G4int idr = id/MAXNDIV/MAXNDIV;

  G4double rad = hit->GetPosition().mag();
  //-  double rndm = CLHEP::RandGauss::shoot( rad, theRResolution );

  // check it is not outside SD volumen
  G4Sphere* sphere = (G4Sphere*)(theLogVol->GetSolid());
  if( rad > sphere->GetOuterRadius() ) rad = sphere->GetOuterRadius();
  if( rad < sphere->GetInsideRadius() ) rad = sphere->GetInsideRadius();

  G4ThreeVector pos(rad, 0., 0. );

  G4int idtheta = (id%(MAXNDIV*MAXNDIV)) / MAXNDIV;
  G4int idphi= id % MAXNDIV;
  G4double theta = theThetaAngle * (idtheta+0.5);
  G4double phi = thePhiAngles[idtheta] * (idphi+0.5);

  pos.setTheta( theta );
  pos.setPhi( phi );

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " GmSDVirtSegmentedSphereThetaPhi::CalculateAndSetPosition idr " << idr << " idtheta " << idtheta << " idphi " << idphi << " pos " << pos << G4endl;
#endif
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " GmSDVirtSegmentedSphereThetaPhi::CalculateAndSetPosition diff pos R " << pos.mag() - hit->GetPosition().mag() << " R " << pos.mag() << " - " << hit->GetPosition().mag() 
	 << " phi " << pos.phi() - hit->GetPosition().phi() << " phi " << pos.phi() << " - " << hit->GetPosition().phi() 
	 << " theta " << pos.theta() - hit->GetPosition().theta() << " theta " << pos.theta() << " - " << hit->GetPosition().theta() 
	 << G4endl;
#endif

  hit->SetPosition( pos );

} 


//-------------------------------------------------------------------
void GmSDVirtSegmentedSphereThetaPhi::EndOfEvent(G4HCofThisEvent*HCE)
{
#ifndef GAMOS_NO_VERBOSE
  if(theHitsInEvent.size() != 0 && SDVerb(infoVerb) ) G4cout << this << "GmSDVirtSegmentedSphereThetaPhi::EndOfEvent nhits " << SensitiveDetectorName << " Nhits " << theHitsInEvent.size() << G4endl;
#endif
  GmVSD::EndOfEvent( HCE );
}
