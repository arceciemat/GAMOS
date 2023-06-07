#include "GmPositionVolumePos.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "CLHEP/Random/RandFlat.h"
#include "G4VSolid.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Ellipsoid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Polycone.hh"
#include "G4Torus.hh"
#include "G4GeometryTolerance.hh"
#include <math.h>
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "G4TouchableHistory.hh"
#include "G4Navigator.hh"
#include "G4TransportationManager.hh"

//------------------------------------------------------------------------
GmPositionVolumePos::GmPositionVolumePos()
{ 
  theParamMgr = GmParameterMgr::GetInstance();
  theTouchable = new G4TouchableHistory;
  theNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
}

//------------------------------------------------------------------------
GmPositionVolumePos::~GmPositionVolumePos()
{
  delete theTouchable;
}


//------------------------------------------------------------------------
G4double GmPositionVolumePos::GetObjectDimension(const G4VSolid* solid) const
{
  G4VSolid* solidnc = const_cast<G4VSolid*>(solid);
  return solidnc->GetCubicVolume();
}

//------------------------------------------------------------------------
G4ThreeVector GmPositionVolumePos::GeneratePosition( const GVSTouchableInfo* tinfo )
{
  const G4VSolid* solid = tinfo->solid;
  G4ThreeVector pos = GeneratePosInSolid( solid );

#ifndef GAMOS_NO_VERBOSE
 if( GenerVerb(infoVerb) ) G4cout << " GmPositionVolumePos::Generate pos before trans " << pos << G4endl;
#endif

  return pos;
}


