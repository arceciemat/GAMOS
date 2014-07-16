#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4Track.hh"
#include "G4LogicalVolume.hh"
#include "G4Sphere.hh"

#include "GmSDVirtSegmentedSphereRThetaPhi.hh"
#include "GmHit.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "GamosCore/GamosSD/include/GmSDVerbosity.hh"


//-------------------------------------------------------------------
GmSDVirtSegmentedSphereRThetaPhi::GmSDVirtSegmentedSphereRThetaPhi(G4String sdtype): GmSDVirtSegmentedSphereThetaPhi( sdtype )
{
  DefineSegmentation(sdtype);
}

//-------------------------------------------------------------------
GmSDVirtSegmentedSphereRThetaPhi::~GmSDVirtSegmentedSphereRThetaPhi(){}

void GmSDVirtSegmentedSphereRThetaPhi::DefineSegmentation(const G4String& sdtype)
{
  DefineThetaPhiSegmentation(sdtype);
  DefineRSegmentation(sdtype);
}

//-------------------------------------------------------------------
void GmSDVirtSegmentedSphereRThetaPhi::DefineRSegmentation(const G4String& sdtype)
{
  GmParameterMgr* parmgr = GmParameterMgr::GetInstance();
  G4double cellR = 10.*CLHEP::mm;
  
  cellR = parmgr->GetNumericValue("SD:VirtSegmentedSphereRThetaPhi;cellR:"+sdtype, 10.*CLHEP::mm);

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << sdtype << " cellR " << cellR << G4endl;
#endif

  if( cellR == 0. ) {
#ifndef GAMOS_NO_VERBOSE
    if( SDVerb(errorVerb) )  G4cerr << sdtype << " cellR " << cellR << G4endl;
#endif
    G4Exception("GmSDVirtSegmentedSphereThetaPhi::DefineRSegmentation",
		"Wrong argument",
		FatalErrorInArgument,
		"CellR cannot be 0. ");
  }

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " GmSDVirtSegmentedSphereThetaPhi::DefineRSegmentation() for an Sphere centered at 0 " << theLogVol->GetName() << G4endl;
#endif

  if( theLogVol->GetSolid()->GetEntityType() != "G4Sphere" ) {
    G4Exception("GmSDVirtSegmentedSphereThetaPhi::DefineRSegmentation",
		"Wrong argument",
		FatalErrorInArgument,
		"For a volume that it is not an sphere ");
  }
  G4Sphere* sphere = (G4Sphere*)(theLogVol->GetSolid());
  theRadius = sphere->GetInsideRadius();
  float ndivr = (sphere->GetOuterRadius() - theRadius)/cellR;
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " sphere R length  " << ndivr*cellR << " ndivR " << ndivr << G4endl;
#endif
  float ext = ndivr - int(ndivr);
  ( ext / (1-ext) < ndivr/(ndivr+1) ) ? ndivr = int(ndivr) : ndivr = int(ndivr+1);
  theRLength = (sphere->GetOuterRadius() - theRadius)/ndivr;
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " theRLength " << theRLength << " ndivr " << ndivr << G4endl;
#endif

}

//-------------------------------------------------------------------
unsigned long long GmSDVirtSegmentedSphereRThetaPhi::GetDetUnitID( G4Step* aStep )
{
  return GetDetUnitIDThetaPhi( aStep ) + GetDetUnitIDR( aStep );
}

//-------------------------------------------------------------------
unsigned long long GmSDVirtSegmentedSphereRThetaPhi::GetDetUnitIDR( G4Step* aStep )
{
 
  G4ThreeVector pos = aStep->GetTrack()->GetPosition();

  G4int nR = int( (pos.mag()-theRadius)/theRLength); 
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " GetDetUnitID nr " << nR << " " << pos.mag() << " - " << theRadius << " / " << theRLength << G4endl; 
#endif

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " GetDetUnitIDR " << nR*MAXNDIV*MAXNDIV << " nr " << nR << G4endl;
#endif
  return nR*MAXNDIV*MAXNDIV;
}


//-------------------------------------------------------------------
void GmSDVirtSegmentedSphereRThetaPhi::CalculateAndSetPosition( GmHit* hit, G4Step* )
{
  // get it as the center of the cell 
  G4int id = hit->GetDetUnitID();
  G4int idr = id/MAXNDIV/MAXNDIV;

  G4ThreeVector pos(theRadius+theRLength*(idr+0.5), 0., 0. );

  //-  G4cout << " GmSDVirtSegmentedSphereRThetaPhi::CalculateAndSetPosition pos " << pos << " theRadius+theRLength*(;
  G4int idtheta = (id%(MAXNDIV*MAXNDIV)) / MAXNDIV;
  G4int idphi= id % MAXNDIV;
  G4double theta = theThetaAngle * (idtheta+0.5);
  G4double phi = thePhiAngles[idtheta] * (idphi+0.5);

  pos.setTheta( theta );
  pos.setPhi( phi );

#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " GmSDVirtSegmentedSphereRThetaPhi::CalculateAndSetPosition id " << id << " idr " << idr << " idtheta " << idtheta << " idphi " << idphi << " pos " << pos << G4endl;
#endif
#ifndef GAMOS_NO_VERBOSE
  if( SDVerb(infoVerb) ) G4cout << " GmSDVirtSegmentedSphereRThetaPhi::CalculateAndSetPosition diff pos R " << pos.mag() - hit->GetPosition().mag() << " R " << pos.mag() << " - " << hit->GetPosition().mag() 
	 << " phi " << pos.phi() - hit->GetPosition().phi() << " phi " << pos.phi() << " - " << hit->GetPosition().phi() 
	 << " theta " << pos.theta() - hit->GetPosition().theta() << " theta " << pos.theta() << " - " << hit->GetPosition().theta() 
	 << G4endl;
#endif

  hit->SetPosition( pos );

} 