//------------------------------------------------------------------------
G4ThreeVector GmPositionVolumePos::GeneratePosInSolid( const G4VSolid* solid )
{
  G4double angTolerance = G4GeometryTolerance::GetInstance()->GetAngularTolerance();

  G4double x = 0.;
  G4double y = 0.;
  G4double z = 0.;
  unsigned int ii = 0;
  if( solid->GetEntityType() == "G4Orb" ) {
    const G4Orb* sphe = static_cast<const G4Orb*>(solid);
    G4double radO = sphe->GetRadius();
    G4double radO2 = radO * radO;

    // Generate (x,y,z) inside sphere
    for( ;; ){
      ii++;
      x = -radO+2*radO*CLHEP::RandFlat::shoot();
      y = -radO+2*radO*CLHEP::RandFlat::shoot();
      z = -radO+2*radO*CLHEP::RandFlat::shoot();
      if( x*x+y*y+z*z > radO2 ) {
	continue;
      }
      break;
    }
    
  } else if( solid->GetEntityType() == "G4Sphere" ) {
    const G4Sphere* sphe = static_cast<const G4Sphere*>(solid);

    G4double radO = sphe->GetOuterRadius();
    G4double radO2 = radO * radO;
    G4double radI2 = sphe->GetInnerRadius() * sphe->GetInnerRadius();
    G4double phiStart = sphe->GetStartPhiAngle();
    G4double phiEnd = sphe->GetStartPhiAngle() + sphe->GetDeltaPhiAngle();
    G4bool bDisp360 = CheckPhiStartEnd(phiStart,phiEnd);
    //    if( phiEnd < 0. || phiEnd > CLHEP::twopi ) phiEnd = fmod(phiEnd+CLHEP::twopi,2*CLHEP::pi);
    G4double thetaS = sphe->GetStartThetaAngle();
    G4double thetaE = sphe->GetStartThetaAngle() + sphe->GetDeltaThetaAngle();
    if( thetaE < 0. || thetaE > CLHEP::pi ) thetaE = fmod(thetaE+CLHEP::pi,CLHEP::pi);
    //----- most cases it will be a full sphere, and not checking will spare some time
    G4bool bFullSphere = (radI2 == 0. && phiStart == 0. && fabs(phiEnd-2*CLHEP::pi) < angTolerance && thetaS == 0. && fabs(thetaE - 2*CLHEP::pi) < angTolerance);

#ifndef GAMOS_NO_VERBOSE
    if( !bFullSphere && GenerVerb(debugVerb) ) G4cout << "GmPositionVolumePos::GeneratePosInSolid solid is sphere but not full " << solid->GetName() << G4endl;
#endif

    // Generate (x,y,z) inside sphere
    for( ;; ){
      ii++;
      x = -radO+2*radO*CLHEP::RandFlat::shoot();
      y = -radO+2*radO*CLHEP::RandFlat::shoot();
      z = -radO+2*radO*CLHEP::RandFlat::shoot();
      if( x*x+y*y+z*z > radO2 ) {
	continue;
      } else if( !bFullSphere ){
	G4ThreeVector pos = G4ThreeVector(x,y,z);
	G4double phi = pos.phi();
	CheckPhiPos( phi, bDisp360 );
	G4double theta = fmod(pos.theta()+CLHEP::pi,CLHEP::pi);
	if( pos.mag2() < radI2 ||
	    theta < thetaS || theta > thetaE || 
	    phi < phiStart || phi > phiEnd ) continue;
      }
      break;
    }

  } else if( solid->GetEntityType() == "G4Ellipsoid" ) {
    const G4Ellipsoid* elli = static_cast<const G4Ellipsoid*>(solid);

    G4double xlength = elli->GetSemiAxisMax(0);
    G4double ylength = elli->GetSemiAxisMax(1);
    G4double zlength = elli->GetSemiAxisMax(2);
    G4double zcutB = elli->GetZBottomCut();
    G4double zcutT = elli->GetZTopCut();

    //----- most cases it will be a full elliposid, and not checking will spare some time
    G4bool bFullElli = (zcutB == -zlength && zcutT == zlength);
#ifndef GAMOS_NO_VERBOSE
    if( !bFullElli &&  GenerVerb(debugVerb) )  G4cout << "GmPositionVolumePos::GeneratePosInSolid solid is ellipsoid but not full " << solid->GetName() << *solid << G4endl;
#endif
    // Generate (x,y,z) inside ellipsoid
    for( ;; ){
      ii++;
      x = -xlength+2*xlength*CLHEP::RandFlat::shoot();
      y = -ylength+2*ylength*CLHEP::RandFlat::shoot();
      z = -zlength+2*zlength*CLHEP::RandFlat::shoot();
      if( pow(x/xlength,2)+pow(y/ylength,2)+pow(z/zlength,2) > 1. ) {
	continue;
      } else if( !bFullElli ){
	if( z < zcutB || z > zcutT ) continue;
      }
      break;
    }

  } else if( solid->GetEntityType() == "G4Tubs" ) {
    const G4Tubs* tub = static_cast<const G4Tubs*>(solid);
  
    // Radius and axial size 
    G4double radO = tub->GetOuterRadius();
    G4double radO2 = radO*radO;
    G4double radI2 = tub->GetInnerRadius() * tub->GetInnerRadius();
    G4double zlength = tub->GetZHalfLength();
    G4double phiStart = tub->GetStartPhiAngle();
    G4double phiEnd = tub->GetStartPhiAngle() + tub->GetDeltaPhiAngle();
    G4bool bDisp360 = CheckPhiStartEnd(phiStart,phiEnd);

    //    if( phiEnd > 180.*CLHEP::deg ) phiEnd -=  180.*CLHEP::deg;
    //    if( phiEnd < 0. || phiEnd > CLHEP::twopi ) phiEnd = fmod(phiEnd+CLHEP::twopi,CLHEP::twopi);
    //----- most cases it will be a full tube section, and not checking will spare some time
    G4bool bFullTubs = (radI2 == 0. && phiStart == 0. && fabs(phiEnd - CLHEP::twopi) < angTolerance );
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout  << "GmPositionVolumePos::GeneratePosInSolid solid is tube section but not full " << solid->GetName() << " phiStart " << phiStart/CLHEP::deg << " phiEnd " << phiEnd/CLHEP::deg << G4endl;
#endif
    
    // Generate (x,y,z) inside tube section
    for( ;; ){
      ii++;
      x = -radO+2*radO*CLHEP::RandFlat::shoot();
      y = -radO+2*radO*CLHEP::RandFlat::shoot();
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(debugVerb) ) G4cout  << "GmPositionVolumePos::GeneratePosInSolid try x " << x << " y " << y << " x*x+y*y " <<  x*x+y*y << " radO2 " << radO2 << G4endl;
#endif
      
      if( x*x+y*y > radO2 ) {
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(debugVerb) ) G4cout  << "GmPositionVolumePos::GeneratePosInSolid too big posPerp " << x*x+y*y << " > " << radO2 << G4endl;
#endif
	continue;
      } else if( !bFullTubs ){
	G4ThreeVector pos = G4ThreeVector(x,y,z);
	G4double phi = pos.phi();
	CheckPhiPos( phi, bDisp360 );
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(debugVerb) ) G4cout << "GmPositionVolumePos::GeneratePosInSolid try  pos " << pos << " mag2 " << pos.mag2() << " phi " << phi << G4endl;
#endif
	if( pos.mag2() < radI2 ||
	    phi < phiStart || phi > phiEnd ) {
#ifndef GAMOS_NO_VERBOSE
	  if( GenerVerb(debugVerb) ) G4cout  << x << " " << y << " BAD GmPositionVolumePos::GeneratePosInSolid small radius " << pos.mag2() << " <? " << radI2 << " phi out " << phiStart/CLHEP::deg << " <? " << phi/CLHEP::deg << " <? " << phiEnd/CLHEP::deg << G4endl;
#endif
	  continue;
	} 
      }
      z = -zlength+2*zlength*CLHEP::RandFlat::shoot();
      break;
    }

  } else if( solid->GetEntityType() == "G4Cons" ) {
    const G4Cons* solidc = static_cast<const G4Cons*>(solid);
  
    // Radius and axial size 
    G4double radOM = solidc->GetOuterRadiusMinusZ();
    G4double radOP = solidc->GetOuterRadiusPlusZ();
    G4double radOdiff = radOP-radOM;
    G4double radIM = solidc->GetInnerRadiusMinusZ() * solidc->GetInnerRadiusMinusZ();
    G4double radIP = solidc->GetInnerRadiusPlusZ() * solidc->GetInnerRadiusPlusZ();
    G4double radIdiff = radIP-radIM;
    G4double zlength = solidc->GetZHalfLength();
    G4double phiStart = solidc->GetStartPhiAngle();
    G4double phiEnd = solidc->GetStartPhiAngle() + solidc->GetDeltaPhiAngle();
    G4bool bDisp360 = CheckPhiStartEnd(phiStart,phiEnd);
    //    if( phiEnd < 0. || phiEnd > CLHEP::twopi ) phiEnd = fmod(phiEnd+CLHEP::twopi,CLHEP::twopi);

#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout  << "GmPositionVolumePos::GeneratePosInSolid radO " << G4endl;
#endif
    
    //----- most cases it will be a full tube section, and not checking will spare some time
    G4bool bFullCons = (radIM == 0. && radIP == 0. && phiStart == 0. && fabs(phiEnd - 2*CLHEP::pi) < angTolerance);
#ifndef GAMOS_NO_VERBOSE
    if( !bFullCons )  if( GenerVerb(debugVerb) ) G4cout << "GmPositionVolumePos::GeneratePosInSolid solid is cone section but not full " << solid->GetName() << G4endl;
#endif

    // Generate (x,y,z) inside cone section
    z = -zlength+2*zlength*CLHEP::RandFlat::shoot();
    for( ;; ){
      G4double radO = radOM + (z+zlength)/zlength/2.*radOdiff;
      G4double radI = radIM + (z+zlength)/zlength/2.*radIdiff;
      ii++;
      x = -radO+2*radO*CLHEP::RandFlat::shoot();
      y = -radO+2*radO*CLHEP::RandFlat::shoot();
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(debugVerb) ) G4cout  << "GmPositionVolumePos::GeneratePosInSolid x " << x << " y " << y << " x*x+y*y " <<  x*x+y*y << " radO2 " << radO*radO << G4endl;
#endif
      
      if( x*x+y*y > radO*radO ) {
	continue;
      } else if( !bFullCons ){
	G4ThreeVector pos = G4ThreeVector(x,y,z);
	G4double phi = pos.phi();
	CheckPhiPos( phi, bDisp360 );
	if( phi < 0. ) phi += CLHEP::pi;
	if( pos.mag2() < radI*radI ||
	    phi < phiStart || phi > phiEnd ) continue;
      }
      break;
    }

  } else if( solid->GetEntityType() == "G4Box" ) {
    const G4Box* box = static_cast<const G4Box*>(solid);    

    // Radius and axial size 
    G4double xlength = box->GetXHalfLength();
    G4double ylength = box->GetYHalfLength();
    G4double zlength = box->GetZHalfLength();
    
    x = -xlength+2*xlength*CLHEP::RandFlat::shoot();
    y = -ylength+2*ylength*CLHEP::RandFlat::shoot();
    z = -zlength+2*zlength*CLHEP::RandFlat::shoot();
    
  } else if(solid->GetEntityType() == "G4Polycone" ){
    
	const G4Polycone* polycone = static_cast<const G4Polycone*>(solid);

	G4double phiStart    = polycone->GetStartPhi();	
	G4double phiEnd      = polycone->GetEndPhi();
	//if( phiEnd < 0. || phiEnd > CLHEP::twopi ) phiEnd = fmod(phiEnd+CLHEP::twopi,CLHEP::twopi);

        std::vector<TRIdata> ListTRI;
#ifndef GAMOS_NO_VERBOSE
	if( GenerVerb(infoVerb) ) G4cout << "GmPositionVolumePos::GeneratePosInSolid New polycone found: " << polycone->GetName() << G4endl;
#endif

	if (MapTRI.count(polycone->GetName())==0){	 // INITIALIZE (only first time, it generates the list of triangles)
	  
	  G4int    numRZCorner = polycone->GetNumRZCorner();
	  
	  G4PolyconeSideRZ PolyconeSideRZ =  polycone->GetCorner(0);
	  /*	  G4double r_min =  PolyconeSideRZ.r;
	  G4double r_max =  PolyconeSideRZ.r;
	  G4double z_min =  PolyconeSideRZ.z;
	  G4double z_max =  PolyconeSideRZ.z; */
	  
	  // OLD: std::vector<G4double> rv;
	  //      std::vector<G4double> zv;
	  // Mcc. 2010: "vp" (vector of points: Vpoint)
	  std::vector<Vpoint> vp;
	  Vpoint newP;        
	  
#ifndef GAMOS_NO_VERBOSE
	  if( GenerVerb(infoVerb) ) G4cout << "GmPositionVolumePos::GeneratePosInSolid. Polycone read (r, z): "<< G4endl;
#endif
	  for (int i=0; i<numRZCorner ; i++){
	    PolyconeSideRZ =  polycone->GetCorner(i);
	    newP.r=PolyconeSideRZ.r;
	    newP.z=PolyconeSideRZ.z;
            
	    vp.push_back(newP);
#ifndef GAMOS_NO_VERBOSE
	    if( GenerVerb(infoVerb) ) G4cout << "  " << newP.r << "  " << newP.z << G4endl; 
#endif
	  };
	  
	  TRIdata newD; newD.Atot=0;
	  ListTRI.push_back(newD);
          ListTRI = trianguliza(vp, ListTRI);       
	  
          // TO DEBUG; list of triangles (compare to Matlab code, Mcc 2010)
#ifndef GAMOS_NO_VERBOSE
	  if( GenerVerb(infoVerb) ) G4cout << "GmPositionVolumePos::GeneratePosInSolid. LisTRI size:"<< ListTRI.size() << G4endl;
	  if( GenerVerb(infoVerb) ) G4cout << "GmPositionVolumePos::GeneratePosInSolid. LisTRI points: r1,z1 r2,z2 r3,z3:" << G4endl;
#endif
	  for (unsigned int i=1; i<ListTRI.size(); i++){ 
#ifndef GAMOS_NO_VERBOSE
	    if( GenerVerb(infoVerb) ) G4cout << " " << ListTRI[i].r1 << " " << ListTRI[i].z1 << " " <<  ListTRI[i].r2 << " " << ListTRI[i].z2 << " " << ListTRI[i].r3 << " " << ListTRI[i].z3 << " " << G4endl;	
#endif
	  }
	  
#ifndef GAMOS_NO_VERBOSE
	  if( GenerVerb(infoVerb) ) G4cout << "GmPositionVolumePos::GeneratePosInSolid. Mcc_Triang.Polyc_Volume: " << (phiEnd-phiStart)*(1.)*ListTRI.back().Atot << G4endl;
	  if( GenerVerb(infoVerb) ) G4cout << "GmPositionVolumePos::GetObjectDimension. Polycone_Volume: "  << GmPositionVolumePos::GetObjectDimension(solid) << G4endl;
#endif
	 
	  MapTRI[polycone->GetName()]=ListTRI;
	}
	else
	{
	  ListTRI=MapTRI[polycone->GetName()];
	}

	
        G4double Aran=CLHEP::RandFlat::shoot()*ListTRI.back().Atot;
	int tr=1;  // First triangle in ListTRI always has Atot=0
	
	while ( tr< G4int(ListTRI.size()) && (Aran >= ListTRI[tr].Atot) ) tr++;    // Search the triangle where insert the seed.
       
	G4double Arr=ListTRI[tr].r1, Azz=ListTRI[tr].z1, Brr=ListTRI[tr].r2, Bzz=ListTRI[tr].z2, Crr=ListTRI[tr].r3, Czz=ListTRI[tr].z3; 
	
	// code repeated in areaPOND. To sort vertex of the triangle  Ar<=Br<=Cr
	G4double Ar=Arr, Az=Azz, Br=Brr, Bz=Bzz, Cr=Crr, Cz=Czz;
	G4double Atr, Atz, Btr, Btz;
	if (Brr<Arr) {Ar=Brr; Az=Bzz; Br=Arr; Bz=Azz;};
	if (Crr<Ar) {Atr=Ar; Atz=Az; Ar=Crr; Az=Czz; Cr=Atr; Cz=Atz;};
	if (Cr<Br) {Btr=Br; Btz=Bz; Br=Cr; Bz=Cz; Cr=Btr; Cz=Btz;};
	
	// Pendientes rectas formando lados del triangulo
	G4double P_ac=(Cz-Az)/(Cr-Ar);
	G4double P_ab=(Bz-Az)/(Br-Ar);
	G4double P_bc=(Cz-Bz)/(Cr-Br);
	G4double DpACAB=P_ac-P_ab; 
	G4double DpACBC=P_ac-P_bc;
	
	// M:: Punto intermedio, divide en dos subtriangulos
	G4double Mr=Br,Mz;
	if (Mr==Ar) {Mz=Az;} 
	else {Mz=P_ac*(Mr-Ar)+Az;};
	
	G4double Dz=Mz-Bz;
	
	G4double Ap_amb=areaPOND(Ar,Az,Mr,Mz,Br,Bz);
	G4double Ap_bmc=areaPOND(Br,Bz,Mr,Mz,Cr,Cz);
	
	// G4cout << " @@@@ DEBUG here @@@@@@  Ap_amb, Ap_bmc: " << Ap_amb << " " << Ap_bmc << G4endl;
	
	G4double Ap_t=Ap_amb+Ap_bmc;
	G4double Zab, Zac,Zbc, Zmax, Zmin, a, b, c, d;
		
        G4double Rr, Zz; // the random point inside polycone in polar coordinates
	G4double ran1=CLHEP::RandFlat::shoot(), ran2=CLHEP::RandFlat::shoot(), ran3=CLHEP::RandFlat::shoot();

	if (Ap_t==0) {Rr=Ar; Zz=Az;}    // TO DO include G4exception ??
	else  {
	  
	  if (ran1*Ap_t <= Ap_amb)  {// case subtriangle AMB
	    // a, b, c, d: coeficientes ec. tercer grado: ax3+bx2+cx+d=0
  
	    if (Mz <= Bz) DpACAB=-DpACAB;
	    
	    a=DpACAB*(1.0/3.0);
	    b=DpACAB*(-Ar/2.0);
	    c=0.0;
	    d=DpACAB*(pow(Ar,3)/6.0)-ran2*Ap_amb;
	    
            Rr=find_R(a,b,c,d,Ar,Mr);	    
	    
	    Zab=P_ab*(Rr-Ar)+Az;
	    Zac=P_ac*(Rr-Ar)+Az;
	    Zmax=Zac; Zmin=Zab;
	    if (Mz <= Bz) { Zmax=Zab; Zmin=Zac;}
	    Zz=Zmin + ran3*(Zmax-Zmin);
	    
	  } //case AMB 
	  
	  else  {  //case BMC

	    if (Mz <= Bz) {DpACBC=-DpACBC; Dz=-Dz;}
	    
	    G4double Cte_bcm = DpACBC*pow(Br,3)/3.-DpACBC*pow(Br,3)/2.+Dz*pow(Br,2)/2.;
	    
	    a=DpACBC*(1./3.);
	    b=Dz/2.-DpACBC*Br/2.;
	    c=0.;
	    d=-Cte_bcm-ran2*Ap_bmc;
	    
	    Rr=find_R(a,b,c,d,Mr,Cr);
	    
	    Zbc=P_bc*(Rr-Br)+Bz;
	    Zac=P_ac*(Rr-Ar)+Az;
	    Zmax=Zac; Zmin=Zbc;
	    if (Mz <= Bz) { Zmax=Zbc; Zmin=Zac;}
	    Zz=Zmin + ran3*(Zmax-Zmin);
	    
	  }
	  
	  G4double thePhi= phiStart + CLHEP::RandFlat::shoot()*(phiEnd-phiStart);
	  
	  x=Rr*cos(thePhi);
	  y=Rr*sin(thePhi);
	  z=Zz;
	  
	  
#ifndef GAMOS_NO_VERBOSE
	  if( GenerVerb(infoVerb) ) G4cout << "GmPositionVolumePos::Polycone x,y,z: "  << x << " " << y << " " << z << G4endl;
#endif
	  
	  
	  /* OLD:
	     
	  G4ThreeVector pos = G4ThreeVector(0,0,0);
	  G4bool control=0;
	  while(control==0){
	  
	  
	  x = -r_max+2*r_max*CLHEP::RandFlat::shoot();
	  y = -r_max+2*r_max*CLHEP::RandFlat::shoot();
	  z = z_min+(z_max-z_min)*CLHEP::RandFlat::shoot();
	  
	  pos = G4ThreeVector(x,y,z);
	  // G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( pos, theTouchable, false );
	  
	  if(polycone->Inside(pos)==kInside || polycone->Inside(pos)==kSurface){ 
	  control = 1;
	  };
	  G4cout <<" Kindie = "  << polycone->Inside(pos)<<" Distance to Out " << polycone->DistanceToOut(pos)   << G4endl; 
	  };
	  G4cout <<" Kindie Acepted = "  << polycone->Inside(pos)  ;
	  G4cout <<" S = " << x << " " <<  y  << " " << z << G4endl;
	  G4cout <<" Distance to Out " << polycone->DistanceToOut(pos) << G4endl; 
	  */
	} 

  } else if( solid->GetEntityType() == "G4Torus" ) {
    const G4Torus* tor = static_cast<const G4Torus*>(solid);
  
    // Radius and axial size
    G4double radO = tor->GetRmax();
    G4double radO2 = radO*radO;
    G4double radI2 = tor->GetRmin() * tor->GetRmin();
    G4double phiStart = tor->GetSPhi();
    G4double phiDelta = tor->GetDPhi();
    G4double radTor = tor->GetRtor();

    //----- most cases it will be a full tube section, and not checking will spare some time
    //    G4bool bFullTubs = (radI2 == 0. && phiStart == 0. && fabs(phiEnd - CLHEP::twopi) < angTolerance );
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout  << "GmPositionVolumePos::GeneratePosInSolid solid is tube section but not full " << solid->GetName() << " phiStart " << phiStart << " phiDelta " << phiDelta << G4endl;
#endif
    
    // Generate (x,y,z) inside tube section
    for( ;; ){
      ii++;
      x = -radO+2*radO*CLHEP::RandFlat::shoot();
      z = -radO+2*radO*CLHEP::RandFlat::shoot();
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(debugVerb) ) G4cout  << "GmPositionVolumePos::GeneratePosInSolid try x " << x << " z " << z << " x*x+z*z " << x*x+z*z << " radO2 " << radO2 << G4endl;
#endif
      G4double posXZ = x*x+z*z;
      
      if( posXZ < radI2 || posXZ > radO2 ) {
	continue;
      }
 
      x += radTor;
      double phi = CLHEP::RandFlat::shoot()*phiDelta + phiStart;
      y = x * sin(phi);
      x *= cos(phi);
#ifndef GAMOS_NO_VERBOSE
      if( GenerVerb(debugVerb) ) {
	G4ThreeVector pos = G4ThreeVector(x,y,z);
	G4cout  << "GmPositionVolumePos::GeneratePosInSolid try  pos " << pos << " phi " << phi/CLHEP::deg << G4endl;
      }
#endif

      break;
    }

  }else {
    G4Exception("GmPositionVolumePos::GeneratePosInSolid",
		    "Wrong argument",
		    FatalErrorInArgument,
		G4String("Solid type not supported " + solid->GetEntityType() + " , for volume " + solid->GetName() ).c_str());
  }

  //check if
  if( int(theParamMgr->GetNumericValue("PositionInVolumes:NotDaughters",0)) == 1 ) {
    theNavigator->LocateGlobalPointAndUpdateTouchable( G4ThreeVector(x,y,z), theTouchable, false );
    std::cout << " CHECK VOL " << G4ThreeVector(x,y,z) << " TOUCH " <<  theTouchable->GetVolume(0)->GetName() << " SOLID " << solid->GetName() << std::endl;
    if( theTouchable->GetVolume(0)->GetName() != solid->GetName() ) return GeneratePosInSolid( solid );
  }
  
  return G4ThreeVector(x,y,z);
  
}

//------------------------------------------------------------------------
G4double GmPositionVolumePos::areaTRI(G4double r1, G4double z1, G4double r2, G4double z2, G4double r3, G4double z3)
{
  G4double area=0.5*fabs((r1*z2+r3*z1+r2*z3-z2*r3-z1*r2-z3*r1));
  return area;
}


//------------------------------------------------------------------------
G4double GmPositionVolumePos::areaPOND(G4double Arr, G4double Azz, G4double Brr, G4double Bzz, G4double Crr, G4double Czz)
{
  
  // To sort vertex of the triangle  Ar<=Br<=Cr
  G4double Ar=Arr; G4double Az=Azz; G4double Br=Brr; G4double Bz=Bzz; G4double Cr=Crr; G4double Cz=Czz;
  G4double Atr, Atz, Btr, Btz;
  if (Brr<Arr) {Ar=Brr; Az=Bzz; Br=Arr; Bz=Azz;};
  if (Crr<Ar) {Atr=Ar; Atz=Az; Ar=Crr; Az=Czz; Cr=Atr; Cz=Atz;};
  if (Cr<Br) {Btr=Br; Btz=Bz; Br=Cr; Bz=Cz; Cr=Btr; Cz=Btz;};
  
  // Pendientes rectas formando lados del triangulo
  G4double P_ac=(Cz-Az)/(Cr-Ar);
  G4double P_ab=(Bz-Az)/(Br-Ar);
  G4double P_bc=(Cz-Bz)/(Cr-Br);
  G4double DpACAB=P_ac-P_ab; G4double DpACBC=P_ac-P_bc;
  
  // M:: Punto intermedio, divide en dos subtriangulos
 G4double Mr=Br,Mz;
 if (Mr==Ar) {Mz=Az;} 
 else {Mz=P_ac*(Mr-Ar)+Az;};
 
 G4double Dz=Mz-Bz;
 G4double a,b,c,d,A1,A2,Ap;
 
 // case AMB
 if ( areaTRI(Ar,Az,Mr,Mz,Br,Bz)==0 || (Cr==Ar) || (Br==Ar) ) {A1=0;} 
 else {
   
   a=DpACAB*(1./3.0);
   b=DpACAB*(-Ar/2.0);
   c=0.0;
   d=DpACAB*(pow(Ar,3)/6.0);
   
   A1=a*pow(Br,3)+b*pow(Br,2)+c*Br+d;
 };
 
 if (Dz<0) A1=-A1; 
 
 
 // case BMC
 
 if ( areaTRI(Br,Bz,Mr,Mz,Cr,Cz)==0 || (Cr==Ar) || (Br==Cr) ) {A2=0;} 
 else 
   {    
     //coeficientes ec. tercer grado: ax3+bx2+cx+d=0
     G4double Cte_bcm=DpACBC*pow(Br,3)/3-DpACBC*pow(Br,3)/2+Dz*pow(Br,2)/2;
     
     a=DpACBC*(1./3.0);
     b=Dz/2.0-DpACBC*Br/2.0;
     c=0.0;
     d=-Cte_bcm;
     
     A2=a*pow(Cr,3)+b*pow(Cr,2)+c*Cr+d; 
     
     if (Dz<0) A2=-A2; 
     
   }
 
 Ap=A1+A2;
 
 return Ap;
}


//------------------------------------------------------------------------
std::vector<TRIdata> GmPositionVolumePos::trianguliza(std::vector<Vpoint> vect_p, std::vector<TRIdata> list_tr)
{
  std::vector<Vpoint> new_vect_p;
  //std::vector<TRIdata>::iterator itt;
  
  //  Points on a line or repeated are erased from "vect_p"
  
  for(  int ii = 0; ii < G4int(vect_p.size()); ii++ ){
    if (ii>0){
      if ( (vect_p[ii].r == vect_p[ii-1].r) && (vect_p[ii].z == vect_p[ii-1].z) ) 
	{vect_p.erase(vect_p.begin()+ii);
	  ii--;
	}
      else if ( (ii>1) &&  areaTRI(vect_p[ii].r,vect_p[ii].z,vect_p[ii-1].r,vect_p[ii-1].z,vect_p[ii-2].r,vect_p[ii-2].z)==0 ){
	
	vect_p.erase(vect_p.begin()+ii-1);
	ii--;
      }
    }
  }
  
  int v_size=vect_p.size();
  
  int V_int=-1;
  int end_pos=v_size-1;
  int end_posL;
  
  while (v_size>=3){
    
    // To ROTATE "vect_p", first Z MIN.
    std::rotate(vect_p.begin(), std::min_element( vect_p.begin(),vect_p.end(),CompVpoints_Z() ) ,vect_p.end() );
    
    end_pos=v_size-1;
    V_int=-1;
    
    // Busca vertices interiores. Condition for v_test: Area(v1,v2,v3;v_test)=Area(v1,v2,v3) 
    for ( int ii = 2; ii < G4int(vect_p.size()-1); ii++ ){
    
	    if ( areaTRI(vect_p[ii].r,vect_p[ii].z,vect_p[0].r,vect_p[0].z,vect_p[1].r,vect_p[1].z)+ areaTRI(vect_p[ii].r,vect_p[ii].z,vect_p[0].r,vect_p[0].z,vect_p[end_pos].r,vect_p[end_pos].z) + areaTRI(vect_p[ii].r,vect_p[ii].z,vect_p[1].r,vect_p[1].z,vect_p[end_pos].r,vect_p[end_pos].z) <= areaTRI(vect_p[0].r,vect_p[0].z,vect_p[1].r,vect_p[1].z,vect_p[end_pos].r,vect_p[end_pos].z)*(1.+doublePRECISION))
	{
	  // Existe un vertice INTERIOR !! -> guardar el de MENOR Z!
	  if ( (V_int<0) || ( vect_p[ii].z < vect_p[V_int].z) ) V_int=ii;
	};
    };
    
    if (V_int<0){  // No vertices interiores
      
      // 28/04/10: no usar AREA!, sino area PONDERADA (volumen de revolucion)...
      // To add new triangle in the "list_tr" (this triangle includes the First point of "vect_p"
      
      TRIdata newData; 
      newData.Atot=0;
      list_tr.push_back(newData);
      
      end_posL = list_tr.size()-1;  // end_posL used for "list_tr"
      
      list_tr[end_posL].Atot=list_tr[end_posL-1].Atot+areaPOND(vect_p[end_pos].r,vect_p[end_pos].z,vect_p[0].r,vect_p[0].z,vect_p[1].r,vect_p[1].z);
      
      list_tr[end_posL].r1=vect_p[0].r; list_tr[end_posL].r2=vect_p[1].r; list_tr[end_posL].r3=vect_p[end_pos].r;
      list_tr[end_posL].z1=vect_p[0].z; list_tr[end_posL].z2=vect_p[1].z; list_tr[end_posL].z3=vect_p[end_pos].z;
      
      // Remove First point from "vect_p"
      vect_p.erase(vect_p.begin());
    }
    else
      {
	// Split "vect_p" into two vectors (the insider point is the splitting point) and to do a Recursive Call to "trianguliza"
	
	new_vect_p.clear();
	new_vect_p.push_back(vect_p[0]);
	new_vect_p.insert(new_vect_p.begin()+1, vect_p.begin()+V_int, vect_p.end()); 
	
	vect_p.erase(vect_p.begin()+V_int+1,vect_p.end());
        
	list_tr=trianguliza(new_vect_p, list_tr);
      }
    
    v_size=vect_p.size();
    
  } //while  
  
  return list_tr;
  
}


//------------------------------------------------------------------------
G4double GmPositionVolumePos::find_R(G4double a, G4double b, G4double c, G4double d, G4double Rmin, G4double Rmax)
{

 std::complex<G4double> b_2, b_3, a_2, CC, m, k, n, w1, w2;
 std::complex<G4double> the_i = std::complex<G4double>(0.,1.);

 G4double the_R;
 
 std::vector< std::complex<G4double> > ceros;
 
 b_2=pow(b,2); b_3=pow(b,3); a_2=pow(a,2);
 CC=pow( 0.5* (-2.*b_3 + 9.*a*b*c - 27.*a_2*d + sqrt( 4.*pow( (-b_2 + 3.*a*c),3 ) + pow( (-2.*b_3 + 9.*a*b*c - 27.*a_2*d),2) )) ,(1./3.) );
 
 //Q=sqrt( pow( (2.*b_3-9.*a*b*c + 27.*a_2*d) ,2) - 4.*pow((b_2 - 3.*a*c),3) );
 //CC=pow( (0.5*(Q + 2.*b_3 - 9.*a*b*c + 27.*a_2*d ) ),(1./3.));
 
 
 m=2.*pow(b/a,3)-9.*(b/a)*(c/a)+27.*(d/a);
 k=pow(b/a,2)-3.*(c/a);
 n= pow(m,2)-4.*pow(k,3);
 w1=-0.5+0.5*sqrt(3.)*the_i;
 w2=-0.5-0.5*sqrt(3.)*the_i;
 
 // case CC = 0 
 ceros.push_back(-(1./3.)*((b/a)+pow( (0.5*(m+sqrt(n))), (1./3.) ) + pow( (0.5*(m-sqrt(n))), (1./3.))) );
 ceros.push_back(-(1./3.)*((b/a)+w2*pow( (0.5*(m+sqrt(n))), (1./3.) ) + w1*pow( (0.5*(m-sqrt(n))), (1./3.))) );
 ceros.push_back(-(1./3.)*((b/a)+w1*pow( (0.5*(m+sqrt(n))), (1./3.) ) + w2*pow( (0.5*(m-sqrt(n))), (1./3.))) );
 // case CC > 0
 ceros.push_back(-b/(3.*a) + CC/(3.*a) - (-b_2+3.*a*c)/(3.*a*CC) );
 
 // G4cout << " @@@@ DEBUG here, compare to MATLAB @@@@@@  AMB y ceros " << a << " " << b << " " << c << " " << d << " " << ceros[0] << " " << ceros[1] << " " << ceros[2] << G4endl;
 //G4cout << " @@@@ DEBUG here, compare to MATLAB @@@@@@  m, k, n, Q, CC " << m << " " << k << " " << n << " " << Q << " " << CC << G4endl;
 
 if (  ceros[3].real() >= Rmin  && ceros[3].real() <= Rmax ) {the_R=ceros[3].real();}
 else if ( (ceros[0].real() >= Rmin  && ceros[0].real() <= Rmax ) && (fabs(ceros[0].imag())<1e-10) ) {the_R=ceros[0].real();}
 else if ( ( ceros[1].real() >= Rmin  && ceros[1].real() <= Rmax )  && (fabs(ceros[1].imag())<1e-10) ) {the_R=ceros[1].real();}
 else if ( ( ceros[2].real() >= Rmin  && ceros[2].real() <= Rmax )  && (fabs(ceros[2].imag())<1e-10) ) {the_R=ceros[2].real();}
 else {G4Exception("GmPositionVolumePos::find_R",
		    "Wrong argument",
		    FatalErrorInArgument,
		   "Mcc2010:Polycone(caseAMB): Polycone.Random_Radial not found!");}
 
 return the_R;
 
}


//------------------------------------------------------------------------
G4bool GmPositionVolumePos::CheckPhiStartEnd(G4double& phiStart, G4double& phiEnd)
{
  G4bool bDisp360 = false;
  if( phiStart < 0. || phiEnd < 0. ) {
    phiStart += 360.*CLHEP::deg;
    phiEnd += 360.*CLHEP::deg;
    bDisp360 = true;
  }

  return bDisp360;
}

//------------------------------------------------------------------------
void GmPositionVolumePos::CheckPhiPos( G4double &phi, G4bool bDisp360 )
{
  if( bDisp360 ) {
    phi += 360.*CLHEP::deg;
  } else if( phi < 0. ) {
    phi += 360.*CLHEP::deg;
  }
}
